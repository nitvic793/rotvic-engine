#include "Transform.h"


/// <summary>
/// Set Euler Angles
/// </summary>
/// <param name="roll"></param>
/// <param name="pitch"></param>
/// <param name="yaw"></param>
void Transform::SetRotation(float roll, float pitch, float yaw)
{
	this->rotation.x = roll;
	this->rotation.y = pitch;
	this->rotation.z = yaw;
}

void Transform::SetPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Transform::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
}

const XMFLOAT3 &Transform::GetPosition()
{
	return position;
}

const XMFLOAT3 &Transform::GetScale()
{
	return scale;
}

const XMFLOAT3 &Transform::GetRotation()
{
	return rotation;
}

const XMFLOAT4X4 &Transform::GetWorldMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
	XMMATRIX scle = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX world = scle * rot * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));
	return worldMatrix;;
}

Transform::Transform()
{
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR sc = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMStoreFloat3(&position, v);
	XMStoreFloat3(&scale, sc);
	XMStoreFloat3(&rotation, v);
}


Transform::~Transform()
{
}
