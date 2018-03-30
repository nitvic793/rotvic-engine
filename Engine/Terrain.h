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
	XMFLOAT3* heightMap;
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
	Terrain(SystemCore* sysCore, rp3d::DynamicsWorld* physicsWorld);
	~Terrain();
};
