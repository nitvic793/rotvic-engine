#include "ResourceManager.h"
#include "MeshLoader.h"


void ResourceManager::LoadResources(ConfigMap config, SystemCore* core)
{
	Material *mat = new Material(core);
	mat->LoadDefaultShaders();
	materials.insert(std::pair<std::string, Material*>("Default", mat));
	MeshLoader loader(core);
	Mesh* mesh = loader.Load(config["mesh"]);
	mesh->SetMaterial(mat);
	meshes.insert(std::pair<std::string, Mesh*>("Default", mesh));
	mesh = loader.Load(config["mesh"], true);
	mesh->SetMaterial(mat);
	meshes.insert(std::pair<std::string, Mesh*>("Green", mesh));
}

Mesh * ResourceManager::GetMesh(std::string meshName)
{
	return meshes[meshName];
}

Material * ResourceManager::GetMaterial(std::string materialName)
{
	return materials[materialName];
}

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
	for (auto item : meshes) {
		delete item.second;
	}

	for (auto item : materials) {
		delete item.second;
	}

	meshes.clear();
	materials.clear();
}
