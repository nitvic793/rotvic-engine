#include "Game.h"

int Game::InstanceCount = 0;

Game::Game()
{
	speed = 10.f;
}


Game::~Game()
{
	InstanceCount--;
	for (auto entity : entities) 
	{
		delete entity.second;
	}
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

void Game::SendInput(Keys key, std::string entityName)
{
	if (key == Keys::Up) {
		entities[entityName]->MoveUp(speed);
	}

	if (key == Keys::Down) {
		entities[entityName]->MoveDown(speed);
	}

	if (key == Keys::Left) {
		entities[entityName]->MoveLeft(speed);
	}

	if (key == Keys::Right) {
		entities[entityName]->MoveRight(speed);
	}
}

void Game::AddEntity(GameEntity *entity, std::string entityName)
{
	entities.insert(std::pair<std::string, GameEntity*>(entityName, entity));
}
