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
/// Initialize game
/// </summary>
void Game::Initialize()
{
	firstPersonCamera = new FirstPersonCamera((float)renderer->screenWidth / renderer->screenHeight);
	freeCam = camera;
	light.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 0);
	light.DiffuseColor = XMFLOAT4(0.4f, 0.4f, 0.9f, 1.f);
	light.Direction = XMFLOAT3(1.f, 0, 0.f);

	secondaryLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 0);
	secondaryLight.DiffuseColor = XMFLOAT4(0.9f, 0.4f, 0.4f, 1);
	secondaryLight.Direction = XMFLOAT3(0, -1, 0);

	pointLight.Color = XMFLOAT4(0.4f, 0.9f, 0.4f, 1);
	pointLight.Position = XMFLOAT3(0, 0, 0);

	lightsMap.insert(std::pair<std::string, Light*>("light", new Light{ &light, Directional }));
	lightsMap.insert(std::pair<std::string, Light*>("secondaryLight", new Light{ &secondaryLight, Directional }));
	lightsMap.insert(std::pair<std::string, Light*>("pointLight", new Light{ &pointLight, Point }));
	skybox = new Skybox(resource->GetMesh("cube"), resource->vertexShaders["sky"], resource->pixelShaders["sky"], resource->GetTexture("skybox"), core->GetDevice());
	LoadLevel();
}

/// <summary>
/// Loads the level. 
/// </summary>
void Game::LoadLevel()
{
	console->WriteLine(L"Level loaded");

	/* Add Entitites */
	auto entity = new Entity(resource->GetMesh("man"), resource->GetMaterial("man"), rp3d::Vector3(0, 0, 5), dynamicsWorld);
	entity->isAnimated = true;
	entity->SetScale(0.03,0.03,0.03);
	entity->SetRotation(0, 3.14, 0);
	AddEntity(entity, "man");

	{auto entity = new Entity(resource->GetMesh("cylinder"), resource->GetMaterial("metal"), rp3d::Vector3(2, 0, 0), dynamicsWorld, { new Flocker() });
	entity->CreateCylinderCollider(.5, 1);
	AddEntity(entity, "Flocker1");  // Flocker 1
	//entity->SetRigidBodyParameters(true); 
	dynamic_cast<Flocker*>(entities["Flocker1"]->scripts[0])->Init(entities["Flocker1"], camera, &centroidForward, &centroidPosition, &entities);

	entity = new Entity(resource->GetMesh("cylinder"), resource->GetMaterial("metal"), rp3d::Vector3(0, 0, 0), dynamicsWorld, { new Flocker() });
	entity->CreateCylinderCollider(.5, 1);
	AddEntity(entity, "Flocker2");
	//entity2->SetRigidBodyParameters(true);
	dynamic_cast<Flocker*>(entities["Flocker2"]->scripts[0])->Init(entities["Flocker2"], camera, &centroidForward, &centroidPosition, &entities);

	entity = new Entity(resource->GetMesh("cylinder"), resource->GetMaterial("metal"), rp3d::Vector3(-2, 0, 0), dynamicsWorld, { new Flocker() });
	entity->CreateCylinderCollider(.5, 1);
	AddEntity(entity, "Flocker3");
	//entity2->SetRigidBodyParameters(true);
	dynamic_cast<Flocker*>(entities["Flocker3"]->scripts[0])->Init(entities["Flocker3"], camera, &centroidForward, &centroidPosition, &entities);

	entity = new Entity(resource->GetMesh("cylinder"), resource->GetMaterial("metal"), rp3d::Vector3(0, 0, 2), dynamicsWorld, { new Flocker() });
	entity->CreateCylinderCollider(.5, 1);
	AddEntity(entity, "Flocker4");
	//entity2->SetRigidBodyParameters(true);
	dynamic_cast<Flocker*>(entities["Flocker4"]->scripts[0])->Init(entities["Flocker4"], camera, &centroidForward, &centroidPosition, &entities);

	entity = new Entity(resource->GetMesh("cylinder"), resource->GetMaterial("metal"), rp3d::Vector3(0, 0, -2), dynamicsWorld, { new Flocker() });
	entity->CreateCylinderCollider(.5, 1);
	AddEntity(entity, "Flocker5");
	//entity2->SetRigidBodyParameters(true);
	dynamic_cast<Flocker*>(entities["Flocker5"]->scripts[0])->Init(entities["Flocker5"], camera, &centroidForward, &centroidPosition, &entities);
	}

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
	AddEntity(entity, "Gravity2");  // Gavrity-laden body 2
	entity->SetRigidBodyParameters(true);

	

	auto terrain = new Terrain(core, dynamicsWorld);
	terrain->Initialize("../../Assets/Terrain/heightmap.bmp");
	terrain->SetMaterial(resource->GetMaterial("grass"));
	terrain->SetPosition(-70, -12, -10);
	AddEntity(terrain, "Terrain");

	terrain = new Terrain(core, dynamicsWorld);
	terrain->SetMaterial(resource->GetMaterial("grass"));
	terrain->_temp_Init();
	AddEntity(terrain, "Terrain2");

	flocking = true;
	console->RegisterCommand("ClearLevel", [=](std::vector<std::string> params)
	{
		flocking = false;
		for (auto entity : entities)
		{
			delete entity.second;
		}
		entities.clear();
		vEntities.clear();
	});

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
		camera = camera == freeCam ? firstPersonCamera : freeCam;
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
	std::ostringstream flockerindex;
	if (flocking)
	{
		for (int i = 1; i < 6; i++) // Update the flocking variables
		{
			flockerindex = std::ostringstream();
			flockerindex << "Flocker" << i;
			centroidForward += entities[flockerindex.str()]->GetForward(); // Add all the directions and positions
			centroidPosition += entities[flockerindex.str()]->GetPosition();
		}
		centroidForward = centroidForward / 5.0f; // Then divide them by the number of Daleks to compute the average
		centroidPosition = centroidPosition / 5.0f;
		entities["Collider2"]->ApplyForce(rp3d::Vector3(-.1, 0, 0));
	}

	//camera = firstPersonCamera;
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
	DebugDraw::Draw<Frustum>(fr, "Test");
	DebugDraw::Draw<Box>(box, "Collision");
	DebugDraw::Draw<Ray>(ray);
	DebugDraw::Draw<Sphere>(sphere, "Collision");
	DebugDraw::Draw<Grid>(grid, "Collision");
	DebugDraw::Draw<Cylinder>(cyl, "Collision");

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

	if (keyboard->IsKeyPressed(Z) && delayTime > 0.4f)
	{
		delayTime = 0.f;
		//resource->blendWeight = 1.0f;
		isAnimationTransitioning = true;
		animTransitionDirection = !animTransitionDirection;
	}
	/*
	if (keyboard->IsKeyPressed(C) && delayTime > 0.2f)
	{
		delayTime = 0.f;
		//resource->blendWeight = 0.0f;
		isAnimationTransitioning = true;
		animTransitionDirection = false;
	}
	*/

	if (isAnimationTransitioning)
	{
		if(animTransitionDirection)
		{ 
			if (resource->blendWeight > 1.0f)
			{
				resource->blendWeight = 1.0f;
				isAnimationTransitioning = false;
			}
			resource->blendWeight += 0.04f;
		}
		else
		{
			if (resource->blendWeight < 0.0f)
			{
				resource->blendWeight = 0.0f;
				isAnimationTransitioning = false;
			}
			resource->blendWeight -= 0.04f;
		}
	}

	delayTime += deltaTime;
}


