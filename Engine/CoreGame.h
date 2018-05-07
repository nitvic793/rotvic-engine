// Most code written by Nitish Victor, rp3d integration by Trevor Walden with help from http://www.reactphysics3d.com/usermanual.html and the rp3d class list
#pragma once
#define NOMINMAX
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SystemCore.h"
#include <string>
#include "IGame.h"
#include "States.h"
#include "Renderer.h"
#include "SaveSystem.h"
#include "Mouse.h"
#include "ResourceManager.h"
#include "Lights.h"
#include "Console.h"
#include "DebugDraw.h"
#include "EventSystem.h"
#include "AsyncLoader.h"
#include "reactphysics3d.h"

#pragma comment(lib, "d3d11.lib")

/// <summary>
/// Core Game class.
/// Should be created and initialized first before binding game instance. 
/// </summary>
class CoreGame
{
	double perfCounterSeconds;
	float totalTime;
	float deltaTime;
	__int64 startTime;
	__int64 currentTime;
	__int64 previousTime;
	// FPS calculation
	int fpsFrameCount;
	float fpsTimeElapsed;

protected:
	SystemCore *Core;
	int screenHeight;
	int screenWidth;
	std::string screenTitle;
	StateEnum State;
	IGame *gameInstance;
	Keyboard *keyboard;
	Mouse *mouse;
	Renderer *renderer;
	ConfigMap config;
	SaveSystem *saveSystem;
	ResourceManager* resourceManager;
	std::unique_ptr<Console> console;
	std::unique_ptr<DebugDraw> debugDraw;
	std::unique_ptr<AsyncLoader> asyncLoader;
	EventSystem* eventSystem;
	rp3d::Vector3 gravity;
	rp3d::DynamicsWorld* dynamicsWorld;
	WorkerThread worker;
	void RegisterConsoleCommands();
public:
	StateEnum GetState();
	void ClearScreen();
	void SetState(StateEnum state);
	Renderer *GetRenderer();
	SystemCore* GetSystemCore();
	ResourceManager* GetResourceManager();
	void LoadHUDFile(std::string fileName);

	virtual void OnResizeCallback(int width, int height);
	virtual void HandleError(std::exception *e);
	virtual void Draw();
	virtual void Run();
	virtual void Bind(IGame* gInstance);

	void InitializeInstance();
	bool Initialize(HINSTANCE hInstance, int nCmdShow);
	void UpdateTimer();
	CoreGame(int height, int width, std::string title);
	CoreGame(std::string configFileName);
	~CoreGame();
};

