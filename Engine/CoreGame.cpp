#include "CoreGame.h"
#include "ConfigLoader.h"
#include <typeinfo>
#include <stdexcept>

/// <summary>
/// Initializes window and bind DirectX to window. Also initializes other required resources. 
/// </summary>
/// <param name="hInstance">The HINSTANCE struct required to initialize the window</param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
bool CoreGame::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	if (4 * screenHeight != 3 * screenWidth && 16 * screenHeight != 9 * screenWidth && 16 * screenHeight != 10 * screenWidth) {
		throw std::exception("Unsupported aspect ratio. Invalid Screen Height or Width. (Supported ratios - 4:3, 16:9, 16:10)");
	}
	State = Init;
#if defined(DEBUG) || defined(_DEBUG)
	Core->CreateConsoleWindow(500, 120, 32, 120);
	printf("Debug mode console init.\n");
#endif
	Core->InitializeWindow(hInstance, nCmdShow, screenHeight, screenWidth, screenTitle);
	Core->InitializeAndBindDirectX();
	Core->SetOnResizeCallback([&](int width, int height)
	{
		this->OnResizeCallback(width, height);
	});

	console = std::unique_ptr<Console>(new Console(Core));
	Core->SetOnKeyPressCallback([&](char key)->void 
	{
		console->OnKeyPress(key);
	});
	mouse = new Mouse(Core->GetWindowHandle());
	resourceManager->LoadResources(config, Core);
	return true;
}

/// <summary>
/// CoreGame constructor. Loads required configuration data from config file.
/// </summary>
/// <param name="configFileName">Name of the config file, including extension</param>
CoreGame::CoreGame(std::string configFileName)
{
	ConfigLoader loader;
	try {
		config = loader.Load(configFileName);
	}
	catch (...)
	{
		throw std::exception("Unable to load config file.");
	}
	auto ht = config[HEIGHT];
	screenTitle = config[TITLE];
	screenHeight = std::stoi(config[HEIGHT]);
	screenWidth = std::stoi(config[WIDTH]);
	if (screenHeight == 0 || screenWidth == 0) {
		throw std::exception("Invalid Screen Height or Width");
	}
	Core = new SystemCore();
	keyboard = new Keyboard();
	gameInstance = nullptr;
	renderer = new Renderer(Core, screenWidth, screenHeight);
	saveSystem = new SaveSystem();
	resourceManager = new ResourceManager();
}

void CoreGame::OnResizeCallback(int width, int height)
{
	screenHeight = height;
	screenWidth = width;
	renderer->screenHeight = height;
	renderer->screenWidth = width;
	gameInstance->GetCamera()->SetProjectionMatrix((float)width / height);
	renderer->SetProjectionMatrix(width, height);
}

/// <summary>
/// Error handler callback. 
/// </summary>
/// <param name="e">The thrown exception. Can be null.</param>
void CoreGame::HandleError(std::exception *e)
{
	if (e)
		MessageBoxA(NULL, e->what(), "Error", MB_ICONERROR);
	else
		MessageBoxA(NULL, "Unexpected error. Aborting.", "Error", MB_ICONERROR);
}

/// <summary>
/// Core game constructor for creating instance without config file.
/// </summary>
/// <param name="height">Height of window</param>
/// <param name="width">Width of window</param>
/// <param name="title">Title of window</param>
CoreGame::CoreGame(int height, int width, std::string title)
{
	screenHeight = height;
	screenWidth = width;
	screenTitle = title;
	Core = new SystemCore();
	keyboard = new Keyboard();
	gameInstance = nullptr;
	renderer = new Renderer(Core, width, height);
	saveSystem = new SaveSystem();
	resourceManager = new ResourceManager();
	mouse = nullptr;
}

/// <summary>
/// Core Game destructor
/// </summary>
CoreGame::~CoreGame()
{
	if (Core) delete Core;
	if (gameInstance) delete gameInstance;
	if (renderer) delete renderer;
	if (keyboard) delete keyboard;
	if (saveSystem) delete saveSystem;
	if (mouse) delete mouse;
	if (resourceManager) delete resourceManager;
}

/// <summary>
/// Returns the SystemCore instance.
/// </summary>
/// <returns>Initialized system core.</returns>
SystemCore* CoreGame::GetSystemCore()
{
	return Core;
}

/// <summary>
/// Gets the resource manager instance. 
/// </summary>
/// <returns></returns>
ResourceManager * CoreGame::GetResourceManager()
{
	return resourceManager;
}

/// <summary>
/// Bind game instance to core game. This binds the input system, renderer, resource manager and initializes the configuration to the game instance.
/// </summary>
/// <param name="gInstance">The concrete game instance. </param>
void CoreGame::Bind(IGame* gInstance)
{
	if (gInstance == nullptr)
	{
		throw std::exception("Null Game Instance");
	}

	gameInstance = gInstance;
	gameInstance->SetCore(Core);
	gameInstance->SetConfig(config);
	gameInstance->SetSaveSystem(saveSystem);
	gameInstance->BindKeyboard(keyboard);
	gameInstance->BindMouse(mouse);
	gameInstance->SetRenderer(renderer);
	gameInstance->SetResourceManager(resourceManager);
	gameInstance->BindConsole(console.get());
}

/// <summary>
/// Get the system renderer
/// </summary>
/// <returns></returns>
Renderer *CoreGame::GetRenderer()
{
	return renderer;
}

/// <summary>
/// Starts the core game loop. 
/// </summary>
/// <remarks>The game instance update is called first before updating the related entities in the loop.</remarks>
void CoreGame::Run()
{
	__int64 perfFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
	perfCounterSeconds = 1.0 / (double)perfFreq;
	State = Running;
	renderer->Initialize();
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	startTime = now;
	currentTime = now;
	previousTime = now;
	try
	{
		Core->Run([&]()
		{
			UpdateTimer();
			switch (State)
			{
			case Running:
			{
				ClearScreen();
				if (console->enabled)console->Update(deltaTime);
				gameInstance->GetCamera()->Update(deltaTime);
				gameInstance->Update(deltaTime);
				gameInstance->UpdateEntities(deltaTime);
				Draw();
				break;
			}
			case Quit:
			{
				PostQuitMessage(WM_CLOSE);
				break;
			}
			}
		});
	}
	catch (std::exception e)
	{
		Core->HandleError(&e);
	}
	catch (...)
	{
		Core->HandleError(nullptr);
	}
}

/// <summary>
/// Gets the current state of the game.
/// </summary>
/// <returns></returns>
StateEnum CoreGame::GetState()
{
	return State;
}

/// <summary>
/// Sets the current state of the game
/// </summary>
/// <param name="state"></param>
void CoreGame::SetState(StateEnum state)
{
	State = state;
}

/// <summary>
/// Clears screen to cornflower blue.
/// </summary>
/// <remarks>Need to allow arguments to pass color.</remarks>
void CoreGame::ClearScreen()
{
	renderer->ClearScreen();
}

/// <summary>
/// Draws all entities present in the bound game instance
/// </summary>
void CoreGame::Draw()
{
	renderer->UseCamera(gameInstance->GetCamera());
	renderer->SetLights(gameInstance->GetLights());
	auto entities = gameInstance->GetEntities();
	for (auto entity : entities)
	{
		renderer->Draw(entity);
	}
	if (console->enabled)console->Render(); //Render console if required.
}

/// <summary>
/// Updates the timer to calculate delta time and total time passed.
/// </summary>
void CoreGame::UpdateTimer()
{
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	currentTime = now;
	deltaTime = max((float)((currentTime - previousTime) * perfCounterSeconds), 0.0f);
	totalTime = (float)((currentTime - startTime) * perfCounterSeconds);
	previousTime = currentTime;
}