#pragma once
#include "Vector3.h"
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "EntityContextWrapper.h"

using namespace DirectX;

/// <summary>
/// Base entity class. Every other game object should inherit this class and implement the update function accordingly.
/// </summary>
/// <para>
/// All game objects should inhert the Entity class and implement its own logic in the virtual Update function which is overrideable. 
/// </para>
class Entity
{
	XMFLOAT4X4 worldMatrix;
protected:
	EntityContextWrapper context;
	Vector3f Position;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
	Mesh *mesh;
	Material *material;
public:
	XMFLOAT4X4 GetWorldMatrix();
	virtual void MoveUp(float);
	virtual void MoveDown(float);
	virtual void MoveLeft(float) ;
	virtual void MoveRight(float);
	virtual void Update(float);

	const Vector3f &GetPosition();

	void SetContext(EntityContextWrapper context);
	void SetPosition(const Vector3f& position);
	void SetRotationZ(float angle);
	void SetRotation(float roll, float pitch, float yaw);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void Move(const Vector3f& offset);
	void SetMesh(Mesh *mesh);
	void SetMaterial(Material *mat);

	XMFLOAT3 GetScale();
	Mesh* GetMesh();
	Material *GetMaterial();
	Entity();
	Entity(Mesh *m, Material* mat);
	virtual ~Entity();
};

