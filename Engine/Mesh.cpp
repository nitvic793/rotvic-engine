#include "Mesh.h"



Mesh::Mesh(SystemCore* core)
{
	if (core == nullptr) {
		throw std::exception("Null core");
	}
	this->core = core;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}


Mesh::~Mesh()
{
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
}

/// <summary>
/// Sets material to use for rendering
/// </summary>
/// <param name="mat"></param>
void Mesh::SetMaterial(Material* mat)
{
	material = mat;
}

/// <summary>
/// Gets material bound to mesh. 
/// </summary>
/// <returns></returns>
Material *Mesh::GetMaterial()
{
	return material;
}

/// <summary>
/// Initialize the mesh using given vertex and index data. Creates the Vertex Buffer and Index Buffer. 
/// </summary>
/// <param name="vertices">The vertex array</param>
/// <param name="vertexCount">Number of vertices</param>
/// <param name="indices">The index array</param>
/// <param name="indexCount">Number of indices</param>
void Mesh::Initialize(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount)
{
	this->indexCount = indexCount;
	auto device = core->GetDevice();
	this->vertices = vertices;
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

/// <summary>
/// Get vertex buffer.
/// </summary>
/// <returns></returns>
ID3D11Buffer *Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

/// <summary>
/// Get index buffer.
/// </summary>
/// <returns></returns>
ID3D11Buffer *Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

/// <summary>
/// Get count of indices. 
/// </summary>
/// <returns></returns>
UINT Mesh::GetIndexCount()
{
	return indexCount;
}