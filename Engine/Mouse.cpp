#include "Mouse.h"

/// <summary>
/// Checks if given mouse button is pressed/clicked. 
/// </summary>
/// <param name="button"></param>
/// <returns></returns>
bool Mouse::IsKeyPressed(MouseButton button)
{
	if ((GetAsyncKeyState(KeyMap[button]) & 0x8000)!=0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// Gets absolute pixel position of mouse pointer in relation to window. Top left corner is (0,0) with inverted Y-axis where Y increases as you go down. 
/// </summary>
/// <returns> Position of mouse pointer in relation to window</returns>
Vector2f Mouse::GetMousePositionScreen()
{
	POINT p;
	if (GetCursorPos(&p))
	{
		if (ScreenToClient(hWnd, &p))
		{
			return Vector2f((float)p.x, (float)p.y);
		}
	}
	return Vector2f();
}

/// <summary>
/// Gets the mouse position in world space 2D coordinates, given camera position. Meant only for 2D mode where only the camera's z position varies. 
/// </summary>
/// <param name="viewMatrix">The camera's view matrix</param>
/// <param name="projectionMatrix">The Projection matrix</param>
/// <param name="screenWidth">Width of window</param>
/// <param name="screenHeight">Height of window</param>
/// <param name="cameraPosition">Position of camera. Only z position is considered</param>
/// <returns>Position of mouse in world space</returns>
Vector2f Mouse::GetMousePositionWorld(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int screenWidth, int screenHeight, Vector3f cameraPosition)
{
	DirectX::XMMATRIX projection = XMLoadFloat4x4(&viewMatrix);
	DirectX::XMMATRIX view = XMLoadFloat4x4(&projectionMatrix);
	projection = XMMatrixTranspose(projection);
	view = XMMatrixTranspose(view);

	DirectX::XMMATRIX invProjectionView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(projection* view), (projection* view));
	auto mousePos = GetMousePositionScreen();
	float x = (((2.0f * mousePos.x) / screenWidth) - 1) * (-cameraPosition.z);
	float y = -(((2.0f * mousePos.y) / screenHeight) - 1) * (-cameraPosition.z);

	DirectX::XMVECTOR mousePosition = DirectX::XMVectorSet(x, y, 0.0f, 0.0f);
	XMFLOAT3 mouseInWorld;
	XMStoreFloat3(&mouseInWorld, DirectX::XMVector3Transform(mousePosition, invProjectionView));
	return Vector2f(mouseInWorld.x, mouseInWorld.y);
}

Mouse::Mouse(HWND hWnd)
{
	this->hWnd = hWnd;
	KeyMap.insert(std::pair<MouseButton, wchar_t>(MBLeft, VK_LBUTTON));
	KeyMap.insert(std::pair<MouseButton, wchar_t>(MBRight, VK_RBUTTON));
}


Mouse::~Mouse()
{
}
