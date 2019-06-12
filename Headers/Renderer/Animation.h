//Dipesh Manandhar 6/8/2019

#ifndef ANIMATION_H
#define ANIMATION_H

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
#include "BoneAnim.h"

using namespace std;

namespace Renderer
{
	class Animation {
	public:
		/*  Data  */


		/*  Constructors  */

		Animation(double dur, double tps, unsigned int numBones);

		/*  Functions  */
		
	private:
		/*  Data  */

		double duration, ticksPerSecond;
		vector<BoneAnim> boneAnims;

		
		/*  Functions    */
		

		friend class Loader;
		friend class Model;
	};
}

#endif // !ANIMATION_H

