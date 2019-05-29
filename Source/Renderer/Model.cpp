//Dipesh Manandhar 5/6/2019

//Created H Files
#include "../../Headers/Renderer/Model.h"

//public:
/* Constructor */

Renderer::Model::Model()
{
	//do nothing
}

/*
Renderer::Model::Model(const char* path)
{
	initialize(path);
}
*/

/*  Functions   */

//TODO: implement destructor (need to deallocate glTextures)

/*
void Renderer::Model::initialize(const char* path)
{
	loadModel(path);
}
*/

void Renderer::Model::Draw(const Shader& shader) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}
