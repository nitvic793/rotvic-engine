/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "UIComponent.h"

void UIComponent::SetPosition(DirectX::XMFLOAT3 position)
{
	mPosition = position;
}

DirectX::XMFLOAT3 UIComponent::GetPosition()
{
	return mPosition;
}

UIComponent::UIComponent()
{
}


UIComponent::~UIComponent()
{
}
