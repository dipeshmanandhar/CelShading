//Dipesh Manandhar 5/6/2019


//External Library for Matrix Math (GLM)
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
//#include <string>
//#include <vector>

//Created H Files
#include "..\..\Headers\GameLogic\GameObject.h"
#include "..\..\Headers\Renderer\Loader.h"

//using namespace std;

//public:
/* Constructor */

GameLogic::GameObject::GameObject(const glm::vec3& pos) :
	Entity()
{
	position = pos;
	acceleration = velocity = glm::vec3(0.0f);
}

GameLogic::GameObject::GameObject(Renderer::Loader::modelID id, const glm::vec3& pos) :
	Entity(id)
{
	position = pos;
}
	
/*  Functions   */

void GameLogic::GameObject::update(float dTime)
{
	velocity += acceleration * dTime;
	position += velocity * dTime;
}

void GameLogic::GameObject::stop()
{
	velocity = glm::vec3(0.0f);
	acceleration = glm::vec3(0.0f);
}

void GameLogic::GameObject::addVelocity(const glm::vec3& v)
{
	velocity += v;
}

void GameLogic::GameObject::setVelocity(const glm::vec3& v)
{
	velocity = v;
}

void GameLogic::GameObject::setAcceleration(const glm::vec3& a)
{
	acceleration = a;
}

glm::vec3 GameLogic::GameObject::getVelocity() const
{
	return velocity;
}

glm::vec3 GameLogic::GameObject::getAcceleration() const
{
	return acceleration;
}

//protected:
	

//private:
/*  Data  */

