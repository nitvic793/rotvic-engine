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
public:
	SimplePixelShader *GetPixelShader();
	SimpleVertexShader *GetVertexShader();
	void LoadDefaultShaders();
	Material(SystemCore*);
	~Material();
};

