#include "Renderer.h"

/// <summary>
/// Draw given mesh object. 
/// </summary>
/// <param name="mesh"></param>
/// <param name="context"></param>
void SystemRenderer::Draw(Mesh *mesh, ID3D11DeviceContext *context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	auto vertexBuffer = mesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}

/// <summary>
/// Sets shader data in the GPU before drawing. 
/// </summary>
/// <param name="entity"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
void SystemRenderer::SetShaders(Entity *entity, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	auto pixelShader = entity->GetMesh()->GetMaterial()->GetPixelShader();
	auto vertexShader = entity->GetMesh()->GetMaterial()->GetVertexShader();
	vertexShader->SetMatrix4x4(WORLD_STR, entity->GetWorldMatrix());
	vertexShader->SetMatrix4x4(VIEW_STR, viewMatrix);
	vertexShader->SetMatrix4x4(PROJECTION_STR, projectionMatrix);
	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();
}

/// <summary>
/// Renderer class constructor.
/// </summary>
/// <param name="core"></param>
/// <param name="width"></param>
/// <param name="height"></param>
Renderer::Renderer(SystemCore* core, int width, int height)
{
	camera = nullptr;
	screenHeight = height;
	screenWidth = width;
	this->core = core;
	internalRenderer = new SystemRenderer();
	XMVECTOR pos = XMVectorSet(0, 0, -25, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,
		dir,
		up);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI,
		(float)width / (float)height,
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

Renderer::~Renderer()
{
	delete internalRenderer;
}

/// <summary>
/// Gets the internal renderer class.
/// </summary>
/// <returns></returns>
SystemRenderer* Renderer::GetInternalRenderer()
{
	return internalRenderer;
}

/// <summary>
/// Initializes the renderer.
/// </summary>
void Renderer::Initialize()
{
	core->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/// <summary>
/// Gets the view matrix.
/// </summary>
/// <returns></returns>
XMFLOAT4X4 Renderer::GetViewMatrix()
{
	return camera->GetViewMatrix();
	//return viewMatrix;
}

/// <summary>
/// Gets the projection matrix. 
/// </summary>
/// <returns></returns>
XMFLOAT4X4 Renderer::GetProjectionMatrix()
{
	return camera->GetProjectionMatrix();
	//return projectionMatrix;
}

void Renderer::SetProjectionMatrix(int width, int height)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI,
		(float)width/height,
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

void Renderer::UseCamera(Camera * camera)
{
	this->camera = camera;
}

/// <summary>
/// Draws the given mesh. 
/// </summary>
/// <param name="mesh"></param>
void Renderer::Draw(Mesh *mesh)
{
	auto context = core->GetDeviceContext();
	if (mesh == nullptr) {
		throw std::exception("Null Mesh");
	}

	internalRenderer->Draw(mesh, context);
	core->Draw();
}

/// <summary>
/// Draws the given entity. Internally references to the mesh of the entity. 
/// </summary>
/// <param name="entity"></param>
void Renderer::Draw(Entity *entity)
{
	if (entity == nullptr) {
		throw std::exception("Null Mesh");
	}
	internalRenderer->SetShaders(entity, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	Draw(entity->GetMesh());
	core->Draw();
}
