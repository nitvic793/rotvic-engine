#include "Game.h"

int Game::InstanceCount = 0;

Game::Game()
{
}


Game::~Game()
{
	InstanceCount--;
}

Game* Game::CreateInstance()
{
	InstanceCount++;
	return new Game();
}

int Game::GetInstanceCount()
{
	return InstanceCount;
}
