#pragma once
#include "Vector3.h"

class Entity
{
protected:
	Vector3f Position;
public:
	virtual void MoveUp(float) = 0;
	virtual void MoveDown(float) = 0;
	virtual void MoveLeft(float) = 0;
	virtual void MoveRight(float) = 0;
	const Vector3f &GetPosition();
	void Move(const Vector3f& offset);
	Entity();
	~Entity();
};

