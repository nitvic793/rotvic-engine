#include "Material.h"

Material::Material(SystemCore* core)
{
	this->core = core;
}

Material::~Material()
{
	delete pixelShader;
	delete vertexShader;
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