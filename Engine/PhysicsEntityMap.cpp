#include "PhysicsEntityMap.h"



void PhysicsEntityMap::Insert(rp3d::ProxyShape * shape, Entity * entity)
{
	map.insert(ProxyEntityMapType(shape, entity));
}

Entity * PhysicsEntityMap::GetEntity(rp3d::ProxyShape* shape)
{
	return map[shape];
}

const ProxyEntityMap & PhysicsEntityMap::GetMap()
{
	return map;
}

void PhysicsEntityMap::Remove(rp3d::ProxyShape * shape)
{
	map.erase(shape);
}

Entity* PhysicsEntityMap::operator[](rp3d::ProxyShape *shape)
{
	return map[shape];
}

PhysicsEntityMap::PhysicsEntityMap()
{
}


PhysicsEntityMap::~PhysicsEntityMap()
{
}
