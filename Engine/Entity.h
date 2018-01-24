#pragma once
#include "Vector3.h"

class Entity
{
	Vector3f Position;
public:
	const Vector3f &GetPosition();
	Entity();
	~Entity();
};

