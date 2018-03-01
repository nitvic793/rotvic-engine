#pragma once
#include "SimpleShader.h"
#include "SystemCore.h"

/// <summary>
/// Material class represents the shader and other material related properties of a mesh. 
/// </summary>
class Material
{
protected:
	SystemCore *core;
	SimplePixelShader *pixelShader;
	SimpleVertexShader *vertexShader;
	ID3D11ShaderResourceView*	textureSRV;
	ID3D11ShaderResourceView*	normalSRV;
	ID3D11SamplerState*			sampler;
public:
	ID3D11ShaderResourceView*	GetSRV();
	ID3D11ShaderResourceView*	GetNormalSRV();
	ID3D11SamplerState*			GetSampler();
	SimplePixelShader *GetPixelShader();
	SimpleVertexShader *GetVertexShader();
	void LoadDefaultShaders();
	Material(SystemCore*);
	Material(SystemCore*, SimpleVertexShader*, SimplePixelShader*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);
	~Material();
};

