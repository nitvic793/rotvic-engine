#include "Entity.h"



Entity::Entity()
{
	mesh = nullptr;
	W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	pos = XMVectorSet(0, 0, -25, 0);
	dir = XMVectorSet(0, 0, 1, 0);
	up = XMVectorSet(0, 1, 0, 0);
	V = XMMatrixLookToLH(
		pos,
		dir,
		up);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		(float)1280 / 720,
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

XMFLOAT4X4 Entity::GetWorldMatrix() 
{
	auto newPos = XMMatrixMultiply(W, XMMatrixTranslation(Position.x, Position.y, Position.z));
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(newPos));
	return worldMatrix;
}

XMFLOAT4X4 Entity::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Entity::GetProjectionMatrix()
{
	return projectionMatrix;
}


Entity::~Entity()
{
	if (mesh) delete mesh;
}

const Vector3f &Entity::GetPosition()
{
	return Position;
}

void Entity::SetPosition(const Vector3f& position)
{
	Position = position;
}

void Entity::Move(const Vector3f& offset)
{
	Position = Position + offset;
}

void Entity::SetMesh(Mesh *mesh)
{
	this->mesh = mesh;
}

Mesh* Entity::GetMesh()
{
	return mesh;
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