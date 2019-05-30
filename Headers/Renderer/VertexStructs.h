//Dipesh Manandhar 5/6/2019

#ifndef VERTEXSTRUCTS_H
#define VERTEXSTRUCTS_H

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Libraries
#include <string>

using namespace std;

namespace Renderer
{
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture {
		unsigned int id;
		string type;
		string path;  // we store the path of the texture to compare with other textures
	};
}

#endif // !MESH_H

