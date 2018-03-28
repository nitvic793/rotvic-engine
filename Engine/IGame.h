#pragma once
#include "Keyboard.h"
#include <vector>
#include "Entity.h"
#include "ConfigLoader.h"
#include "SaveSystem.h"
#include "Mouse.h"
#include "GameEntity.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Lights.h"
#include "Console.h"
#include "reactphysics3d.h"
#include "Skybox.h"

/// <summary>
/// The game instance interface. The actual game should inherit and implement this abstract class and bind it to the core game instance. 
/// </summary>
class IGame
{
protected:
	std::vector<Entity*> vEntities;
	std::map<std::string, GameEntity*> entities;
	Keyboard *keyboard;
	Mouse *mouse; 
	SystemCore *core;
	ConfigMap config;
	SaveSystem *saveSystem;
	Renderer* renderer;
	EntityContextWrapper context;
	ResourceManager *resource;
	Camera* camera;
	LightsMap lightsMap;
	Console* console;
	rp3d::Vector3 gravity;
	rp3d::DynamicsWorld* dynamicsWorld;
	Skybox* skybox;
	void ClearEntities();
	const Vector2f& GetMousePosition2D();
public:
	void BindConsole(Console* console);
	void BindKeyboard(Keyboard* keyboard);
	void SetCore(SystemCore* core);
	void SetConfig(const ConfigMap& map);
	void SetSaveSystem(SaveSystem *saveSys);
	void BindMouse(Mouse *mouse);
	void SetRenderer(Renderer* renderer);
	void SetResourceManager(ResourceManager *rm);
	void SetPhysics(rp3d::Vector3 grav, rp3d::DynamicsWorld* world);
	virtual void UpdateEntities(float deltaTime);
	virtual void Update(float) = 0;
	virtual void Initialize() = 0;
	virtual void OnResize(int width, int height);
	virtual bool Save();
	virtual bool Load();
	virtual std::vector<Entity*> GetEntities();
	GameEntity* GetEntity(std::string entity);
	void AddEntity(GameEntity *entity, std::string entityName);
	Camera* GetCamera();
	LightsMap GetLights();
	Skybox* GetSkybox();

	IGame();
	virtual ~IGame();
};

