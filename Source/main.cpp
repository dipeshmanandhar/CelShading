//Dipesh Manandhar 4/27/2019

#ifndef MAIN_CPP
#define MAIN_CPP

//External Libraries for OpenGL (GLFW and GLAD)
#include <glad/glad.h> // NOTE: always include GLAD before GLFW
#include <GLFW/glfw3.h>

#ifndef STB_IMAGE_IMPLEMENTATION
//External Library For loading images (STB)
#define STB_IMAGE_IMPLEMENTATION
#include "../Headers/stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
#include <iostream>
#include <string> // use of to_str() requires C++11

//Created H Files
#include "../Headers/Shader.h"
#include "../Headers/Camera.h"
#include "../Headers/Mesh.h"
#include "../Headers/Model.h"
#include "../Headers/TextRenderer.h"
#include "../Headers/Loader.h"
#include "../Headers/Screen.h"

using namespace std;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

//For frame rate adjustments
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float accumulator = 0.0f;	// Time Counter
unsigned int frames = 0;
float fps = 0.0f;

// method called every time screen is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// method called every frame to check input keys
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.processKeyboard(DOWN, deltaTime);
}
// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

int main()
{
	//Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Required for Mac OS X

	//Instantiate the Window
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		glfwTerminate();
		return -1;
	}

	//Set Rendering Display Dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Tell GLFW how to resize the Rendering Dispay Dimensions each time the screen is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//Tell GLFW how to handle mouse movement
	glfwSetCursorPosCallback(window, mouse_callback);
	//Tell GLFW how to handle mouse scrolling
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialization
	
	//Create Shader Programs
	Shader lightingShader("Resources/Shaders/LightingShader.vert", "Resources/Shaders/LightingShader.frag");
	Shader borderShader("Resources/Shaders/BorderShader.vert", "Resources/Shaders/BorderShader.frag");
	Shader lampShader("Resources/Shaders/LampShader.vert", "Resources/Shaders/LampShader.frag");
	Shader skyboxShader("Resources/Shaders/SkyboxShader.vert", "Resources/Shaders/SkyboxShader.frag");

	//Model nanosuit("Resources/Models/nanosuit/nanosuit.obj");
	//Model nanosuit("Resources/Models/yagyuu/scene.gltf");
	Model nanosuit("Resources/Models/Chiya/Chiya.fbx");

	float vertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};

	const int NUM_POINT_LIGHTS=1;
	glm::vec3 pointLightPositions[] = {
	glm::vec3(1.0f,  2.0f,  0.0f)
	//glm::vec3(2.3f, -3.3f, -4.0f),
	//glm::vec3(-4.0f,  2.0f, -12.0f),
	//glm::vec3(0.0f,  0.0f, -3.0f)
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// Create VBO
	unsigned int lampVBO,skyboxVBO;	// Vertex Buffer Object
	glGenBuffers(1, &lampVBO);	// Objects in C are structs, thus in OpenGL, int ids "point to" object memory
	glGenBuffers(1, &skyboxVBO);	// Objects in C are structs, thus in OpenGL, int ids "point to" object memory

	unsigned int lampVAO, skyboxVAO;
	//light VAO
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	// Copy vertex array data into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	// Copy vertex array data into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// don't forget to 'use' the corresponding shader program first (to set the uniform)
	lightingShader.use();

	//set light attributes
	// Directional Light
	lightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
	lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
	//Point Lights
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		lightingShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("pointLights[" + to_string(i) + "].diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
		lightingShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.09f);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032f);
	}
	//Spot Light
	lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	lightingShader.setFloat("spotLight.constant", 1.0f);
	lightingShader.setFloat("spotLight.linear", 0.09f);
	lightingShader.setFloat("spotLight.quadratic", 0.032f);
	lightingShader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
	lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f); // darken the light a bit to fit the scene
	lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

	//load Skybox
	vector<string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	 
	for (int i=0 ; i<faces.size(); i++)
		faces[i] = "Resources/CubeMaps/skybox/" + faces[i];

	unsigned int cubemapTexture = Loader::loadCubemap(faces);

	//initialize texture renderer
	TextRenderer textRenderer("font1.png");

	//enable z-buffer
	glEnable(GL_DEPTH_TEST);
	//enable stencil buffer
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_CULL_FACE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		accumulator += deltaTime;
		frames++;
		if (accumulator >= 1)
		{
			fps = frames / accumulator;
			accumulator = 0;
			frames = 0;
		}

		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		

		//matrices for transformations

		//view matrix
		glm::mat4 view = camera.getViewMatrix();

		//perspective projection matrix
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		glStencilMask(0x00); //Don't update stencil buffer for certain draws
		//draw lamp
		lampShader.use();

		//bind lamp's VAO
		glBindVertexArray(lampVAO);

		//lampShader.setMat4("view", view);
		//lampShader.setMat4("projection", projection);

		for (int i = 0; i < NUM_POINT_LIGHTS; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));

			lampShader.setMat4("mvp", projection * view * model);

			//Draw call
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //use EBO
			glDrawArrays(GL_TRIANGLES, 0, 36);	//use VBO
		}
		//unbind VAO
		glBindVertexArray(0);


		//update stencil buffer for draw call
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);


		//draw scene
		lightingShader.use();

		
		//set light position uniforms (they change based on the camera's view matrix)
		//Directional Light
		lightingShader.setVec3("dirLight.direction", glm::vec3(view * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
		//Point Lights
		pointLightPositions[0] = glm::vec3(cos(glfwGetTime()), 2.0f, sin(glfwGetTime()));
		for (int i = 0; i < NUM_POINT_LIGHTS; i++)
			lightingShader.setVec3("pointLights[" + to_string(i) + "].position", glm::vec3(view * glm::vec4(pointLightPositions[i], 1.0f)));
		
		//model matrix
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f));
		//model = glm::translate(model, cubePositions[i]);
		//model = glm::rotate(model, i * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f));

		//set transformatix matrices uniforms
		glm::mat4 mv = view * model;
		glm::mat4 mvp = projection * mv;
		lightingShader.setMat4("mvp", mvp);
		lightingShader.setMat4("mv", mv);
		lightingShader.setMat3("mvNormal", transpose(inverse(glm::mat3(mv))));

		//draw model
		nanosuit.Draw(lightingShader);

		//scale model and use stencil to draw only outline, using BorderShader.frag
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);

		borderShader.use();


		//set transformatix matrices uniforms
		borderShader.setMat4("mvp", mvp);
		//borderShader.setMat4("mv", mv);
		//borderShader.setMat3("mvNormal", transpose(inverse(glm::mat3(mv))));
		
		//Draw scaled border
		nanosuit.Draw(borderShader);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);


		//skybox
		glDisable(GL_STENCIL_TEST); // dont use stencil for skybox
		glStencilMask(0x00);
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		// ... set view and projection matrix
		skyboxShader.setMat4("vp", projection* glm::mat4(glm::mat3(view)));

		// ... draw call
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(skyboxVAO);
		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
		glDepthFunc(GL_LESS); // set depth function back to default


		//draw text
		static GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		textRenderer.Draw(("FPS: " + to_string(fps)).c_str());
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		if(polygonMode == GL_LINE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//else if(polygonMode == GL_FILL)
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//else if (polygonMode == GL_POINTS)
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//properly clean/delete all of GLFW's resources that were allocated
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &lampVBO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteTextures(1, &cubemapTexture);
	
	glfwTerminate();

	return 0;
}

#endif // !MAIN_CPP