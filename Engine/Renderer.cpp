#include "Renderer.h"



Renderer::Renderer(SystemCore* core)
{
	this->core = core;
}


Renderer::~Renderer()
{
}

void Renderer::Draw(Mesh *mesh)
{
	core->Draw();
}
