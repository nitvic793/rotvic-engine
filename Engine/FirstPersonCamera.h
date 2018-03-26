#pragma once
#include "Camera.h"

class FirstPersonCamera :
	public Camera
{
public:
	void Update(float deltaTime) override;
	void OnMouseMove(WPARAM wParam, int x, int y) override;
	FirstPersonCamera(float aspectRatio);
	~FirstPersonCamera();
};

