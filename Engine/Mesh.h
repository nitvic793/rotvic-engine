#pragma once
#define NOMINMAX
#include "SystemCore.h"
#include "Material.h"
#include "Vertex.h"
#include <fstream>
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// Mesh class represents the shape/dimensions of an object. Meshes should be bound to a Game Entity object for rendering.  
/// </summary>
class Mesh
{
protected:
	SystemCore* core;
	Vertex *vertices;
	unsigned int *indices;
	int indexCount;
	ID3D11Buffer *vertexBuffer = nullptr;
	ID3D11Buffer *indexBuffer = nullptr;
	VertexAnimated *verticesAnim;
public:
	Mesh(SystemCore* core);
	Mesh::Mesh(const char *objFile, SystemCore* core);
	~Mesh();
	ID3D11Buffer *GetVertexBuffer();
	ID3D11Buffer *GetIndexBuffer();
	void CalculateTangents(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount);
	UINT GetIndexCount();
	void Mesh::Initialize(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount);
	Mesh::Mesh(VertexAnimated *vertices, UINT vertexCount, UINT *indices, UINT indexCountAnim, ID3D11Device *Device); // For animation only
};

