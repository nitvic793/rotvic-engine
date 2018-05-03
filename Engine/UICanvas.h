/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "SystemCore.h"
#include <memory>
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "UIComponent.h"

class UICanvas
{
	SystemCore *core;
	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;
	std::map<std::string, UIComponent*> mUIElements;
public:
	/// <summary>
	/// Adds given component to the canvas. 
	/// </summary>
	/// <param name="component">Component instance pointer.</param>
	/// <param name="name">Name of component. Should be unique</param>
	void AddComponent(UIComponent* component, std::string name);

	/// <summary>
	/// Remove component from canvas
	/// </summary>
	/// <param name="name">Name of component</param>
	void RemoveComponent(std::string name);

	/// <summary>
	/// Renders the canvas with the components added to this instance of the canvas.
	/// </summary>
	virtual void Render();
	UICanvas(SystemCore *core);
	~UICanvas();
};

