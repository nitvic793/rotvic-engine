#pragma once
#include "Vector3.h"
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

class Entity
{
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMMATRIX W ;
	XMVECTOR pos ;
	XMVECTOR dir ;
	XMVECTOR up ;
	XMMATRIX V;
	XMMATRIX P;
protected:
	Vector3f Position;
	Mesh *mesh;
public:
	XMFLOAT4X4 GetWorldMatrix();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	virtual void MoveUp(float);
	virtual void MoveDown(float);
	virtual void MoveLeft(float) ;
	virtual void MoveRight(float);
	const Vector3f &GetPosition();
	void SetPosition(const Vector3f& position);
	void Move(const Vector3f& offset);
	void SetMesh(Mesh *mesh);
	Mesh* GetMesh();
	Entity();
	~Entity();
};

