/// <summary>
/// Authors
/// Base Code: Nitish Victor
/// RP3D integration, Scripting, HUD loading: Trevor Walden
/// Animations: Rahul SV
/// 3D Model Credits:
///		Axe (player model) by Dota2
///		Weapon models by 3dregenerator at free3d.com
///		Shedinja (enemy model) by poke_master at free3d.com
/// </summary>
/// <remarks>
/// Code written by Nitish Victor and Trevor Walden, rp3d integration and scripting written by Trevor Walden.
/// Animation integration by Rahul SV
/// </remarks>

#include "Game.h"

int Game::InstanceCount = 0;

Game::Game(float speed)
{
	deltaTime = 0.f;
	delayTime = 0.f;
	this->speed = speed;
}

Game::~Game()
{
	camera = freeCam;
	delete skybox;
	delete firstPersonCamera;
	InstanceCount--;
	for (auto l : lightsMap)
	{
		delete l.second;
	}
	asyncWorker.Stop();
}

void Game::SetSpeed(float speed)
{
	this->speed = speed;
}

const float& Game::GetSpeed()
{
	return speed;
}

/// <summary>
/// Creates new game instance.
/// </summary>
/// <returns></returns>
Game* Game::CreateInstance()
{
	InstanceCount++;
	auto game = new Game();
	return game;
}

/// <summary>
/// Runs any pre-initialization logic required by the game. 
/// </summary>
void Game::PreInitialize()
{
	IGame::PreInitialize();
	auto spriteFont = new SpriteFont(core->GetDevice(), L"../../Assets/Fonts/segoeUI.spritefont");
	loadingText = std::unique_ptr<UIText>(
		new UIText(XMFLOAT3(
			(float)core->GetScreenWidth() - 250.f, 
			(float)core->GetScreenHeight() - 100.f, 0), 
			L"Loading...", 
			1, 
			spriteFont, 
			XMFLOAT4(255,255,255,255)));
	uiCanvas->AddComponent(loadingText.get(), "LoadingText");
}

/// <summary>
/// Initialize game
/// </summary>
void Game::Initialize()
{
	asyncWorker.Start();

	firstPersonCamera = new FirstPersonCamera((float)renderer->screenWidth / renderer->screenHeight);
	thirdPersonCamera = std::unique_ptr<ThirdPersonCamera>(new ThirdPersonCamera((float)renderer->screenWidth / renderer->screenHeight));
	freeCam = camera;
	light.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 0);
	light.DiffuseColor = XMFLOAT4(0.4f, 0.4f, 0.9f, 1.f);
	light.Direction = XMFLOAT3(-1.f, -1, 0.f);

	secondaryLight.AmbientColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 0);
	secondaryLight.DiffuseColor = XMFLOAT4(0.6f, 0.9f, 0.6f, 1);
	secondaryLight.Direction = XMFLOAT3(1, -1, 0);

	pointLight.Color = XMFLOAT4(0.5f, 0.5f, 0.6f, 1);
	pointLight.Position = XMFLOAT3(0, 2, 0);

	lightsMap.insert(std::pair<std::string, Light*>("light", new Light{ &light, Directional }));
	lightsMap.insert(std::pair<std::string, Light*>("secondaryLight", new Light{ &secondaryLight, Directional }));
	lightsMap.insert(std::pair<std::string, Light*>("pointLight", new Light{ &pointLight, Point }));
	skybox = new Skybox(resource->GetMesh("cube"), resource->vertexShaders["sky"], resource->pixelShaders["sky"], resource->GetTexture("skybox"), core->GetDevice());
	LoadLevel();
	LoadHUDFile("..\\..\\Assets\\GameHUD.json");
	hasLoaded = true;
	uiCanvas->RemoveComponent("LoadingText");
}

/// <summary>
/// Loads the level. 
/// </summary>
void Game::LoadLevel()
{
	console->WriteLine(L"Level loaded");

	/* Add Entitites */

	auto asyncEntity = new AsyncEntity("palm_tree.obj", rp3d::Vector3(-20, -8.65f, 1), dynamicsWorld);
	AddEntity(asyncEntity, "AsyncEntity");
	// Player Entity
	auto pEntity = new ThirdPersonPlayerEntity(resource->GetMesh("man"), resource->GetMaterial("man"), rp3d::Vector3(0, -2, 5), dynamicsWorld);
	asyncEntity->SetProximityLoadTarget(pEntity);
	pEntity->isAnimated = true;
	pEntity->fbx = resource->fbxLoader;
	pEntity->SetScale(0.03f, 0.03f, 0.03f);
	pEntity->SetRigidBodyParameters(true);
	pEntity->GetRigidBody()->setMass(0.1f);
	pEntity->GetRigidBody()->getMaterial().setFrictionCoefficient(1.1f);
	//entity->SetRotation(0, 3.14, 0);
	pEntity->CreateBoxCollider(rp3d::Vector3(1, 1, 1), rp3d::Vector3(0, 1, 0), "player");
	//entity->CreateCapsuleCollider(rp3d::decimal(1), rp3d::decimal(2));
	AddEntity(pEntity, "man");
	thirdPersonCamera->AttachEntity(pEntity);
	camera = thirdPersonCamera.get();

	// Weapon Entity
	auto wEntity = new WeaponEntity(resource->GetMesh("mace"), resource->GetMaterial("mace"), rp3d::Vector3(0, 0, 5), dynamicsWorld);
	wEntity->isWeapon = true;
	wEntity->AttachToEntity(pEntity, 20);
	wEntity->AddWeapon(resource->GetMesh("mace2"), resource->GetMaterial("mace2"));
	wEntity->AddWeapon(resource->GetMesh("mace3"), resource->GetMaterial("mace3"));
	wEntity->fbx = resource->fbxLoader;
	AddEntity(wEntity, "weapon");


	auto entity = new Entity(resource->GetMesh("bee"), resource->GetMaterial("bee"), rp3d::Vector3(0, -5, 25), dynamicsWorld, { new Flocker() });
	entity->isAnimated = true;
	entity->fbx = resource->enemyFBXLoader;
	//entity->SetPosition(0, 0, 25);
	entity->SetScale(0.03f, 0.03f, 0.03f);
	entity->SetRotation(-1.57f, 3.14f, 0);
	//entity->CreateBoxCollider(rp3d::Vector3(2, 2, 2));
	AddEntity(entity, "bee1");
	dynamic_cast<Flocker*>(entities["bee1"]->scripts[0])->Init(entities["bee1"], pEntity, &centroidForward, &centroidPosition, &entities);

	entity = new Entity(resource->GetMesh("bee"), resource->GetMaterial("bee"), rp3d::Vector3(4, -5, 25), dynamicsWorld, { new Flocker() });
	entity->isAnimated = true;
	entity->fbx = resource->enemyFBXLoader;
	entity->enemyInstanceNumber = 1;
	//entity->SetPosition(4, 0, 25);
	entity->SetScale(0.03f, 0.03f, 0.03f);
	entity->SetRotation(-1.57f, 3.14f, 0);
	AddEntity(entity, "bee2");
	dynamic_cast<Flocker*>(entities["bee2"]->scripts[0])->Init(entities["bee2"], pEntity, &centroidForward, &centroidPosition, &entities);

	entity = new Entity(resource->GetMesh("bee"), resource->GetMaterial("bee"), rp3d::Vector3(-4, -5, 25), dynamicsWorld, { new Flocker() });
	entity->isAnimated = true;
	entity->fbx = resource->enemyFBXLoader;
	entity->enemyInstanceNumber = 2;
	//entity->SetPosition(-4, 0, 25);
	entity->SetScale(0.03f, 0.03f, 0.03f);
	entity->SetRotation(-1.57f, 3.14f, 0);
	AddEntity(entity, "bee3");
	dynamic_cast<Flocker*>(entities["bee3"]->scripts[0])->Init(entities["bee3"], pEntity, &centroidForward, &centroidPosition, &entities);


	entity = new Entity(resource->GetMesh("sphere"), resource->GetMaterial("metal"), rp3d::Vector3(5, 5, 0), dynamicsWorld);
	entity->CreateSphereCollider(.5);
	AddEntity(entity, "Collider1");  // Collider 1
	//entity->SetRigidBodyParameters(true); 

	entity = new Entity(resource->GetMesh("cube"), resource->GetMaterial("metal"), rp3d::Vector3(7, 5, 0), dynamicsWorld);
	entity->CreateBoxCollider(rp3d::Vector3(.5, .5, .5));
	AddEntity(entity, "Collider2");  // Collider 2



	auto events = EventSystem::GetInstance();
	events->RegisterEventCallback("Collision", entity, [&](void* args)
	{

	});

	entity = new Entity(resource->GetMesh("sphere"), resource->GetMaterial("metal"), rp3d::Vector3(-4, 2, 1), dynamicsWorld);
	entity->CreateSphereCollider(.5);
	AddEntity(entity, "Gravity1");  // Gravity-laden body 1
	entity->SetRigidBodyParameters(true);

	entity = new Entity(resource->GetMesh("cube"), resource->GetMaterial("metal"), rp3d::Vector3(-2.5, 2, 0), dynamicsWorld);
	entity->CreateBoxCollider(rp3d::Vector3(.5, .5, .5));
	AddEntity(entity, "Gravity2");  // Gravity-laden body 2
	entity->SetRigidBodyParameters(true);

	//auto terrain = new Terrain(core, dynamicsWorld);
	//terrain->Initialize("../../Assets/Terrain/heightmap.bmp");
	//terrain->SetMaterial(resource->GetMaterial("grass"));
	//terrain->SetPosition(0, -12, 0);
	//AddEntity(terrain, "Terrain");

	auto terrain = new Terrain(core, dynamicsWorld);
	terrain->SetMaterial(resource->GetMaterial("grass"));
	terrain->_temp_Init();
	AddEntity(terrain, "Terrain2");

	flocking = true;
	console->RegisterCommand("AddTerrain", [=](std::vector<std::string> params)
	{
		if (params.size() < 1)
		{
			console->WriteLine(L"No params found");
			return;
		}
		auto terrainName = params[0];
		auto terrain = new Terrain(core, dynamicsWorld);
		terrain->Initialize("../../Assets/Terrain/heightmap.bmp");
		terrain->SetMaterial(resource->GetMaterial("grass"));
		terrain->SetPosition(-70, -12, -10);
		AddEntity(terrain, terrainName);
	});

	console->RegisterCommand("Unload", [=](std::vector<std::string> params)
	{
		asyncEntity->UnloadEntity();
	});

	console->RegisterCommand("Load", [=](std::vector<std::string> params)
	{
		asyncEntity->LoadEntity();
	});

	console->RegisterCommand("AddPhysicsTerrain", [=](std::vector<std::string> params)
	{
		if (params.size() < 1)
		{
			console->WriteLine(L"No params found");
			return;
		}
		auto terrainName = params[0];
		auto terrain = new Terrain(core, dynamicsWorld);
		terrain->SetMaterial(resource->GetMaterial("grass"));
		terrain->_temp_Init();
		AddEntity(terrain, terrainName);
	});

	console->RegisterCommand("SwitchCam", [=](std::vector<std::string> params)
	{
		camera = camera == freeCam ? thirdPersonCamera.get() : freeCam;
	});
}

/// <summary>
/// Save current game state.
/// </summary>
/// <returns></returns>
bool Game::Save()
{
#ifdef _DEBUG
	printf("\nSave state");
#endif
	SaveState state;
	//Save here
	saveSystem->Save(&state, "save.dat");
	return true;
}

/// <summary>
/// Load previous game state if available. 
/// </summary>
/// <returns></returns>
bool Game::Load()
{
	try
	{
#ifdef _DEBUG
		printf("\nLoad state");
#endif
		//Load Here
	}
	catch (std::exception)
	{
#ifdef _DEBUG
		printf("\nLoad save failed");
#endif
		return false;
	}
	return true;
}

int Game::GetInstanceCount()
{
	return InstanceCount;
}

/// <summary>
/// The game update function which is called in the core game loop. 
/// </summary>
/// <param name="deltaTime"></param>
void Game::Update(float deltaTime)
{
	Ray ray;
	ray.color = XMFLOAT4(1, 1, 1, 1);
	ray.origin = XMFLOAT3(0, 0, 0);
	ray.direction = XMFLOAT3(0, 1, 0);
	ray.length = 2.f;

	Sphere sphere;
	sphere.color = XMFLOAT4(1, 1, 1, 1);
	sphere.bounding.Center = XMFLOAT3(-1, 5, 0);
	sphere.bounding.Radius = 0.5;

	Box box;
	box.color = XMFLOAT4(1, 1, 1, 1);
	box.bounding.Center = XMFLOAT3(2, 5, 0);
	box.bounding.Extents = XMFLOAT3(1, 1, 1);

	Frustum fr;
	fr.color = XMFLOAT4(1, 1, 1, 1);
	fr.bounding.Origin = XMFLOAT3(-4, 5, 1);

	Cylinder cyl;
	cyl.color = XMFLOAT4(1, 1, 1, 1);
	cyl.Center = XMFLOAT4(0, 0, 0, 0);
	cyl.Height = 1.f;
	cyl.Radius = 0.5f;

	Grid grid = Grid::GetDefaultGrid();
	XMStoreFloat4(&grid.color, Colors::Green);

	if (hasLoaded)
	{
		std::ostringstream flockerindex;
		if (flocking)
		{
			for (int i = 1; i < 4; i++) // Update the flocking variables
			{
				flockerindex = std::ostringstream();
				flockerindex << "bee" << i;
				centroidForward += entities[flockerindex.str()]->GetForward(); // Add all the directions and positions
				centroidPosition += entities[flockerindex.str()]->GetPosition();
			}
			centroidForward = centroidForward / 5.0f; // Then divide them by the number of Daleks to compute the average
			centroidPosition = centroidPosition / 5.0f;

		}
		entities["Collider2"]->ApplyForce(rp3d::Vector3(-.1f, 0, 0));
		DebugDraw::Draw<Frustum>(fr, "Test");
		DebugDraw::Draw<Box>(box, "Collision");
		DebugDraw::Draw<Ray>(ray);
		DebugDraw::Draw<Sphere>(sphere, "Collision");
		DebugDraw::Draw<Cylinder>(cyl, "Collision");
	}
	else
	{
		DebugDraw::Draw<Grid>(grid, "Collision");
	}

	delayTime += deltaTime;
	if (keyboard->IsKeyPressed(Tilde) && delayTime > 0.2f)
	{
		delayTime = 0.f;
		console->enabled = !console->enabled;
	}

	if (keyboard->IsActionPressed("up") && delayTime > 0.2f)
	{
		delayTime = 0.f;
		//console->WriteLine(L"Up action pressed");
	}

	if (keyboard->IsKeyPressed(Z) && delayTime > 0.3f)
	{
		delayTime = 0.f;
		WeaponEntity * weapon = (WeaponEntity*)entities["weapon"];
		weapon->SwitchWeapon();
	}

	delayTime += deltaTime;
}


