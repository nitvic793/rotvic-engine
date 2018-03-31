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
	auto deltaX = float(x - mousePrevX);
	auto deltaY = float(y - mousePrevY);

	if (rotationX + deltaY >= (90 - 10) * XM_PI / 180)
	{
		deltaX *= 0.2f;
	}

	if (rotationX + deltaY <= -(90 - 10) * XM_PI / 180)
	{
		deltaX *= 0.2f;
	}

	rotationY += deltaX * 0.6f * XM_PI / 180;
	rotationX += deltaY * 0.6f * XM_PI / 180;
	if (rotationX >= XM_PI / 2) rotationX = XM_PI / 2;
	if (rotationX <= -XM_PI /2) rotationX = -XM_PI/2;
	printf("%f %f\n", rotationX * 180/XM_PI, rotationY * 180 / XM_PI);
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
