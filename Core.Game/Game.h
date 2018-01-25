#pragma once

#include "Core.h"

class Game
{
protected:
	static int InstanceCount;
public:
	static Game* CreateInstance();
	static int GetInstanceCount();
	Game();
	~Game();
};


