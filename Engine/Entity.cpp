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
