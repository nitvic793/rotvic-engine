#pragma once
#include "Core.h"

class Flocker : Script
{
public:
	Flocker();
	~Flocker();

	void Start(Entity* parent); // Called to do initialization
	void Update(float deltaTime); // Called by the parent GameObject each frame
};

