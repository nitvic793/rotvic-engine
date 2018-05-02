/// <summary>
/// Author: Nitish Victor
/// FBX Animation Author: Rahul SV
/// </summary>

#include "ResourceManager.h"
#include "MeshLoader.h"
#include "DDSTextureLoader.h"
#include "ObjLoader.h"
#include <locale>
#include <codecvt>
#include <string>
#include "EventSystem.h"

std::wstring to_wstring(std::string narrow)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::string narrow = converter.to_bytes(wide_utf16_source_string);
	return converter.from_bytes(narrow);
}

Vertex MapObjlToVertex(objl::Vertex vertex)
{
	auto pos = XMFLOAT3(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
	auto normal = XMFLOAT3(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
	auto uv = XMFLOAT2(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y);
	return { pos, normal, uv };
}

ID3D11ShaderResourceView* LoadSRV(std::wstring filename, ID3D11Device* device)
{
	ID3D11ShaderResourceView* srv = nullptr;
	if (filename.find(L".dds") != std::string::npos)
	{
		CreateDDSTextureFromFile(device, filename.c_str(), nullptr, &srv);
	}
	else
	{
		CreateWICTextureFromFile(device, filename.c_str(), nullptr, &srv);
	}

	return srv;
}

ResourceManager* ResourceManager::instance = nullptr;

void ResourceManager::SetAsyncLoader(AsyncLoader* loader)
{
	asyncLoader = loader;
}

void ResourceManager::SetAsyncWorker(WorkerThread * worker)
{
	asyncWorker = worker;
}

void ResourceManager::LoadTextureAsync(std::string filename, std::string texName, std::function<void()> onLoad)
{
	auto device = core->GetDevice();
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>(texName, nullptr));
	asyncLoader->AsyncLoadFile(filename, [=](void* texData, UINT size)
	{
		ID3D11ShaderResourceView* srv;
		CreateWICTextureFromMemory(device, (uint8_t*)texData, (size_t)size, nullptr, &srv);
		textures[texName] = srv;
		if (onLoad)
		{
			onLoad();
		}
	});
}

void ResourceManager::LoadResourcesAsync(ConfigMap config, SystemCore * core, std::function<void()> callback)
{
	std::vector<Vertex> planeVertices = {
		{ XMFLOAT3(-0.5f,0,0.5f), XMFLOAT3(0,1,0), XMFLOAT2(0,0) },
		{ XMFLOAT3(0.5f,0,0.5f), XMFLOAT3(0,1,0), XMFLOAT2(1,0) },
		{ XMFLOAT3(0.5f,0,-0.5f), XMFLOAT3(0,1,0), XMFLOAT2(1,1) },
		{ XMFLOAT3(-0.5f,0,-0.5f), XMFLOAT3(0,1,0), XMFLOAT2(0,1) }
	};

	UINT indices[] = { 0,1,2,0,2,3 };
	auto plane = new Mesh(core);
	plane->Initialize(planeVertices.data(), 4, indices, 6);
	meshes.insert(std::pair<std::string, Mesh*>("plane", plane));
	this->core = core;
	auto device = core->GetDevice();
	auto context = core->GetDeviceContext();
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);

	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");
	vertexShaders.insert(VertexShaderMapType("default", vertexShader));

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	pixelShaders.insert(PixelShaderMapType("default", pixelShader));

	debugShader = new SimplePixelShader(device, context);
	debugShader->LoadShaderFile(L"DebugShader.cso");
	pixelShaders.insert(PixelShaderMapType("debug", debugShader));

	debugVertexShader = new SimpleVertexShader(device, context);
	debugVertexShader->LoadShaderFile(L"DebugVertexShader.cso");
	vertexShaders.insert(VertexShaderMapType("debug", debugVertexShader));

	auto skyVS = new SimpleVertexShader(device, context);
	skyVS->LoadShaderFile(L"SkyVS.cso");
	vertexShaders.insert(VertexShaderMapType("sky", skyVS));

	auto skyPS = new SimplePixelShader(device, context);
	skyPS->LoadShaderFile(L"SkyPS.cso");
	pixelShaders.insert(PixelShaderMapType("sky", skyPS));

	vertexShaderAnimated = new SimpleVertexShader(device, context);
	vertexShaderAnimated->LoadShaderFile(L"AnimationVS.cso");
	vertexShaders.insert(VertexShaderMapType("man", vertexShaderAnimated));

	pixelShaderAnimated = new SimplePixelShader(device, context);
	pixelShaderAnimated->LoadShaderFile(L"AnimationPS.cso");
	pixelShaders.insert(PixelShaderMapType("man", pixelShaderAnimated));

	auto events = EventSystem::GetInstance();
	auto job = [this, device, core, callback, events](void* args)
	{
		ID3D11CommandList *commandList;
		CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
		ID3D11DeviceContext *deferredContext;
		device->CreateDeferredContext(0, &deferredContext);
		auto context = deferredContext;
		
		Material *material = nullptr;
		ID3D11ShaderResourceView *srv = nullptr;
		ID3D11ShaderResourceView *normalSrv = nullptr;

		CreateDDSTextureFromFile(device, L"../../Assets/Textures/SunnyCubeMap.dds", 0, &srv);
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("skybox", srv));

		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/metal.jpg", nullptr, &srv);
		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/metalNormal.png", nullptr, &normalSrv);
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("metal", srv));
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("metalNormal", normalSrv));
		material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
		materials.insert(std::pair<std::string, Material*>("metal", material));

		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/fabric.jpg", nullptr, &srv);
		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/fabricNormal.png", nullptr, &normalSrv);
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("fabric", srv));
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("fabricNormal", normalSrv));
		material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);

		materials.insert(std::pair<std::string, Material*>("fabric", material));

		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/default.png", nullptr, &srv);
		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/defaultNormal.png", nullptr, &normalSrv);
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("default", srv));
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("defaultNormal", normalSrv));
		material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
		materials.insert(std::pair<std::string, Material*>("default", material));

		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/grass01.jpg", nullptr, &srv);
		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/grass01_n.jpg", nullptr, &normalSrv);
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("grass", srv));
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("grassNormal", normalSrv));
		material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
		materials.insert(std::pair<std::string, Material*>("grass", material));

		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/wood.jpg", nullptr, &srv);
		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/woodNormal.png", nullptr, &normalSrv);

		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("wood", srv));
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("woodNormal", normalSrv));
		material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
		materials.insert(std::pair<std::string, Material*>("wood", material));

		meshes.insert(std::pair<std::string, Mesh*>("sphere", new Mesh("../../Assets/Models/sphere.obj", core)));
		meshes.insert(std::pair<std::string, Mesh*>("cone", new Mesh("../../Assets/Models/cone.obj", core)));
		meshes.insert(std::pair<std::string, Mesh*>("cylinder", new Mesh("../../Assets/Models/cylinder.obj", core)));
		meshes.insert(std::pair<std::string, Mesh*>("cube", new Mesh("../../Assets/Models/cube.obj", core)));
		meshes.insert(std::pair<std::string, Mesh*>("helix", new Mesh("../../Assets/Models/helix.obj", core)));
		meshes.insert(std::pair<std::string, Mesh*>("torus", new Mesh("../../Assets/Models/torus.obj", core)));


		// Animation data
		fbxLoader.LoadNodes(fbxLoader.scene->GetRootNode(), fbxLoader.skeleton.mJoints);
		fbxLoader.LoadNodes(fbxLoader.scene2->GetRootNode(), fbxLoader.skeleton.mJoints2);
		int numChildren = fbxLoader.scene->GetRootNode()->GetChildCount();


		FbxNode* childNode = fbxLoader.scene->GetRootNode()->GetChild(1);
		FbxString name1 = childNode->GetName();
		meshes.insert(std::pair<std::string, Mesh*>("man", fbxLoader.GetMesh(childNode, device)));

		childNode = fbxLoader.scene2->GetRootNode()->GetChild(1);
		fbxLoader.GetMatricesFromMesh(childNode, device, fbxLoader.skeleton.mJoints2);

		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/axe_base_color2.png", nullptr, &srv);
		CreateWICTextureFromFile(device, context, L"../../Assets/Textures/axe_base_normal2.png", nullptr, &normalSrv);
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("man", srv));
		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("manNormal", normalSrv));
		material = new Material(core, vertexShaderAnimated, pixelShaderAnimated, srv, normalSrv, sampler);
		materials.insert(std::pair<std::string, Material*>("man", material));		
		deferredContext->FinishCommandList(false, &commandList);

		events->EmitEventQueued("ResourceLoadComplete", GENERIC, commandList, this);
		callback();
		deferredContext->Release();
	};

	events->RegisterEventCallback("ResourceLoadComplete", this, [=](void* args)
	{
		auto commandList = (ID3D11CommandList*)args;
		context->ExecuteCommandList(commandList, true);
		commandList->Release();

	});
	asyncWorker->EnqueueJob({ job, nullptr });
}

void ResourceManager::LoadResources(ConfigMap config, SystemCore* core)
{
	std::vector<Vertex> planeVertices = {
		{ XMFLOAT3(-0.5f,0,0.5f), XMFLOAT3(0,1,0), XMFLOAT2(0,0) },
		{ XMFLOAT3(0.5f,0,0.5f), XMFLOAT3(0,1,0), XMFLOAT2(1,0) },
		{ XMFLOAT3(0.5f,0,-0.5f), XMFLOAT3(0,1,0), XMFLOAT2(1,1) },
		{ XMFLOAT3(-0.5f,0,-0.5f), XMFLOAT3(0,1,0), XMFLOAT2(0,1)}
	};

	UINT indices[] = { 0,1,2,0,2,3 };
	auto plane = new Mesh(core);
	plane->Initialize(planeVertices.data(), 4, indices, 6);
	meshes.insert(std::pair<std::string, Mesh*>("plane", plane));
	this->core = core;
	auto device = core->GetDevice();
	auto context = core->GetDeviceContext();

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);

	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");
	vertexShaders.insert(VertexShaderMapType("default", vertexShader));

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	pixelShaders.insert(PixelShaderMapType("default", pixelShader));

	debugShader = new SimplePixelShader(device, context);
	debugShader->LoadShaderFile(L"DebugShader.cso");
	pixelShaders.insert(PixelShaderMapType("debug", debugShader));

	debugVertexShader = new SimpleVertexShader(device, context);
	debugVertexShader->LoadShaderFile(L"DebugVertexShader.cso");
	vertexShaders.insert(VertexShaderMapType("debug", debugVertexShader));

	auto skyVS = new SimpleVertexShader(device, context);
	skyVS->LoadShaderFile(L"SkyVS.cso");
	vertexShaders.insert(VertexShaderMapType("sky", skyVS));

	auto skyPS = new SimplePixelShader(device, context);
	skyPS->LoadShaderFile(L"SkyPS.cso");
	pixelShaders.insert(PixelShaderMapType("sky", skyPS));

	vertexShaderAnimated = new SimpleVertexShader(device, context);
	vertexShaderAnimated->LoadShaderFile(L"AnimationVS.cso");
	vertexShaders.insert(VertexShaderMapType("man", vertexShaderAnimated));

	pixelShaderAnimated = new SimplePixelShader(device, context);
	pixelShaderAnimated->LoadShaderFile(L"AnimationPS.cso");
	pixelShaders.insert(PixelShaderMapType("man", pixelShaderAnimated));

	Material *material = nullptr;
	ID3D11ShaderResourceView *srv = nullptr;
	ID3D11ShaderResourceView *normalSrv = nullptr;

	CreateDDSTextureFromFile(device, L"../../Assets/Textures/SunnyCubeMap.dds", 0, &srv);
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("skybox", srv));

	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/metal.jpg", nullptr, &srv);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/metalNormal.png", nullptr, &normalSrv);
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("metal", srv));
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("metalNormal", normalSrv));
	material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
	materials.insert(std::pair<std::string, Material*>("metal", material));

	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/fabric.jpg", nullptr, &srv);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/fabricNormal.png", nullptr, &normalSrv);
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("fabric", srv));
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("fabricNormal", normalSrv));
	material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);

	materials.insert(std::pair<std::string, Material*>("fabric", material));

	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/default.png", nullptr, &srv);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/defaultNormal.png", nullptr, &normalSrv);
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("default", srv));
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("defaultNormal", normalSrv));
	material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
	materials.insert(std::pair<std::string, Material*>("default", material));

	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/grass01.jpg", nullptr, &srv);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/grass01_n.jpg", nullptr, &normalSrv);
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("grass", srv));
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("grassNormal", normalSrv));
	material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
	materials.insert(std::pair<std::string, Material*>("grass", material));

	//CreateWICTextureFromFile(device, context, L"../../Assets/Textures/wood.jpg", nullptr, &srv);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/woodNormal.png", nullptr, &normalSrv);
	LoadTextureAsync("../../Assets/Textures/wood.jpg", "wood", [&] {
		auto material = new Material(core, vertexShader, pixelShader, textures["wood"], normalSrv, sampler);
		materials.insert(std::pair<std::string, Material*>("wood", material));
	});
	//textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("wood", srv));
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("woodNormal", normalSrv));
	/*material = new Material(core, vertexShader, pixelShader, srv, normalSrv, sampler);
	materials.insert(std::pair<std::string, Material*>("wood", material));*/

	meshes.insert(std::pair<std::string, Mesh*>("sphere", new Mesh("../../Assets/Models/sphere.obj", core)));
	meshes.insert(std::pair<std::string, Mesh*>("cone", new Mesh("../../Assets/Models/cone.obj", core)));
	meshes.insert(std::pair<std::string, Mesh*>("cylinder", new Mesh("../../Assets/Models/cylinder.obj", core)));
	meshes.insert(std::pair<std::string, Mesh*>("cube", new Mesh("../../Assets/Models/cube.obj", core)));
	meshes.insert(std::pair<std::string, Mesh*>("helix", new Mesh("../../Assets/Models/helix.obj", core)));
	meshes.insert(std::pair<std::string, Mesh*>("torus", new Mesh("../../Assets/Models/torus.obj", core)));


	// Animation data
	fbxLoader.LoadNodes(fbxLoader.scene->GetRootNode(), fbxLoader.skeleton.mJoints);
	fbxLoader.LoadNodes(fbxLoader.scene2->GetRootNode(), fbxLoader.skeleton.mJoints2);
	int numChildren = fbxLoader.scene->GetRootNode()->GetChildCount();


	FbxNode* childNode = fbxLoader.scene->GetRootNode()->GetChild(1);
	FbxString name1 = childNode->GetName();
	meshes.insert(std::pair<std::string, Mesh*>("man", fbxLoader.GetMesh(childNode, device)));

	childNode = fbxLoader.scene2->GetRootNode()->GetChild(1);
	fbxLoader.GetMatricesFromMesh(childNode, device, fbxLoader.skeleton.mJoints2);

	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/axe_base_color2.png", nullptr, &srv);
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/axe_base_normal2.png", nullptr, &normalSrv);
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("man", srv));
	textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>("manNormal", normalSrv));
	material = new Material(core, vertexShaderAnimated, pixelShaderAnimated, srv, normalSrv, sampler);
	materials.insert(std::pair<std::string, Material*>("man", material));
}

Mesh * ResourceManager::GetMesh(std::string meshName)
{
	if (meshes.find(meshName) != meshes.end())
		return meshes[meshName];
	return nullptr;
}

Material * ResourceManager::GetMaterial(std::string materialName)
{
	if (materials.find(materialName) != materials.end())
		return materials[materialName];
	return nullptr;
}

ID3D11ShaderResourceView* ResourceManager::GetTexture(std::string textureName)
{
	if (textures.find(textureName) != textures.end())
		return textures[textureName];
	return nullptr;
}

ResourceManager* ResourceManager::GetInstance()
{
	return instance;
}

ResourceManager::ResourceManager()
{
	instance = this;
}

ResourceManager::~ResourceManager()
{
	for (auto item : meshes) {
		delete item.second;
	}

	for (auto item : materials) {
		delete item.second;
	}

	for (auto item : textures) {
		item.second->Release();
	}

	for (auto item : vertexShaders) {
		delete item.second;
	}

	for (auto item : pixelShaders) {
		delete item.second;
	}

	textures.clear();
	meshes.clear();
	materials.clear();
	if (sampler)
		sampler->Release();
}
