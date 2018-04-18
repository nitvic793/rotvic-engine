#pragma once

#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Tangent;
};

struct VertexColor
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT4 Color;
};

struct VertexAnimated
{
	DirectX::XMFLOAT4 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT4 Boneids{0,0,0,0};
	DirectX::XMFLOAT4 Weights{0,0,0,0};
};