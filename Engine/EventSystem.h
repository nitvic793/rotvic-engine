#pragma once

#include <functional>
#include <string>
#include <map>
#include "Entity.h"

/// <summary>
/// The event system
/// </summary>
class EventSystem
{
	static EventSystem* instance;
	std::map <std::string, std::map<void*, std::function<void(void*)>>> eventMap;
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
	/// Emit given event type to invoke all registered callback functions. 
	/// </summary>
	/// <param name="eventType">Name of Event Type</param>
	/// <param name="args">Arguments to pass to callback</param>
	/// <param name="instance">Instance to refer for callback function.</param>
	/// <remarks>Instances are used to map event type with given callback function.</remarks>
	void EmitEvent(std::string eventType, void* args = nullptr, void* instance = nullptr);
	EventSystem();
	~EventSystem();
};

