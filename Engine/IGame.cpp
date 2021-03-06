// Most code written by Nitish Victor, rp3d integration written by Trevor Walden with help from http://www.reactphysics3d.com/usermanual.html and the rp3d class list

#include "IGame.h"
#include "EventSystem.h"
#include <sstream>
#include "UIText.h"
#include "UIImage.h"
using namespace DirectX;

IGame::IGame()
{
	camera = nullptr;
	skybox = nullptr;
}

IGame::~IGame()
{
	ClearEntities();
	delete physicsEventListener;
	delete physicsEntityMap;
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

rp3d::DynamicsWorld * IGame::GetDynamicsWorld()
{
	return dynamicsWorld;
}

bool IGame::IsResourcesInitialized() const
{
	return AreResourcesInitialized;
}

Entity* IGame::GetEntity(std::string entity)
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
/// Bind physics world to game instance.
/// </summary>
/// <param name="grav">Gravity force vector</param>
/// <param name="world">Pointer to instantiated dynamics world</param>
void IGame::SetPhysics(rp3d::Vector3 grav, rp3d::DynamicsWorld* world)
{
	this->gravity = grav;
	this->dynamicsWorld = world;
	physicsEventListener = new PhysicsEventListener();
	physicsEntityMap = new PhysicsEntityMap();
	physicsEventListener->SetEntityMap(physicsEntityMap);
	world->setEventListener(physicsEventListener);
}

void IGame::LoadHUDFile(std::string fileName) // Reference used from Trevor's State Data project and the jsoncpp documentation
{
	Json::Value root;
	try
	{
		std::ifstream config_doc(fileName, std::ifstream::binary);
		config_doc >> root;
		auto memberNames = root.getMemberNames();
		for (auto member : memberNames)
		{
			try
			{
				auto element = root.get(member, "");
				if (element.get("type", "") == "text")
				{
					XMFLOAT3 pos;
					sscanf_s(element.get("position", "0,0").asCString(), "%f,%f", &pos.x, &pos.y);
					pos.z = 0;

					std::wstring text = core->s2ws(element.get("text", "").asCString());
					SpriteFont* spriteFont = new SpriteFont(core->GetDevice(), L"../../Assets/Fonts/segoeUI.spritefont"); // TODO: Register a dictionary of spritefonts on load and access pointers from it
					XMFLOAT4 color;
					sscanf_s(element.get("color", "0,0,0,1").asCString(), "%f,%f,%f,%f", &color.x, &color.y, &color.z, &color.w);

					float size;
					sscanf_s(element.get("size", "1").asCString(), "%f", &size);

					uiCanvas->AddComponent(new UIText(pos, text, size, spriteFont, color), member);
				}
				else if (element.get("type", "") == "image")
				{
					XMFLOAT3 pos;
					sscanf_s(element.get("position", "0,0").asCString(), "%f,%f", &pos.x, &pos.y);
					pos.z = 0;
					XMFLOAT4 color;
					sscanf_s(element.get("color", "1,1,1,1").asCString(), "%f,%f,%f,%f", &color.x, &color.y, &color.z, &color.w);

					XMFLOAT2 scale;
					sscanf_s(element.get("scale", "1,1").asCString(), "%f,%f", &scale.x, &scale.y);

					std::string filepath = element.get("source", "..\\..\\Assets\\Images\\image.png").asCString();

					uiCanvas->AddComponent(new UIImage(pos, color, filepath, core, scale), member);
				}
			}
			catch (std::exception e)
			{
				continue;
			}
			
		}
		config_doc.close();
	}
	catch (std::exception b)
	{
		// Console message here
		return;
	}
}

void IGame::SetResourceInitialized(bool init)
{
	AreResourcesInitialized = init;
}

void IGame::SetPhysicsActive(bool active)
{
	isPhysicsEnabled = active;
	if (active)
	{
		physicsTimer = 0.f;
	}
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
	this->renderer->GetResourceManager(rm);
}

/// <summary>
/// Updates all entities. Calls the update function for each entity in game instance.
/// </summary>
/// <param name="deltaTime">The delta time between frames</param>
void IGame::UpdateEntities(float deltaTime)
{
	if (isPhysicsEnabled)
	{
		while (physicsTimer >= timeStep) { // Catch phyics up to time elapsed between frames

			dynamicsWorld->update(timeStep); // Update the Dynamics world using the constant time step (1/60)

			physicsTimer -= timeStep; // Reset time difference

			auto events = EventSystem::GetInstance();
			events->ProcessQueuedEvents(PHYSICS);
		}
	}

	for (auto entity : entities) {
		entity.second->Update(deltaTime);
	}

	physicsTimer += deltaTime; // Track time

}

void IGame::PreInitialize()
{
	uiCanvas = std::unique_ptr<UICanvas>(new UICanvas(core));
}

/// <summary>
/// Add game entity to game instance. All entities in game instance will be updated and passed on to the renderer for drawing. 
/// </summary>
/// <param name="entity">Entity to be added</param>
/// <param name="entityName">Name of entity</param>
void IGame::AddEntity(Entity *entity, std::string entityName)
{
	entities.insert(std::pair<std::string, Entity*>(entityName, entity));
	vEntities.push_back(entity);
	auto shape = entity->GetProxyShape();
	physicsEntityMap->Insert(shape, entity);
}

void IGame::RemoveEntity(std::string entityName)
{
	if (entities.find(entityName) != entities.end())
	{
		vEntities.erase(std::remove(vEntities.begin(), vEntities.end(), entities[entityName]), vEntities.end());
		physicsEntityMap->Remove(entities[entityName]->GetProxyShape());
		delete entities[entityName];
		entities.erase(entityName);
	}
}

Camera* IGame::GetCamera()
{
	return camera;
}

LightsMap IGame::GetLights()
{
	return lightsMap;
}

Skybox* IGame::GetSkybox()
{
	return skybox;
}

UICanvas * IGame::GetCanvas()
{
	return uiCanvas.get();
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
