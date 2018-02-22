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
	Collision2D::DestroyQuadTree();
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
	context = EntityContextWrapper(keyboard, mouse);
	context.GetMousePosition2D = [&]() { return GetMousePosition2D(); };
	mesh = resource->GetMesh("Green");
	float speed = (float)std::atof(config["speed"].c_str());
	if (speed == 0.0f) {
		speed = DefaultSpeed;
	}
	SetSpeed(speed);
	GameEntity *entity = new PlayerEntity(speed);
	entity->SetMesh(mesh);
	entity->SetContext(context);
	AddEntity(entity, "MainEntity");
	LoadLevel();
}

/// <summary>
/// Loads the level. 
/// </summary>
void Game::LoadLevel()
{
	mesh = resource->GetMesh("Default");
	GameEntity *entity = new GameEntity();
	entity->SetMesh(mesh);
	entity->SetContext(context);
	entity->SetPosition(Vector3f(7, 5, 0));
	AddEntity(entity, "Object1");

	mesh = resource->GetMesh("Default");
	entity = new GameEntity();
	entity->SetMesh(mesh);
	entity->SetContext(context);
	entity->SetPosition(Vector3f(-10, 5, 0));
	AddEntity(entity, "Object2");

	mesh = resource->GetMesh("Default");
	entity = new GameEntity();
	entity->SetMesh(mesh);
	entity->SetContext(context);
	entity->SetPosition(Vector3f(-8, -8, 0));
	AddEntity(entity, "Object3");

	auto bounds = Utility::GetScreenBounds2D(camera->GetViewMatrix(), camera->GetProjectionMatrix(), renderer->screenWidth, renderer->screenHeight, Vector3f(0, 0, -25));
	Collision2D::InstantiateQuadTree2D(bounds);
	std::vector<GameEntity*> qtObjects;
	for (auto entity : entities) {
		if (entity.first == "MainEntity")continue;;
		qtObjects.push_back(entity.second);
	}
	Collision2D::InsertStaticObjectsInQuadTree(qtObjects);
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
	if (keyboard->IsKeyPressed(F5) && delayTime > 0.5f) {
		Save();
		delayTime = 0.f;
	}

	if (keyboard->IsKeyPressed(F6) && delayTime > 0.5f) {
		Load();
		delayTime = 0.f;
	}

	auto player = entities["MainEntity"];
	auto collidingEntity = Collision2D::IsCollidingQuadTree(player);
	if (collidingEntity != nullptr)
	{
		auto collider = dynamic_cast<ICollider2D*>(player);
		collider->OnCollision(collidingEntity);
	}
}


