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
void SystemRenderer::SetShaders(Entity *entity, Camera *camera, LightsMap lights)
{
	auto material = entity->GetMaterial();
	auto pixelShader = material->GetPixelShader();
	auto vertexShader = material->GetVertexShader();
	vertexShader->SetMatrix4x4(WORLD_STR, entity->GetWorldMatrix());
	vertexShader->SetMatrix4x4(VIEW_STR, camera->GetViewMatrix());
	vertexShader->SetMatrix4x4(PROJECTION_STR, camera->GetProjectionMatrix());
	pixelShader->SetSamplerState("basicSampler", material->GetSampler());
	pixelShader->SetShaderResourceView("diffuseTexture", material->GetSRV());
	if (material->GetNormalSRV())
		pixelShader->SetShaderResourceView("normalTexture", material->GetNormalSRV());
	else pixelShader->SetShaderResourceView("normalTexture", nullptr);
	pixelShader->SetFloat3("cameraPosition", camera->GetPosition());
	for (auto lightPair : lights)
	{
		auto light = lightPair.second;
		switch (light->Type)
		{
		case Directional:
			pixelShader->SetData(lightPair.first, light->GetLight<DirectionalLight>(), sizeof(DirectionalLight));
			break;
		case Point:
			pixelShader->SetData(lightPair.first, light->GetLight<PointLight>(), sizeof(PointLight));
			break;
		}
	}
	vertexShader->CopyAllBufferData();
	pixelShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();
}

void Renderer::SetShadersAndDrawAnimated(Entity * entity, Camera * camera, LightsMap lights ,ID3D11DeviceContext * context)
{
	auto material = entity->GetMaterial();
	auto pixelShader = material->GetPixelShader();
	auto vertexShader = material->GetVertexShader();
	vertexShader->SetMatrix4x4("world", entity->GetWorldMatrix());
	vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix());

	int bonesSize = 0;
	bonesSize = (sizeof(XMFLOAT4X4) * 20 * 2);
	Bones bones[20];
	int numBones = resourceManager->fbxLoader.skeleton.mJoints.size();

	resourceManager->fbxLoader.GetAnimatedMatrixExtra();

	for (int i = 0; i < numBones; i++)
	{

		XMMATRIX jointTransformMatrix = XMLoadFloat4x4(&resourceManager->fbxLoader.skeleton.mJoints[i].mTransform);
		XMMATRIX invJointTransformMatrix = XMLoadFloat4x4(&resourceManager->fbxLoader.skeleton.mJoints[i].mGlobalBindposeInverse);

		XMFLOAT4X4 trans = {};
		XMStoreFloat4x4(&trans, XMMatrixTranspose(jointTransformMatrix));
		bones[i].BoneTransform = trans;
		XMFLOAT4X4 trans2 = {};
		XMStoreFloat4x4(&trans2, XMMatrixTranspose(invJointTransformMatrix));
		bones[i].InvBoneTransform = trans2;
	}
	vertexShader->SetData("bones", &bones, bonesSize);


	for (auto lightPair : lights)
	{
		auto light = lightPair.second;
		switch (light->Type)
		{
		case Directional:
			pixelShader->SetData("light1", light->GetLight<DirectionalLight>(), sizeof(DirectionalLight));
			break;
		}
	}
	vertexShader->CopyAllBufferData();
	pixelShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();

	UINT stride = sizeof(VertexAnimated);
	UINT offset = 0;
	auto vertexBuffer = entity->GetMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(entity->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(entity->GetMesh()->GetIndexCount(), 0, 0);
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

void Renderer::GetResourceManager(ResourceManager * rm)
{
	resourceManager = rm;
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
}

/// <summary>
/// Gets the projection matrix. 
/// </summary>
/// <returns></returns>
XMFLOAT4X4 Renderer::GetProjectionMatrix()
{
	return camera->GetProjectionMatrix();
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

void Renderer::SetLights(LightsMap lightsMap)
{
	lights = lightsMap;
}

void Renderer::ClearScreen()
{
	const float color[4] = { 0.11f, 0.11f, 0.11f, 0.0f };

	core->GetDeviceContext()->ClearRenderTargetView(core->GetBackBufferRenderTargetView(), color);
	core->GetDeviceContext()->ClearDepthStencilView(
		core->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void Renderer::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitive)
{
	core->GetDeviceContext()->IASetPrimitiveTopology(primitive);
}

void Renderer::Present()
{
	core->GetSwapChain()->Present(0, 0);
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
	if (!entity->isAnimated)
	{
		internalRenderer->SetShaders(entity, camera, lights);
		Draw(entity->GetMesh());
	}
	else
		SetShadersAndDrawAnimated(entity, camera, lights, core->GetDeviceContext());
	
	core->Draw();
}

void Renderer::Draw(Skybox* sky)
{
	auto rm = ResourceManager::GetInstance();
	auto mesh = sky->GetMesh();
	auto ps = sky->GetPixelShader();
	auto vs = sky->GetVertexShader();
	vs->SetMatrix4x4("view", camera->GetViewMatrix());
	vs->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	vs->CopyAllBufferData();
	vs->SetShader();

	ps->SetShaderResourceView("SkyTexture", sky->GetSkySRV());
	ps->SetSamplerState("BasicSampler", rm->sampler);
	ps->SetShader();

	core->GetDeviceContext()->RSSetState(sky->GetRasterizerState());
	core->GetDeviceContext()->OMSetDepthStencilState(sky->GetDepthStencilState(), 0);
	Draw(mesh);

	//Reset pipeline
	core->GetDeviceContext()->OMSetBlendState(0, 0, 0xFFFFFFFF);
	core->GetDeviceContext()->OMSetDepthStencilState(0, 0);
	core->GetDeviceContext()->RSSetState(nullptr);
}
