#pragma once
#include "Mesh.h"
#include "Material.h"
#include "ConfigLoader.h"
#include "WICTextureLoader.h"
#include <map>

typedef std::map<std::string, SimpleVertexShader*> VertexShaderMap;
typedef std::map<std::string, SimplePixelShader*> PixelShaderMap;
typedef std::pair<std::string, SimpleVertexShader*> VertexShaderMapType;
typedef std::pair<std::string, SimplePixelShader*> PixelShaderMapType;

/// <summary>
/// Resource manager class. A central class to manage all resources required by the game. 
/// </summary>
class ResourceManager
{
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::map<std::string, ID3D11ShaderResourceView*> textures;
	static ResourceManager* instance;
public:
	SimpleVertexShader *vertexShader;
	SimplePixelShader *pixelShader;
	SimplePixelShader *debugShader;
	SimpleVertexShader *debugVertexShader;
	ID3D11SamplerState *sampler;
	VertexShaderMap vertexShaders;
	PixelShaderMap pixelShaders;

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

	/// <summary>
	/// Gets the Shader Resource View associated with given name
	/// </summary>
	/// <param name="textureName">Name of shader resource view.</param>
	/// <returns>Texuture/Shader Resource View</returns>
	ID3D11ShaderResourceView* GetTexture(std::string textureName);

	static ResourceManager* GetInstance();
	ResourceManager();
	~ResourceManager();
};


