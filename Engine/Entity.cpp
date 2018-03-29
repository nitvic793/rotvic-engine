#include "Entity.h"

/// <summary>
/// Constructor initializes the world matrix
/// </summary>
Entity::Entity()
{
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR sc = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	Position = Vector3f(0, 0, 0);
	XMStoreFloat3(&position, v);
	XMStoreFloat3(&scale, sc);
	XMStoreFloat3(&rotation, v);
	mesh = nullptr;	
	material = nullptr;
}

Entity::Entity(Mesh *m, Material* mat)
{
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR sc = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	Position = Vector3f(0,0,0);
	XMStoreFloat3(&position, v);
	XMStoreFloat3(&scale, sc);
	XMStoreFloat3(&rotation, v);
	mesh = m;
	material = mat;
}

/// <summary>
/// Gets the world matrix of entity
/// </summary>
/// <returns>Returns 4x4 world matrix for this entity.</returns>
XMFLOAT4X4 Entity::GetWorldMatrix() 
{
	/*XMMATRIX trans = XMMatrixTranslation(Position.x, Position.y, Position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
	XMMATRIX scle = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX world = scle * rot * trans;*/
	rp3d::Transform transform = rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);
	XMMATRIX world = XMMATRIX(matrix);
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));
	return worldMatrix;
}

Entity::~Entity()
{
	//if (rigidBody) delete rigidBody;
}

/// <summary>
/// Get current position of entity
/// </summary>
/// <returns>3 Float Vector of current position of entity</returns>
const Vector3f &Entity::GetPosition()
{
	return Position;
}

void Entity::SetRotationZ(float angle)
{
	rotation.z = angle;
}

/// <summary>
/// Sets the euler rotation angles(in degrees) of given entity.
/// </summary>
/// <param name="roll">The roll angle in degrees</param>
/// <param name="pitch">The pitch in degrees</param>
/// <param name="yaw">The yaw in degrees</param>
void Entity::SetRotation(float roll, float pitch, float yaw)
{
	rotation.x = roll * XM_PI/180;
	rotation.y = pitch * XM_PI / 180;
	rotation.z = yaw * XM_PI / 180;
}

void Entity::SetPosition(float x, float y, float z)
{
	this->Position.x = x;
	this->Position.y = y;
	this->Position.z = z;
}

void Entity::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
}

XMFLOAT3 Entity::GetScale()
{
	return scale;
}

/// <summary>
/// Set context for entity. This context will be used by the entity in the Update function or other overridable functions to 
/// read input values or perform other logic.
/// </summary>
/// <param name="context">The context to be used by the entity.</param>
void Entity::SetContext(EntityContextWrapper context)
{
	this->context = context;
}

/// <summary>
/// Set rigidbody for the entity. This keeps track of the object's position and physics
/// </summary>
/// <param name="position">The position to be used by the entity. (rp3d vector3)</param>
/// <param name="orientation">The rotation to be used by the entity. (Use rp3d quaternion identity for 0)</param>
/// <param name="physicsWorld">The dynamics world to be used by the entity.</param>
void Entity::StartRigidBody(rp3d::Vector3 position, rp3d::Quaternion orientation, rp3d::DynamicsWorld* physicsWorld)
{
	rp3d::Transform transform(position, orientation);

	// Create a rigid body in the world 
	rigidBody = physicsWorld->createRigidBody(transform);
	rigidBody->enableGravity(false);
	shape = new rp3d::SphereShape(.5);
	proxyShape = rigidBody->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(1.0));
}


/// <summary>
/// Set position of entity
/// </summary>
/// <param name="position"></param>
void Entity::SetPosition(const Vector3f& position)
{
	Position = position;
}

/// <summary>
/// Move entity by given offset
/// </summary>
/// <param name="offset"></param>
void Entity::Move(const Vector3f& offset)
{
	Position = Position + offset;
	rigidBody->applyForceToCenterOfMass(rp3d::Vector3(offset.x, offset.y, offset.z));
}

/// <summary>
/// Sets the mesh for given entity.
/// </summary>
/// <param name="mesh"></param>
/// <remarks>Currently, the mesh has to be set after creation of every entity. Need a cleaner way to represent {Game Object, Mesh, Material, Texture etc.}</remarks>
void Entity::SetMesh(Mesh *mesh)
{
	this->mesh = mesh;
}

void Entity::SetMaterial(Material * mat)
{
	material = mat;
}

/// <summary>
/// Gets the mesh for this entity
/// </summary>
/// <returns>Pointer to mesh</returns>
Mesh* Entity::GetMesh()
{
	return mesh;
}

Material * Entity::GetMaterial()
{
	return material;
}

/**
Stubs
**/

void Entity::MoveUp(float offset)
{

}

void Entity::MoveDown(float offset)
{

}

void Entity::MoveLeft(float offset)
{

}

void Entity::MoveRight(float offset)
{

}

void Entity::Update(float)
{
}

/**
End of Stubs
**/