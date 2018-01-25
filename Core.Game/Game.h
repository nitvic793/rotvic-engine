#pragma once

#include "Core.h"

class Game
{
protected:
	static int InstanceCount;
	GameEntity entity;
	float speed;
public:
	static Game* CreateInstance();
	static int GetInstanceCount();
	void SendInput(Keys key);
	Game();
	~Game();
};


