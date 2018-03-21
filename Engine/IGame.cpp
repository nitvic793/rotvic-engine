#include "IGame.h"



IGame::IGame()
{
	camera = nullptr;
}

IGame::~IGame()
{
	ClearEntities();
}

void IGame::OnResize(int width, int height)
{
	camera->SetProjectionMatrix((float)width / height);
}

bool IGame::Save()
{
	return true;
}

bool IGame::Load()
{
	return true;
}

/// <summary>
/// Get all entities in this game instance.
/// </summary>
/// <returns></returns>
std::vector<Entity*> IGame::GetEntities()
{
	return vEntities;
}

GameEntity* IGame::GetEntity(std::string entity)
{
	if (entities.find(entity) != entities.end())
	{
		return entities[entity];
	}

	return nullptr;
}

void IGame::BindConsole(Console * console)
{
	this->console = console;
}

/// <summary>
/// Bind keyboard to game instance.
/// </summary>
/// <param name="keyboard"></param>
void IGame::BindKeyboard(Keyboard* keyboard)
{
	this->keyboard = keyboard;
}

/// <summary>
/// Set system core to game instance.
/// </summary>
/// <param name="core"></param>
void IGame::SetCore(SystemCore* core)
{
	this->core = core;
}

/// <summary>
/// Set configuration of game instance.
/// </summary>
/// <param name="map"></param>
void IGame::SetConfig(const ConfigMap& map)
{
	config = map;
}

/// <summary>
/// Set save system to game instance.
/// </summary>
/// <param name="saveSys"></param>
void IGame::SetSaveSystem(SaveSystem* saveSys)
{
	this->saveSystem = saveSys;
}

/// <summary>
/// Bind mouse input to game instance.
/// </summary>
/// <param name="mouse"></param>
void IGame::BindMouse(Mouse * mouse)
{
	this->mouse = mouse;
}

/// <summary>
/// Set the renderer of game instance.
/// </summary>
/// <param name="renderer"></param>
void IGame::SetRenderer(Renderer * renderer)
{
	this->renderer = renderer;
	camera = new Camera((float)renderer->screenWidth / renderer->screenHeight);
}

/// <summary>
/// Set resource manager of game instance.
/// </summary>
/// <param name="rm"></param>
void IGame::SetResourceManager(ResourceManager* rm)
{
	this->resource = rm;
}

/// <summary>
/// Updates all entities. Calls the update function for each entity in game instance.
/// </summary>
/// <param name="deltaTime">The delta time between frames</param>
void IGame::UpdateEntities(float deltaTime)
{
	for (auto entity : entities) {
		entity.second->Update(deltaTime);
	}
}

/// <summary>
/// Add game entity to game instance. All entities in game instance will be updated and passed on to the renderer for drawing. 
/// </summary>
/// <param name="entity">Entity to be added</param>
/// <param name="entityName">Name of entity</param>
void IGame::AddEntity(GameEntity *entity, std::string entityName)
{
	entities.insert(std::pair<std::string, GameEntity*>(entityName, entity));
	vEntities.push_back(entity);
}

Camera * IGame::GetCamera()
{
	return camera;
}

LightsMap IGame::GetLights()
{
	return lightsMap;
}

/// <summary>
/// Deletes all entities in game instance.
/// </summary>
void IGame::ClearEntities()
{
	for (auto entity : entities)
	{
		delete entity.second;
	}
	entities.clear();
	vEntities.clear();
	if (camera)
		delete camera;
}

/// <summary>
/// Get mouse position in relation to the 2D View-Projection matrix.
/// </summary>
/// <returns></returns>
const Vector2f& IGame::GetMousePosition2D()
{
	return mouse->GetMousePositionWorld(
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix(),
		renderer->screenWidth, renderer->screenHeight,
		Vector3f(0, 0, -25.f));
}
