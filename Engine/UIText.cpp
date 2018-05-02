/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "UIText.h"

void UIText::SetText(std::wstring text)
{
	mText = text;
}

std::wstring UIText::GetText()
{
	return mText;
}

UIText::UIText()
{
}

ComponentType UIText::GetType() const
{
	return UI_TEXT;
}


UIText::~UIText()
{
}
