#pragma once

#include "GameEntity.h"


/// <summary>
/// ICollider 2D class represents a collidable 2D object. 
/// This abstract class provides the collision callbacks to the entity. This class/interface should be inherited and implemented if collision callbacks are required.
/// </summary>
class ICollider2D 
{
public:
	virtual void OnCollision(GameEntity *entity) = 0;
};