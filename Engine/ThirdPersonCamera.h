/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "Camera.h"
#include "ThirdPersonPlayerEntity.h"

class ThirdPersonCamera :
	public Camera
{
	ThirdPersonPlayerEntity* entity;
public:
	void AttachEntity(ThirdPersonPlayerEntity* thirdPersonEntity);
	void Update(float deltaTime) override;
	void OnMouseMove(WPARAM wParam, int x, int y) override;
	ThirdPersonCamera(float aspectRatio);
	~ThirdPersonCamera();
};

