#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <functional>

/// <summary>
/// System core class. DirectX related functionality is encapsulated in System Core. 
/// </summary>
class SystemCore
{
protected:
	int height;
	int width;
	std::function<void(int, int)> onResizeCallback;
	HWND hWnd;
	D3D_FEATURE_LEVEL		dxFeatureLevel;
	IDXGISwapChain*			swapChain;
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;
public:
	virtual void	InitializeWindow(HINSTANCE hInstance, int nCmdShow, int screenHeight, int screenWidth, std::string screenTitle);
	virtual HRESULT InitializeAndBindDirectX();
	virtual LRESULT	ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void    CreateConsoleWindow(int bufferLines, int bufferColumns, int windowLines, int windowColumns);
	virtual void	Run(std::function<void()> updateCallback);
	virtual void	ClearScreen();
	virtual void	Draw();
	virtual void	HandleError(std::exception *ex);
	virtual void	OnResize();
	virtual ID3D11DeviceContext*	GetDeviceContext();

	void					SetOnResizeCallback(std::function<void(int, int)> callback);
	IDXGISwapChain*			GetSwapChain();
	ID3D11Device*			GetDevice();	
	D3D_FEATURE_LEVEL		GetDirectXFeatureLevel();
	ID3D11RenderTargetView* GetBackBufferRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();
	HWND					GetWindowHandle();
	static SystemCore*		SystemCoreInstance;
	SystemCore();
	~SystemCore();
};

