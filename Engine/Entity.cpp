#include "Entity.h"

/// <summary>
/// Constructor initializes the world matrix
/// </summary>
Entity::Entity()
{
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	mesh = nullptr;	

}

/// <summary>
/// Gets the world matrix of entity
/// </summary>
/// <returns>Returns 4x4 world matrix for this entity.</returns>
XMFLOAT4X4 Entity::GetWorldMatrix() 
{
	auto newPos = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixTranslation(Position.x, Position.y, Position.z));
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(newPos));
	return worldMatrix;
}

Entity::~Entity()
{
}

/// <summary>
/// Get current position of entity
/// </summary>
/// <returns>3 Float Vector of current position of entity</returns>
const Vector3f &Entity::GetPosition()
{
	return Position;
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

/// <summary>
/// Gets the mesh for this entity
/// </summary>
/// <returns>Pointer to mesh</returns>
Mesh* Entity::GetMesh()
{
	return mesh;
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