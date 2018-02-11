#pragma once

#include <vector>
#include "Rect.h"
#include "GameEntity.h"

class QuadTree
{
	const int MAX_OBJECTS = 1;
	const int MAX_LEVELS = 5;

	int level;
	std::vector<GameEntity*> objects;
	QuadTree *nodes[4];
	Rect bounds;
	int GetIndex(GameEntity* entity);
public:
	/// <summary>
	/// Clears all child nodes and objects in current node
	/// </summary>
	void Clear();

	/// <summary>
	/// Split current quad into 4 more child quad nodes. The child nodes are instantiated.
	/// </summary>
	void Split();

	/// <summary>
	/// Insert entity into quadtree
	/// </summary>
	/// <param name="rect"></param>
	void Insert(GameEntity* rect);

	/// <summary>
	/// Retrieve all nearby game entities to given game entity
	/// </summary>
	/// <param name="outObjects">List of objects which are nearby entity</param>
	/// <param name="rect">The object to check proximity with</param>
	/// <returns></returns>
	std::vector<GameEntity*> Retrieve(std::vector<GameEntity*> &outObjects, GameEntity* rect);

	
	QuadTree(int level, Rect bounds);
	~QuadTree();
};

