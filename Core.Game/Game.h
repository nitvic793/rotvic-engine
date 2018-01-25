#pragma once

#include "Core.h"
#include <map>

const float DefaultSpeed = 0.01f;

class Game : public IGame
{
protected:
	static int InstanceCount;
	std::map<std::string, GameEntity*> entities;
	float speed;
	std::vector<Entity*> vEntities;
public:
	static Game* CreateInstance();
	static int GetInstanceCount();
	void SetSpeed(float);
	const float& GetSpeed();
	void AddEntity(GameEntity *entity, std::string entityName);
	void Initialize();

	virtual void SendInput(Keys key, std::string entityName);
	virtual void Update();
	virtual std::vector<Entity*> GetEntities();
	Game(float speed = DefaultSpeed);
	~Game();
};


