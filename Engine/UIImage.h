// Written by Trevor Walden
#pragma once
#include "UIComponent.h"
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include "SystemCore.h"
using namespace DirectX;

class UIImage :
	public UIComponent
{
public:
	XMFLOAT4 tint; // Tint color
	ID3D11ShaderResourceView* img;
	XMFLOAT2 scale;

	UIImage(XMFLOAT3 pos, XMFLOAT4 tColor, std::string image, SystemCore* core, XMFLOAT2 scale);
	~UIImage();

	/// <summary>
	/// Get type of component. 
	/// </summary>
	/// <returns>Component Type Enum</returns>
	ComponentType GetType() const override;
};

