#pragma once

#include "Core.h"
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


