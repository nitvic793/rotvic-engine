/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "Camera.h"
#include "Entity.h"

class ThirdPersonCamera :
	public Camera
{
	Entity* entity;
public:
	void AttachEntity(Entity* thirdPersonEntity);
	void Update(float deltaTime) override;
	void OnMouseMove(WPARAM wParam, int x, int y) override;
	ThirdPersonCamera(float aspectRatio);
	~ThirdPersonCamera();
};

