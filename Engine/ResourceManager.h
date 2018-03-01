#pragma once
#include "Mesh.h"
#include "Material.h"
#include "ConfigLoader.h"
#include "WICTextureLoader.h"
#include <map>

/// <summary>
/// Resource manager class. A central class to manage all resources required by the game. 
/// </summary>
class ResourceManager
{
	ID3D11SamplerState *sampler;
	SimpleVertexShader *vertexShader;
	SimplePixelShader *pixelShader;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::map<std::string, ID3D11ShaderResourceView*> textures;

public:
	/// <summary>
	/// Loads resources based on given config data.
	/// </summary>
	/// <param name="config">Loaded Config data</param>
	/// <param name="core">System Core</param>
	virtual void LoadResources(ConfigMap config, SystemCore *core);

	/// <summary>
	/// Returns the mesh associated with given string
	/// </summary>
	/// <param name="meshName">Name of mesh</param>
	/// <returns>Mesh</returns>
	Mesh* GetMesh(std::string meshName);

	/// <summary>
	/// Returns the material associated with given string
	/// </summary>
	/// <param name="materialName">Name of material</param>
	/// <returns>Material</returns>
	Material* GetMaterial(std::string materialName);

	ResourceManager();
	~ResourceManager();
};

