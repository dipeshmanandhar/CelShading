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
	glm::vec3(1.0f, 2.0f, 0.0f),
	glm::vec3(-0.5f, 1.0f, 1.0f)
};
const unsigned int NUM_POINT_LIGHTS = pointLightPositions.size();
vector<float> pointLightColors = {
	//R	    G    B
	0.0f, 0.0f, 100.0f,
	10.0f, 0.0f, 0.0f
};
	// camera
Renderer::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	// sybox
const string skyboxFiletype = "jpg";
const string skyboxFilename = "Resources/CubeMaps/skybox/";

void setup()
{
	//cout << "Setting Up..." << endl;
	player.setModel(Renderer::Loader::CHIYA);
	
	Renderer::TextRenderer::setTextColor(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.5f, 0.8f));
}

void update(float dTime)
{

}

void render()
{
	player.Draw();
}