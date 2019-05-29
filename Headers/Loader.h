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

//Created H Files
#include "Model.h"

using namespace std;

class Loader
{
public:
	// DATA ---------------------------------------------------------------------------------------
	
	// All valid model indecies (except for the length, the last enum)
	enum modelID
	{
		CHIYA,
		NUM_MODELS
	};

	static vector<Model>& models()
	{
		static vector<Model> models(NUM_MODELS);
		return models;
	}

	//static vector<Model> models;
	
	// FUNCTIONS ------------------------------------------------------------------------------------

	static void loadModels()
	{
		//models = vector<Model>(NUM_MODELS, Model());
		loadModel(models()[CHIYA], "Resources/Models/Chiya/Test.fbx");
	}

	static unsigned int TextureFromFile(char const* p, string dir)
	{
		string path = dir + "/" + p;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format1, format2;
			if (nrComponents == 1)
			{
				format1 = GL_RED;
				format2 = GL_RED;
			}
			else if (nrComponents == 3)
			{
				format1 = GL_SRGB;
				format2 = GL_RGB;
			}
			else if (nrComponents == 4)
			{
				format1 = GL_SRGB_ALPHA;
				format2 = GL_RGBA;
			}
			else
				cout << "ERROR: nrComponents not specified" << endl;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format1, width, height, 0, format2, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << string(path) << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
	
	// method to load cubemaps (for skybox)
	static unsigned int loadCubemap(vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}

private:
	static void loadModel(Model& model, const string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			return;
		}
		model.directory = path.substr(0, path.find_last_of('/'));

		processNode(model, scene->mRootNode, scene);
	}

	static void processNode(Model& model, aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			model.meshes.push_back(processMesh(model, mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(model, node->mChildren[i], scene);
		}
	}

	static Mesh processMesh(Model& model, aiMesh* mesh, const aiScene* scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			// positions
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			//normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			//texels
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture> diffuseMaps = loadMaterialTextures(model, material,
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<Texture> specularMaps = loadMaterialTextures(model, material,
				aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	static vector<Texture> loadMaterialTextures(Model& model, aiMaterial * mat, aiTextureType type, const string & typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < model.textures_loaded.size(); j++)
			{
				if (std::strcmp(model.textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(model.textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.id = Loader::TextureFromFile(str.C_Str(), model.directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				model.textures_loaded.push_back(texture); // add to loaded textures
			}
		}
		return textures;
	}
};

//vector<Model> Loader::models = vector<Model>(NUM_MODELS);

#endif // !LOADER_H