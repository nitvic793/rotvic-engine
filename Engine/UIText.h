/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include "UIComponent.h"
#include <string>

class UIText : public UIComponent
{
	std::wstring mText;
public:
	void SetText(std::wstring text);
	std::wstring GetText();
	UIText();
	ComponentType GetType() const override;
	~UIText();
};

