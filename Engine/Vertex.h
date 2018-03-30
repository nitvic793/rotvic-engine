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

struct BoneVertex
{
	DirectX::XMFLOAT4 Position;	   
	DirectX::XMFLOAT3 Normal;
	//DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT4 Boneids{ -1,-1,-1,-1 };
	DirectX::XMFLOAT4 Weights{ -1,-1,-1,-1 };
};