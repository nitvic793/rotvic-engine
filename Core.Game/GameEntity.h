#pragma once
#include "../Engine/Engine.h"

class GameEntity : public Entity
{

public:
	void MoveUp(float);
	void MoveDown(float);
	void MoveLeft(float);
	void MoveRight(float);
	GameEntity();
	~GameEntity();
};

