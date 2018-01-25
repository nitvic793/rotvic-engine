#pragma once
#include "SystemCore.h"
#include "Mesh.h"
#include "Entity.h"

class SystemRenderer {
public:
	virtual void Draw(Mesh *mesh, ID3D11DeviceContext *context);
	virtual void SetShaders(Entity *entity);
};

class Renderer
{
	SystemRenderer *internalRenderer;
	SystemCore *core;
public:
	virtual SystemRenderer* GetInternalRenderer();
	virtual void Draw(Mesh *mesh);
	virtual void Draw(Entity *entity);
	virtual void Initialize();
	Renderer(SystemCore* core);
	~Renderer();
};

