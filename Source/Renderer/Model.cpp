//Dipesh Manandhar 5/6/2019

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//Created H Files
#include "../../Headers/Renderer/Model.h"

//public:
/* Constructor */

Renderer::Model::Model()
{
	//do nothing
	currentTime = 0.0f;
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



void Renderer::Model::update(float deltaTime)
{
	// first find interpolated transforms
	findInterpolatedTransforms(deltaTime);

	// then update all bone transforms
	updateAllBones(bones[0]); //assumed root node is at index 0 (which is gaurenteed to be true)
}

void Renderer::Model::Draw(const Shader& shader) const
{
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

void Renderer::Model::updateAllBones(Bone& bone, const glm::mat4& modelToParent)
{
	glm::mat4 modelToBone = bone.updateBone(modelToParent);
	//for (Bone* child : bone.children)
	//	updateAllBones(*child, boneToModel);
	for (unsigned int i = 0; i < bone.children.size(); i++)
		updateAllBones(bones[bone.children[i]], modelToBone);
}

void Renderer::Model::setUpHelper(Bone& currBone, const glm::mat4& modelToParentStatic)
{
	currBone.setUp(modelToParentStatic);
	for (unsigned int i = 0; i < currBone.children.size(); i++)
		setUpHelper(bones[currBone.children[i]], currBone.modelToBoneStatic);
}

void Renderer::Model::setUp()
{
	setUpHelper(bones[0]);

	cout << "Number of animations: " << animations.size() << endl;
	cout << "Number of meshes: " << meshes.size() << endl;
	cout << "Number of bones: " << bones.size() << endl;
}

void Renderer::Model::findInterpolatedTransforms(float deltaTime)
{
	unsigned int animationNum = 0;

	//time in ticks
	currentTime = glm::mod(currentTime + (float)(deltaTime * animations[animationNum].ticksPerSecond), (float)animations[animationNum].duration);

	//cout << currentTime << endl;

	for (unsigned int i = 0; i < bones.size(); i++)
	{
		BoneAnim ba = animations[animationNum].boneAnims[i];

		glm::mat4 translation, rotation, scale;

		/*
		if (ba.translations.size() == 0 || ba.rotations.size() == 0 || ba.scales.size() == 0)
		{
			bones[i].interpolatedBoneTransform = glm::mat4(0.0f);
			continue;
		}
		else if (ba.translations.size() == 1 || ba.rotations.size() == 1 || ba.scales.size() == 1)
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), ba.translations[0]);
			glm::mat4 rotation = glm::toMat4(ba.rotations[0]);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), ba.scales[0]);

			bones[i].interpolatedBoneTransform = translation * rotation * scale;
			continue;
		}
		*/

		/*
		for (unsigned int j = 0; j < ba.translations.size(); j++)
		{
			if (currentTime < ba.tTimes[(ba.prevTIndex + j + 1) % ba.tTimes.size()])
			{
				ba.prevTIndex = (ba.prevTIndex + j) % ba.tTimes.size();
				break;
			}
		}
		for (unsigned int j = 0; j < ba.rotations.size(); j++)
		{
			if (currentTime < ba.rTimes[(ba.prevRIndex + j + 1) % ba.rTimes.size()])
			{
				ba.prevRIndex = (ba.prevRIndex + j) % ba.rTimes.size();
				break;
			}
		}
		for (unsigned int j = 0; j < ba.scales.size(); j++)
		{
			if (currentTime < ba.sTimes[(ba.prevSIndex + j + 1) % ba.sTimes.size()])
			{
				ba.prevSIndex = (ba.prevSIndex + j) % ba.sTimes.size();
				break;
			}
		}
		*/

		if (ba.translations.size() == 0)
			translation = glm::mat4(1.0f);
		else if(ba.translations.size() == 1)
			translation = glm::translate(glm::mat4(1.0f), ba.translations[0]);
		else
		{
			for (unsigned int j = 0; j < ba.translations.size() - 1; j++)
			{
				if (currentTime < ba.tTimes[j + 1])
				{
					ba.prevTIndex = j;
					break;
				}
			}
			
			translation = glm::translate(glm::mat4(1.0f), glm::mix(ba.translations[ba.prevTIndex], ba.translations[ba.prevTIndex + 1], (currentTime - ba.tTimes[ba.prevTIndex]) / (ba.tTimes[ba.prevTIndex + 1] - ba.tTimes[ba.prevTIndex])));
		}
		

		if (ba.rotations.size() == 0)
			rotation = glm::mat4(1.0f);
		else if (ba.rotations.size() == 1)
			rotation = glm::toMat4(ba.rotations[0]);
		else
		{
			for (unsigned int j = 0; j < ba.rotations.size() - 1; j++)
			{
				if (currentTime < ba.rTimes[j + 1])
				{
					ba.prevRIndex = j;
					break;
				}
			}
			
			rotation = glm::toMat4(glm::slerp(ba.rotations[ba.prevRIndex], ba.rotations[ba.prevRIndex + 1], (currentTime - ba.rTimes[ba.prevRIndex]) / (ba.rTimes[ba.prevRIndex + 1] - ba.rTimes[ba.prevRIndex])));
		}
		
		if (ba.scales.size() == 0)
			scale = glm::mat4(1.0f);
		else if (ba.scales.size() == 1)
			scale = glm::scale(glm::mat4(1.0f), ba.scales[0]);
		else
		{
			for (unsigned int j = 0; j < ba.scales.size() - 1; j++)
			{
				if (currentTime < ba.sTimes[j + 1])
				{
					ba.prevSIndex = j;
					break;
				}
			}

			scale = glm::scale(glm::mat4(1.0f), glm::mix(ba.scales[ba.prevSIndex], ba.scales[ba.prevSIndex + 1], (currentTime - ba.sTimes[ba.prevSIndex]) / (ba.sTimes[ba.prevSIndex + 1] - ba.sTimes[ba.prevSIndex])));
		}

		//assert(currentTime >= ba.tTimes[ba.prevTIndex] && currentTime < ba.tTimes[ba.prevTIndex + 1]);

		bones[i].interpolatedBoneTransform = translation * rotation * scale;
	}
}
