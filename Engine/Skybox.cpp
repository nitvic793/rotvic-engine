#include "Skybox.h"



Skybox::Skybox(Mesh* skyMesh, SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* skyTexture, ID3D11Device* device):
	mesh(skyMesh),
	pixelShader(ps),
	vertexShader(vs),
	skyTextureSRV(skyTexture)
{

	D3D11_RASTERIZER_DESC rs = {};
	rs.CullMode = D3D11_CULL_FRONT;
	rs.FillMode = D3D11_FILL_SOLID;
	device->CreateRasterizerState(&rs, rasterizerState.ReleaseAndGetAddressOf());

	D3D11_DEPTH_STENCIL_DESC ds = {};
	ds.DepthEnable = true;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&ds, depthStencilState.ReleaseAndGetAddressOf());
}

Mesh* Skybox::GetMesh() const
{
	return mesh;
}

ID3D11ShaderResourceView* Skybox::GetSkySRV() const
{
	return skyTextureSRV;
}

SimpleVertexShader * Skybox::GetVertexShader() const
{
	return vertexShader;
}

SimplePixelShader * Skybox::GetPixelShader() const
{
	return pixelShader;
}

ID3D11RasterizerState * Skybox::GetRasterizerState() const
{
	return rasterizerState.Get();
}

ID3D11DepthStencilState * Skybox::GetDepthStencilState() const
{
	return depthStencilState.Get();
}


Skybox::~Skybox()
{
	rasterizerState.Reset();
	depthStencilState.Reset();
	rasterizerState = nullptr;
	depthStencilState = nullptr;
}
