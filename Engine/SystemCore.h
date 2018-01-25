#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

class SystemCore
{
protected:
	int height;
	int width;
	HWND hWnd;
public:
	virtual void InitializeWindow(HINSTANCE hInstance, int nCmdShow, int screenHeight, int screenWidth, std::string screenTitle);
	virtual HRESULT InitializeAndBindDirectX();
	SystemCore();
	~SystemCore();
};

