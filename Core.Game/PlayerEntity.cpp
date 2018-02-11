#include "PlayerEntity.h"

/// <summary>
/// Updates the player object. 
/// </summary>
/// <param name="deltaTime">Delta time between frames</param>
/// <remarks>The player object moves for keyboard input as well as mouse input. For mouse input, the player object lerps to target position.</remarks>
void PlayerEntity::Update(float deltaTime)
{
	prevPosition = Position;
	std::vector<Keys> inputs = { Up, Down, Left, Right };
	auto prevPos = Position;
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
		Position = Vector3f::Lerp(Position, target, 2.5f * deltaTime);
	}

	if (hasTarget && Vector3f::Distance(Position, target) < 0.1f) {
		hasTarget = false;
	}

	if (inCollision) {
		if (Collision2D::IsColliding(GetRect(), collisionEntity->GetRect())) {
			Position = prevPos; //Stop movement if next step is still causing collision
		}
		else {
			inCollision = false; //Allow movement if not colliding.
		}
	}
}

/// <summary>
/// On collision callback.
/// </summary>
/// <param name="entity">Entity with which this object collided</param>
void PlayerEntity::OnCollision(GameEntity* entity)
{
	hasTarget = false;
	inCollision = true;
	collisionEntity = entity;
	Position = prevPosition;
}

/// <summary>
/// Process keyboard input.
/// </summary>
/// <param name="key"></param>
/// <param name="deltaTime"></param>
void PlayerEntity::SendInput(Keys key, float deltaTime)
{
	hasTarget = false;
	if (key == Keys::Up) {
		MoveUp(speed * deltaTime);
	}

	if (key == Keys::Down) {
		MoveDown(speed * deltaTime);
	}

	if (key == Keys::Left) {
		MoveLeft(speed * deltaTime);
	}

	if (key == Keys::Right) {
		MoveRight(speed * deltaTime);
	}
}


PlayerEntity::PlayerEntity(float speed)
{
	this->speed = speed;
	hasTarget = false;
	inCollision = false;
}


PlayerEntity::~PlayerEntity()
{
}


