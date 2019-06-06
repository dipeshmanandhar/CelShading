//Dipesh Manandhar 5/15/2019

//C++ libraries
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

//Created H Files
#include "../../Headers/Renderer/Loader.h"

//public:
// DATA ---------------------------------------------------------------------------------------
	
vector<Renderer::Model>& Renderer::Loader::models()
{
	static vector<Model> models(Loader::NUM_MODELS);
	return models;
}

//static vector<Model> models;
	
// FUNCTIONS ------------------------------------------------------------------------------------

void Renderer::Loader::loadModels()
{
	//models = vector<Model>(NUM_MODELS, Model());
	loadModel(models()[CHIYA], "Resources/Models/Chiya/Test.fbx");
	loadModel(models()[STORM_TROOPER], "Resources/Models/dancing-stormtrooper/Test.fbx");
}

unsigned int Renderer::Loader::TextureFromFile(char const* p, string dir)
{
	string path = dir + "/" + p;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format1 = GL_RED, format2 = GL_RED;
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

unsigned int Renderer::Loader::TextureFromFile(string path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format1 = GL_RED, format2 = GL_RED;
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
unsigned int Renderer::Loader::loadCubemap(vector<std::string>& faces)
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

//private:
void Renderer::Loader::loadModel(Model& model, const string& path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	model.directory = path.substr(0, path.find_last_of('/'));

	unordered_set<string> necessaryNodes;

	Bone::globalInverseTransform = glm::inverse(assimpToGLM(scene->mRootNode->mTransformation));

	discoverBoneNodes(scene->mRootNode, scene, necessaryNodes);

	processNodeBones(model, scene->mRootNode, necessaryNodes);

	processNodeMeshes(model, scene->mRootNode, scene);

	model.setUp();
}

void Renderer::Loader::discoverBoneNodes(aiNode* node, const aiScene* scene, unordered_set<string>& necessaryNodes)
{
	// process all the node's meshes for bones (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		aiBone** bones = mesh->mBones;
		for (unsigned int j = 0; j < mesh->mNumBones; j++)
		{
			aiBone* bone = bones[j];
			string name = bone->mName.C_Str();

			//necessaryNodes.emplace(name);
			
			// also add all parents to set (go up tree until find root node or find another node already found as necessary)
			aiNode* discoveredNode = findNode(scene->mRootNode, name);
			while (discoveredNode != NULL && necessaryNodes.count(discoveredNode->mName.C_Str()) == 0)
			{
				necessaryNodes.emplace(discoveredNode->mName.C_Str());
				discoveredNode = discoveredNode->mParent;
			}
		}
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		discoverBoneNodes(node->mChildren[i], scene, necessaryNodes);
	}
}

int Renderer::Loader::processNodeBones(Model& model, aiNode* node, const unordered_set<string>& necessaryNodes)
{
	string name = node->mName.C_Str();
	
	if (necessaryNodes.count(name) != 0)
	{
		// process the node's bone
		glm::mat4 nodeTransform = assimpToGLM(node->mTransformation);

		Bone bone;
		bone.boneToParentStatic = assimpToGLM(node->mTransformation);
		unsigned int boneID = model.bones.size();
		model.addBone(bone, name);

		//process children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			int childID = processNodeBones(model, node->mChildren[i], necessaryNodes);
			if (childID >= 0)
				model.bones[boneID].children.push_back((unsigned int)childID);
		}
		return boneID;
	}
	return -1;
}

void Renderer::Loader::processNodeMeshes(Model& model, aiNode* node, const aiScene* scene, const glm::mat4& modelToParentSpace)
{
	glm::mat4 modelToNodeSpace = glm::inverse(assimpToGLM(node->mTransformation)) * modelToParentSpace;
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh myMesh = processMesh(model, mesh, scene);
		aiBone** bones = mesh->mBones;
		for (unsigned int j = 0; j < mesh->mNumBones; j++)
		{
			aiBone* bone = bones[j];
			string name = bone->mName.C_Str();
			unsigned int boneId = model.nameToBoneID[name];

			model.bones[boneId].modelToBoneStatic = assimpToGLM(bone->mOffsetMatrix) * modelToNodeSpace;

			for (unsigned int k = 0; k < bone->mNumWeights; k++)
			{
				aiVertexWeight vw = bone->mWeights[k];
				Vertex v = myMesh.vertices[vw.mVertexId];
				for (unsigned int l = 0; l < 4; l++)
				{
					if (v.BoneWeights[l] == 0.0f)
					{
						v.BoneWeights[l] = vw.mWeight;
						v.BoneIDs[l] = boneId;
						break;
					}
				}
			}
			
			//myMesh.boneIDs.push_back(boneId);
		}
		model.meshes.push_back(myMesh);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNodeMeshes(model, node->mChildren[i], scene, modelToNodeSpace);
	}
}

Renderer::Mesh Renderer::Loader::processMesh(Model& model, aiMesh* mesh, const aiScene* scene)
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
		//bone indices
		vertex.BoneIDs = glm::uvec4(0);
		//bone weights
		vertex.BoneWeights = glm::vec4(0.0f);

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

vector<Renderer::Texture> Renderer::Loader::loadMaterialTextures(Model& model, aiMaterial* mat, aiTextureType type, const string & typeName)
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

glm::mat4 Renderer::Loader::assimpToGLM(const aiMatrix4x4& matrix)
{
	glm::mat4 toRet;

	for (unsigned int i = 0; i < 16; i++)
		toRet[i % 4][i / 4] = matrix[i / 4][i % 4];

	return toRet;
}

aiNode* Renderer::Loader::findNode(aiNode* node, const string& name)
{
	if (node->mName.C_Str() == name)
		return node;
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		aiNode* childSearchResult = findNode(node->mChildren[i], name);
		if (childSearchResult)
			return childSearchResult;
	}
	return NULL;
}