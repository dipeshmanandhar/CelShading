//Dipesh Manandhar 5/6/2019


//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
//#include <string>
//#include <vector>

//Created H Files
#include "..\..\Headers\GameLogic\GameObject.h"
#include "..\..\Headers\Renderer\Loader.h"
#include "..\..\Headers\Renderer\Camera.h"

//using namespace std;

//public:
/* Constructor */

GameLogic::GameObject::GameObject(const glm::vec3& pos) :
	WORLD_UP(glm::vec3(0.0f, 1.0f, 0.0f)), Entity()
{
	position = pos;
	acceleration = velocity = glm::vec3(0.0f);
	movementSpeed = Renderer::SPEED;
	//yaw = Renderer::YAW;
	//pitch = Renderer::PITCH;
	updateDirectionVectors();
	inAir = false;
}

GameLogic::GameObject::GameObject(Renderer::Loader::modelID id, const glm::vec3& pos) :
	WORLD_UP(glm::vec3(0.0f, 1.0f, 0.0f)), Entity(id)
{
	position = pos;
	acceleration = velocity = glm::vec3(0.0f);
	movementSpeed = Renderer::SPEED;
	//yaw = Renderer::YAW;
	//pitch = Renderer::PITCH;
	updateDirectionVectors();
	inAir = false;
}
	
/*  Functions   */

void GameLogic::GameObject::update(float dTime)
{
	Entity::update(dTime);
	velocity += acceleration * dTime * GAME_SPEED;
	position += (velocity.x * right + velocity.y * up + velocity.z * front) * dTime * GAME_SPEED;
}

void GameLogic::GameObject::jump()
{
	if (!inAir)
	{
		velocity.y = movementSpeed;
		inAir = true;
	}
}

void GameLogic::GameObject::land()
{
	velocity.y = 0.0f;
	acceleration.y = 0.0f;
	inAir = false;
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

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void GameLogic::GameObject::processKeyboard(Renderer::Camera_Movement direction)
{
	if (direction == Renderer::FORWARD || direction == Renderer::BACKWARD || direction == Renderer::LEFT || direction == Renderer::RIGHT)
		velocity.z = movementSpeed;
	else if (direction == Renderer::RELEASE_FORWARD || direction == Renderer::RELEASE_BACKWARD || direction == Renderer::RELEASE_LEFT || direction == Renderer::RELEASE_RIGHT)
		velocity.z = 0.0f;
	else if (direction == Renderer::UP)
		jump();
	//else if (direction == DOWN)
	//	velocity -= Up * velocity;
	//Position.y = 0.0f; //prevents the user from "flying" by keeping them on the xz-plane
}

/*
// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void GameLogic::GameObject::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= Renderer::SENSITIVITY;
	yoffset *= Renderer::SENSITIVITY;

	yaw += xoffset;
	yaw = glm::mod(yaw, 360.0f);
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateDirectionVectors();
}
*/

void GameLogic::GameObject::setYaw(float y)
{
	yaw = y;
	updateDirectionVectors();
}



//protected:
	

//private:

void GameLogic::GameObject::updateDirectionVectors()
{
	// Calculate the new Front vector
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, WORLD_UP));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

