/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "Material.h"
#include "ResourceManager.h"

Material::Material(SystemCore* core)
{
	this->core = core;
}

Material::Material(SystemCore* core, SimpleVertexShader *vertexShader, SimplePixelShader *pixelShader, ID3D11ShaderResourceView *srv, ID3D11ShaderResourceView *normal, ID3D11SamplerState *samplerState)
{
	auto rm = ResourceManager::GetInstance();
	this->core = core;
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	textureSRV = srv;
	sampler = samplerState;
	normalSRV = normal;
	roughnessSRV = rm->GetTexture("defaultSpecular");
}

Material::~Material()
{
}

/// <summary>
/// Loads default available shaders. 
/// </summary>
void Material::LoadDefaultShaders()
{
	auto device = core->GetDevice();
	auto context = core->GetDeviceContext();
	vertexShader = new SimpleVertexShader(device, context);
	auto isVertexShaderValid = vertexShader->LoadShaderFile(L"VertexShader.cso");
	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

/// <summary>
/// Get pixel shader.
/// </summary>
/// <returns></returns>
SimplePixelShader *Material::GetPixelShader() 
{
	return pixelShader;
}

/// <summary>
/// Get vertex shader.
/// </summary>
/// <returns></returns>
SimpleVertexShader *Material::GetVertexShader()
{
	return vertexShader;
}


void Material::SetRoughnessTexture(ID3D11ShaderResourceView * roughness)
{
	roughnessSRV = roughness;
}

ID3D11ShaderResourceView * Material::GetSRV()
{
	return textureSRV;
}

ID3D11ShaderResourceView * Material::GetNormalSRV()
{
	return normalSRV;
}

ID3D11ShaderResourceView * Material::GetRoughnessSRV()
{
	return roughnessSRV;
}

ID3D11SamplerState * Material::GetSampler()
{
	return sampler;
}