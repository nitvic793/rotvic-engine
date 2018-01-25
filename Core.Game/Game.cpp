#include "Game.h"

int Game::InstanceCount = 0;

Game::Game(float speed)
{
	this->speed = speed;
}

Game::~Game()
{
	InstanceCount--;
	for (auto entity : entities) 
	{
		delete entity.second;
	}
}

void Game::SetSpeed(float speed)
{
	this->speed = speed;
}

const float& Game::GetSpeed()
{
	return speed;
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
	vEntities.push_back(entity);
}

void Game::Update()
{
	if (keyboard->IsKeyPressed(Up)) {
		SendInput(Up, "MainEntity");
	}
}

std::vector<Entity*> Game::GetEntities()
{
	return vEntities;
}
