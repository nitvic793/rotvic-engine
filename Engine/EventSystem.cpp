/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "EventSystem.h"

EventSystem* EventSystem::instance = nullptr;

void EventSystem::EmitEvent(std::string eventType, void * args, void * instance)
{
	if (eventMap.find(eventType) == eventMap.end())
	{
		return; //Return if no callbacks registered to event
	}

	if (instance == nullptr)
	{
		for (auto callback : eventMap[eventType])
		{
			callback.second(args);
		}
	}
	else
	{
		if (eventMap[eventType].find(instance) == eventMap[eventType].end())
		{
			return; //Don't do anything if key value not found
		}

		eventMap[eventType][instance](args);
	}
}

EventSystem * EventSystem::GetInstance()
{
	return instance;
}

void EventSystem::RegisterEventCallback(std::string eventType, void* instance, std::function<void(void*)> callback)
{
	if (eventMap.find(eventType) == eventMap.end()) //Event is present in map
	{
		eventMap.insert(std::pair<std::string, std::map<void*, std::function<void(void*)>>>(eventType, std::map<void*, std::function<void(void*)>>()));
	}

	eventMap[eventType].insert(std::pair<void*, std::function<void(void*)>>(instance, callback));
}

void EventSystem::EmitEventImmediate(std::string eventType, void* args, void* instance)
{
	EmitEvent(eventType, args, instance);
}

void EventSystem::EmitEventQueued(std::string eventType, EventUpdateType update, void* args, void* instance)
{
	eventQueue[update].push(EventArgs{ eventType, instance, args });
}

void EventSystem::ProcessQueuedEvents(EventUpdateType updateType)
{
	if (!eventQueue[updateType].empty())
	{
		auto eventArgs = eventQueue[updateType].front();
		eventQueue[updateType].pop();
		EmitEvent(eventArgs.eventType, eventArgs.args, eventArgs.instance);
	}
}

EventSystem::EventSystem()
{
	instance = this;

	//Initialize event queue map
	eventQueue.insert(std::pair<EventUpdateType, std::queue<EventArgs>>(GENERIC, std::queue<EventArgs>()));
	eventQueue.insert(std::pair<EventUpdateType, std::queue<EventArgs>>(PHYSICS, std::queue<EventArgs>()));
}


EventSystem::~EventSystem()
{
}
