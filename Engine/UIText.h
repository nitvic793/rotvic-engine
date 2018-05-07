/// <summary>
/// Authors: Nitish Victor and Trevor Walden
/// </summary>

#pragma once
#include "UIComponent.h"
#include <string>
#include "SpriteBatch.h"
#include "SpriteFont.h"
using namespace DirectX;

/// <summary>
/// UI Text component. Inherits UI Component so that it can be used with Canvas.
/// </summary>
class UIText : public UIComponent
{
	std::wstring mText;
public:
	SpriteFont* spriteFont;
	float size; // Scale size
	XMFLOAT4 color; // Font color
	UIText(std::wstring text, float pSize, SpriteFont* font, XMFLOAT4 pColor);
	~UIText();

	/// <summary>
	/// Sets the text of the component.
	/// </summary>
	/// <param name="text">String to set the text field.</param>
	void SetText(std::wstring text);

	/// <summary>
	/// Gets the text of the component. 
	/// </summary>
	/// <returns>Wide char String</returns>
	std::wstring GetText();

	/// <summary>
	/// Get type of component. 
	/// </summary>
	/// <returns>Component Type Enum</returns>
	ComponentType GetType() const override;
};

