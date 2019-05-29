// Dipesh Manandhar 5/26/2019

// C++ Libraries
#include <iostream>

// Project Libraries
#include "../Headers/Entity.h"
#include "../Headers/Renderer/Loader.h"

//#include "../../Headers/Renderer/Renderer.h"

using namespace std;

Entity player;

void setup()
{
	cout << "Setting Up..." << endl;
	player.setModel(Renderer::Loader::CHIYA);
}

void update(float dTime)
{

}

void render()
{
	player.Draw();
}