#pragma once

#ifndef UTILITY_H
#define UTILITY_H

#include "Rect.h"
#include "Vector3.h"
#include <DirectXMath.h>

using namespace DirectX;

template <class DstType, class SrcType>
bool IsType(const SrcType* src)
{
	return dynamic_cast<const DstType*>(src) != nullptr;
}

/// <summary>
/// Contains all utility/helper functions
/// </summary>
class Utility
{
public:
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
	static Rect GetScreenBounds2D(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int screenWidth, int screenHeight, Vector3f cameraPosition);
};


#endif // !UTILITY_H