/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "Mesh.h"
#include "Material.h"
#include "ConfigLoader.h"
#include "WICTextureLoader.h"
#include <map>
#include "FBXLoader.h"
#include "AsyncLoader.h"

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
	AsyncLoader* asyncLoader;
	WorkerThread *asyncWorker;
	SystemCore* core;
public:
	SimpleVertexShader *vertexShader;
	SimplePixelShader *pixelShader;
	SimplePixelShader *debugShader;
	SimpleVertexShader *debugVertexShader;
	ID3D11SamplerState *sampler;
	VertexShaderMap vertexShaders;
	PixelShaderMap pixelShaders;
	SimpleVertexShader *vertexShaderAnimated;
	SimplePixelShader *pixelShaderAnimated;
	
	// Animation Data
	FBXLoader fbxLoader;
	float blendWeight = 0;

	void SetAsyncLoader(AsyncLoader* loader);

	void SetAsyncWorker(WorkerThread* worker);

	void LoadTextureAsync(std::string filename, std::string texName, std::function<void()> onLoad);

	/// <summary>
	/// Loads all resources asynchronously.
	/// </summary>
	/// <param name="config">Config map</param>
	/// <param name="core">System Core</param>
	/// <param name="callback">Callback to be invoked after resources are loaded.</param>
	/// <remarks>
	/// Uses deferred context to create textures. Once the textures are loaded in memory an 
	/// event is emitted which copies the buffer data to the GPU in the main thread.
	/// </remarks>
	virtual void LoadResourcesAsync(ConfigMap config, SystemCore *core, std::function<void()> callback);

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

	/// <summary>
	/// Gets instance of Resource Manager.
	/// </summary>
	/// <returns></returns>
	static ResourceManager* GetInstance();
	ResourceManager();
	~ResourceManager();
};


