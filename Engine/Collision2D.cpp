#include "Collision2D.h"
#include <exception>

QuadTree* Collision2D::quadTree = nullptr;

/// <summary>
/// Checks if given two rects are intersecting or not. 
/// </summary>
/// <param name="rectA">First rect object</param>
/// <param name="rectB">Second rect object</param>
/// <returns>True if both rects are intersecting, false if not</returns>
bool Collision2D::IsColliding(const Rect & rectA, const Rect & rectB)
{
	return rectA.x < rectB.x + rectB.width &&
		rectA.x + rectA.width > rectB.x &&
		rectA.y < rectB.y + rectB.height &&
		rectA.height + rectA.y > rectB.y;
}

void Collision2D::InstantiateQuadTree2D(Rect bounds)
{
	DestroyQuadTree();
	quadTree = new QuadTree(0, bounds);
}

void Collision2D::InsertStaticObjectsInQuadTree(std::vector<GameEntity*> objects)
{
	if (!quadTree) return;
	for (auto rect : objects)
	{
		quadTree->Insert(rect);
	}
}

GameEntity* Collision2D::IsCollidingQuadTree(GameEntity* rect)
{
	if (!quadTree) return nullptr;
	std::vector<GameEntity*> out;
	quadTree->Retrieve(out, rect);
	for (auto obj : out) 
	{
		if (IsColliding(obj->GetRect(), rect->GetRect())) {
			return obj;
		}
	}
	return nullptr;
}

void Collision2D::DestroyQuadTree()
{
	if (quadTree) delete quadTree;
}

Collision2D::Collision2D()
{
}


Collision2D::~Collision2D()
{
}
