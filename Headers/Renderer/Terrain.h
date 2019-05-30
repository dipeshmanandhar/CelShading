//Dipesh Manandhar 5/29/2019

#ifndef TERRAIN_H
#define TERRAIN_H

//External Libraries for OpenGL (GLFW and GLAD)
//#include <glad/glad.h> // NOTE: always include GLAD before GLFW
//#include <GLFW/glfw3.h>

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
#include <vector>
#include <string>

//Created H Files
#include "Shader.h"
#include "VertexStructs.h"

using namespace std;

namespace Renderer
{
	class Terrain {
	public:
		/*  Constructors  */

		Terrain(float terScale = 0.0f, float terHeight = 0.0f, float terImageTiles = 0.0f);

		Terrain(string heightmapFile, vector<string> textureFiles, float terScale, float terHeight, float terImageTiles);

		/*  Functions  */
		
		static void initialize(Shader& s, glm::mat4& v, glm::mat4& p);

		void setupVertexData(string heightmapFile);

		void setupTextures(vector<string> textureFiles);

		void Draw() const;
	
	private:
		/*  Data  */
		unsigned int VAO, VBO, EBO;
		vector<unsigned int> indices;
		vector<Vertex> vertices;
		vector<Texture> textures; //TODO: make into a vector to support multiple texture files (i.e., diffuse and specular)
		const float TERRAIN_SCALE, TERRAIN_HEIGHT, TERRAIN_IMAGE_TILES;

		static glm::mat4 *view, *projection;
		static Shader* shader;

		/*  Functions    */
	};
}

#endif // !TERRAIN_H

