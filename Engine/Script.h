#pragma once
class Entity; // Eliminates circular dependency
// All code in this file written by Trevor Walden
class Script // Generic script class that all scripts inherit from
{
public:
	Script();
	~Script();

	virtual void Start(Entity* parent); // Called to do initialization
	virtual void Update(float deltaTime); // Called by the parent GameObject each frame
};