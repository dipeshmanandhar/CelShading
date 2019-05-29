// Dipesh Manandhar 5/26/2019

//#include "pch.h"

// C++ Libraries
#include <iostream>

// Project Libraries
#include "../Headers/Entity.h"
#include "../Headers/Loader.h"

//#include "../Headers/Renderer.h"

using namespace std;

Entity player;

void setup()
{
	cout << "Setting Up..." << endl;
	player.setModel(Loader::CHIYA);
}

void update(float dTime)
{

}

void render()
{
	player.Draw();
}