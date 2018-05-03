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

	/// <summary>
	/// Get vertex buffer associated with mesh. 
	/// </summary>
	/// <returns>ID3D11Buffer resource. Will return nullptr if not instantiated.</returns>
	ID3D11Buffer *GetVertexBuffer();

	/// <summary>
	/// Get index buffer associated with mesh. 
	/// </summary>
	/// <returns>ID3D11Buffer resource. Will return nullptr if not instantiated.</returns>
	ID3D11Buffer *GetIndexBuffer();

	/// <summary>
	/// Calculate the tangents of given vertices.
	/// </summary>
	/// <param name="vertices">Vertices of the mesh.</param>
	/// <param name="vertexCount">Vertex count</param>
	/// <param name="indices">Indices of the mesh</param>
	/// <param name="indexCount">index count</param>
	void CalculateTangents(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount);

	/// <summary>
	/// Calculate the tangents of given vertices for Animation Vertex.
	/// </summary>
	/// <param name="vertices">Vertices of the mesh.</param>
	/// <param name="vertexCount">Vertex count</param>
	/// <param name="indices">Indices of the mesh</param>
	void CalculateTangents(VertexAnimated *vertices, UINT vertexCount, UINT *indices, UINT indexCount);

	/// <summary>
	/// Gets the index count.
	/// </summary>
	/// <returns></returns>
	UINT GetIndexCount();

	/// <summary>
	/// Initialize mesh with given vertices and indices.
	/// </summary>
	/// <param name="vertices">Vertices array</param>
	/// <param name="vertexCount">vertex count</param>
	/// <param name="indices">Index array</param>
	/// <param name="indexCount">index count</param>
	void Mesh::Initialize(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount);
	Mesh::Mesh(VertexAnimated *vertices, UINT vertexCount, UINT *indices, UINT indexCountAnim, ID3D11Device *Device); // For animation only
};

