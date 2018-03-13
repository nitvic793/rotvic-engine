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

using namespace DirectX;

enum PrimitiveShapesType 
{
	CUBE,
	SPHERE,
	CONE
};

class DebugDraw
{
	SystemCore *core;
	std::unique_ptr<CommonStates> states;
	PrimitiveShape* cube;
	void Draw(ID3D11DeviceContext *context, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);
public:
	void Draw(PrimitiveShapesType shape, Transform transform);
	void Render(Camera* camera);
	DebugDraw(SystemCore* core);
	~DebugDraw();
};

