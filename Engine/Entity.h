#pragma once
#include "Vector3.h"
#include "Mesh.h"

class Entity
{
protected:
	Vector3f Position;
	Mesh *mesh;
public:
	virtual void MoveUp(float);
	virtual void MoveDown(float);
	virtual void MoveLeft(float) ;
	virtual void MoveRight(float);
	const Vector3f &GetPosition();
	void SetPosition(const Vector3f& position);
	void Move(const Vector3f& offset);
	void SetMesh(Mesh *mesh);
	Mesh* GetMesh();
	Entity();
	~Entity();
};

