//Dipesh Manandhar 5/6/2019

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
//#include <string>
//#include <vector>

//Created H Files
#include "../Entity.h"
#include "../Renderer/Loader.h"
#include "../Renderer/Camera.h"

//using namespace std;

#define GAME_SPEED 1.0f

namespace GameLogic
{
	class GameObject : public Entity
	{
	public:
		/* Constructor */

		GameObject(const glm::vec3& pos = glm::vec3(0.0f));

		GameObject(Renderer::Loader::modelID id, const glm::vec3& pos = glm::vec3(0.0f));

		/*  Functions   */

		void update(float dTime);

		void jump();

		void land();

		void addVelocity(const glm::vec3& v);

		void setVelocity(const glm::vec3& v);

		void setAcceleration(const glm::vec3& a);

		glm::vec3 getVelocity() const;

		glm::vec3 getAcceleration() const;
		
		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void processKeyboard(Renderer::Camera_Movement direction);

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		//void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

		void setYaw(float y);

	protected:
		/*  Data  */
		
		glm::vec3 velocity, acceleration;
		float movementSpeed;

		glm::vec3 front, right, up;
		const glm::vec3 WORLD_UP;

	private:
		/*  Data  */

		bool inAir;

		/*  Functions  */

		// Calculates the front vector from the (updated) Euler Angles
		void updateDirectionVectors();
	};
}

#endif // !GAME_OBJECT_H