//Dipesh Manandhar 5/6/2019

#ifndef MODEL_H
#define MODEL_H

//External Libraries for OpenGL (GLFW and GLAD)
#include <glad/glad.h> // NOTE: always include GLAD before GLFW
#include <GLFW/glfw3.h>

//C++ Libraries
#include <string>
#include <vector>
#include <unordered_map>

//Created H Files
#include "Shader.h"
#include "Mesh.h"
#include "Bone.h"
#include "Animation.h"

using namespace std;

// constants
//#define MAX_BONES 50

namespace Renderer
{
	class Model
	{
	public:

		vector<Bone> bones;

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

		void update(float deltaTime);

		void Draw(const Shader& shader) const;

	private:
		/*  Model Data  */

		vector<Mesh> meshes;
		string directory;
		vector<Texture> textures_loaded;
		unordered_map<string, unsigned int> nameToBoneID;
		vector<Animation> animations;
		
		float currentTime;

		/*  Functions  */

		void addBone(const Bone& bone, const string& name);

		Bone& getBone(const string& name);

		bool containsBone(const string& name) const;

		void updateAllBones(Bone& bone, const glm::mat4& modelToParent = glm::mat4(1.0f));

		void setUpHelper(Bone& currBone, const glm::mat4& modelToParentStatic = glm::mat4(1.0f));

		void setUp();

		void findInterpolatedTransforms(float deltaTime);

		friend class Loader;
	};
}

#endif // !MODEL_H


