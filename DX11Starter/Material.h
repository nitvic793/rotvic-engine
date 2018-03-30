#pragma once
#include "SimpleShader.h"
#include <memory>

using namespace std;

class Material
{
	shared_ptr<SimpleVertexShader> vertexShader;
	shared_ptr<SimplePixelShader>  pixelShader;

	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* sampler;

public:

	Material(shared_ptr<SimpleVertexShader>, shared_ptr<SimplePixelShader>, ID3D11ShaderResourceView*, ID3D11SamplerState*);
	
	ID3D11ShaderResourceView* getShaderResourceView();
	ID3D11SamplerState* getSamplerState();
	shared_ptr<SimpleVertexShader>  getVertexShader();
	shared_ptr<SimplePixelShader> getPixelShader();
};