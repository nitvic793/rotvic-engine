#pragma once
#include "Vector3.h"

class Entity
{
protected:
	Vector3f Position;
public:
	const Vector3f &GetPosition();
	void Move(const Vector3f& offset);
	Entity();
	~Entity();
};

