#pragma once
#include "SystemCore.h"
#include "Mesh.h"

class Renderer
{
	SystemCore *core;
public:
	void Draw(Mesh *mesh);
	Renderer(SystemCore* core);
	~Renderer();
};

