/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "PrimitiveShape.h"

void PrimitiveShape::Initialize(VertexColor *vertices, UINT vertexCount, UINT *indices, UINT indexCount, ID3D11Device* device)
{
	this->indexCount = indexCount;
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexColor) * vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
}

PrimitiveShape::PrimitiveShape()
{
	indexBuffer = vertexBuffer = nullptr;
}


PrimitiveShape *PrimitiveShape::InstantiateCube(SystemCore * core)
{
	auto shape = new PrimitiveShape();
	static const XMVECTORF32 s_verts[8] =
	{
		{ -1.f, -1.f, -1.f, 0.f },
		{ 1.f, -1.f, -1.f, 0.f },
		{ 1.f, -1.f,  1.f, 0.f },
		{ -1.f, -1.f,  1.f, 0.f },
		{ -1.f,  1.f, -1.f, 0.f },
		{ 1.f,  1.f, -1.f, 0.f },
		{ 1.f,  1.f,  1.f, 0.f },
		{ -1.f,  1.f,  1.f, 0.f }
	};

	static UINT s_indices[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	}; //12
	auto identity = XMMatrixIdentity();
	VertexColor verts[8];
	for (size_t i = 0; i < 8; ++i)
	{
		verts[i] = {};
		XMVECTOR v = XMVector3Transform(s_verts[i], identity);
		XMStoreFloat3(&verts[i].Position, v);
		XMStoreFloat4(&verts[i].Color, XMVectorSet(1, 1, 1, 0));
	}

	shape->Initialize(verts, 8, s_indices, 24, core->GetDevice());
	return shape;
}

PrimitiveShape * PrimitiveShape::Instantiate(PrimitiveShapesType type, SystemCore* core)
{
	return nullptr;
}

PrimitiveShape::~PrimitiveShape()
{
	if (indexBuffer)indexBuffer->Release();
	if (vertexBuffer)vertexBuffer->Release();
}

Grid Grid::GetDefaultGrid()
{
	Grid grid;
	grid.color = XMFLOAT4(1, 1, 1, 1);
	grid.origin = XMFLOAT3(0, -2, 0);
	grid.xAxis = XMFLOAT3(100, 0, 0);
	grid.yAxis = XMFLOAT3(0, 0, 100);
	grid.xDivs = 100;
	grid.yDivs = 100;
	return grid;
}
