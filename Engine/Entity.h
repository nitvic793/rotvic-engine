#pragma once
#include "Vector3.h"

class Entity
{
protected:
	Vector3f Position;
public:
	virtual void MoveUp(float);
	virtual void MoveDown(float);
	virtual void MoveLeft(float) ;
	virtual void MoveRight(float);
	const Vector3f &GetPosition();
	void Move(const Vector3f& offset);
	Entity();
	~Entity();
};

