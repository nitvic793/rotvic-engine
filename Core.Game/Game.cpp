#include "Game.h"

int Game::InstanceCount = 0;

Game::Game(float speed)
{
	deltaTime = 0.f;
	delayTime = 0.f;
	this->speed = speed;
}

Game::~Game()
{
	InstanceCount--;
}

void Game::SetSpeed(float speed)
{
	this->speed = speed;
}

const float& Game::GetSpeed()
{
	return speed;
}

/// <summary>
/// Creates new game instance.
/// </summary>
/// <returns></returns>
Game* Game::CreateInstance()
{
	InstanceCount++;
	auto game = new Game();
	return game;
}

/// <summary>
/// Initialize game
/// </summary>
void Game::Initialize()
{
	light.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 0);
	light.DiffuseColor = XMFLOAT4(0.4f, 0.4f, 0.9f, 1.f);
	light.Direction = XMFLOAT3(1.f, 0, 0.f);

	secondaryLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 0);
	secondaryLight.DiffuseColor = XMFLOAT4(0.9f, 0.4f, 0.4f, 1);
	secondaryLight.Direction = XMFLOAT3(0, -1, 0);

	pointLight.Color = XMFLOAT4(0.4f, 0.9f, 0.4f, 1);
	pointLight.Position = XMFLOAT3(0, 0, 0);

	lightsMap.insert(std::pair<std::string, Light*>("light", new Light{ &light, Directional }));
	lightsMap.insert(std::pair<std::string, Light*>("secondaryLight", new Light{ &secondaryLight, Directional }));
	lightsMap.insert(std::pair<std::string, Light*>("pointLight", new Light{ &pointLight, Point }));
	LoadLevel();
}

/// <summary>
/// Loads the level. 
/// </summary>
void Game::LoadLevel()
{
	auto entity = new GameEntity(resource->GetMesh("sphere"), resource->GetMaterial("metal"));
	AddEntity(entity, "Test");
	entity->SetPosition(0, 2, 0);
}

/// <summary>
/// Save current game state.
/// </summary>
/// <returns></returns>
bool Game::Save()
{
#ifdef _DEBUG
	printf("\nSave state");
#endif
	if (vEntities.size() == 0)return false;
	SaveState state;
	for (auto entity : entities)
	{
		state.EntityName = entity.first;
		state.Position = entity.second->GetPosition();
		break;
	}
	saveSystem->Save(&state, "save.dat");
	return true;
}

/// <summary>
/// Load previous game state if available. 
/// </summary>
/// <returns></returns>
bool Game::Load()
{
	try
	{
#ifdef _DEBUG
		printf("\nLoad state");
#endif
		SaveState state;
		saveSystem->Load(&state, "save.dat");
		GameEntity *entity = new PlayerEntity();
		mesh = resource->GetMesh("Green");
		entity->SetMesh(mesh);
		entity->SetPosition(state.Position);
		entity->SetContext(context);
		ClearEntities();
		AddEntity(entity, state.EntityName);
		LoadLevel();
	}
	catch (std::exception)
	{
#ifdef _DEBUG
		printf("\nLoad save failed");
#endif
		return false;
	}
	return true;
}

int Game::GetInstanceCount()
{
	return InstanceCount;
}

/// <summary>
/// The game update function which is called in the core game loop. 
/// </summary>
/// <param name="deltaTime"></param>
void Game::Update(float deltaTime)
{
	camera->Update(deltaTime);
	delayTime += deltaTime;
}


