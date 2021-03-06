//Dipesh Manandhar 5/15/2019

#ifndef LOADER_H
#define LOADER_H

//#ifndef STB_IMAGE_IMPLEMENTATION
//External Library For loading images (STB)
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#endif // !STB_IMAGE_IMPLEMENTATION

//External Libraries for OpenGL (GLFW and GLAD)
#include <glad/glad.h> // NOTE: always include GLAD before GLFW
#include <GLFW/glfw3.h>

//External Library for Model Loading (Assimp)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//C++ Libraries
#include <string>
#include <vector>
#include <unordered_set>

//Created H Files
#include "Model.h"
#include "Mesh.h"

using namespace std;

namespace Renderer
{
	class Loader
	{
	public:
		// DATA ---------------------------------------------------------------------------------------

		// All valid model indecies (except for the length, the last enum)
		enum modelID
		{
			CHIYA,
			STORM_TROOPER,
			NUM_MODELS
		};

		static vector<Model>& models();

		//static vector<Model> models;

		// FUNCTIONS ------------------------------------------------------------------------------------

		static void loadModels();

		static unsigned int TextureFromFile(char const* p, string dir);

		static unsigned int TextureFromFile(string path);

		// method to load cubemaps (for skybox)
		static unsigned int loadCubemap(vector<std::string>& faces);

	private:
		static void loadModel(Model& model, const string& path);

		static void discoverBoneNodes(aiNode* node, const aiScene* scene, unordered_set<string>& necessaryNodes);

		static int processNodeBones(Model& model, aiNode* node, const unordered_set<string>& necessaryNodes, const glm::mat4& modelToParentStatic = glm::mat4(1.0f));

		static void processNodeMeshes(Model& model, aiNode* node, const aiScene* scene, const glm::mat4& modelToParentSpace = glm::mat4(1.0f));

		static Mesh processMesh(Model& model, aiMesh* mesh, const aiScene* scene);

		static void processAnimations(Model& model, const aiScene* scene);

		static vector<Texture> loadMaterialTextures(Model& model, aiMaterial* mat, aiTextureType type, const string& typeName);

		static glm::mat4 assimpToGLM(const aiMatrix4x4& matrix);

		//static aiNode* findNode(aiNode* node, const string& name);

		static void printMatrix(const glm::mat4& matrix);
	};

	//vector<Model> Loader::models = vector<Model>(NUM_MODELS);
}

#endif // !LOADER_H