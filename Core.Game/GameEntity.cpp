#include "GameEntity.h"



GameEntity::GameEntity()
{
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
