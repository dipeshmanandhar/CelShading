//Dipesh Manandhar 5/29/2019

#ifndef RENDERINGOPTIONS_H
#define RENDERINGOPTIONS_H

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Libraries
#include <vector>

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


#endif // !RENDERINGOPTIONS_H