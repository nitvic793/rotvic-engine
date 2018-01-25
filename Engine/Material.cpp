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

void Material::LoadDefaultShaders()
{
	auto device = core->GetDevice();
	auto context = core->GetDeviceContext();
	vertexShader = new SimpleVertexShader(device, context);
	auto isVertexShaderValid = vertexShader->LoadShaderFile(L"VertexShader.cso");
	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

SimplePixelShader *Material::GetPixelShader() 
{
	return pixelShader;
}

SimpleVertexShader *Material::GetVertexShader()
{
	return vertexShader;
}