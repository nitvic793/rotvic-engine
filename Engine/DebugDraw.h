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
#include "PrimitiveShape.h"
#include <queue>
#include <wrl.h>
#include <map>
#include <queue>

using namespace DirectX;



struct DrawCallPayLoad
{
	PrimitiveShapesType shape;
	Transform transform;
};

class DebugDraw
{
	std::unique_ptr<DirectX::BasicEffect> effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	static DebugDraw* instance;
	SystemCore *core;
	std::unique_ptr<CommonStates> states;
	PrimitiveShape* GetShape(PrimitiveShapesType type);
	std::queue<DrawCallPayLoad> drawCalls;
	std::map<PrimitiveShapesType, PrimitiveShape*> shapeBuffers;
	void Draw(ID3D11DeviceContext *context, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);
public:
	static DebugDraw* GetInstance();
	void Draw(PrimitiveShapesType shape, Transform transform);
	void Render(Camera* camera);
	DebugDraw(SystemCore* core);
	~DebugDraw();
};

