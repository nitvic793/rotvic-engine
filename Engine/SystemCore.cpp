/*
Code referenced from Prof. Chris Cascioli DX11 Starter code - provided in class. 
*/

#include "SystemCore.h"
#include <WindowsX.h>

SystemCore*	SystemCore::SystemCoreInstance = nullptr;

/// <summary>
/// Windows process callback.
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return SystemCore::SystemCoreInstance->ProcessMessage(hWnd, message, wParam, lParam);
}

/// <summary>
/// System core constructor.
/// </summary>
SystemCore::SystemCore()
{
	SystemCoreInstance = this;
}

/// <summary>
/// System core destructor.
/// </summary>
SystemCore::~SystemCore()
{
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBufferRTV) { backBufferRTV->Release(); }
	if (swapChain) { swapChain->Release(); }
	if (context) { context->Release(); }
	if (device) { device->Release(); }
}

/// <summary>
/// Creates console window. 
/// </summary>
/// <param name="bufferLines"></param>
/// <param name="bufferColumns"></param>
/// <param name="windowLines"></param>
/// <param name="windowColumns"></param>
void SystemCore::CreateConsoleWindow(int bufferLines, int bufferColumns, int windowLines, int windowColumns)
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = bufferLines;
	coninfo.dwSize.X = bufferColumns;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = windowColumns;
	rect.Bottom = windowLines;
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);

	FILE *stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);

	HWND consoleHandle = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(consoleHandle, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
}

/// <summary>
/// Handle error callback.
/// </summary>
/// <param name="ex"></param>
void SystemCore::HandleError(std::exception *ex)
{
	if (ex)
		MessageBoxA(hWnd, ex->what(), "Error", MB_ICONERROR);
	else
		MessageBoxA(hWnd, "Unexpected error. Aborting.", "Error", MB_ICONERROR);
}

void SystemCore::OnResize()
{
	// Release existing DirectX views and buffers
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBufferRTV) { backBufferRTV->Release(); }

	// Resize the underlying swap chain buffers
	swapChain->ResizeBuffers(
		1,
		width,
		height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);

	// Recreate the render target view for the back buffer
	// texture, then release our local texture reference
	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));
	device->CreateRenderTargetView(backBufferTexture, 0, &backBufferRTV);
	backBufferTexture->Release();

	// Set up the description of the texture to use for the depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
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
}

LRESULT SystemCore::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		if (onKeyPressCallBack)
			onKeyPressCallBack((char)wParam);
		return 0;
	}
	case WM_KEYDOWN:
		break;

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		//printf("KeyUp: %c\n", (char)wParam);
		return 0;
	}
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		if (device)
			OnResize();

		if (onResizeCallback)
			onResizeCallback(width, height);

		return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	// Mouse button being pressed (while the cursor is currently over our window)
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		// Mouse button being released (while the cursor is currently over our window)
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		// Cursor moves over the window (or outside, while we're currently capturing it)
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		// Mouse wheel is scrolled
	case WM_MOUSEWHEEL:
		OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SystemCore::OnMouseUp(WPARAM wParam, int x, int y)
{
	mouse->OnMouseUp(wParam, x, y);
	ReleaseCapture();
}

void SystemCore::OnMouseDown(WPARAM wParam, int x, int y)
{
	SetCapture(hWnd);
	mouse->OnMouseDown(wParam, x, y);
}

void SystemCore::OnMouseMove(WPARAM wParam, int x, int y)
{
	mouse->OnMouseMove(wParam, x, y);
}

void SystemCore::OnMouseWheel(float wheelData, int x, int y)
{
	mouse->OnMouseWheel(wheelData, x, y);
}

/// <summary>
/// Initializes the main window. 
/// </summary>
/// <param name="hInstance"></param>
/// <param name="nCmdShow"></param>
/// <param name="screenHeight"></param>
/// <param name="screenWidth"></param>
/// <param name="screenTitle"></param>
void SystemCore::InitializeWindow(HINSTANCE hInstance, int nCmdShow, int screenHeight, int screenWidth, std::string screenTitle = "DXGame")
{
	height = screenHeight;
	width = screenWidth;
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	int width = screenWidth;
	int height = screenHeight;
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

	RECT desktopRect;
	GetClientRect(GetDesktopWindow(), &desktopRect);
	int centeredX = (desktopRect.right / 2) - (clientRect.right / 2);
	int centeredY = (desktopRect.bottom / 2) - (clientRect.bottom / 2);

	hWnd = CreateWindow(
		wndClass.lpszClassName,
		screenTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		centeredX,
		centeredY,
		clientRect.right - clientRect.left,	// Calculated width
		clientRect.bottom - clientRect.top,	// Calculated height
		0,			// No parent window
		0,			// No menu
		hInstance,	// The app's handle
		0);	  // used with multiple windows, NULL
	ShowWindow(hWnd, nCmdShow);
}

/// <summary>
/// Initialize DirectX and bind to window. 
/// </summary>
/// <returns></returns>
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

/// <summary>
/// Starts the core game loop. Requires a callback function which should encapsulate all update related logic. 
/// </summary>
/// <param name="updateCallback">Update Callback</param>
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
			//swapChain->Present(0, 0);
		}
	}
}

/// <summary>
/// Clear screen to cornflower blue.
/// </summary>
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

void SystemCore::BindMouse(Mouse * m)
{
	mouse = m;
}

void SystemCore::SetOnResizeCallback(std::function<void(int, int)> callback)
{
	onResizeCallback = callback;
}

void SystemCore::SetOnKeyPressCallback(std::function<void(char)> callback)
{
	onKeyPressCallBack = callback;
}

/// <summary>
/// Returns the swap chain
/// </summary>
/// <returns></returns>
IDXGISwapChain* SystemCore::GetSwapChain()
{
	return swapChain;
}

/// <summary>
/// Returns the device.
/// </summary>
/// <returns></returns>
ID3D11Device* SystemCore::GetDevice()
{
	return device;
}

/// <summary>
/// Returns the device context. 
/// </summary>
/// <returns></returns>
ID3D11DeviceContext* SystemCore::GetDeviceContext()
{
	return context;
}

/// <summary>
/// Returns the DirectX feature level. 
/// </summary>
/// <returns></returns>
D3D_FEATURE_LEVEL SystemCore::GetDirectXFeatureLevel()
{
	return dxFeatureLevel;
}


ID3D11RenderTargetView* SystemCore::GetBackBufferRenderTargetView()
{
	return backBufferRTV;
}

ID3D11DepthStencilView* SystemCore::GetDepthStencilView()
{
	return depthStencilView;
}

HWND SystemCore::GetWindowHandle()
{
	return hWnd;
}

void SystemCore::Draw()
{

}
