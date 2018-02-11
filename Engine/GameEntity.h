#pragma once

#include "Entity.h"
#include "Rect.h"

/// <summary>
/// Game Entity class represents an object in game. 
/// </summary>
class GameEntity : public Entity
{
	Rect rect;
public:
	virtual void MoveUp(float) ;
	virtual void MoveDown(float);
	virtual void MoveLeft(float);
	virtual void MoveRight(float);
	Rect GetRect();
	void SetRect(const Rect& rect);
	GameEntity();
	~GameEntity();
};

