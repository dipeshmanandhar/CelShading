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

// compile-time constants
#define PI 3.14159265358979323846f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

// Global Variables ------------------------------------------------------------------------------------

// Window
GLFWwindow* window = NULL;

// Camera variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// For frame rate adjustments
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float accumulator = 0.0f;	// Time Counter
unsigned int frames = 0;
float fps = 0.0f;

// Rendering Variables
	// Framebuffers 
unsigned int gBuffer;
	// Textures
unsigned int gPosition, gNormal, gColorSpec, cubemapTexture;
	// Shaders
Shader geometryShader, lampShader, skyboxShader, lightVolumeShader, stencilShader;
	// Transformation Matrices
glm::mat4 view, projection;
	// Models
Model nanosuit;
	// VBOs
unsigned int lampVBO, skyboxVBO, screenVBO, sphereVBO;
	// EBOs
unsigned int sphereEBO;
	// VAOs
unsigned int lampVAO, skyboxVAO, screenVAO, sphereVAO;
	// point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(1.0f,  2.0f,  0.0f),
	glm::vec3(-0.5f, 1.0f, 1.0f)
	//glm::vec3(-4.0f,  2.0f, -12.0f),
	//glm::vec3(0.0f,  0.0f, -3.0f)
};
const unsigned int NUM_POINT_LIGHTS = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
float pointLightColors[] = {
	//R	    G    B
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f
};
float lightVolumeRadius;
unsigned int numSpherePoints;
	//Text
TextRenderer textRenderer;


// Functions ---------------------------------------------------------------------------------------------

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

/*
//TODO: implement rendering in such methods
void drawScene()
{
	//draw model
}
void drawLights()
{
	
}
*/

int initializeOpenGL()
{
	//Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Required for Mac OS X

	//Instantiate the Window
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
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

	return 0;
}

void initializeShaders()
{
	geometryShader.initialize("Resources/Shaders/GeometryShader.vert", "Resources/Shaders/GeometryShader.frag");
	//borderShader.initialize("Resources/Shaders/BorderShader.vert", "Resources/Shaders/BorderShader.frag");
	lampShader.initialize("Resources/Shaders/LampShader.vert", "Resources/Shaders/LampShader.frag");
	skyboxShader.initialize("Resources/Shaders/SkyboxShader.vert", "Resources/Shaders/SkyboxShader.frag");
	lightVolumeShader.initialize("Resources/Shaders/LightVolumeShader.vert", "Resources/Shaders/LightVolumeShader.frag");
	stencilShader.initialize("Resources/Shaders/StencilShader.vert", "Resources/Shaders/StencilShader.frag");
}

void initializeVAOs()
{
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

	float screenVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// create 12 vertices of a icosahedron
	float t = (1.0f + sqrt(5.0f)) / 2.0f;
	float magnitude = sqrt(1.0f + t * t);
	float small = 1 / magnitude;
	float large = t / magnitude;

	float sphereVertices[] = {
		// part 1 - first 4 points
		-small, large, 0.0f,	//point 0
		small, large, 0.0f,
		-small, -large, 0.0f,
		small, -large, 0.0f,

		// part 2 - second 4 points
		0.0f, -small, large,	//point 4
		0.0f, small, large,
		0.0f, -small, -large,
		0.0f, small, -large,

		// part 3 - third 4 points
		large, 0.0f, -small,	//point 8
		large, 0.0f, small,
		-large, 0.0f, -small,
		-large, 0.0f, small		//point 11
	};
	unsigned int sphereIndices[] = {
		// 5 faces around point 0
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,
		// 5 adjacent faces
		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,
		// 5 faces around point 3
		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,
		// 5 adjacent faces
		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};
	numSpherePoints = sizeof(sphereIndices) / sizeof(sphereIndices[0]);

	// Create VBOs
	//unsigned int lampVBO, skyboxVBO, screenVBO, sphereVBO;	// moved to global variables
	glGenBuffers(1, &lampVBO);	// Objects in C are structs, thus in OpenGL, int ids "point to" object memory
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &screenVBO);
	glGenBuffers(1, &sphereVBO);
	// Create EBOs
	//unsigned int sphereEBO; //moved to global variable
	glGenBuffers(1, &sphereEBO);

	// Create VAOs
	//unsigned int lampVAO, skyboxVAO, screenVAO, sphereVAO;	// moved to global variables
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
	// set the vertex attributes (only position data for skybox, as the position data is used for the texel as well)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//screen quad VAO
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	// Copy vertex array data into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
	// set the vertex attributes (position and texel data for the screen quad)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//sphere (for light volumes) VAO
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);
	// Copy vertex array data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
	// Copy index array data into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
	// set the vertex attributes (only (x,y,z) position data for spheres)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind VAO
	glBindVertexArray(0);

}

void initializeGBuffer()
{
	//Framebuffer setup
	//unsigned int gBuffer;	// made into global variable
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	//unsigned int gPosition, gNormal, gColorSpec;	// made into global variables

	// - position color buffer (16 or 32 bit float per component accuracy)
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - normal color buffer (16 or 32 bit float per component accuracy)
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - color + specular color buffer (8 bit byte per component accuracy)
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// generate render buffer for depth and stencil tests
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// attach it to currently bound framebuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// perform framebuffer check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initializeSkybox()
{
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

	for (int i = 0; i < faces.size(); i++)
		faces[i] = "Resources/CubeMaps/skybox/" + faces[i];

	cubemapTexture = Loader::loadCubemap(faces);
}

void initializeTextRenderer()
{
	textRenderer.initialize("font1.png");
}

void initialize()
{
	initializeShaders();
	
	nanosuit.initialize("Resources/Models/Chiya/Chiya.fbx");

	initializeVAOs();
	initializeGBuffer();
	initializeSkybox();
	initializeTextRenderer();
}

void sendConstantUniforms()
{
	// light volume shader ------------------------------------------------------------------------------------------

	// don't forget to 'use' the corresponding shader program first (to set the uniforms)
	lightVolumeShader.use();

	//set light attributes
	// Directional Light
	//lightVolumeShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	//lightVolumeShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
	//lightVolumeShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
	lightVolumeShader.setVec3("dirLight.Color", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene

	//Point Lights
	//light coverage distance at 13 (pixels ?)
	float constant = 1.0f;
	float linear = 0.35f;
	float quadratic = 0.44f;
	//float lightMax = fmaxf(fmaxf(lightColor.r, lightColor.g), lightColor.b);
	float lightMax = 1.0f;
	float threshold = 0.2f; //should be 0.25f ? (calculated as 1 / (numShades + 1) )
	lightVolumeRadius = (-linear + sqrt(linear * linear - 4 * quadratic * (constant - lightMax / threshold))) / (2 * quadratic);

	cout << "Light Volume Radius: " << lightVolumeRadius << endl;

	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		//lightVolumeShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.2f, 0.2f, 0.2f);
		lightVolumeShader.setVec3("pointLights[" + to_string(i) + "].Color", pointLightColors[3 * i], pointLightColors[3 * i + 1], pointLightColors[3 * i + 2]); // darken the light a bit to fit the scene
		//lightVolumeShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);

		//lightVolumeShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f); // moved to a constant value in the fragment shader
		lightVolumeShader.setFloat("pointLights[" + to_string(i) + "].Linear", linear);
		lightVolumeShader.setFloat("pointLights[" + to_string(i) + "].Quadratic", quadratic);
	}

	/*
	//Spot Light
	lightVolumeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightVolumeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	lightVolumeShader.setFloat("spotLight.constant", 1.0f);
	lightVolumeShader.setFloat("spotLight.linear", 0.09f);
	lightVolumeShader.setFloat("spotLight.quadratic", 0.032f);
	lightVolumeShader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
	lightVolumeShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f); // darken the light a bit to fit the scene
	lightVolumeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	*/

	lightVolumeShader.setInt("gPosition", 0);
	lightVolumeShader.setInt("gNormal", 1);
	lightVolumeShader.setInt("gColorSpec", 2);
}

void enableConstantTests()
{
	// enable z-buffer
	glEnable(GL_DEPTH_TEST);

	// enable backface culling (using default CCW winding order)
	glEnable(GL_CULL_FACE);

	// set stencil testing properties
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
}

void setTransformationMatrices()
{
	//matrices for transformations
		//view matrix
	view = camera.getViewMatrix();
		//perspective projection matrix
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
}

void geometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glClearColor(-1.0f, -1.0f, -1.0f, 1.0f);	//the clear color for the G Buffer must be an invalid normal color
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//enable & disable tests
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_STENCIL_TEST);

	//draw scene
	geometryShader.use();

	//model matrix
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(0.1f));
	//model = glm::translate(model, cubePositions[i]);
	//model = glm::rotate(model, i * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//set transformatix matrices uniforms
	glm::mat4 mv = view * model;
	glm::mat4 mvp = projection * mv;
	geometryShader.setMat4("mvp", mvp);
	geometryShader.setMat4("mv", mv);
	geometryShader.setMat3("mvNormal", transpose(inverse(glm::mat3(mv))));

	//draw model
	nanosuit.Draw(geometryShader);
}

void stencilPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilMask(0xFF);

	// "render" light volumes (spheres)
	stencilShader.use();

	glBindVertexArray(sphereVAO);
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		//set mvp matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(lightVolumeRadius));

		stencilShader.setMat4("mvp", projection * view * model);
		
		//Draw call
		glDrawElements(GL_TRIANGLES, numSpherePoints, GL_UNSIGNED_INT, 0); //use EBO
		//glDrawArrays(GL_TRIANGLES, 0, 36);	//use VBO
	}

	glEnable(GL_CULL_FACE);
}

void lightVolumePass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	lightVolumeShader.use();

	//set light position uniforms (they change based on the camera's view matrix)
	//Directional Light
	lightVolumeShader.setVec3("dirLight.Direction", glm::vec3(view * glm::vec4(1.0f, -1.0f, -1.0f, 0.0f)));
	//Point Lights
	pointLightPositions[0] = glm::vec3(cos(glfwGetTime()), 2.0f, sin(glfwGetTime()));
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
		lightVolumeShader.setVec3("pointLights[" + to_string(i) + "].Position", glm::vec3(view * glm::vec4(pointLightPositions[i], 1.0f)));

	//send gBuffer textures

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);

	lightVolumeShader.use();

	//render directional and ambient light
	glBindVertexArray(screenVAO);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
	lightVolumeShader.setMat4("mvp", glm::mat4(1.0f));
	lightVolumeShader.setInt("lightID", -1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDepthFunc(GL_LESS);

	//render light volume spheres
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_ONE);
	//glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ONE, GL_ZERO);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glStencilMask(0x00);	// disable writing to stencil buffer
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glBindVertexArray(sphereVAO);
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		//set mvp matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(lightVolumeRadius));

		lightVolumeShader.setMat4("mvp", projection * view * model);
		//lightVolumeShader.setMat4("mv", view * model);

		lightVolumeShader.setInt("lightID", i);

		//Draw call
		glDrawElements(GL_TRIANGLES, numSpherePoints, GL_UNSIGNED_INT, 0); //use EBO
		//glDrawArrays(GL_TRIANGLES, 0, 36);	//use VBO
	}
	glCullFace(GL_BACK);
	glDisable(GL_BLEND);

	//unbind VAO
	glBindVertexArray(0);
}

void drawLamps()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	//glStencilMask(0x00); //Don't update stencil buffer for certain draws
	//draw lamp
	lampShader.use();

	//bind lamp's VAO
	glBindVertexArray(lampVAO);

	//lampShader.setMat4("view", view);
	//lampShader.setMat4("projection", projection);

	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		//set mvp matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));

		lampShader.setMat4("mvp", projection * view * model);

		//set Lamp Color
		lampShader.setVec3("LampColor", pointLightColors[3 * i], pointLightColors[3 * i + 1], pointLightColors[3 * i + 2]);

		//Draw call
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //use EBO
		glDrawArrays(GL_TRIANGLES, 0, 36);	//use VBO
	}
	//unbind VAO
	glBindVertexArray(0);
}

void drawSkybox()
{
	//skybox
	glDisable(GL_STENCIL_TEST); // dont use stencil for skybox
	//glStencilMask(0x00);
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	// ... set view and projection matrix
	skyboxShader.setMat4("vp", projection * glm::mat4(glm::mat3(view)));

	// ... draw call
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(skyboxVAO);
	//glEnable(GL_STENCIL_TEST);
	//glStencilMask(0xFF);
	glDepthFunc(GL_LESS); // set depth function back to default
	
	//unbind VAO
	glBindVertexArray(0);
}

void drawText()
{
	//draw text
	static GLint polygonMode;
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	textRenderer.Draw(("FPS: " + to_string(fps)).c_str());
	//glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	if (polygonMode == GL_LINE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else if(polygonMode == GL_FILL)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//else if (polygonMode == GL_POINTS)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
}

void forwardRenderingPass()
{
	//copy the depth buffer of the gBuffer into the default framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	drawLamps();
	drawSkybox();
	drawText();

	//unbind VAO
	glBindVertexArray(0);
}

void clean()
{
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &lampVBO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteBuffers(1, &screenVBO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereEBO);
	glDeleteTextures(1, &cubemapTexture);
	glDeleteFramebuffers(1, &gBuffer);
	glDeleteTextures(1, &gPosition);
	glDeleteTextures(1, &gNormal);
	glDeleteTextures(1, &gColorSpec);
}

int main()
{
	int test = initializeOpenGL();
	if (test) // equivalent to if initializeOpenGL() != 0
		return test;

	//initialization
	initialize();
	sendConstantUniforms();

	enableConstantTests();
	
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

		setTransformationMatrices();

		//First Pass --------------------------------------------------------------------------------------------
		geometryPass();

		// Second pass ----------------------------------------------------------------------------------------------
		stencilPass();

		// Third pass ----------------------------------------------------------------------------------------------
		lightVolumePass();

		//now do forward rendering --------------------------------------------------------------------------------
		forwardRenderingPass();

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//properly clean/delete all of GLFW's resources that were allocated
	clean();
	
	glfwTerminate();

	return 0;
}

#endif // !MAIN_CPP