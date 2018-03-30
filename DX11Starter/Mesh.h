#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <fstream>
#include <vector>
#include <string>
#include <fbxsdk.h>



using namespace DirectX;


class Mesh
{
public:

	ID3D11Buffer * vertexBufferPointer;
	ID3D11Buffer * indexBufferPointer;
	int indexCount;
	int vertexCount;
	//Vertex *vertexArray;
	//unsigned int* indexArray;

public:

	Mesh(Vertex[], int, unsigned int[], int, ID3D11Device*);
	Mesh(char*, ID3D11Device*);
	Mesh( ID3D11Device*, int);
	~Mesh();
	ID3D11Buffer * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();
	int GetIndexCount();
	int GetVertexCount();
	//Vertex *GetVertexArray();
	//unsigned int* GetIndexArray();

	// FBX
	FbxNode* childNode;
	FbxManager* lSdkManager = NULL;
	FbxScene* scene = NULL;
	bool lResult;



};



