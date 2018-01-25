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
	gameInstance = nullptr;
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
	gameInstance = nullptr;
}


CoreGame::~CoreGame()
{
	delete Core;
	if(gameInstance) delete gameInstance;
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
	Core->Run([&]() 
	{
		if (State != Quit) {
			gameInstance->Update();
		}
		else {
			PostQuitMessage(WM_CLOSE);
		}
	});

}

StateEnum CoreGame::GetState()
{
	return State;
}

void CoreGame::SetState(StateEnum state)
{
	State = state;
}