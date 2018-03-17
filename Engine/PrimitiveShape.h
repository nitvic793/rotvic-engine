#pragma once

#include "Vertex.h"
#include "SystemCore.h"
#include <d3d11.h>
#include "Transform.h"
#include <memory>

enum PrimitiveShapesType
{
	CUBE,
	SPHERE,
	CONE
};

class PrimitiveShape
{
	void Initialize(VertexColor *vertices, UINT vertexCount, UINT *indices, UINT indexCount, ID3D11Device* device);
	PrimitiveShape();
public:
	Transform transform;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	UINT indexCount;
	static PrimitiveShape* InstantiateCube(SystemCore* core);
	static PrimitiveShape* Instantiate(PrimitiveShapesType type, SystemCore* core);
	~PrimitiveShape();
};

