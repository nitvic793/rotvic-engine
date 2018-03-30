#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "GameEntity.h"
#include <memory>
#include <iostream>
#include "Camera.h"
#include "Lights.h"
#include "WICTextureLoader.h"
#include "FBXLoader.h"


class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const wchar_t*, ID3D11Resource**, ID3D11ShaderResourceView**);
	void CreateMatrices();
	void CreateBasicGeometry();
	void CreateGameObjects();

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	shared_ptr<SimpleVertexShader>  vertexShader;
	shared_ptr<SimplePixelShader>  pixelShader;

	// The matrices to go from model space to screen space
	//DirectX::XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	shared_ptr<Mesh> mesh1,mesh2,mesh3,mesh4,mesh5;
	shared_ptr<Material> material1;
	shared_ptr<GameEntity> gobj1, gobj2, gobj3;

	DirectionalLight light1,light2;

	Camera camera;

	//FBX Loader
	FBXLoader fbxLoader;
	Bones bones[72];

	int i = 0;
};

