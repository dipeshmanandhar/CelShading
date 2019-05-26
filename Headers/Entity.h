//Dipesh Manandhar 5/6/2019

#ifndef ENTITY_H
#define ENTITY_H

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ Libraries
//#include <string>
//#include <vector>

//Created H Files
#include "Model.h"
#include "Loader.h"
#include "Shader.h"

//using namespace std;

class Entity
{
public:
	/* Constructor */
	
	Entity()
	{
		//do nothing
		model = NULL;
		position = glm::vec3(0.0f);
		yaw = 0.0f;
		pitch = -90.0f;
	}
	
	Entity(Loader::modelID id) : Entity()
	{
		setModel(id);
	}
	
	/*  Functions   */
	
	void setModel(Loader::modelID id)
	{
		model = &Loader::models[id];
	}

	void Draw(Shader& shader, glm::mat4& view, glm::mat4& projection)
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mv = view * modelMatrix;
		shader.setMat4("mv", mv);
		shader.setMat4("mvp", projection * mv);
		shader.setMat3("mvNormal", transpose(inverse(glm::mat3(mv))));
		
		model->Draw(shader);
	}

private:
	/*  Data  */
	Model* model;
	glm::vec3 position; //in world space
	float yaw, pitch; //in degrees
};

#endif // !ENTITY_H