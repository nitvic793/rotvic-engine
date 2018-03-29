#pragma once
#define NOMINMAX
#include "Core.h"

const float DEFAULTSPEED = 20.f;

/// <summary>
/// The player entity class. Represents the player in the game. 
/// </summary>
class PlayerEntity : public GameEntity, public ICollider2D
{
	float speed;
	Vector3f target;
	bool hasTarget;
	bool inCollision;
	GameEntity *collisionEntity = nullptr;
	Vector3f prevPosition;
public:
	virtual void Update(float deltaTime) override;
	void SendInput(Keys key, float deltaTime);
	PlayerEntity(float speed = DEFAULTSPEED);
	~PlayerEntity();

	// Inherited via ICollider2D
	virtual void OnCollision(GameEntity * entity) override;
};

