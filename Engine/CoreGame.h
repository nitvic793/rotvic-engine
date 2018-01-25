#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "SystemCore.h"
#include <string>
#include "SystemCore.h"

#pragma comment(lib, "d3d11.lib")

class CoreGame
{
protected:
	SystemCore *Core;
	int screenHeight;
	int screenWidth;
	std::string screenTitle;
public:
	bool Initialize(HINSTANCE hInstance);
	SystemCore* GetSystemCore();
	CoreGame(int height, int width, std::string title);
	CoreGame();
	~CoreGame();
};

