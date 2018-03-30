#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Vertex.h"
#include <stdio.h>
#include <memory>
#include <math.h>  
#include <iostream>

using namespace DirectX;

class GameEntity 
{
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	

public:

	GameEntity(std::shared_ptr<Mesh>, std::shared_ptr<Material>);
	~GameEntity();

	void prepareMaterial(XMFLOAT4X4, XMFLOAT4X4);

	XMFLOAT4X4 getWorldMatrix();
	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();
	std::shared_ptr<Mesh>getMesh();

	void updateWorldMatrix();
	void setPosition(float,float,float);
	void setRotation(float, float, float);
	void setScale(float, float, float);

	void translateTo(float, float, float, float, float);
	void rotateAt(float,float,float,float);

	
	// FBX
	std::vector<std::shared_ptr<Mesh>> meshes;

	FbxNode* childNode;
	FbxNode* animNode;
	FbxManager* lSdkManager = NULL;
	FbxScene* scene = NULL;
	bool lResult;


	// Animation Data
	//XMFLOAT4X4 GetAnimatedMatrix();
	FbxTime time;
	double T = 0.0;
	//FbxAnimStack* lAnimStack;
	FbxAnimEvaluator* lEvaluator;

	FbxAnimStack* currAnimStack;
	FbxString animStackName;
	FbxTakeInfo* takeInfo;
};