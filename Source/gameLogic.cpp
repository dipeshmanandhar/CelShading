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

GameLogic::GameObject player;

// Renderer Options
	// lights
glm::vec3 dirLightColor(1.0f);
glm::vec3 dirLightDirection(1.0f, -1.0f, -1.0f);
vector<glm::vec3> pointLightPositions = {
	glm::vec3(51.0f, 7.0f, 50.0f),
	glm::vec3(49.5f, 6.0f, 51.0f)
};
const unsigned int NUM_POINT_LIGHTS = pointLightPositions.size();
vector<float> pointLightColors = {
	//R	    G    B
	0.0f, 0.0f, 100.0f,
	10.0f, 0.0f, 0.0f
};
	// camera
Renderer::Camera camera(glm::vec3(50.0f, 5.0f, 53.0f));
	// sybox
const string skyboxFiletype = "jpg";
const string skyboxFilename = "Resources/CubeMaps/skybox/";
	// Terrain
Renderer::Terrain terrain(100, 10, 40);

// method called only when a key event occurs
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//player
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		player.addVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
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

	//camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(Renderer::FORWARD, dTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Renderer::BACKWARD, dTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Renderer::LEFT, dTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Renderer::RIGHT, dTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.processKeyboard(Renderer::UP, dTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.processKeyboard(Renderer::DOWN, dTime);
}

void setup()
{
	//cout << "Setting Up..." << endl;
	player.setModel(Renderer::Loader::CHIYA);

	player.setPosition(glm::vec3(50.0f, 50.0f, 50.0f));
	
	Renderer::TextRenderer::setTextColor(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.5f, 0.8f));
	
	terrain.setupVertexData("Resources/Terrains/heightmap1.png");
	terrain.setupTextures(vector<string>{"Resources/Terrains/grass.jpg"});
}

void update(float dTime)
{
	player.setAcceleration(glm::vec3(0.0f, -GRAVITY, 0.0f)); //apply gravity
	
	pointLightPositions[0] = glm::vec3(50.0f + cos(glfwGetTime()), 7.0f, 50.0f + sin(glfwGetTime()));
	
	player.update(dTime);

	//cout << player.getAcceleration().x << ", " << player.getAcceleration().y << ", " << player.getAcceleration().z << endl;
	//cout << player.getVelocity().x << ", " << player.getVelocity().y << ", " << player.getVelocity().z << endl << endl;

	float height = terrain.getHeightAt(player.getPosition());

	if (height >= player.getPosition().y)
	{
		player.stop();
		player.setPosition(glm::vec3(player.getPosition().x, height, player.getPosition().z));
	}
}

void render()
{
	player.Draw();

	//always draw terrain last
	terrain.Draw();
}