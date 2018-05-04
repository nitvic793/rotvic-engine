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
		auto forward = entity->GetForwardVector();
		//forward.normalize();
		auto target = entityPos - (forward);
		
		//SetPosition(XMFLOAT3(target.x + 2, target.y + 2, target.z - 10));
		//SetRotation(XMFLOAT3(30 * XM_PI / 180, rotation.y, 0));
		
	}
}

void ThirdPersonCamera::OnMouseMove(WPARAM wParam, int x, int y)
{
	auto deltaX = float(x - mousePrevX);
	auto deltaY = float(y - mousePrevY);

	if (rotation.x + deltaY >= (90 - 10) * XM_PI / 180)
	{
		deltaX *= 0.2f;
	}

	if (rotation.x + deltaY <= -(90 - 10) * XM_PI / 180)
	{
		deltaX *= 0.2f;
	}

	rotation.y += deltaX * 0.2f * XM_PI / 180;
	//rotation.x += deltaY * 0.6f * XM_PI / 180;
	/*if (rotation.x >= XM_PI / 2) rotation.x = XM_PI / 2;
	if (rotation.x <= -XM_PI / 2) rotation.x = -XM_PI / 2;*/
	SetRotation(XMFLOAT3(0, rotation.y, 0));
	mousePrevX = x;
	mousePrevY = y;
}

XMFLOAT4X4 ThirdPersonCamera::GetViewMatrix()
{
	auto rotQuaternion = XMQuaternionRotationRollPitchYaw(0, rotation.y, 0);
	auto entityPos = entity->GetPosition();
	auto forward = entity->GetForwardVector();
	//forward.normalize();
	auto left = forward.cross(rp3d::Vector3(0, 1, 0));
	auto target = entityPos - (forward * 2) - (left * 1.2f);
	XMVECTOR pos = XMVectorSet(target.x, target.y + 5, target.z, 0);
	XMVECTOR dir = XMVectorSet(entityPos.x + 3, entityPos.y + 3, entityPos.z, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	//pos = XMVector3Rotate(pos, rotQuaternion);
	
	XMMATRIX V = XMMatrixLookAtLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	return viewMatrix;
}

ThirdPersonCamera::ThirdPersonCamera(float aspectRatio)
	:Camera(aspectRatio)
{
	entity = nullptr;
}


ThirdPersonCamera::~ThirdPersonCamera()
{
}
