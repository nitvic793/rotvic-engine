/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once
#include <DirectXMath.h>

enum ComponentType
{
	UI_TEXT,
	UI_IMAGE
};

class UIComponent
{
	DirectX::XMFLOAT3 mPosition;
public:
	void SetPosition(DirectX::XMFLOAT3 position);
	DirectX::XMFLOAT3 GetPosition();
	UIComponent();
	virtual ComponentType GetType() const = 0;
	virtual ~UIComponent();
};

