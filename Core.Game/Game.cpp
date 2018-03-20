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
	keyboard->AddAction("up", { Up,W });
	console->WriteLine(L"Level loaded");
	auto entity = new GameEntity(resource->GetMesh("sphere"), resource->GetMaterial("metal"));
	AddEntity(entity, "Test");
	entity->SetPosition(0, 2, 0);

	console->RegisterCommand("AddSphere", [&](std::vector<std::string> params)->void 
	{
		if (params.size() < 3)return;
		float x = (float)atof(params[0].c_str());
		float y = (float)atof(params[1].c_str());
		float z = (float)atof(params[2].c_str());
		auto entity = new GameEntity(resource->GetMesh("sphere"), resource->GetMaterial("metal"));
		AddEntity(entity, "Test");
		entity->SetPosition(x, y, z);
	});
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
	SaveState state;
	//Save here
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
		//Load Here
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
	Ray ray;
	ray.color = XMFLOAT4(1,1,1,1);
	ray.origin = XMFLOAT3(0, 0, 0);
	ray.direction = XMFLOAT3(0, 1, 0);
	ray.length = 2.f;

	Sphere sphere;	
	sphere.color = XMFLOAT4(1, 1, 1, 1);
	sphere.bounding.Center = XMFLOAT3(-1, 0, 0);
	sphere.bounding.Radius = 0.5;

	DebugDraw::Draw<Ray>(ray);
	DebugDraw::Draw<Sphere>(sphere, "Collision");

	delayTime += deltaTime;
	if (keyboard->IsKeyPressed(Tilde) && delayTime>0.2f)
	{
		delayTime = 0.f;
		console->enabled = !console->enabled;
	}
	
	if (keyboard->IsActionPressed("up") && delayTime > 0.2f)
	{
		delayTime = 0.f;
		//console->WriteLine(L"Up action pressed");
	}

	delayTime += deltaTime;
}


