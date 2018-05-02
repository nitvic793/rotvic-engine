/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "PhysicsEntityMap.h"
#include "reactphysics3d.h"

/// <summary>
/// Listens to physics events by physics middleware
/// </summary>
class PhysicsEventListener : public rp3d::EventListener
{
	PhysicsEntityMap* map;
public:
	/// <summary>
	/// Set entity map to refer to in case of physics events
	/// </summary>
	/// <param name="proxyMap"></param>
	void SetEntityMap(PhysicsEntityMap* proxyMap);

	/// <summary>
	/// Invoked when two bodies begin contact.
	/// </summary>
	/// <param name="contact"></param>
	void beginContact(const rp3d::ContactPointInfo& contact) override;

	/// <summary>
	/// Invoked when two bodies have a new contact point. 
	/// </summary>
	/// <param name="contact"></param>
	void newContact(const rp3d::ContactPointInfo& contact) override;
	PhysicsEventListener();
	~PhysicsEventListener();
};

