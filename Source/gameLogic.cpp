// Dipesh Manandhar 5/26/2019

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Libraries
#include <iostream>

// Project Libraries
#include "../Headers/GameLogic/GameObject.h"
#include "../Headers/Renderer/Loader.h"
#include "../Headers/Renderer/Camera.h"
#include "../Headers/Renderer/TextRenderer.h"

//#include "../../Headers/Renderer/Renderer.h"

#include "../Headers/RenderingOptions.h"

using namespace std;

//constants
#define GRAVITY 9.8f
#define TOLERANCE 0.001f

GameLogic::GameObject player;

// Renderer Options
	// lights
glm::vec3 dirLightColor(1.0f);
glm::vec3 dirLightDirection(1.0f, -1.0f, -1.0f);
vector<glm::vec3> pointLightPositions = {
	glm::vec3(51.0f, 18.0f, 50.0f),
	glm::vec3(49.5f, 17.0f, 51.0f)
};
const unsigned int NUM_POINT_LIGHTS = pointLightPositions.size();
vector<float> pointLightColors = {
	//R	    G    B
	0.0f, 0.0f, 100.0f,
	10.0f, 0.0f, 0.0f
};
	// camera
Renderer::Camera camera;
	// sybox
const string skyboxFiletype = "jpg";
const string skyboxFilename = "Resources/CubeMaps/skybox/";
glm::vec3 skyColor(1.0f, 0.3f, 0.3f);
	// Terrain
Renderer::Terrain terrain(1000, 50, 400);



// method called only when a key event occurs
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	//player
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		if (terrain.getHeightAt(player.getPosition()) >= player.getPosition().y - TOLERANCE) //if player is on the ground
			player.processKeyboard(Renderer::UP);
	*/
	/*
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		player.processKeyboard(Renderer::FORWARD);
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		player.processKeyboard(Renderer::RELEASE_FORWARD);
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		player.processKeyboard(Renderer::BACKWARD);
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		player.processKeyboard(Renderer::RELEASE_BACKWARD);
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		player.processKeyboard(Renderer::RIGHT);
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		player.processKeyboard(Renderer::RELEASE_RIGHT);
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		player.processKeyboard(Renderer::LEFT);
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		player.processKeyboard(Renderer::RELEASE_LEFT);
	*/
}

// method called every frame to check input keys
void processInput(GLFWwindow* window, float dTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);

	//player
	bool moved = false;
	const static float ERROR_YAW = -900.0f;
	float yawOffset = ERROR_YAW;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		yawOffset = 0.0f;
		player.processKeyboard(Renderer::FORWARD);
		moved = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		yawOffset = - 180.0f;
		player.processKeyboard(Renderer::BACKWARD);
		moved = true;
	}
	//else //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
	//	player.processKeyboard(Renderer::RELEASE_BACKWARD);
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		yawOffset = (yawOffset == ERROR_YAW) ? (- 90.0f) : ((yawOffset - 90.0f) / 2.0f);
		player.processKeyboard(Renderer::LEFT);
		//cout << "LEFT Pressed" << endl;
		moved = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		yawOffset = (yawOffset == ERROR_YAW) ? (90.0f) : ((-yawOffset + 90.0f) / 2.0f); // the -yawOffset simply makes -180 degrees to +180 degrees, and leaves 0 degrees unaffected
		player.processKeyboard(Renderer::RIGHT);
		//cout << "RIGHT Pressed" << endl;
		moved = true;
	}
	else if (!moved)
		player.processKeyboard(Renderer::RELEASE_RIGHT);

	if (moved)
		player.setYaw(camera.getYaw() + yawOffset);

	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		if (terrain.getHeightAt(player.getPosition()) >= player.getPosition().y - TOLERANCE) //if player is on the ground
			player.processKeyboard(Renderer::UP);
}

// method called only on mouse events
void processMouseMovement(float xOffset, float yOffset)
{
	camera.processMouseMovement(xOffset, yOffset);
}

void setup()
{
	//cout << "Setting Up..." << endl;
	player.setModel(Renderer::Loader::CHIYA);

	player.setPosition(glm::vec3(50.0f,50.0f, 50.0f));
	
	Renderer::TextRenderer::setTextColor(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.5f, 0.8f));

	terrain.setupVertexData("Resources/Terrains/heightmap1.png");
	terrain.setupTextures(vector<string>{"Resources/Terrains/grass.jpg"});
}

void update(float dTime)
{
	pointLightPositions[0] = glm::vec3(50.0f + cos(glfwGetTime()), 18.0f, 50.0f + sin(glfwGetTime()));

	float height = terrain.getHeightAt(player.getPosition());

	if (height >= player.getPosition().y)
	{
		if(height > player.getPosition().y)
			player.setVelocity(glm::vec3(player.getVelocity().x, 0.0f, player.getVelocity().z));
		
		player.setAcceleration(glm::vec3(player.getAcceleration().x, 0.0f, player.getAcceleration().z));
		player.setPosition(glm::vec3(player.getPosition().x, height, player.getPosition().z));
	}
	else
		player.setAcceleration(glm::vec3(0.0f, -GRAVITY, 0.0f)); //apply gravity

	player.update(dTime);

	camera.follow(player.getPosition());

	//cout << player.getAcceleration().x << ", " << player.getAcceleration().y << ", " << player.getAcceleration().z << endl;
	//cout << player.getVelocity().x << ", " << player.getVelocity().y << ", " << player.getVelocity().z << endl << endl;
	//cout << player.getPosition().x << ", " << player.getPosition().y << ", " << player.getPosition().z << endl << endl;
}

void render()
{
	player.Draw();

	//always draw terrain last
	terrain.Draw();
}