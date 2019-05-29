//Dipesh Manandhar 5/6/2019

#ifndef MODEL_H
#define MODEL_H

//External Libraries for OpenGL (GLFW and GLAD)
#include <glad/glad.h> // NOTE: always include GLAD before GLFW
#include <GLFW/glfw3.h>

//C++ Libraries
#include <string>
#include <vector>

//Created H Files
#include "Shader.h"
#include "Mesh.h"

using namespace std;

namespace Renderer
{
	class Model
	{
	public:
		/* Constructor */
		Model();

		/*
		Model(const char* path)
		{
			initialize(path);
		}
		*/

		/*  Functions   */

		//TODO: implement destructor (need to deallocate glTextures)

		/*
		void initialize(const char* path)
		{
			loadModel(path);
		}
		*/

		void Draw(const Shader& shader) const;

	private:
		/*  Model Data  */

		vector<Mesh> meshes;
		string directory;
		vector<Texture> textures_loaded;
		friend class Loader;
	};
}

#endif // !MODEL_H


