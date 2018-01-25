#include "Renderer.h"

void SystemRenderer::Draw(Mesh *mesh, ID3D11DeviceContext *context)
{

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	auto vertexBuffer = mesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}

void SystemRenderer::SetShaders(Entity *entity)
{
	auto pixelShader = entity->GetMesh()->GetMaterial()->GetPixelShader();
	auto vertexShader = entity->GetMesh()->GetMaterial()->GetVertexShader();
	vertexShader->SetMatrix4x4("world", entity->GetWorldMatrix());
	vertexShader->SetMatrix4x4("view", entity->GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", entity->GetProjectionMatrix());
	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();
}

Renderer::Renderer(SystemCore* core)
{
	this->core = core;
	internalRenderer = new SystemRenderer();
}

Renderer::~Renderer()
{
	delete internalRenderer;
}

SystemRenderer* Renderer::GetInternalRenderer()
{
	return internalRenderer;
}

void Renderer::Initialize()
{
	core->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::Draw(Mesh *mesh)
{
	auto context = core->GetDeviceContext();
	if (mesh == nullptr) {
		throw std::exception("Null Mesh");
	}

	internalRenderer->Draw(mesh, context);
	core->Draw();
}

void Renderer::Draw(Entity *entity)
{
	if (entity == nullptr) {
		throw std::exception("Null Mesh");
	}
	internalRenderer->SetShaders(entity);
	Draw(entity->GetMesh());
	core->Draw();
}
