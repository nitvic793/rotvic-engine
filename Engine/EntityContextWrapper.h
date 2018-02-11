#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include <functional>

/// <summary>
/// The context struct which will be passed to every entity.
/// </summary>
/// <remarks>This context will expand depending on the requirements of entities</remarks>
struct EntityContextWrapper
{
	Keyboard *keyboard;
	Mouse *mouse;

	/// <summary>
	/// Callback to get the mouse position in 2D game mode. 
	/// </summary>
	std::function<Vector2f()> GetMousePosition2D;

	EntityContextWrapper(Keyboard *keyboard, Mouse *mouse)
	{
		this->mouse = mouse;
		this->keyboard = keyboard;
	}

	EntityContextWrapper() {}
};