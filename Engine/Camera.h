#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include <Windows.h>
#include "Keyboard.h"

using namespace DirectX;

/// <summary>
/// Base camera class. Encapsulates all basic camera functions. Can be inherited to extend functionality. 
/// </summary>
/// <remarks>
/// This class implements a basic "Free Cam" mode which can be plopped into any game instance. 
/// </remarks>
class Camera
{
protected:
	/// <summary>
	/// The view matrix.
	/// </summary>
	XMFLOAT4X4 viewMatrix;

	/// <summary>
	/// The projection matrix.
	/// </summary>
	XMFLOAT4X4 projectionMatrix;

	/// <summary>
	/// The position of the camera.
	/// </summary>
	XMFLOAT3 position;

	/// <summary>
	/// The direction of the camera.
	/// </summary>
	XMFLOAT3 direction;

	/// <summary>
	/// The rotation of the camera.
	/// </summary>
	XMFLOAT3 rotation;
	float rotationY;
	float rotationX;

	/// <summary>
	/// Boolean to indicate if is mouse down or not.
	/// </summary>
	bool isMouseDown;

	/// <summary>
	/// Mouse X-Position in previous frame.
	/// </summary>
	int mousePrevX;

	/// <summary>
	/// Mouse Y-Position in previous frame.
	/// </summary>
	int mousePrevY;

	/// <summary>
	/// Keyboard instance for camera.
	/// </summary>
	Keyboard* keyboard;
public:
	/// <summary>
	/// Set Position of camera in world space.
	/// </summary>
	/// <param name="pos">Position vector to set the position of the camera</param>
	void SetPosition(XMFLOAT3 pos);

	/// <summary>
	/// Gets the position of this camera in world space. 
	/// </summary>
	/// <returns>
	/// Returns position vector in world space. 
	/// </returns>
	XMFLOAT3 GetPosition();

	/// <summary>
	/// Rotate camera around X-axis
	/// </summary>
	/// <param name="x">Angle in radians</param>
	void RotateX(float x);

	/// <summary>
	/// Rotate camera around Y-axis
	/// </summary>
	/// <param name="y">Angle in radians</param>
	void RotateY(float y);

	/// <summary>
	/// Sets the Euler rotation angles(roll, pitch and yaw) rotation for this matrix. 
	/// </summary>
	/// <param name="rotation">Rotation vector with x,y,z euler angles in radians.</param>
	void SetRotation(XMFLOAT3 rotation);

	/// <summary>
	/// Gets the view matrix of this camera.
	/// </summary>
	/// <returns>The view matrix.</returns>
	virtual XMFLOAT4X4 GetViewMatrix();

	/// <summary>
	/// Gets the projection matrix of this camera.
	/// </summary>
	/// <returns>Calculated projection matrix</returns>
	XMFLOAT4X4 GetProjectionMatrix();

	/// <summary>
	/// Reset the projection matrix with given aspect ratio.
	/// </summary>
	/// <param name="aspectRatio">Aspect ratio to set the projection matrix with. </param>
	void SetProjectionMatrix(float aspectRatio);

	/// <summary>
	/// Updates the camera.
	/// </summary>
	/// <param name="deltaTime">The delta time to update the camera by. </param>
	/// <remarks>
	/// Can be overriden to supply custom logic if so required.
	/// </remarks>
	virtual void Update(float deltaTime);

	/// <summary>
	/// This callback function is called on the OnMouseMove event. 
	/// </summary>
	/// <param name="wParam">The WPARAM</param>
	/// <param name="x">Position on X-axis</param>
	/// <param name="y">Position on Y-axi</param>
	/// <remarks>
	/// Can be overriden to define custom logic for OnMouseMove event. 
	/// </remarks>
	virtual void OnMouseMove(WPARAM wParam, int x, int y);

	/// <summary>
	/// This callback function is called on the OnMouseUp event. 
	/// </summary>
	/// <param name="wParam">The WPARAM</param>
	/// <param name="x">Position on X-axis</param>
	/// <param name="y">Position on Y-axi</param>
	/// <remarks>
	/// Can be overriden to define custom logic for OnMouseUp event. 
	/// </remarks>
	virtual void OnMouseUp(WPARAM wParam, int x, int y);

	/// <summary>
	/// This callback function is called on the OnMouseDown event. 
	/// </summary>
	/// <param name="wParam">The WPARAM</param>
	/// <param name="x">Position on X-axis</param>
	/// <param name="y">Position on Y-axi</param>
	/// <remarks>
	/// Can be overriden to define custom logic for OnMouseDown event. 
	/// </remarks>
	virtual void OnMouseDown(WPARAM wParam, int x, int y);

	Camera(float aspectRatio);
	~Camera();
};
