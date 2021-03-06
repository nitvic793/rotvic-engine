// Most code written by Nitish Victor, rp3d integration and HUD loading written by Trevor Walden
#include "CoreGame.h"
#include "ConfigLoader.h"
#include <typeinfo>
#include <stdexcept>
#include <fstream>
#include "json\json.h"

/// <summary>
/// Initializes bound game instance.
/// </summary>
void CoreGame::InitializeInstance()
{
	gameInstance->PreInitialize();
	eventSystem->RegisterEventCallback("ResourceLoadComplete", gameInstance, [&](void*)
	{
		gameInstance->SetResourceInitialized(true);
		gameInstance->Initialize();
		gameInstance->SetPhysicsActive(true);
	});

	resourceManager->LoadResourcesAsync(config, Core, [&]()
	{
		eventSystem->EmitEventQueued("ResourceLoadComplete", GENERIC, nullptr, gameInstance);
	});
}

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

	//Initialize other items, register callbacks. 
	Core->SetOnResizeCallback([&](int width, int height)->void
	{
		this->OnResizeCallback(width, height);
	});

	console = std::unique_ptr<Console>(new Console(Core));
	Core->SetOnKeyPressCallback([&](char key)->void
	{
		console->OnKeyPress(key);
	});
	mouse = new Mouse(Core->GetWindowHandle());
	Core->BindMouse(mouse);
	worker.Start(); //Should be called before resource manager as it depends on worker threads.
	resourceManager->SetAsyncWorker(&worker);
	//resourceManager->LoadResources(config, Core);

	debugDraw = std::unique_ptr<DebugDraw>(new DebugDraw(Core));
	eventSystem = new EventSystem();
	RegisterConsoleCommands();
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
	asyncLoader = std::unique_ptr<AsyncLoader>(new AsyncLoader(&worker));
	resourceManager = new ResourceManager();
	resourceManager->SetAsyncLoader(asyncLoader.get());
	gravity = rp3d::Vector3(0, -9.81f, 0);
	dynamicsWorld = new rp3d::DynamicsWorld(gravity);
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
	gravity = rp3d::Vector3(0, -9.81f, 0);
	dynamicsWorld = new rp3d::DynamicsWorld(gravity);
	mouse = nullptr;
}

/// <summary>
/// Core Game destructor
/// </summary>
CoreGame::~CoreGame()
{
	worker.Stop();
	if (Core) delete Core;
	if (gameInstance) delete gameInstance;
	if (renderer) delete renderer;
	if (keyboard) delete keyboard;
	if (saveSystem) delete saveSystem;
	if (mouse) delete mouse;
	if (resourceManager) delete resourceManager;
	if (dynamicsWorld) delete dynamicsWorld;
	if (eventSystem) delete eventSystem;
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
	gameInstance->SetPhysics(gravity, dynamicsWorld);
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
	auto events = EventSystem::GetInstance();
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
				if (console->enabled)console->Update(deltaTime); //Update console if console is enabled.
				events->ProcessQueuedEvents(GENERIC); // Process all generic events
				gameInstance->GetCamera()->Update(deltaTime); //Update game's current active camera
				gameInstance->Update(deltaTime); //Call update of game instance itself
				gameInstance->UpdateEntities(deltaTime); //Update all entities of game instance
				Draw(); //Render scene
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

void CoreGame::RegisterConsoleCommands()
{
	console->RegisterCommand("Rebind", [&](std::vector<std::string> params)
	{
		if (params.size() < 2)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}
		auto action = params[0];
		params.erase(params.begin());
		auto keys = keyboard->GetKeyEnumValue(params);
		keyboard->AddAction(action, keys);
	},
	{
		"Syntax: Rebind actionName key1 key2 ..."
	});

	console->RegisterCommand("ToggleDebug", [&](std::vector<std::string> params)
	{
		if (params.size() != 2)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}
		auto group = params[0];
		auto active = std::stoi(params[1].c_str());
		debugDraw->SetGroupActive(group, active);
	},
	{
		"Syntax: ToggleDebug groupName 0",
		"The value can be 0(false) or 1(true)."
	});

	console->RegisterCommand("SetDebugDraw", [&](std::vector<std::string> params)
	{
		if (params.size() != 1)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}
		auto enable = std::stoi(params[0].c_str());
		debugDraw->SetEnabled(enable);
	},
	{
		"Syntax: SetDebugDraw 0",
		"Set 0 to disable, 1 to enable Debug Draw."
	});

	console->RegisterCommand("Instantiate", [&](std::vector<std::string> params)
	{
		if (params.size() < 2)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}
		auto shapeName = params[0];
		auto uniqueName = params[1];
		auto shapeMesh = resourceManager->GetMesh(shapeName);
		if (shapeMesh == nullptr)
		{
			console->WriteLine(L"Invalid shape");
			return;
		}
		auto entity = new Entity(shapeMesh, resourceManager->GetMaterial("default"), rp3d::Vector3(0, 0, 0), gameInstance->GetDynamicsWorld());
		gameInstance->AddEntity(entity, uniqueName);

		if (params.size() > 2)
		{
			auto collider = params[2];
			
			if (collider == "Box")
			{
				entity->CreateBoxCollider(rp3d::Vector3(0.5f, 0.5f, 0.5f));
			}
			else if (collider == "Sphere")
			{
				entity->CreateSphereCollider(1);
			}
			entity->SetRigidBodyParameters(true);
		}
	},
	{
		"Syntax: Instantiate meshName entityName",
		"This will instantiate an entity with given mesh and entity name in the scene.",
		"Note: It is assumed that given mesh has been loaded as a resource."
	});

	console->RegisterCommand("SetPosition", [&](std::vector<std::string> params)
	{
		if (params.size() != 4)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}

		auto entityName = params[0];
		float x = (float)std::atof(params[1].c_str());
		float y = (float)std::atof(params[2].c_str());
		float z = (float)std::atof(params[3].c_str());

		auto entity = gameInstance->GetEntity(entityName);
		if (entity == nullptr)
		{
			console->WriteLine(L"Entity with given name not found");
			return;
		}
		entity->SetPosition(x, y, z);
	},
	{
		"Syntax: SetPosition entityName x y z",
		"Where x,y and z are decimal/float values."
	});

	console->RegisterCommand("SetRotation", [&](std::vector<std::string> params)
	{
		if (params.size() != 4)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}

		auto entityName = params[0];
		float x = (float)std::atof(params[1].c_str());
		float y = (float)std::atof(params[2].c_str());
		float z = (float)std::atof(params[3].c_str());

		auto entity = gameInstance->GetEntity(entityName);
		if (entity == nullptr)
		{
			console->WriteLine(L"Entity with given name not found", Error);
			return;
		}
		entity->SetRotation(x, y, z);
	},
	{
		"Syntax: SetRotation entityName x y z",
		"Where x,y and z are decimal/float values."
	});

	console->RegisterCommand("SetScale", [&](std::vector<std::string> params)
	{
		if (params.size() != 4)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}

		auto entityName = params[0];
		float x = (float)std::atof(params[1].c_str());
		float y = (float)std::atof(params[2].c_str());
		float z = (float)std::atof(params[3].c_str());

		auto entity = gameInstance->GetEntity(entityName);
		if (entity == nullptr)
		{
			console->WriteLine(L"Entity with given name not found", Error);
			return;
		}
		entity->SetScale(x, y, z);
	},
	{
		"Syntax: SetScale entityName x y z",
		"Where x,y and z are decimal/float values."
	});

	console->RegisterCommand("SetMaterial", [&](std::vector<std::string> params)
	{
		if (params.size() != 2)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}

		auto entityName = params[0];
		auto materialName = params[1];
		auto material = resourceManager->GetMaterial(materialName);

		auto entity = gameInstance->GetEntity(entityName);
		if (entity == nullptr || material == nullptr)
		{
			console->WriteLine(L"Entity or Material not found", Error);
			return;
		}
		entity->SetMaterial(material);
	},
	{
		"Syntax: SetMaterial entityName materialName",
		"This will apply given material to given entity.",
		"It is assumed that given material is loaded as a resource"
	});

	console->RegisterCommand("SetWireframe", [&](std::vector<std::string> params)
	{
		if (params.size() != 1)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}
		auto enable = std::stoi(params[0].c_str());
		debugDraw->SetWireframeDrawEnabled(enable);
	},
	{
		"Syntax: SetWireframe 0",
		"Set 0 to disable, 1 to enable wireframe rendering"
	});

	console->RegisterCommand("Remove", [&](std::vector<std::string> params)
	{
		if (params.size() != 1)
		{
			console->WriteLine(L"Invalid params supplied", Error);
			return;
		}
		auto eName = params[0];
		gameInstance->RemoveEntity(eName);
	},
	{
		"Syntax: Remove entityName",
		"Removes given entity."
	});

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
	debugDraw->DrawAsWireframeIfEnabled();
	for (auto entity : entities)
	{
		renderer->Draw(entity);
		if (debugDraw->IsEnabled())entity->DrawDebugShape();
	}
	if (gameInstance->IsResourcesInitialized() && gameInstance->GetSkybox() != nullptr)
	{
		renderer->Draw(gameInstance->GetSkybox());
	}
	if (debugDraw->IsEnabled())debugDraw->Render(gameInstance->GetCamera()); //Debug Draw
	if (gameInstance->GetCanvas() != nullptr)
	{
		gameInstance->GetCanvas()->Render();
	}
	if (console->enabled)console->Render(); //Render console if required.
	Core->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderer->Present();
}

/// <summary>
/// Updates the timer to calculate delta time and total time passed.
/// </summary>
void CoreGame::UpdateTimer() // This method taken mostly from Chris Cascioli's DX11 Engine
{
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	currentTime = now;
	deltaTime = std::max((float)((currentTime - previousTime) * perfCounterSeconds), 0.0f);
	totalTime = (float)((currentTime - startTime) * perfCounterSeconds);
	previousTime = currentTime;
	fpsFrameCount++;

	// Only calc FPS and update title bar once per second
	float timeDiff = totalTime - fpsTimeElapsed;
	if (timeDiff < 1.0f)
		return;

	// How long did each frame take?  (Approx)
	float mspf = 1000.0f / (float)fpsFrameCount; std::ostringstream output;
	output.precision(6);
	output << screenTitle <<
		"    FPS: " << fpsFrameCount <<
		"    Frame Time: " << mspf << "ms";

	// Actually update the title bar and reset fps data
	SetWindowText(Core->GetWindowHandle(), output.str().c_str());
	fpsFrameCount = 0;
	fpsTimeElapsed += 1.0f;
}