#include "Material.h"

Material::Material(shared_ptr<SimpleVertexShader>  VS, shared_ptr<SimplePixelShader>  PS, ID3D11ShaderResourceView* SRV,ID3D11SamplerState* Sampler)
{
	vertexShader = VS;
	pixelShader = PS;
	srv = SRV;
	sampler = Sampler;

	pixelShader->SetShaderResourceView("DiffuseTexture", srv);
	pixelShader->SetSamplerState("Sampler", sampler);
}



ID3D11ShaderResourceView * Material::getShaderResourceView()
{
	return srv;
}

ID3D11SamplerState * Material::getSamplerState()
{
	return sampler;
}

shared_ptr<SimpleVertexShader>  Material::getVertexShader()
{
	return vertexShader;
}

shared_ptr<SimplePixelShader>  Material::getPixelShader()
{
	return pixelShader;
}
