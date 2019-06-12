//Dipesh Manandhar 6/5/2019

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
#include <vector>
//#include <string>
#include <iostream>

//Created H Files
#include "../../Headers/Renderer/Bone.h"

using namespace std;
//	public:
/*  Data  */

glm::mat4 Renderer::Bone::globalInverseTransform;

/*  Constructors  */

Renderer::Bone::Bone()
{
	modelToBoneStatic = boneToParentStatic = glm::mat4(1.0f);
}

glm::mat4 Renderer::Bone::updateBone(const glm::mat4& parentToModel)
{
	/*
	//glm::mat4 modelToBone = glm::inverse(interpolatedBoneTransform) * parentToBoneStatic * modelToParent;
	glm::mat4 modelToBone = glm::inverse(interpolatedBoneTransform) * modelToParent;
	
	finalTransform = globalInverseTransform * boneToModelStatic * modelToBone;
	
	return modelToBone;
	*/
	
	glm::mat4 boneToModel = parentToModel * ((interpolatedBoneTransform == glm::mat4(0.0f)) ? boneToParentStatic : interpolatedBoneTransform);
	//glm::mat4 boneToModel = parentToModel * interpolatedBoneTransform;
	//glm::mat4 boneToModel = parentToModel * boneToParentStatic;

	finalTransform = globalInverseTransform * boneToModel * modelToBoneStatic;

	return boneToModel;
}

void Renderer::Bone::setUp(const glm::mat4& modelToParentStatic)
{
	//parentToBoneStatic = glm::inverse(boneToParentStatic);
	boneToParentStatic = modelToParentStatic * glm::inverse(modelToBoneStatic);
}

/*  Functions  */
		

//private:
/*  Data  */

/*  Functions    */

