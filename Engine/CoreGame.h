#pragma once
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
public:
	StateEnum GetState();
	void ClearScreen();
	void SetState(StateEnum state);
	Renderer *GetRenderer();
	SystemCore* GetSystemCore();
	ResourceManager* GetResourceManager();

	virtual void OnResizeCallback(int width, int height);
	virtual void HandleError(std::exception *e);
	virtual void Draw();
	virtual void Run();
	virtual void Bind(IGame* gInstance);
	bool Initialize(HINSTANCE hInstance, int nCmdShow);
	void UpdateTimer();
	CoreGame(int height, int width, std::string title);
	CoreGame(std::string configFileName);
	~CoreGame();
};

