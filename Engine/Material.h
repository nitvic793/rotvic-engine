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
	/// <summary>
	/// Gets the main texture associated with material
	/// </summary>
	/// <returns></returns>
	ID3D11ShaderResourceView*	GetSRV();

	/// <summary>
	/// Gets the normal texture associated with the material.
	/// </summary>
	/// <returns></returns>
	ID3D11ShaderResourceView*	GetNormalSRV();

	/// <summary>
	/// Gets the D3D11 sampler. 
	/// </summary>
	/// <returns></returns>
	ID3D11SamplerState*			GetSampler();

	/// <summary>
	/// Gets the pixel shader. 
	/// </summary>
	/// <returns></returns>
	SimplePixelShader *GetPixelShader();

	/// <summary>
	/// Gets the vertex shader. 
	/// </summary>
	/// <returns></returns>
	SimpleVertexShader *GetVertexShader();
	void LoadDefaultShaders();
	Material(SystemCore*);
	Material(SystemCore*, SimpleVertexShader*, SimplePixelShader*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11SamplerState*);
	~Material();
};

