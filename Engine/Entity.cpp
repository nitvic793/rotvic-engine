#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

const Vector3f &Entity::GetPosition()
{
	return Position;
}

void Entity::Move(const Vector3f& offset)
{
	Position = Position + offset;
}

/**
Stubs
**/

void Entity::MoveUp(float offset)
{

}

void Entity::MoveDown(float offset)
{

}

void Entity::MoveLeft(float offset)
{

}

void Entity::MoveRight(float offset)
{

}

/**
End of Stubs
**/