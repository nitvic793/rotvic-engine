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
	void AddComponent(UIComponent* component, std::string name);
	void RemoveComponent(std::string name);
	virtual void Render();
	UICanvas(SystemCore *core);
	~UICanvas();
};

