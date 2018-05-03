/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "Camera.h"

class FirstPersonCamera :
	public Camera
{
public:
	/// <summary>
	/// Inherited from camera. Called on update of frame.
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime) override;

	/// <summary>
	/// Inherited from camera. Called on update of frame.
	/// </summary>
	/// <param name="wParam"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	void OnMouseMove(WPARAM wParam, int x, int y) override;
	FirstPersonCamera(float aspectRatio);
	~FirstPersonCamera();
};

