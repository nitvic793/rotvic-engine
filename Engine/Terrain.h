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
	XMFLOAT3* GetHeightMap();
	const int GetTerrainHeight();
	const int GetTerrainWidth();
	void CalculateNormals();
	void CalculateUVCoordinates();
	void Initialize();
	bool Initialize(const char* filename);
	void _temp_Init();
	Terrain(SystemCore* sysCore, rp3d::DynamicsWorld* physicsWorld);
	~Terrain();
};
