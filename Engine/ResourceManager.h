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

namespace objl {
	class Loader;
}


typedef std::map<std::string, SimpleVertexShader*> VertexShaderMap;
typedef std::map<std::string, SimplePixelShader*> PixelShaderMap;
typedef std::pair<std::string, SimpleVertexShader*> VertexShaderMapType;
typedef std::pair<std::string, SimplePixelShader*> PixelShaderMapType;

struct MTLData
{
	std::string meshName;
	std::string diffuseTexture;
	std::string normalTexture;
};

struct MeshReference
{
	std::vector<std::string> meshNames;
	unsigned int referenceCount;
};

/// <summary>
/// Resource manager class. A central class to manage all resources required by the game. 
/// </summary>
class ResourceManager
{
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::map<std::string, ID3D11ShaderResourceView*> textures;
	std::map<std::string, MTLData> mtlMap;
	std::map<std::string, MeshReference> meshReferenceMap;

	static ResourceManager* instance;
	AsyncLoader* asyncLoader;
	WorkerThread *asyncWorker;
	SystemCore* core;

	std::vector<std::string> AddToMeshMap(objl::Loader loader, std::string prefix, ID3D11DeviceContext* context, bool loadTex = true);
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
	SimpleVertexShader *vertexShaderWeapon;

	// Animation Data
	FBXLoader * fbxLoader;   // Player FBX Loader
	FBXLoader * enemyFBXLoader;    // Enemy FBX Loader

	void LoadMeshAsync(std::string meshName, std::string eventType, void* instance);

	void UnloadMeshAsync(std::string meshName, std::string eventType, void* instance);

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


