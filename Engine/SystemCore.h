#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <functional>

class SystemCore
{
protected:
	int height;
	int width;
	HWND hWnd;
	D3D_FEATURE_LEVEL		dxFeatureLevel;
	IDXGISwapChain*			swapChain;
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;

	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;
public:
	virtual void InitializeWindow(HINSTANCE hInstance, int nCmdShow, int screenHeight, int screenWidth, std::string screenTitle);
	virtual HRESULT InitializeAndBindDirectX();
	virtual void Run(std::function<void()> updateCallback);
	virtual void ClearScreen();
	SystemCore();
	~SystemCore();
};

