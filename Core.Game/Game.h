#pragma once
// Code written by Nitish Victor and Trevor Walden, rp3d integration and scripting written by Trevor Walden
#include "Core.h"
#include "Flocker.h"
#include <map>

const float DefaultSpeed = 20.0f;

/// <summary>
/// Implementation of IGame interface. All custom game logic goes here. 
/// </summary>
class Game : public IGame
{
protected:
	static int InstanceCount;
	float speed;
	DirectionalLight light;
	DirectionalLight secondaryLight;
	PointLight pointLight;
	float deltaTime;
	float delayTime;
	Mesh *mesh;
	Material *mat;
	rp3d::Vector3 centroidForward = rp3d::Vector3(0, 0, 0);
	rp3d::Vector3 centroidPosition = rp3d::Vector3(0, 0, 0);
	bool flocking;
	Camera* freeCam;
	FirstPersonCamera* firstPersonCamera;

public:
	static Game* CreateInstance();
	static int GetInstanceCount();
	void SetSpeed(float);
	const float& GetSpeed();

	void Initialize();

	virtual void LoadLevel();
	virtual bool Save();
	virtual bool Load();
	virtual void Update(float);
	Game(float speed = DefaultSpeed);
	~Game();
};


