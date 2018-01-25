#pragma once

#include "SystemCore.h"
#include "Material.h"
#include "Vertex.h"

class Mesh
{
protected:
	SystemCore* core;
	Material* material;
	Vertex *vertices;
	unsigned int *indices;
	int indexCount;
	ID3D11Buffer *vertexBuffer = nullptr;
	ID3D11Buffer *indexBuffer = nullptr;
public:
	Mesh(SystemCore* core);
	~Mesh();
	void SetMaterial(Material*);
	Material *GetMaterial();
	ID3D11Buffer *GetVertexBuffer();
	ID3D11Buffer *GetIndexBuffer();
	UINT GetIndexCount();
	void Mesh::Initialize(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount);
};

