#pragma once
#include "reactphysics3d.h"

class PhysicsEventListener : public rp3d::EventListener
{
public:
	void beginContact(const rp3d::ContactPointInfo& contact) override;
	void newContact(const rp3d::ContactPointInfo& contact) override;
	PhysicsEventListener();
	~PhysicsEventListener();
};

