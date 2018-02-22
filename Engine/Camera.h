#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	float rotationY;
	float rotationX;
public:
	XMFLOAT3 GetPosition();
	void RotateX(float x);
	void RotateY(float y);
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	void SetProjectionMatrix(float aspectRatio);
	virtual void Update(float deltaTime);
	Camera(float aspectRatio);
	~Camera();
};
