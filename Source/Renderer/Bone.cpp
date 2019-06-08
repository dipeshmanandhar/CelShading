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
	boneToModelStatic = parentToBoneStatic = glm::mat4(1.0f);
}

glm::mat4 Renderer::Bone::updateBone(const glm::mat4& modelToParent)
{
	glm::mat4 modelToBone = parentToBoneStatic * modelToParent;

	finalTransform = globalInverseTransform * boneToModelStatic * modelToBone;
	//finalTransform = boneToModel * modelToBoneStatic;
	//finalTransform = glm::mat4(1.0f);
	return modelToBone;
}

void Renderer::Bone::setUp()
{
	//parentToBoneStatic = glm::inverse(boneToParentStatic);
}

/*  Functions  */
		

//private:
/*  Data  */

/*  Functions    */

