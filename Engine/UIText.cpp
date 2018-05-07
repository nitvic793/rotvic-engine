/// <summary>
/// Authors: Nitish Victor and Trevor Walden
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

UIText::UIText(std::wstring text, float pSize, SpriteFont* font, XMFLOAT4 pColor) 
{
	mText = text;
	size = pSize;
	spriteFont = font;
	color = pColor;
}

ComponentType UIText::GetType() const
{
	return UI_TEXT;
}


UIText::~UIText()
{
}
