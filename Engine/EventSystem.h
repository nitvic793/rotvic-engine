#pragma once

#include <functional>
#include <string>
#include <map>
#include <queue>
#include "Entity.h"

enum EventUpdateType
{
	GENERIC,
	PHYSICS
};

struct EventArgs
{
	std::string eventType;
	void* instance;
	void* args;
};

/// <summary>
/// The event system
/// </summary>
class EventSystem
{
	static EventSystem* instance;
	std::unordered_map<std::string, std::map<void*, std::function<void(void*)>>> eventMap;
	std::unordered_map<EventUpdateType, std::queue<EventArgs>> eventQueue;
	void EmitEvent(std::string eventType, void* args = nullptr, void* instance = nullptr);
public:
	/// <summary>
	/// Get instance of event system.
	/// </summary>
	/// <returns></returns>
	static EventSystem* GetInstance();

	/// <summary>
	/// Registers callback for given event type.
	/// </summary>
	/// <param name="eventType">Name of Event type</param>
	/// <param name="instance">Instance to associate event with</param>
	/// <param name="callback">Callback function</param>
	void RegisterEventCallback(std::string eventType, void* instance, std::function<void(void*)> callback);

	/// <summary>
	/// Emit given event type to invoke all registered callback functions immediately. 
	/// </summary>
	/// <param name="eventType">Name of Event Type</param>
	/// <param name="args">Arguments to pass to callback</param>
	/// <param name="instance">Instance to refer for callback function.</param>
	/// <remarks>Instances are used to map event type with given callback function.</remarks>
	void EmitEventImmediate(std::string eventType, void* args = nullptr, void* instance = nullptr);

	/// <summary>
	/// Queue emit event to required update cycle. On update cycle, all registered event callback functions will be invoked. 
	/// </summary>
	/// <param name="eventType"></param>
	/// <param name="update"></param>
	/// <param name="args"></param>
	/// <param name="instance"></param>
	void EmitEventQueued(std::string eventType, EventUpdateType update, void* args = nullptr, void* instance = nullptr);

	void ProcessQueuedEvents(EventUpdateType updateType);

	EventSystem();
	~EventSystem();
};

