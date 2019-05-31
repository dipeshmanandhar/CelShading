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

//using namespace std;

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

		void stop();

		void addVelocity(const glm::vec3& v);

		void setVelocity(const glm::vec3& v);

		void setAcceleration(const glm::vec3& a);

		glm::vec3 getVelocity() const;

		glm::vec3 getAcceleration() const;

	protected:
		glm::vec3 velocity, acceleration;

	private:
		/*  Data  */

	};
}

#endif // !GAME_OBJECT_H