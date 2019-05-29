//Dipesh Manandhar 5/27/2019

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
//#include <string>
//#include <vector>

//Created H Files
#include "../Headers/Entity.h"
#include "../../Headers/Renderer/Model.h"
#include "../../Headers/Renderer/Loader.h"
#include "../../Headers/Renderer/Shader.h"

//using namespace std;

/* Constructor */

Entity::Entity()
{
	//do nothing
	model = NULL;
	position = glm::vec3(0.0f);
	yaw = 0.0f;
	pitch = -90.0f;
	shader() = NULL;
	view() = projection() = NULL;
}

Entity::Entity(Loader::modelID id) : Entity()
{
	setModel(id);
}

/*  Functions   */

void Entity::setModel(Loader::modelID id)
{
	model = &Loader::models()[id];
}

void Entity::initialize(Shader& s, glm::mat4& v, glm::mat4& p)
{
	shader() = &s;
	view() = &v;
	projection() = &p;
}

void Entity::Draw() const
{
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mv = *view() * modelMatrix;
	shader()->setMat4("mv", mv);
	shader()->setMat4("mvp", *projection() * mv);
	shader()->setMat3("mvNormal", transpose(inverse(glm::mat3(mv))));

	model->Draw(*shader());
}
