/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "UIComponent.h"
#include <string>

/// <summary>
/// UI Text component. Inherits UI Component so that it can be used with Canvas.
/// </summary>
class UIText : public UIComponent
{
	std::wstring mText;
public:
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
	UIText();

	/// <summary>
	/// Get type of component. 
	/// </summary>
	/// <returns>Component Type Enum</returns>
	ComponentType GetType() const override;
	~UIText();
};

