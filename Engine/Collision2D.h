#pragma once
#include "Rect.h"
#include "QuadTree.h"

/// <summary>
/// Collision 2D class encapsulates 2D collision systems.
/// </summary>
class Collision2D
{
	static QuadTree *quadTree;
public:
	/// <summary>
	/// Checks if given two rects are intersecting or not. 
	/// </summary>
	/// <param name="rectA">First rect object</param>
	/// <param name="rectB">Second rect object</param>
	/// <returns>True if both rects are intersecting, false if not</returns>
	static bool IsColliding(const Rect& rectA, const Rect& rectB);

	/// <summary>
	/// Instantiates quad tree. Used only for 2D operations.
	/// </summary>
	/// <param name="bounds">The bounds of the screen rect</param>
	static void InstantiateQuadTree2D(Rect bounds);

	/// <summary>
	/// Insert objects into the quad tree. This should be called only after the quad tree is instantiated. 
	/// </summary>
	/// <param name="objects">List of objects which need to be part of the quad tree. </param>
	static void InsertStaticObjectsInQuadTree(std::vector<GameEntity*> objects);

	/// <summary>
	/// Uses the quadtree to check if given object is colliding with any other object. The quadtree must be instantiated with objects. Returns the colliding object. 
	/// </summary>
	/// <param name="rect">The object to check collision with</param>
	/// <returns>The collding object. If there is no collision, returns null.</returns>
	static GameEntity* IsCollidingQuadTree(GameEntity* rect);

	/// <summary>
	/// Destroys the quad tree. Must be invoked to clean up memory.
	/// </summary>
	static void DestroyQuadTree();

	/// <summary>
	/// Collision2D constructor
	/// </summary>
	Collision2D();

	/// <summary>
	/// Collision2D destructor
	/// </summary>
	~Collision2D();
};



