#pragma once

#include <Windows.h>
#include <string>

class SystemCore
{
public:
	virtual void InitializeWindow(HINSTANCE hInstance, int screenHeight, int screenWidth, std::string screenTitle);
	virtual void InitializeAndBindDirectX();
	SystemCore();
	~SystemCore();
};

