#pragma once

#include <DirectXCollision.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
#include <CommonStates.h>
#include "SystemCore.h"
#include "Camera.h"
#include "ResourceManager.h"
#include <queue>
#include <wrl.h>

using namespace DirectX;

enum PrimitiveShapes 
{
	CUBE,
	SPHERE,
	CONE
};

class DebugDraw
{
	SystemCore *core;
	std::unique_ptr<PrimitiveBatch<VertexPositionColor>> batch;
	std::unique_ptr<BasicEffect> effect;
	std::unique_ptr<CommonStates> states;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	ID3D11Buffer *cubeIndexBuffer = nullptr;
	ID3D11Buffer *cubeVertexBuffer = nullptr;
	void Draw(ID3D11DeviceContext *context, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);
	void XM_CALLCONV Draw(
		const BoundingSphere& sphere,
		FXMVECTOR color = Colors::White);
	inline void XM_CALLCONV DrawCube(SystemCore* core,
		CXMMATRIX matWorld);
public:
	void Draw(PrimitiveShapes shape, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale);
	void Render(Camera* camera);
	DebugDraw(SystemCore* core);
	~DebugDraw();
};

