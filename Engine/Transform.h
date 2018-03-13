#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

class Transform
{
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
public:
	void SetRotation(float roll, float pitch, float yaw);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	const XMFLOAT3 &GetPosition();
	const XMFLOAT3 &GetScale();
	const XMFLOAT3 &GetRotation();
	const XMFLOAT4X4 &GetWorldMatrix();
	
	Transform();
	~Transform();
};

