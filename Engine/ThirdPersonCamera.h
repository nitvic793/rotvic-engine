/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "Camera.h"
#include "ThirdPersonPlayerEntity.h"

/// <summary>
/// Third person camera to be used with Third Person Player Entity class.
/// </summary>
class ThirdPersonCamera :
	public Camera
{
	ThirdPersonPlayerEntity* entity;
public:
	/// <summary>
	/// Attach player entity to camera.
	/// </summary>
	/// <param name="thirdPersonEntity">Third person player entity instance.</param>
	void AttachEntity(ThirdPersonPlayerEntity* thirdPersonEntity);

	/// <summary>
	/// Update called on every frame.
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime) override;

	/// <summary>
	/// Inherited from base class Camera. Callback for onMouseEvent.
	/// </summary>
	/// <param name="wParam"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	void OnMouseMove(WPARAM wParam, int x, int y) override;

	/// <summary>
	/// Gets the view matrix of this camera.
	/// </summary>
	/// <returns>The view matrix.</returns>
	XMFLOAT4X4 GetViewMatrix() override;

	ThirdPersonCamera(float aspectRatio);
	~ThirdPersonCamera();
};

