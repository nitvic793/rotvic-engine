#include "PhysicsEventListener.h"
#include "EventSystem.h"

void PhysicsEventListener::SetEntityMap(PhysicsEntityMap* proxyMap)
{
	map = proxyMap;
}

void PhysicsEventListener::beginContact(const rp3d::ContactPointInfo& contact)
{

}

void PhysicsEventListener::newContact(const rp3d::ContactPointInfo& contact)
{
 	auto shape1 = contact.shape1;
	auto shape2 = contact.shape2;
	auto entity1 = map->GetEntity(shape1);
	auto entity2 = map->GetEntity(shape2);
	auto events = EventSystem::GetInstance();
	events->EmitEventQueued("Collision", PHYSICS, entity2, entity1); //Entity1 will recieve the event with Entity2 as argument
	events->EmitEventQueued("Collision", PHYSICS, entity1, entity2);
}

PhysicsEventListener::PhysicsEventListener()
{
	map = nullptr;
}


PhysicsEventListener::~PhysicsEventListener()
{
}
