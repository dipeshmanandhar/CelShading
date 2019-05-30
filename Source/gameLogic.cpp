// Dipesh Manandhar 5/26/2019

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Libraries
#include <iostream>

// Project Libraries
#include "../Headers/Entity.h"
#include "../Headers/Renderer/Loader.h"
#include "../Headers/Renderer/Camera.h"
#include "../Headers/Renderer/TextRenderer.h"

//#include "../../Headers/Renderer/Renderer.h"

#include "../Headers/RenderingOptions.h"

using namespace std;

Entity player;

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

void setup()
{
	//cout << "Setting Up..." << endl;
	player.setModel(Renderer::Loader::CHIYA);

	player.setPosition(glm::vec3(50.0f, 5.0f, 50.0f));
	
	Renderer::TextRenderer::setTextColor(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.5f, 0.8f));
	
	terrain.setupVertexData("Resources/Terrains/heightmap1.png");
	terrain.setupTextures(vector<string>{"Resources/Terrains/grass.jpg"});
}

void update(float dTime)
{
	pointLightPositions[0] = glm::vec3(50.0f + cos(glfwGetTime()), 7.0f, 50.0f + sin(glfwGetTime()));
}

void render()
{
	player.Draw();

	//always draw terrain last
	terrain.Draw();
}