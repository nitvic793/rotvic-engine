#include "Mesh.h"



Mesh::Mesh(SystemCore* core)
{
	this->core = core;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}


Mesh::~Mesh()
{
}

void Mesh::SetMaterial(Material* mat)
{
	material = mat;
}

Material *Mesh::GetMaterial()
{
	return material;
}

void Mesh::Initialize(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount)
{
	this->indexCount = indexCount;
	auto device = core->GetDevice();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertexCount;
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

ID3D11Buffer *Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

ID3D11Buffer *Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

UINT Mesh::GetIndexCount()
{
	return indexCount;
}