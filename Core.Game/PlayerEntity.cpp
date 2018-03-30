#include "PlayerEntity.h"

/// <summary>
/// Updates the player object. 
/// </summary>
/// <param name="deltaTime">Delta time between frames</param>
/// <remarks>The player object moves for keyboard input as well as mouse input. For mouse input, the player object lerps to target position.</remarks>
void PlayerEntity::Update(float deltaTime)
{
	rp3d::Transform t = rigidBody->getTransform();
	prevPosition = t.getPosition();
	std::vector<Keys> inputs = { Up, Down, Left, Right };
	for (auto input : inputs)
	{
		if (context.keyboard->IsKeyPressed(input)) {
			SendInput(input, deltaTime);
		}
	}	

	if (context.mouse->IsKeyPressed(MBLeft)) {
		auto pos = context.GetMousePosition2D();
		target = Vector3f(pos.x, pos.y, 0.f);
		hasTarget = true;
	}

	if (hasTarget)
	{
		t.setPosition(Vector3f::Lerp(Vector3f(t.getPosition()), target, 2.5f * deltaTime).rp3dVec3());
	}

	if (hasTarget && Vector3f::Distance(Vector3f(t.getPosition()), target) < 0.1f) {
		hasTarget = false;
	}

	//if (inCollision) {
	//	if (Collision2D::IsColliding(GetRect(), collisionEntity->GetRect())) {
	//		Position = prevPos; //Stop movement if next step is still causing collision
	//	}
	//	else {
	//		inCollision = false; //Allow movement if not colliding.
	//	}
	//}
}

///// <summary>
///// On collision callback.
///// </summary>
///// <param name="entity">Entity with which this object collided</param>
//void PlayerEntity::OnCollision(GameEntity* entity)
//{
//	hasTarget = false;
//	inCollision = true;
//	collisionEntity = entity;
//	Position = prevPosition;
//}

/// <summary>
/// Process keyboard input.
/// </summary>
/// <param name="key"></param>
/// <param name="deltaTime"></param>
void PlayerEntity::SendInput(Keys key, float deltaTime)
{
	hasTarget = false;
	if (key == Keys::Up) {
	}

	if (key == Keys::Down) {
	}

	if (key == Keys::Left) {
	}

	if (key == Keys::Right) {
	}
}


PlayerEntity::PlayerEntity(rp3d::DynamicsWorld* physicsWorld, float speed):
	Entity(physicsWorld)
{
	this->speed = speed;
	hasTarget = false;
	inCollision = false;
}


PlayerEntity::~PlayerEntity()
{
}


