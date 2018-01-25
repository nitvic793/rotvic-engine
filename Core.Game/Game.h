#pragma once

#include "Core.h"
#include <map>

class Game : public IGame
{
protected:
	static int InstanceCount;
	std::map<std::string, GameEntity*> entities;
	float speed;
public:
	static Game* CreateInstance();
	static int GetInstanceCount();
	virtual void SendInput(Keys key, std::string entityName);
	void AddEntity(GameEntity *entity, std::string entityName);
	virtual void Update();
	Game();
	~Game();
};


