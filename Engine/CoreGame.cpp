#include "CoreGame.h"

bool CoreGame::Initialize(HINSTANCE hInstance)
{
	Core = new SystemCore();
	Core->InitializeWindow(hInstance, screenHeight, screenWidth, screenTitle);
	Core->InitializeAndBindDirectX();
	return true;
}

CoreGame::CoreGame()
{
}

CoreGame::CoreGame(int height, int width, std::string title)
{
	screenHeight = height;
	screenWidth = width;
	screenTitle = title;
}


CoreGame::~CoreGame()
{
}
