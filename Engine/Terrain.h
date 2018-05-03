/// <summary>
/// Author: Nitish Victor
/// Part credits: Trevor Walden
/// </summary>

#pragma once
#define NOMINMAX
#include <d3d11.h>
#include <DirectXMath.h>
#include "Entity.h"
#include "SystemCore.h"

using namespace DirectX;

const int TEXTURE_REPEAT = 16;

class Terrain : public Entity
{
	int terrainHeight;
	int terrainWidth;
	float minHeight;
	float maxHeight;
	XMFLOAT3* heightMap;
	float* heightValues;
	XMFLOAT3* heightNormals;
	XMFLOAT2* textureCoords;
	Vertex *vertices;
	UINT *indices;
	int indexCount;
	SystemCore* core;
public:
	/// <summary>
	/// Get height map of Terrain.
	/// </summary>
	/// <returns></returns>
	XMFLOAT3* GetHeightMap();

	/// <summary>
	/// Get max height of terrain
	/// </summary>
	/// <returns></returns>
	const int GetTerrainHeight();

	/// <summary>
	/// Get width of terrain.
	/// </summary>
	/// <returns></returns>
	const int GetTerrainWidth();

	/// <summary>
	/// Calculate the normals of the terrain.
	/// </summary>
	void CalculateNormals();

	/// <summary>
	/// Calculate the texture UV coordinates of the terrain.
	/// </summary>
	void CalculateUVCoordinates();
	void Initialize();

	/// <summary>
	/// Initializes given height map.
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	bool Initialize(const char* filename);
	void _temp_Init();
	Terrain(SystemCore* sysCore, rp3d::DynamicsWorld* physicsWorld);
	~Terrain();
};
