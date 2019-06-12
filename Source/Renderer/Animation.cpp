//Dipesh Manandhar 6/8/2019

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
#include "../../Headers/Renderer/Animation.h"
#include "../../Headers/Renderer/BoneAnim.h"

using namespace std;

//public:
/*  Data  */


/*  Constructors  */

Renderer::Animation::Animation(double dur, double tps, unsigned int numBones)
{
	duration = dur;
	ticksPerSecond = tps;
	boneAnims = vector<BoneAnim>(numBones);
}

/*  Functions  */
		
//private:
/*  Data  */


		
/*  Functions    */
