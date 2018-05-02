/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "Utility.h"

/// <summary>
/// Calculates the bounds of the screen in world space and gets the Rect object for it. 
/// Works only in 2D mode where only the camera's z position is taken into account
/// </summary>
/// <param name="viewMatrix">The camera's view matrix</param>
/// <param name="projectionMatrix">The projection matrix</param>
/// <param name="screenWidth">Width of screen</param>
/// <param name="screenHeight">Height of screen</param>
/// <param name="cameraPosition">Position of camera</param>
/// <returns>Rect which bounds the screen in world space.</returns>
Rect Utility::GetScreenBounds2D(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int screenWidth, int screenHeight, Vector3f cameraPosition)
{
	DirectX::XMMATRIX projection = XMLoadFloat4x4(&viewMatrix);
	DirectX::XMMATRIX view = XMLoadFloat4x4(&projectionMatrix);
	projection = XMMatrixTranspose(projection);
	view = XMMatrixTranspose(view);

	Rect rect;
	DirectX::XMMATRIX invProjectionView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(projection* view), (projection* view));
	float x1 = (((2.0f * 0) / screenWidth) - 1) * (-cameraPosition.z);
	float y1 = -(((2.0f * 0) / screenHeight) - 1) * (-cameraPosition.z);

	float x2 = (((2.0f * screenWidth) / screenWidth) - 1) * (-cameraPosition.z);
	float y2 = -(((2.0f * screenHeight) / screenHeight) - 1) * (-cameraPosition.z);
	DirectX::XMVECTOR pos1 = DirectX::XMVectorSet(x1, y1, 0.0f, 0.0f);
	DirectX::XMVECTOR pos2 = DirectX::XMVectorSet(x2, y2, 0.0f, 0.0f);
	XMFLOAT3 position1;
	XMFLOAT3 position2;
	XMStoreFloat3(&position1, DirectX::XMVector3Transform(pos1, invProjectionView));
	XMStoreFloat3(&position2, DirectX::XMVector3Transform(pos2, invProjectionView));

	rect.width = fabsf(position2.x - position1.x);
	rect.height = fabsf(position2.y - position1.y);
	
	rect.x = position1.x;
	rect.y = position1.y;
	
	return rect;
}
