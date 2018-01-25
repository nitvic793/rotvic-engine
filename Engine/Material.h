#pragma once
#include "SimpleShader.h"
#include "SystemCore.h"

class Material
{
protected:
	SystemCore *core;
	SimplePixelShader *pixelShader;
	SimpleVertexShader *vertexShader;
public:
	SimplePixelShader *GetPixelShader();
	SimpleVertexShader *GetVertexShader();
	void LoadDefaultShaders();
	Material(SystemCore*);
	~Material();
};

