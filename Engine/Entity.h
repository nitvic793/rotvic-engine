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
#include "PrimitiveShape.h"
#include "FBXLoader.h"

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
	BasicShape* basicShape;
	PrimitiveShapesType shapeType;
	std::string collisionGroup;
	rp3d::Vector3 centerOffset;
public:
	/// <summary>
	/// Gets the world matrix of entity
	/// </summary>
	/// <returns>Returns 4x4 world matrix for this entity.</returns>
	XMFLOAT4X4 GetWorldMatrix();
	virtual void Update(float deltaTime);
	std::vector<Script*> scripts;

	/// <summary>
	/// Returns proxy shape associated with entity.
	/// </summary>
	/// <returns></returns>
	rp3d::ProxyShape* GetProxyShape() const;

	/// <summary>
	/// Get current position of entity
	/// </summary>
	/// <returns>3 Float Vector of current position of entity</returns>
	rp3d::Vector3 GetPosition();
	rp3d::Quaternion GetRotation();


	/// <summary>
	/// Get current linear velocity of entity
	/// </summary>
	/// <returns>3 Float Vector of current velocity of entity</returns>
	rp3d::Vector3 GetLinearVelocity();

	/// <summary>
	/// Get current forward of entity
	/// </summary>
	/// <returns>3 Float Vector of current forward of entity</returns>
	rp3d::Vector3 GetForward();


	rp3d::RigidBody *GetRigidBody();

	/// <summary>
	/// Renders the debug shape associated with this entity. 
	/// </summary>
	void DrawDebugShape();

	/// <summary>
	/// Set rigidbody for the entity. This keeps track of the object's position and physics
	/// </summary>
	/// <param name="position">The position to be used by the entity. (rp3d vector3)</param>
	/// <param name="orientation">The rotation to be used by the entity. (Use rp3d quaternion identity for 0)</param>
	/// <param name="physicsWorld">The dynamics world to be used by the entity.</param>
	void CreateRigidBody(rp3d::Vector3 position, rp3d::Quaternion orientation);

	/// <summary>
	/// Set context for entity. This context will be used by the entity in the Update function or other overridable functions to 
	/// read input values or perform other logic.
	/// </summary>
	/// <param name="context">The context to be used by the entity.</param>
	void SetContext(EntityContextWrapper context);

	void SetPosition(const Vector3f& position);

	/// <summary>
	/// Sets the euler rotation angles(in degrees) of given entity.
	/// </summary>
	/// <param name="pitch">The pitch angle in degrees</param>
	/// <param name="yaw">The yaw in degrees</param>
	/// <param name="roll">The roll in degrees</param>
	void SetRotation(float pitch, float yaw, float roll);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void Move(const Vector3f& offset);
	void ApplyForce(rp3d::Vector3 force);
	void SetMesh(Mesh *mesh);
	void SetMaterial(Material *mat);
	void SetRigidBodyParameters(bool enableGravity, rp3d::BodyType bodyType = rp3d::DYNAMIC);

	/// <summary>
	/// Set a new collider for the entity
	/// </summary>
	/// <param name="radius">The radius of the sphere collider</param>
	void CreateSphereCollider(rp3d::decimal radius, std::string collisionGroupName = "default");
	void CreateBoxCollider(rp3d::Vector3 halfwidths, rp3d::Vector3 offset = rp3d::Vector3(0, 0, 0), std::string collisionGroupName = "default");
	void CreateCapsuleCollider(rp3d::decimal radius, rp3d::decimal height);
	void CreateCylinderCollider(rp3d::decimal radius, rp3d::decimal height);

	XMFLOAT3 GetScale();
	Mesh* GetMesh();
	Material *GetMaterial();
	Entity(rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts = std::vector<Script*>());
	Entity(Mesh *m, Material* mat, rp3d::Vector3 position, rp3d::Quaternion orientation, rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts = std::vector<Script*>());
	Entity(Mesh *m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts = std::vector<Script*>());
	Entity(Mesh *m, Material* mat);
	virtual ~Entity();

	bool isAnimated = false;
	FBXLoader* fbx;
	int enemyInstanceNumber = 0;
};

