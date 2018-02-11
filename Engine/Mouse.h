#pragma once

#include "Vector2f.h"
#include "Vector3.h"
#include <Windows.h>
#include <map>
#include <DirectXMath.h>

using namespace DirectX;

enum MouseButton{MBLeft, MBRight, MBMiddle};

/// <summary>
/// Mouse class to encapsulate all mouse related input. 
/// </summary>
class Mouse
{
	HWND hWnd;
	std::map<MouseButton, wchar_t> KeyMap;
public:
	/// <summary>
	/// Checks if given mouse button is pressed/clicked. 
	/// </summary>
	/// <param name="button">The mouse button</param>
	/// <returns></returns>
	virtual bool IsKeyPressed(MouseButton button);

	/// <summary>
	/// Gets absolute pixel position of mouse pointer in relation to window. Top left corner is (0,0) with inverted Y-axis where Y increases as you go down. 
	/// </summary>
	/// <returns> Position of mouse pointer in relation to window</returns>
	virtual Vector2f GetMousePositionScreen();

	/// <summary>
	/// Gets the mouse position in world space 2D coordinates, given camera position. Meant only for 2D mode where only the camera's z position varies. 
	/// </summary>
	/// <param name="viewMatrix">The camera's view matrix</param>
	/// <param name="projectionMatrix">The Projection matrix</param>
	/// <param name="screenWidth">Width of window</param>
	/// <param name="screenHeight">Height of window</param>
	/// <param name="cameraPosition">Position of camera. Only z position is considered</param>
	/// <returns>Position of mouse in world space</returns>
	virtual Vector2f GetMousePositionWorld(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int screenWidth, int screenHeight, Vector3f cameraPosition);
	Mouse(HWND hInstance);
	~Mouse();
};

