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

UIText::UIText(XMFLOAT3 pos, std::wstring text, float pSize, SpriteFont* font, XMFLOAT4 pColor) 
{
	mPosition = pos;
	mText = text;
	size = pSize;
	spriteFont = font;
	color = XMFLOAT4(pColor.x / 255, pColor.y / 255, pColor.z / 255, pColor.w / 255);
}

ComponentType UIText::GetType() const
{
	return UI_TEXT;
}


UIText::~UIText()
{
	delete spriteFont;
}
