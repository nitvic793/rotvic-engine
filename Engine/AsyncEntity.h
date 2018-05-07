#pragma once
#include "Entity.h"
#include "ResourceManager.h"

class AsyncEntity :
	public Entity
{
	Entity* proximityTarget;
	std::string meshName;
	ResourceManager* rm;
	bool isInit;
	bool isBusy = false;
	float distanceThreshold;
public:
	/// <summary>
	/// Lo
	/// </summary>
	void LoadEntity();
	void UnloadEntity();
	bool IsInitialized();
	void Update(float deltaTime) override;

	/// <summary>
	/// Sets the proximity load target. Loads or unloads entity depending on distance from player. 
	/// </summary>
	/// <param name="entity"></param>
	/// <param name="threshold"></param>
	void SetProximityLoadTarget(Entity* entity, float threshold = 10.f);

	AsyncEntity(std::string meshName, rp3d::Vector3 position, rp3d::DynamicsWorld* world, std::vector<Script*> scripts = std::vector<Script*>());
	~AsyncEntity();
};

