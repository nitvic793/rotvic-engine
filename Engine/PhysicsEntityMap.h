#pragma once

#include "Entity.h"

typedef std::map<rp3d::ProxyShape*, Entity*> ProxyEntityMap;
typedef std::pair<rp3d::ProxyShape*, Entity*> ProxyEntityMapType;

/// <summary>
/// Physics entity map. Maps proxy shape to entity.
/// </summary>
class PhysicsEntityMap
{
	ProxyEntityMap map;
public:
	/// <summary>
	/// Inserts shape-entity key-value pair into given map. 
	/// </summary>
	/// <param name="shape"></param>
	/// <param name="entity"></param>
	void Insert(rp3d::ProxyShape* shape, Entity* entity);

	/// <summary>
	/// Gets entity with given proxy shape. 
	/// </summary>
	/// <param name="shape"></param>
	/// <returns></returns>
	Entity* GetEntity(rp3d::ProxyShape* shape);

	/// <summary>
	/// Gets the underlying map representing the data. 
	/// </summary>
	/// <returns></returns>
	const ProxyEntityMap& GetMap();

	/// <summary>
	/// Remove key-value pair with given proxy shape. 
	/// </summary>
	/// <param name="shape"></param>
	void Remove(rp3d::ProxyShape* shape);
	Entity* operator[](rp3d::ProxyShape* shape);
	PhysicsEntityMap();
	~PhysicsEntityMap();
};

