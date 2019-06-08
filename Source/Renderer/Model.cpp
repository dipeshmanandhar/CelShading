//Dipesh Manandhar 5/6/2019

//Created H Files
#include "../../Headers/Renderer/Model.h"

//public:
/* Constructor */

Renderer::Model::Model()
{
	//do nothing
}

/*
Renderer::Model::Model(const char* path)
{
	initialize(path);
}
*/

/*  Functions   */

//TODO: implement destructor (need to deallocate glTextures)

/*
void Renderer::Model::initialize(const char* path)
{
	loadModel(path);
}
*/

void Renderer::Model::Draw(const Shader& shader)
{
	// First update all bone transforms
	updateAllBones(bones[0]); //assumed root node is at index 0 (which is gaurenteed to be true)

	// Send bone Transforms as uniform data
	for (unsigned int i = 0; i < bones.size(); i++)
	{
		shader.setMat4("bones[" + to_string(i) + "]", bones[i].finalTransform);
		//cout << bones[i].finalTransform[3][3] << endl;
	}
	// Then draw all meshes
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Renderer::Model::addBone(const Bone& bone, const string& name)
{
	if (nameToBoneID.count(name) == 0)
	{
		nameToBoneID[name] = bones.size();
		bones.push_back(bone);
	}
}

Renderer::Bone& Renderer::Model::getBone(const string& name)
{
	return bones[nameToBoneID[name]];
}

bool Renderer::Model::containsBone(const string& name) const
{
	return nameToBoneID.count(name) != 0;
}

void Renderer::Model::updateAllBones(Bone& bone, const glm::mat4& parentsTransform)
{
	glm::mat4 boneToModel = bone.updateBone(parentsTransform);
	//for (Bone* child : bone.children)
	//	updateAllBones(*child, boneToModel);
	for (unsigned int i = 0; i < bone.children.size(); i++)
		updateAllBones(bones[bone.children[i]], boneToModel);
}

void Renderer::Model::setUp()
{
	for (Bone bone : bones)
		bone.setUp();
	//cout << "Number of bones: " << bones.size() << endl;
}
