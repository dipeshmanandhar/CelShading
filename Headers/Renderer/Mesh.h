//Dipesh Manandhar 5/6/2019

#ifndef MESH_H
#define MESH_H

//External Libraries for OpenGL (GLFW and GLAD)
#include <glad/glad.h> // NOTE: always include GLAD before GLFW
#include <GLFW/glfw3.h>

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
	class Mesh {
	public:
		/*  Mesh Data  */
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		/*  Functions  */
		Mesh(const vector<Vertex>& v, const vector<unsigned int>& i, const vector<Texture>& t);

		void Draw(const Shader& shader) const;
	private:
		/*  Render data  */
		unsigned int VAO, VBO, EBO;
		/*  Functions    */
		void setupMesh();
	};
}

#endif // !MESH_H

