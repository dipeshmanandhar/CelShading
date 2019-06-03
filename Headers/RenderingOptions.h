//Dipesh Manandhar 5/29/2019

#ifndef RENDERINGOPTIONS_H
#define RENDERINGOPTIONS_H

// External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Libraries
#include <vector>

// Project Files
#include "Renderer/Camera.h"
#include "Renderer/Terrain.h"

using namespace std;

// Rendering Variables
	
	// directional light
extern "C" glm::vec3 dirLightColor;
extern "C" glm::vec3 dirLightDirection;

	// point lights
extern "C" vector<glm::vec3> pointLightPositions;
extern "C" const unsigned int NUM_POINT_LIGHTS;
extern "C" vector<float> pointLightColors;

	// camera
extern "C" Renderer::Camera camera;
	
	// Skybox
extern "C" const string skyboxFiletype;
extern "C" const string skyboxFilename;
extern "C" glm::vec3 skyColor;

	// Terrain
extern "C" Renderer::Terrain terrain;

	// Keyboard Input
// method called every frame to check input keys
extern "C" void processInput(GLFWwindow* window, float dTime);
//method called only on key events
extern "C" void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//Mouse input
//method called only on mouse events
extern "C" void processMouseMovement(float xOffset, float yOffset);

#endif // !RENDERINGOPTIONS_H