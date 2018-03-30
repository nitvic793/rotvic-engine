#pragma once
#define NOMINMAX
#include "Core.h"
#include "reactphysics3d.h"

const float DEFAULTSPEED = 20.f;

/// <summary>
/// The player entity class. Represents the player in the game. 
/// </summary>
class PlayerEntity : public Entity
{
	float speed;
	Vector3f target;
	bool hasTarget;
	bool inCollision;
	Vector3f prevPosition;
public:
	virtual void Update(float deltaTime) override;
	void SendInput(Keys key, float deltaTime);
	PlayerEntity(rp3d::DynamicsWorld* physicsWorld, float speed = DEFAULTSPEED);
	~PlayerEntity();

	// Inherited via ICollider2D
	//virtual void OnCollision(GameEntity * entity) override;
};

