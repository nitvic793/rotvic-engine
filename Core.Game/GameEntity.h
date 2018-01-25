#pragma once
#include "../Engine/Engine.h"

class GameEntity : public Entity
{

public:
	virtual void MoveUp(float) ;
	virtual void MoveDown(float);
	virtual void MoveLeft(float);
	virtual void MoveRight(float);
	GameEntity();
	~GameEntity();
};

