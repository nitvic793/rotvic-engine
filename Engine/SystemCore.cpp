#include "SystemCore.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

SystemCore::SystemCore()
{
}


SystemCore::~SystemCore()
{
}

void SystemCore::InitializeWindow(HINSTANCE hInstance, int nCmdShow, int screenHeight, int screenWidth, std::string screenTitle)
{
	height = screenHeight;
	width = screenWidth;
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	int width = 1280;
	int height = 720;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClass.lpszClassName = "WindowClass1";

	RegisterClassEx(&wndClass);
	RECT clientRect;
	SetRect(&clientRect, 0, 0, width, height);
	AdjustWindowRect(
		&clientRect,
		WS_OVERLAPPEDWINDOW,	// Has a title bar, border, min and max buttons, etc.
		false);

	// create the window and use the result as the handle
	RECT desktopRect;
	GetClientRect(GetDesktopWindow(), &desktopRect);
	int centeredX = (desktopRect.right / 2) - (clientRect.right / 2);
	int centeredY = (desktopRect.bottom / 2) - (clientRect.bottom / 2);

	// Actually ask Windows to create the window itself
	// using our settings so far.  This will return the
	// handle of the window, which we'll keep around for later
	hWnd = CreateWindow(
		wndClass.lpszClassName,
		"DXGame",
		WS_OVERLAPPEDWINDOW,
		centeredX,
		centeredY,
		clientRect.right - clientRect.left,	// Calculated width
		clientRect.bottom - clientRect.top,	// Calculated height
		0,			// No parent window
		0,			// No menu
		hInstance,	// The app's handle
		0);	  // used with multiple windows, NULL

			  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);
}

HRESULT SystemCore::InitializeAndBindDirectX()
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = 0;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;

	// Result variable for below function calls
	HRESULT hr = S_OK;

	UINT deviceFlags = 0;
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	hr = D3D11CreateDeviceAndSwapChain(
		0,							// Video adapter (physical GPU) to use, or null for default
		D3D_DRIVER_TYPE_HARDWARE,	// We want to use the hardware (GPU)
		0,							// Used when doing software rendering
		deviceFlags,				// Any special options
		0,							// Optional array of possible verisons we want as fallbacks
		0,							// The number of fallbacks in the above param
		D3D11_SDK_VERSION,			// Current version of the SDK
		&swapDesc,					// Address of swap chain options
		&swapChain,					// Pointer to our Swap Chain pointer
		&device,					// Pointer to our Device pointer
		&dxFeatureLevel,			// This will hold the actual feature level the app will use
		&context);					// Pointer to our Device Context pointer
	if (FAILED(hr)) return hr;

	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backBufferTexture);

	device->CreateRenderTargetView(
		backBufferTexture,
		0,
		&backBufferRTV);
	backBufferTexture->Release();

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();

	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	return S_OK;
};

void SystemCore::Run(std::function<void()> updateCallback)
{
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			updateCallback();
			swapChain->Present(0, 0);
		}
	}
}

void SystemCore::ClearScreen()
{
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}
