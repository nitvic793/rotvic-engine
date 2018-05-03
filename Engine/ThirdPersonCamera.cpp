#include "ThirdPersonCamera.h"



void ThirdPersonCamera::AttachEntity(Entity* thirdPersonEntity)
{
	entity = thirdPersonEntity;
}

void ThirdPersonCamera::Update(float deltaTime)
{
	if (entity != nullptr)
	{

	}
}

void ThirdPersonCamera::OnMouseMove(WPARAM wParam, int x, int y)
{
}

ThirdPersonCamera::ThirdPersonCamera(float aspectRatio)
	:Camera(aspectRatio)
{
	entity = nullptr;
}


ThirdPersonCamera::~ThirdPersonCamera()
{
}
