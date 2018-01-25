#pragma once
#include "SystemCore.h"
#include "Mesh.h"
#include "Entity.h"

class Renderer
{
	SystemCore *core;
public:
	virtual void Draw(Mesh *mesh);
	virtual void Draw(Entity *entity);
	Renderer(SystemCore* core);
	~Renderer();
};

