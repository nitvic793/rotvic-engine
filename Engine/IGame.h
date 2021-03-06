// Most code written by Nitish Victor, rp3d integration by Trevor Walden with help from http://www.reactphysics3d.com/usermanual.html and the rp3d class list#pragma once
#include "Keyboard.h"
#include <vector>
#include "Entity.h"
#include "ConfigLoader.h"
#include "SaveSystem.h"
#include "Mouse.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Lights.h"
#include "Console.h"
#include "reactphysics3d.h"
#include "PhysicsEventListener.h"
#include "PhysicsEntityMap.h"
#include "Skybox.h"
#include "UICanvas.h"

/// <summary>
/// The game instance interface. The actual game should inherit and implement this abstract class and bind it to the core game instance. 
/// </summary>
class IGame
{
protected:
	PhysicsEventListener* physicsEventListener;
	std::vector<Entity*> vEntities;
	std::map<std::string, Entity*> entities;
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
	Skybox* skybox;
	PhysicsEntityMap* physicsEntityMap;
	std::unique_ptr<UICanvas> uiCanvas;
	rp3d::DynamicsWorld* dynamicsWorld;
	void ClearEntities();
	const Vector2f& GetMousePosition2D();
	const float timeStep = 1.0f / 60.0f; // Constant physics time step 
	float physicsTimer = 0;
	bool AreResourcesInitialized = false;
	bool isPhysicsEnabled = false;
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
	void SetResourceInitialized(bool init);
	void SetPhysicsActive(bool active);
	void LoadHUDFile(std::string fileName);

	virtual void UpdateEntities(float deltaTime);
	virtual void Update(float) = 0;
	virtual void PreInitialize();
	virtual void Initialize() = 0;
	virtual void OnResize(int width, int height);
	virtual bool Save();
	virtual bool Load();
	virtual std::vector<Entity*> GetEntities();

	rp3d::DynamicsWorld* GetDynamicsWorld();
	bool IsResourcesInitialized() const;
	Entity* GetEntity(std::string entity);
	void AddEntity(Entity *entity, std::string entityName);
	void RemoveEntity(std::string entityName);
	Camera* GetCamera();
	LightsMap GetLights();
	Skybox* GetSkybox();
	UICanvas* GetCanvas();

	IGame();
	virtual ~IGame();
};

