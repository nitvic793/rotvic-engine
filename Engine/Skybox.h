#pragma once

#include "Mesh.h"
#include "SimpleShader.h"
#include <d3d11.h>
#include <memory>
#include <wrl.h>

class Skybox
{
	Mesh* mesh;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* skyTextureSRV;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
public:
	Skybox(Mesh* skyMesh, SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* skyTexture, ID3D11Device* device);
	Mesh* GetMesh() const;
	ID3D11ShaderResourceView* GetSkySRV() const;
	SimpleVertexShader* GetVertexShader() const;
	SimplePixelShader* GetPixelShader() const;
	ID3D11RasterizerState* GetRasterizerState() const;
	ID3D11DepthStencilState* GetDepthStencilState() const;
	~Skybox();
};

