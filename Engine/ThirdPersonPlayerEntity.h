#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include "reactphysics3d.h"
#include "Script.h"
#include "ResourceManager.h"

class ThirdPersonPlayerEntity :
	public Entity
{
	float rotationAngle = 0.f;
	rp3d::Vector3 forwardDir = rp3d::Vector3(0, 0, 0);
	bool isAnimationTransitioning = false;
	bool animTransitionDirection = true;
	Keyboard *keyboard;
	ResourceManager *resource;
	float maxSpeed = 4.f;
public:
	void Update(float deltaTime) override;
	rp3d::Vector3 GetForwardVector() const;
	ThirdPersonPlayerEntity(Mesh* m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* world, std::vector<Script*> scripts = std::vector<Script*>());
	~ThirdPersonPlayerEntity();
};

