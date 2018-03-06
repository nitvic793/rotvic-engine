#include "ResourceManager.h"
#include "MeshLoader.h"

ResourceManager* ResourceManager::instance = nullptr;

void ResourceManager::LoadResources(ConfigMap config, SystemCore* core)
{
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
	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	debugShader = new SimplePixelShader(device, context);
	debugShader->LoadShaderFile(L"DebugShader.cso");

	Material *material = nullptr;
	ID3D11ShaderResourceView *srv = nullptr;
	ID3D11ShaderResourceView *normalSrv = nullptr; 
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
}

Mesh * ResourceManager::GetMesh(std::string meshName)
{
	return meshes[meshName];
}

Material * ResourceManager::GetMaterial(std::string materialName)
{
	return materials[materialName];
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

	textures.clear();
	meshes.clear();
	materials.clear();
	delete vertexShader;
	delete pixelShader;
	delete debugShader;
	if (sampler)
		sampler->Release();
}
