#pragma once

#include <DirectXCollision.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
#include "SystemCore.h"

using namespace DirectX;

class DebugDraw
{
	SystemCore *core;
	PrimitiveBatch<DirectX::VertexPositionColor>* batch;
public:
	void XM_CALLCONV Draw(
		const BoundingSphere& sphere,
		FXMVECTOR color = Colors::White);
	DebugDraw();
	~DebugDraw();
};

