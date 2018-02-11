#include "QuadTree.h"



int QuadTree::GetIndex(GameEntity* entity)
{
	int index = -1;
	auto rect = entity->GetRect();
	double verticalMidpoint = bounds.x + (bounds.width / 2);
	double horizontalMidpoint = bounds.y + (bounds.height / 2);

	// Object can completely fit within the top quadrants
	bool topQuadrant = (rect.y < horizontalMidpoint && rect.y + rect.height < horizontalMidpoint);
	// Object can completely fit within the bottom quadrants
	bool bottomQuadrant = (rect.y > horizontalMidpoint);

	// Object can completely fit within the left quadrants
	if (rect.x < verticalMidpoint && rect.x + rect.width < verticalMidpoint) {
		if (topQuadrant) {
			index = 1;
		}
		else if (bottomQuadrant) {
			index = 2;
		}
	}
	// Object can completely fit within the right quadrants
	else if (rect.x > verticalMidpoint) {
		if (topQuadrant) {
			index = 0;
		}
		else if (bottomQuadrant) {
			index = 3;
		}
	}

	return index;
}

void QuadTree::Clear()
{
	objects.clear();
	for (int i = 0; i < 4; ++i) {
		delete nodes[i];
		nodes[i] = nullptr;
	}
}

void QuadTree::Split()
{
	int subWidth = (int)(bounds.width / 2);
	int subHeight = (int)(bounds.height / 2);
	int x = (int)bounds.x;
	int y = (int)bounds.y;

	nodes[0] = new QuadTree(level + 1, Rect((float)(x + subWidth), (float)y, (float)subWidth, (float)subHeight));
	nodes[1] = new QuadTree(level + 1, Rect((float)x, (float)y, (float)subWidth, (float)subHeight));
	nodes[2] = new QuadTree(level + 1, Rect((float)x, (float)(y + subHeight), (float)subWidth, (float)subHeight));
	nodes[3] = new QuadTree(level + 1, Rect((float)(x + subWidth), (float)(y + subHeight), (float)subWidth, (float)subHeight));
}

void QuadTree::Insert(GameEntity* rect)
{
	if (nodes[0] != nullptr) {
		int index = GetIndex(rect);

		if (index != -1) {
			nodes[index]->Insert(rect);

			return;
		}
	}

	objects.push_back(rect);


	if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
		if (nodes[0] == nullptr) {
			Split();
		}

		int i = 0;
		while (i < objects.size()) {
			int index = GetIndex(objects[i]);
			if (index != -1) {
				auto currentRect = objects[i];
				nodes[index]->Insert(currentRect);
				objects.erase(objects.begin() + i);
			}
			else {
				i++;
			}
		}
	}
}

std::vector<GameEntity*> QuadTree::Retrieve(std::vector<GameEntity*>& outObjects, GameEntity* rect)
{
	int index = GetIndex(rect);
	if (index != -1 && nodes[index] != nullptr) {
		nodes[index]->Retrieve(outObjects, rect);
	}

	for (auto object : objects) {
		outObjects.push_back(object);
	}

	return outObjects;
}

QuadTree::QuadTree(int level, Rect bounds)
{
	this->level = level;
	this->bounds = bounds;
	for (int i = 0; i < 4; ++i) {
		nodes[i] = nullptr;
	}
}


QuadTree::~QuadTree()
{
	Clear();
}
