#include "EventSystem.h"

EventSystem* EventSystem::instance = nullptr;

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

void EventSystem::EmitEvent(std::string eventType, void* args, void* instance)
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
			return; //Key value not found
		}

		eventMap[eventType][instance](args);
	}
}

EventSystem::EventSystem()
{
	instance = this;
}


EventSystem::~EventSystem()
{
}
