#include "CoreGame.h"

bool CoreGame::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	State = Init;
	Core->InitializeWindow(hInstance, nCmdShow, screenHeight, screenWidth, screenTitle);
	Core->InitializeAndBindDirectX();
	return true;
}

CoreGame::CoreGame()
{
	Core = new SystemCore();
	keyboard = new Keyboard();
}

CoreGame::CoreGame(int height, int width, std::string title)
{
	screenHeight = height;
	screenWidth = width;
	screenTitle = title;
	Core = new SystemCore();
	keyboard = new Keyboard();
}


CoreGame::~CoreGame()
{
	delete Core;
}

SystemCore* CoreGame::GetSystemCore()
{
	return Core;
}

void CoreGame::Bind(IGame* gInstance) 
{
	gameInstance = gInstance;
	gameInstance->BindKeyboard(keyboard);
}

void CoreGame::Run()
{
	while (State != Quit)
	{
		gameInstance->Update();
	}
}

StateEnum CoreGame::GetState()
{
	return State;
}

void CoreGame::SetState(StateEnum state)
{
	State = state;
}