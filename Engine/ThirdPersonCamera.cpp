#include "ThirdPersonCamera.h"



void ThirdPersonCamera::AttachEntity(ThirdPersonPlayerEntity* thirdPersonEntity)
{
	entity = thirdPersonEntity;
}

void ThirdPersonCamera::Update(float deltaTime)
{
	if (entity != nullptr)
	{
		auto entityPos = entity->GetPosition();
		auto forward = entity->GetForward();
		//forward.normalize();
		auto target = entityPos - (forward * 5);
		SetPosition(XMFLOAT3(target.x, -1, target.z - 10));
		auto quaternionV = entity->GetRotation();
		XMVECTOR quat = XMVectorSet(quaternionV.x, quaternionV.y, quaternionV.z, 0);
		XMVector3Rotate(XMVectorSet(0, 0, 0, 0), quat);
	}
}

void ThirdPersonCamera::OnMouseMove(WPARAM wParam, int x, int y)
{
	/*auto deltaX = float(x - mousePrevX);
	auto deltaY = float(y - mousePrevY);

	if (rotationX + deltaY >= (90 - 10) * XM_PI / 180)
	{
		deltaX *= 0.2f;
	}

	if (rotationX + deltaY <= -(90 - 10) * XM_PI / 180)
	{
		deltaX *= 0.2f;
	}

	rotation.y += deltaX * 0.6f * XM_PI / 180;
	rotation.x += deltaY * 0.6f * XM_PI / 180;
	if (rotation.x >= XM_PI / 2) rotation.x = XM_PI / 2;
	if (rotation.x <= -XM_PI / 2) rotation.x = -XM_PI / 2;

	mousePrevX = x;
	mousePrevY = y;*/
}

ThirdPersonCamera::ThirdPersonCamera(float aspectRatio)
	:Camera(aspectRatio)
{
	entity = nullptr;
}


ThirdPersonCamera::~ThirdPersonCamera()
{
}
