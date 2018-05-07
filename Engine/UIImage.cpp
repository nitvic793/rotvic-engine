// Written by Trevor Walden
#include "UIImage.h"
using namespace DirectX;

UIImage::UIImage(XMFLOAT3 pos, XMFLOAT4 tColor, std::string image, SystemCore* core, XMFLOAT2 scale)
{
	this->scale = scale;
	mPosition = pos;
	tint = XMFLOAT4(tColor.x/255, tColor.y/255, tColor.z/255, tColor.w/255);
	CreateWICTextureFromFile(core->GetDevice(), core->GetDeviceContext(), core->s2ws(image).c_str(), nullptr, &img);
}


UIImage::~UIImage()
{
	img->Release();
}

ComponentType UIImage::GetType() const
{
	return UI_IMAGE;
}
