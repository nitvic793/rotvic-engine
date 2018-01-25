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
	auto game = new Game();
	return game;
}

void Game::Initialize()
{
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	Mesh *mesh = new Mesh(core);
	Material *mat = new Material(core);
	Vertex vertices[] = {
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), blue },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), green }
	};
	UINT indices[] = { 0, 1, 2 };
	mat->LoadDefaultShaders();
	mesh->Initialize(vertices, 3, indices, 3);
	GameEntity *entity = new GameEntity();
	mesh->SetMaterial(mat);
	entity->SetMesh(mesh);
	AddEntity(entity, "MainEntity");
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
	std::vector<Keys> inputs = { Up, Down, Left, Right };
	for (auto input : inputs)
	{
		if (keyboard->IsKeyPressed(input)) {
			SendInput(input, "MainEntity");
		}
	}	
}

std::vector<Entity*> Game::GetEntities()
{
	return vEntities;
}
