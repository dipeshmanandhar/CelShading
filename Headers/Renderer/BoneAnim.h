//Dipesh Manandhar 6/8/2019

#ifndef BONEANIM_H
#define BONEANIM_H

//External Libraries for OpenGL (GLFW and GLAD)
//#include <glad/glad.h> // NOTE: always include GLAD before GLFW
//#include <GLFW/glfw3.h>

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//C++ Libraries
#include <vector>
//#include <string>

//Created H Files
//#include "Shader.h"
//#include "VertexStructs.h"

using namespace std;

namespace Renderer
{
	class BoneAnim {
	public:
		/*  Data  */


		/*  Constructors  */

		BoneAnim();
		
		BoneAnim(unsigned int numKeyframes);

		/*  Functions  */
		
	private:
		/*  Data  */

		vector<glm::vec3> translations, scales;
		vector<glm::quat> rotations;
		vector<float> tTimes, rTimes, sTimes;

		unsigned int prevTIndex, prevRIndex, prevSIndex;
		
		/*  Functions    */
		

		friend class Loader;
		friend class Model;
	};
}

#endif // !BONEANIM_H

