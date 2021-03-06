#include "ThirdPersonPlayerEntity.h"

void ThirdPersonPlayerEntity::Update(float deltaTime)
{
	forwardDir = rp3d::Vector3(5 * sin(rotationAngle), 0, 5 * cos(rotationAngle));
	if (keyboard->IsKeyPressed(W))
	{
		isAnimationTransitioning = true;
		animTransitionDirection = false;
		ApplyForce(forwardDir * 2);
	}
	else
	{
		isAnimationTransitioning = true;
		animTransitionDirection = true;
	}


	if (keyboard->IsKeyPressed(D))
	{
		rotationAngle += 2 * deltaTime;

	}

	if (keyboard->IsKeyPressed(A))
	{
		rotationAngle -= 2 * deltaTime;
	}

	SetRotation(0, rotationAngle, 0);


	// Animation State Transitions
	if (isAnimationTransitioning)
	{
		if (animTransitionDirection)
		{
			resource->fbxLoader->blendWeight += 0.04f;
			if (resource->fbxLoader->blendWeight > 1.0f)
			{
				resource->fbxLoader->blendWeight = 1.0f;
				isAnimationTransitioning = false;
			}
		}
		else
		{
			resource->fbxLoader->blendWeight -= 0.04f;
			if (resource->fbxLoader->blendWeight < 0.0f)
			{
				resource->fbxLoader->blendWeight = 0.0f;
				isAnimationTransitioning = false;
			}
		}
	}

	//Clamp velocity
	auto velocity = rigidBody->getLinearVelocity();
	if (velocity.x > maxSpeed)velocity.x = maxSpeed;
	if (velocity.x < -maxSpeed)velocity.x = -maxSpeed;
	if (velocity.z > maxSpeed)velocity.z = maxSpeed;
	if (velocity.z < -maxSpeed)velocity.z = -maxSpeed;
	rigidBody->setLinearVelocity(velocity);

}

rp3d::Vector3 ThirdPersonPlayerEntity::GetForwardVector() const
{
	return forwardDir;
}

ThirdPersonPlayerEntity::ThirdPersonPlayerEntity(Mesh* m, Material* mat, rp3d::Vector3 position, rp3d::DynamicsWorld* world, std::vector<Script*> scripts)
	:Entity(m, mat, position, world, scripts)
{
	keyboard = Keyboard::GetInstance();
	resource = ResourceManager::GetInstance();
}


ThirdPersonPlayerEntity::~ThirdPersonPlayerEntity()
{
}
