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
	if (mesh == nullptr) {
		throw std::exception("Null Mesh");
	}
	core->Draw();
}

void Renderer::Draw(Entity *entity)
{
	if (entity == nullptr) {
		throw std::exception("Null Mesh");
	}
	core->Draw();
}
