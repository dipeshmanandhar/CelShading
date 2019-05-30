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
#include "Renderer/Model.h"
#include "Renderer/Loader.h"
#include "Renderer/Shader.h"

//using namespace std;

class Entity
{
public:
	/* Constructor */
	
	Entity();
	
	Entity(Renderer::Loader::modelID id);
	
	/*  Functions   */
	
	void setModel(Renderer::Loader::modelID id);

	static void initialize(Renderer::Shader& s, glm::mat4& v, glm::mat4& p);

	void Draw() const;

	void setPosition(glm::vec3 pos);

private:
	/*  Data  */
	Renderer::Model* model;
	glm::vec3 position; //in world space
	float yaw, pitch; //in degrees

	static Renderer::Shader*& shader();
	static glm::mat4*& view();
	static glm::mat4*& projection();

	/*
	static Renderer::Shader* shader;
	static glm::mat4* view;
	static glm::mat4* projection;
	*/
};

#endif // !ENTITY_H