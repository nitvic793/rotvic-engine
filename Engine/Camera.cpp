/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "Camera.h"
#define NOMINMAX
#include "Mouse.h"
#include <Windows.h>
#include <algorithm>
#include "Console.h"

void Camera::SetPosition(XMFLOAT3 pos)
{
	position = pos;
}

XMFLOAT3 Camera::GetPosition()
{
	return position;
}

void Camera::RotateX(float x)
{
	rotation.x += x;
}

void Camera::RotateY(float y)
{
	rotation.y += y;
}

void Camera::SetRotation(XMFLOAT3 rotation)
{
	this->rotation = rotation;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	auto rotQuaternion = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0);
	XMVECTOR dir = XMVectorSet(direction.x, direction.y, direction.z, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	dir = XMVector3Rotate(dir, rotQuaternion);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::SetProjectionMatrix(float aspectRatio)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI,
		aspectRatio,
		0.1f,
		800.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

void Camera::Update(float deltaTime)
{
	auto console = Console::GetInstance();
	float speed = 10.f;
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0);
	XMVECTOR dir = XMVectorSet(direction.x, direction.y, direction.z, 0);
	auto rotQuaternion = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	dir = XMVector3Rotate(dir, rotQuaternion);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0); // Y is up!

	if (console->enabled) return;

	if (keyboard->IsActionPressed("up"))
	{
		pos = pos + dir * speed * deltaTime;;
	}

	if (keyboard->IsActionPressed("down"))
	{
		pos = pos - dir * speed * deltaTime;;
	}

	if (keyboard->IsActionPressed("left"))
	{
		auto leftDir = XMVector3Cross(dir, up);
		pos = pos + leftDir * speed * deltaTime;;
	}

	if (keyboard->IsActionPressed("right"))
	{
		auto rightDir = XMVector3Cross(-dir, up);
		pos = pos + rightDir * speed * deltaTime;;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		pos = pos + XMVectorSet(0, speed * deltaTime, 0, 0);
	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		pos = pos + XMVectorSet(0, -speed * deltaTime, 0, 0);
	}

	XMStoreFloat3(&position, pos);
}

void Camera::OnMouseMove(WPARAM wParam, int x, int y)
{
	if (isMouseDown)
	{
		rotation.y += (float)(x - mousePrevX) * 0.1f * XM_PI / 180;
		rotation.x += (float)(y - mousePrevY) * 0.1f * XM_PI / 180;
	}
	mousePrevX = x;
	mousePrevY = y;
}

void Camera::OnMouseUp(WPARAM wParam, int x, int y)
{
	isMouseDown = false;
}

void Camera::OnMouseDown(WPARAM wParam, int x, int y)
{
	isMouseDown = true;
}

Camera::Camera(float aspectRatio)
{
	mousePrevX = mousePrevY = 0;
	isMouseDown = false;
	position = XMFLOAT3(0.f, 0.f, -8.f);
	direction = XMFLOAT3(0.f, 0.f, 1.f);
	rotation = XMFLOAT3(0.f, 0.f, 0.f);
	rotationX = rotationY = 0.f;

	XMVECTOR pos = XMVectorSet(0, 0, -8, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));

	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI,		// Field of View Angle
		aspectRatio,		// Aspect ratio
		0.1f,						// Near clip plane distance
		800.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
	auto mouse = Mouse::GetInstance();
	mouse->RegisterOnMouseMoveCallback([&](WPARAM wParam, int x, int y)
	{
		OnMouseMove(wParam, x, y);
	});

	mouse->RegisterOnButtonUpCallback([&](WPARAM wParam, int x, int y)
	{
		OnMouseUp(wParam, x, y);
	});

	mouse->RegisterOnButtonDownCallback([&](WPARAM wParam, int x, int y)
	{
		OnMouseDown(wParam, x, y);
	});
	keyboard = Keyboard::GetInstance();
	keyboard->AddAction("up", { Up, W });
	keyboard->AddAction("left", { Left, A });
	keyboard->AddAction("right", { Right, D });
	keyboard->AddAction("down", { Down, S });
}


Camera::~Camera()
{
}
