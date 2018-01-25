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
	renderer = new Renderer(Core);
}

CoreGame::CoreGame(int height, int width, std::string title)
{
	screenHeight = height;
	screenWidth = width;
	screenTitle = title;
	Core = new SystemCore();
	keyboard = new Keyboard();
	gameInstance = nullptr;
	renderer = new Renderer(Core);
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
	if (gInstance == nullptr) 
	{
		throw std::exception("Null Game Instance");
	}
	gameInstance = gInstance;
	gameInstance->SetCore(Core);
	gameInstance->BindKeyboard(keyboard);
}

Renderer *CoreGame::GetRenderer()
{
	return renderer;
}

void CoreGame::Run()
{
	State = Running;
	renderer->Initialize();
	Core->Run([&]() 
	{
		if (State != Quit) {
			ClearScreen();
			gameInstance->Update();
			Draw();
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

void CoreGame::ClearScreen()
{
	Core->ClearScreen();
}

void CoreGame::Draw()
{
	auto entities = gameInstance->GetEntities();
	for (auto entity : entities)
	{
		renderer->Draw(entity);
	}
}