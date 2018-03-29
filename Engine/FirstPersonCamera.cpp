#include "FirstPersonCamera.h"
#include "Console.h"

void FirstPersonCamera::Update(float deltaTime)
{
	auto console = Console::GetInstance();
	float speed = 10.f;
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0);
	XMVECTOR dir = XMVectorSet(direction.x, direction.y, direction.z, 0);
	auto rotQuaternion = XMQuaternionRotationRollPitchYaw(0, rotationY, 0);
	dir = XMVector3Rotate(dir, rotQuaternion);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0); // Y is up!

	if (console->enabled) return;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		pos = pos + dir * speed * deltaTime;;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		pos = pos - dir * speed * deltaTime;;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		auto leftDir = XMVector3Cross(dir, up);
		pos = pos + leftDir * speed * deltaTime;;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		auto rightDir = XMVector3Cross(-dir, up);
		pos = pos + rightDir * speed * deltaTime;;
	}

	XMStoreFloat3(&position, pos);
}

void FirstPersonCamera::OnMouseMove(WPARAM wParam, int x, int y)
{
	rotationY += (float)(x - mousePrevX) * 0.6f * XM_PI / 180;
	rotationX += (float)(y - mousePrevY) * 0.6f * XM_PI / 180;

	mousePrevX = x;
	mousePrevY = y;
}

FirstPersonCamera::FirstPersonCamera(float aspectRatio):
	Camera(aspectRatio)
{
}


FirstPersonCamera::~FirstPersonCamera()
{
}