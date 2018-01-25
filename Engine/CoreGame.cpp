#include "CoreGame.h"

bool CoreGame::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	Core->InitializeWindow(hInstance, nCmdShow, screenHeight, screenWidth, screenTitle);
	Core->InitializeAndBindDirectX();
	return true;
}

CoreGame::CoreGame()
{
	Core = new SystemCore();
}

CoreGame::CoreGame(int height, int width, std::string title)
{
	screenHeight = height;
	screenWidth = width;
	screenTitle = title;
	Core = new SystemCore();
}


CoreGame::~CoreGame()
{
	delete Core;
}

SystemCore* CoreGame::GetSystemCore()
{
	return Core;
}
