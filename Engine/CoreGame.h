#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SystemCore.h"
#include <string>
#include "SystemCore.h"
#include "IGame.h"
#include "States.h"
#pragma comment(lib, "d3d11.lib")

class CoreGame
{
protected:
	SystemCore *Core;
	int screenHeight;
	int screenWidth;
	std::string screenTitle;
	StateEnum State;
	IGame *gameInstance;
	Keyboard *keyboard;
public:
	StateEnum GetState();
	void SetState(StateEnum state);
	virtual void Run();
	virtual void Bind(IGame* gInstance);
	bool Initialize(HINSTANCE hInstance, int nCmdShow);
	SystemCore* GetSystemCore();
	CoreGame(int height, int width, std::string title);
	CoreGame();
	~CoreGame();
};

