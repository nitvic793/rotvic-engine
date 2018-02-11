#include "GameEntity.h"



GameEntity::GameEntity()
{
	rect.x = -1.5f;
	rect.y = 1.f;
	rect.width = 3;
	rect.height = 2;
}


GameEntity::~GameEntity()
{
}

void GameEntity::MoveUp(float offset)
{
	Position = Position + Vector3f(0.f, offset, 0.f);
}

void GameEntity::MoveDown(float offset)
{
	Position = Position + Vector3f(0.f, -offset, 0.f);
}

void GameEntity::MoveLeft(float offset)
{
	Position = Position + Vector3f(-offset, 0.f, 0.f);
}

void GameEntity::MoveRight(float offset)
{
	Position = Position + Vector3f(offset, 0.f, 0.f);
}

/// <summary>
/// Gets the bounding rectangle for entity.
/// </summary>
/// <returns></returns>
Rect GameEntity::GetRect()
{
	Rect tempRect = rect;
	tempRect.x = Position.x + rect.x;
	tempRect.y = Position.y + rect.y;
	return tempRect;
}

/// <summary>
/// Set the bounding rectangle for entity.
/// </summary>
/// <param name="rect"></param>
void GameEntity::SetRect(const Rect & rect)
{
	this->rect = rect;
}
