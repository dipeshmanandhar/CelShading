//Dipesh Manandhar 6/8/2019

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
#include "../../Headers/Renderer/BoneAnim.h"

using namespace std;

//public:
/*  Data  */


/*  Constructors  */

Renderer::BoneAnim::BoneAnim()
{
	prevTIndex = prevRIndex = prevSIndex = 0;
}

Renderer::BoneAnim::BoneAnim(unsigned int numKeyframes) :
	BoneAnim()
{
	translations = vector<glm::vec3>(numKeyframes, glm::vec3(0.0f));
	scales = vector<glm::vec3>(numKeyframes, glm::vec3(1.0f));
	rotations = vector<glm::quat>(numKeyframes, glm::quat(glm::vec3(0.0f)));
	tTimes = rTimes = sTimes = vector<float>(numKeyframes);
}

/*  Functions  */
		
//private:
/*  Data  */


/*  Functions    */
