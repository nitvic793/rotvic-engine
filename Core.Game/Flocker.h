#pragma once
#define NOMINMAX
#include "Core.h"
#include "reactphysics3d.h"

class Flocker : public Script
{
public:
	Flocker();
	~Flocker();
	Entity* gameObject;
	Camera* followObject;
	rp3d::Vector3* centroidForward;
	rp3d::Vector3* centroidPosition;
	std::map<std::string, Entity*>* entities;

	void Init(Entity* parent, Camera* follow, rp3d::Vector3* centroidFor, rp3d::Vector3* centroidPos, std::map<std::string, Entity*>* entitymap); // Called to do initialization
	void Update(float deltaTime); // Called by the parent GameObject each frame
protected:
	rp3d::Vector3 desired;
	rp3d::Vector3 steer;
	float maxSpeed = 1.5f;
	float maxForce = 1.5f;
	float followWeight = 5.0f; // Weights for following, alignment, cohesion, seperation, and obstacle avoidance
	float alignWeight = 1.2f;
	float cohereWeight = 1.0f;
	float separateWeight = 0.8f;
	float arrivalDist = 1.0f; // Radius and square for the arrival area around the follow point
	float arrivalDistSq;
	float desiredSeparation; // Separation variables
	float sepDist; // I also use this one for arrival
	rp3d::Vector3 sPartial; // Vectors for separation
	rp3d::Vector3 sTotal;
	rp3d::Vector3 steeringForce; // Force for steering
	rp3d::Vector3 separateForce; // Force for separation
};

