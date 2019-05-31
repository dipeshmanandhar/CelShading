//Dipesh Manandhar 5/27/2019

//Created H Files
#include "../Headers/Entity.h"

//public:
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

Entity::Entity(Renderer::Loader::modelID id) : Entity()
{
	setModel(id);
}

/*  Functions   */

void Entity::setModel(Renderer::Loader::modelID id)
{
	model = &Renderer::Loader::models()[id];
}

void Entity::initialize(Renderer::Shader& s, glm::mat4& v, glm::mat4& p)
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

void Entity::setPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 Entity::getPosition() const
{
	return position;
}

//private:
Renderer::Shader*& Entity::shader()
{
	static Renderer::Shader* shader = NULL;
	return shader;
}
glm::mat4*& Entity::view()
{
	static glm::mat4* view = NULL;
	return view;
}
glm::mat4*& Entity::projection()
{
	static glm::mat4* projection = NULL;
	return projection;
}