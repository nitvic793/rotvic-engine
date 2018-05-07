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
	void LoadEntity();
	void UnloadEntity();
	bool IsInitialized();
	void Update(float deltaTime) override;
	void SetProximityLoadTarget(Entity* entity, float threshold = 10.f);
	AsyncEntity(std::string meshName, rp3d::Vector3 position, rp3d::DynamicsWorld* world, std::vector<Script*> scripts = std::vector<Script*>());
	~AsyncEntity();
};

