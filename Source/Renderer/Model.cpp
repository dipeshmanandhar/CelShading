//Dipesh Manandhar 5/6/2019

//Created H Files
#include "../../Headers/Renderer/Model.h"

//public:
/* Constructor */

Model::Model()
{
	//do nothing
}

/*
Model(const char* path)
{
	initialize(path);
}
*/

/*  Functions   */

//TODO: implement destructor (need to deallocate glTextures)

/*
void Model::initialize(const char* path)
{
	loadModel(path);
}
*/

void Model::Draw(const Shader& shader) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}
