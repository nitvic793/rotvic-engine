#pragma once
#undef min
#undef max
#include "Vector3.h"
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "EntityContextWrapper.h"
#include "reactphysics3d.h"
#include "Script.h"

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
	DirectX::XMFLOAT3 scale;
	Mesh *mesh;
	Material *material;
	rp3d::RigidBody *rigidBody;
	rp3d::ProxyShape* proxyShape;
	rp3d::CollisionShape* shape;
	rp3d::DynamicsWorld* dynamicsWorld;
public:
	XMFLOAT4X4 GetWorldMatrix();
	virtual void Update(float deltaTime);
	std::vector<Script*> scripts;

	rp3d::Vector3 GetPosition();
	rp3d::Vector3 GetLinearVelocity();
	rp3d::Vector3 GetForward();

	void CreateRigidBody(rp3d::Vector3 position, rp3d::Quaternion orientation);
	void SetContext(EntityContextWrapper context);
	void SetPosition(const Vector3f& position);
	void SetRotation(float pitch, float yaw, float roll);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void Move(const Vector3f& offset);
	void ApplyForce(rp3d::Vector3 force);
	void SetMesh(Mesh *mesh);
	void SetMaterial(Material *mat);
	void SetRigidBodyParameters(bool enableGravity, rp3d::BodyType bodyType = rp3d::DYNAMIC);
	void CreateSphereCollider(rp3d::decimal radius);
	void Entity::CreateBoxCollider(rp3d::Vector3 halfwidths);
	void Entity::CreateCapsuleCollider(rp3d::decimal radius, rp3d::decimal height);
	void Entity::CreateCylinderCollider(rp3d::decimal radius, rp3d::decimal height);

	XMFLOAT3 GetScale();
	Mesh* GetMesh();
	Material *GetMaterial();
	Entity(rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts = std::vector<Script*>());
	Entity(Mesh *m, Material* mat, rp3d::Vector3 position, rp3d::Quaternion orientation, rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts = std::vector<Script*>());
	Entity(Mesh *m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts = std::vector<Script*>());
	virtual ~Entity();
};

