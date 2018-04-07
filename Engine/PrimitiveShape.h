#pragma once

#include "Vertex.h"
#include "SystemCore.h"
#include <d3d11.h>
#include "Transform.h"
#include <memory>
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

enum PrimitiveShapesType
{
	UNDEFINED = -1,
	BOX,
	SPHERE,
	CONE, 
	GRID,
	RAY,
	FRUSTUM,
	CYLINDER
};

class BasicShape
{};

struct Ray : public BasicShape
{
	XMFLOAT4 color;
	XMFLOAT3 origin;
	float length;
	XMFLOAT3 direction;
};

struct Grid  : public BasicShape
{
	XMFLOAT4 color;
	XMFLOAT3 origin;
	XMFLOAT3 xAxis;
	XMFLOAT3 yAxis;
	size_t xDivs;
	size_t yDivs;

	static Grid GetDefaultGrid();
};

struct Sphere : public BasicShape
{
	XMFLOAT4 color;
	BoundingSphere bounding;
};

struct Box : public BasicShape
{
	XMFLOAT4 color;
	BoundingOrientedBox bounding;
};

struct Frustum : public BasicShape
{
	XMFLOAT4 color;
	BoundingFrustum bounding;
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

