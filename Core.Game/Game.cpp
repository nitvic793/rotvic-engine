#include "Game.h"

int Game::InstanceCount = 0;

Game::Game()
{
	speed = 10.f;
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

void Game::SendInput(Keys key)
{
	if (key == Keys::Up) {
		entity.MoveUp(speed);
	}
}
