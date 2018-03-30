#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT4 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;
	//DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT4 Boneids{-1,-1,-1,-1};
	DirectX::XMFLOAT4 Weights{-1,-1,-1,-1};
	//DirectX::XMFLOAT4 Color;        // The color of the vertex
};