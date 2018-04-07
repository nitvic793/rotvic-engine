#include "Entity.h"
#include "DebugDraw.h"

/// <summary>
/// Constructor initializes the world matrix
/// </summary>
Entity::Entity(rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts) :
	Entity(nullptr, nullptr, rp3d::Vector3(0, 0, 0), rp3d::Quaternion::identity(), physicsWorld, pScripts)
{
}

Entity::Entity(Mesh *m, Material* mat, rp3d::Vector3 position, rp3d::Quaternion orientation, rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts)
{
	collisionGroup = "default";
	shapeType = UNDEFINED;
	dynamicsWorld = physicsWorld;
	XMVECTOR sc = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMStoreFloat3(&scale, sc);
	CreateRigidBody(position, orientation);
	XMMATRIX scle = XMMatrixScaling(scale.x, scale.y, scale.z);
	rp3d::Transform transform = rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);
	XMMATRIX world = scle * XMMATRIX(matrix);
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));
	mesh = m;
	material = mat;
	scripts = pScripts;

	if (scripts.size() > 0) // Call the update for each script each cycle
	{
		for each (Script* s in scripts)
		{
			s->Start(this);
		}
	}
}

Entity::Entity(Mesh *m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* physicsWorld, std::vector<Script*> pScripts) :
	Entity(m, mat, position, rp3d::Quaternion::identity(), physicsWorld, pScripts)
{
}

/// <summary>
/// Gets the world matrix of entity
/// </summary>
/// <returns>Returns 4x4 world matrix for this entity.</returns>
XMFLOAT4X4 Entity::GetWorldMatrix()
{
	XMMATRIX scle = XMMatrixScaling(scale.x, scale.y, scale.z);
	rp3d::Transform transform = rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);
	XMMATRIX world = scle * XMMATRIX(matrix);
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));
	return worldMatrix;
}

Entity::~Entity()
{
	if (basicShape) delete basicShape;
	if (rigidBody) dynamicsWorld->destroyRigidBody(rigidBody);
	dynamicsWorld = nullptr;

	if (scripts.size() > 0) // Call the update for each script each cycle
	{
		for each (Script* s in scripts)
		{
			delete s;
		}
	}
}

/// <summary>
/// Get current position of entity
/// </summary>
/// <returns>3 Float Vector of current position of entity</returns>
rp3d::Vector3 Entity::GetPosition()
{
	return rigidBody->getTransform().getPosition();
}

rp3d::Quaternion Entity::GetRotation()
{
	return rigidBody->getTransform().getOrientation();
}

/// <summary>
/// Get current linear velocity of entity
/// </summary>
/// <returns>3 Float Vector of current velocity of entity</returns>
rp3d::Vector3 Entity::GetLinearVelocity()
{
	return rigidBody->getLinearVelocity();
}

/// <summary>
/// Get current forward of entity
/// </summary>
/// <returns>3 Float Vector of current forward of entity</returns>
rp3d::Vector3 Entity::GetForward()
{
	return rigidBody->getTransform().getOrientation().getVectorV().getUnit();
}

void Entity::ApplyForce(rp3d::Vector3 force)
{
	rigidBody->applyForceToCenterOfMass(rp3d::Vector3(force.x, force.y, force.z));
	//rigidBody->setLinearVelocity(force);
}

/// <summary>
/// Sets the euler rotation angles(in degrees) of given entity.
/// </summary>
/// <param name="pitch">The pitch angle in degrees</param>
/// <param name="yaw">The yaw in degrees</param>
/// <param name="roll">The roll in degrees</param>
void Entity::SetRotation(float pitch, float yaw, float roll)
{
	rp3d::Transform t = rigidBody->getTransform();
	t.setOrientation(rp3d::Quaternion(pitch, yaw, roll));
	rigidBody->setTransform(t);

}

void Entity::SetPosition(float x, float y, float z)
{
	rp3d::Transform t = rigidBody->getTransform();
	t.setPosition(rp3d::Vector3(x, y, z));
	rigidBody->setTransform(t);
	rigidBody->setIsSleeping(false);
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

void Entity::DrawDebugShape()
{
	switch (shapeType)
	{
	case BOX:
	{
		auto box = *(Box*)basicShape;
		auto pos = GetPosition();
		auto rot = GetRotation();
		box.bounding.Center.x = pos.x;
		box.bounding.Center.y = pos.y;
		box.bounding.Center.z = pos.z;
		box.bounding.Orientation = XMFLOAT4(rot.x, rot.y, rot.z, rot.w);
		box.color = XMFLOAT4(1, 1, 1, 1);
		DebugDraw::Draw<Box>(box, collisionGroup);
		break;
	}
	case SPHERE:
	{
		auto sphere = *(Sphere*)basicShape;
		auto pos = GetPosition();
		sphere.color = XMFLOAT4(1, 1, 1, 1);
		sphere.bounding.Center = XMFLOAT3(pos.x, pos.y, pos.z);
		DebugDraw::Draw<Sphere>(sphere, collisionGroup);
		break;
	}
	}
}

/// <summary>
/// Set rigidbody for the entity. This keeps track of the object's position and physics
/// </summary>
/// <param name="position">The position to be used by the entity. (rp3d vector3)</param>
/// <param name="orientation">The rotation to be used by the entity. (Use rp3d quaternion identity for 0)</param>
/// <param name="physicsWorld">The dynamics world to be used by the entity.</param>
void Entity::CreateRigidBody(rp3d::Vector3 position, rp3d::Quaternion orientation)
{
	rp3d::Transform transform(position, orientation);

	// Create a rigid body in the world 
	rigidBody = dynamicsWorld->createRigidBody(transform);
	rigidBody->enableGravity(false);
}

/// <summary>
/// Set a new collider for the entity
/// </summary>
/// <param name="radius">The radius of the sphere collider</param>
void Entity::CreateSphereCollider(rp3d::decimal radius, std::string collisionGroupName)
{
	collisionGroup = collisionGroupName;
	shape = new rp3d::SphereShape(radius);
	proxyShape = rigidBody->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(1.0));
	shapeType = SPHERE;
	auto sphere = new Sphere();
	sphere->bounding.Center = XMFLOAT3(0, 0, 0);
	sphere->bounding.Radius = radius;
	basicShape = sphere;
}

/// <summary>
/// Set a new collider for the entity
/// </summary>
/// <param name="halfwidths">The halfwidth for each axis of the box</param>
void Entity::CreateBoxCollider(rp3d::Vector3 halfwidths, std::string collisionGroupName)
{
	collisionGroup = collisionGroupName;
	shape = new rp3d::BoxShape(halfwidths);
	proxyShape = rigidBody->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(1.0));
	shapeType = BOX;
	auto box = new Box();
	box->bounding.Center = XMFLOAT3(0, 0, 0);
	box->bounding.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	basicShape = box;
}

/// <summary>
/// Set a new collider for the entity
/// </summary>
/// <param name="radius">The radius of the capsule collider</param>
/// <param name="height">The height of the capsule collider</param>
void Entity::CreateCapsuleCollider(rp3d::decimal radius, rp3d::decimal height)
{
	shape = new rp3d::CapsuleShape(radius, height);
	proxyShape = rigidBody->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(1.0));
}

/// <summary>
/// Set a new collider for the entity
/// </summary>
/// <param name="radius">The radius of the cylinder collider</param>
/// <param name="height">The height of the cylinder collider</param>
void Entity::CreateCylinderCollider(rp3d::decimal radius, rp3d::decimal height)
{
	shape = new rp3d::CylinderShape(radius, height, .001);
	proxyShape = rigidBody->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(1.0));
	rigidBody->setAngularDamping(rp3d::decimal(1)); // 1 is maximum
}

/// <summary>
/// Set rigidbody for the entity. This keeps track of the object's position and physics
/// </summary>
/// <param name="enableGravity">Apply gravity force to this object?</param>
/// <param name="bodyType">Kinematic and static do not check against each other, static can't move. Dynamic is default.</param>
void Entity::SetRigidBodyParameters(bool enableGravity, rp3d::BodyType bodyType)
{
	rigidBody->enableGravity(enableGravity);
	rigidBody->setType(bodyType);
}


/// <summary>
/// Set position of entity
/// </summary>
/// <param name="position"></param>
void Entity::SetPosition(const Vector3f& position)
{
	rp3d::Transform t = rigidBody->getTransform();
	t.setPosition(rp3d::Vector3(position.x, position.y, position.z));
	rigidBody->setTransform(t);
}

/// <summary>
/// Move entity by given offset
/// </summary>
/// <param name="offset"></param>
void Entity::Move(const Vector3f& offset)
{
	rp3d::Transform t = rigidBody->getTransform();
	t.setPosition(rp3d::Vector3(offset.x, offset.y, offset.z) + t.getPosition());
	rigidBody->setTransform(t);
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

void Entity::Update(float deltaTime)
{
	if (scripts.size() > 0) // Call the update for each script each cycle
	{
		for each (Script* s in scripts)
		{
			s->Update(deltaTime);
		}
	}
}