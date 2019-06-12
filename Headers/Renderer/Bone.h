//Dipesh Manandhar 6/4/2019

#ifndef BONE_H
#define BONE_H

//External Libraries for OpenGL (GLFW and GLAD)
//#include <glad/glad.h> // NOTE: always include GLAD before GLFW
//#include <GLFW/glfw3.h>

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
#include <vector>
//#include <string>

//Created H Files
//#include "Shader.h"
//#include "VertexStructs.h"

using namespace std;

namespace Renderer
{
	class Bone {
	public:
		static glm::mat4 globalInverseTransform;
		vector<unsigned int> children;

		/*  Data  */


		/*  Constructors  */

		Bone();

		/*  Functions  */
		glm::mat4 updateBone(const glm::mat4& parentToModel);

		void setUp(const glm::mat4& modelToParentStatic);

	private:
		/*  Data  */

		glm::mat4 modelToBoneStatic;	// converts model space to this bone space in static pose (i.e., no animation)
		//glm::mat4 boneToModelStatic;	// converts this bone space to model space in static pose (i.e., no animation)
		
		glm::mat4 boneToParentStatic;		// transformation matrix converting this bone space to parent bone space
		//glm::mat4 parentToBoneStatic;		// transformation matrix converting parent bone space to this bone space

		glm::mat4 interpolatedBoneTransform;	// interpolated bone to parent transform

		glm::mat4 finalTransform;

		/*  Functions    */
		

		friend class Loader;
		friend class Model;
	};
}

#endif // !BONE_H

