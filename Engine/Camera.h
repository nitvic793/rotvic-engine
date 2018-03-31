#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include <Windows.h>
#include "Keyboard.h"

using namespace DirectX;

class Camera
{
protected:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	float rotationY;
	float rotationX;
	bool isMouseDown;
	int mousePrevX;
	int mousePrevY;
	Keyboard* keyboard;
public:
	XMFLOAT3 GetPosition();
	void RotateX(float x);
	void RotateY(float y);
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	void SetProjectionMatrix(float aspectRatio);
	virtual void Update(float deltaTime);
	virtual void OnMouseMove(WPARAM wParam, int x, int y);
	virtual void OnMouseUp(WPARAM wParam, int x, int y);
	virtual void OnMouseDown(WPARAM wParam, int x, int y);
	Camera(float aspectRatio);
	~Camera();
};
