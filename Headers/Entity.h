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
	
	Entity(Loader::modelID id);
	
	/*  Functions   */
	
	void setModel(Loader::modelID id);

	static void initialize(Shader& s, glm::mat4& v, glm::mat4& p);

	void Draw() const;

private:
	/*  Data  */
	Model* model;
	glm::vec3 position; //in world space
	float yaw, pitch; //in degrees

	static Shader*& shader()
	{
		static Shader* shader = NULL;
		return shader;
	}
	static glm::mat4*& view()
	{
		static glm::mat4* view = NULL;
		return view;
	}
	static glm::mat4*& projection()
	{
		static glm::mat4* projection = NULL;
		return projection;
	}
	/*
	static Shader* shader;
	static glm::mat4* view;
	static glm::mat4* projection;
	*/
};

#endif // !ENTITY_H