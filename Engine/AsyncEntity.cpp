#include "AsyncEntity.h"
#include "EventSystem.h"


void AsyncEntity::LoadEntity()
{
	if (isBusy)return;
	isBusy = true;
	auto eventType = "FinishLoadingEntity";
	auto events = EventSystem::GetInstance();
	events->RegisterEventCallback(eventType, this, [&](void*) {
		isInit = true;
		isBusy = false;
	});

	rm->LoadMeshAsync(meshName, eventType, this);
}

void AsyncEntity::UnloadEntity()
{
	if (isBusy)return;
	isBusy = true;
	auto eventType = "FinishUnLoadingEntity";
	auto events = EventSystem::GetInstance();
	events->RegisterEventCallback(eventType, this, [&](void*) {
		isInit = false;
		isBusy = false;
	});

	rm->UnloadMeshAsync(meshName, eventType, this);
}

bool AsyncEntity::IsInitialized()
{
	return isInit;
}

void AsyncEntity::Update(float deltaTime)
{
	if (proximityTarget == nullptr || isBusy) return;
	auto distance = (proximityTarget->GetPosition() - GetPosition()).length();
	if (distance < distanceThreshold)
	{
		if (!isBusy && !isInit)LoadEntity();
	}
	else if (isInit)
	{
		if (!isBusy)UnloadEntity();
	}
}

void AsyncEntity::SetProximityLoadTarget(Entity* entity, float threshold)
{
	distanceThreshold = threshold;
	proximityTarget = entity;
}

AsyncEntity::AsyncEntity(std::string meshName, rp3d::Vector3 position, rp3d::DynamicsWorld* world, std::vector<Script*> scripts)
	:Entity(nullptr, nullptr, position, world, scripts)
{
	isBusy = false;
	isInit = false;
	this->meshName = meshName;
	rm = ResourceManager::GetInstance();
	LoadEntity();
}


AsyncEntity::~AsyncEntity()
{
}
