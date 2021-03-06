/// <summary>
/// Authors: Nitish Victor and Trevor Walden
/// </summary>

#include "UICanvas.h"
#include "UIText.h"
#include "UIImage.h"

void UICanvas::AddComponent(UIComponent* component, std::string name)
{
	mUIElements.insert(std::pair<std::string, UIComponent*>(name, component));
}

void UICanvas::RemoveComponent(std::string name)
{
	if (mUIElements.find(name) != mUIElements.end())
	{
		mUIElements.erase(name);
	}
}

void UICanvas::Render()
{
	//Return if there are no elements to render. 
	if (mUIElements.size() == 0)
	{
		return;
	}

	spriteBatch->Begin();
	for (auto elementPair : mUIElements)
	{
		auto element = elementPair.second;
		switch (element->GetType())
		{
			case ComponentType::UI_TEXT:
			{
				auto text = (UIText*)element;
			
				text->spriteFont->DrawString(spriteBatch.get(), text->GetText().c_str(), XMLoadFloat3(&text->GetPosition()), XMLoadFloat4(&text->color), 0, g_XMZero, text->size);
				break;
			}
			case ComponentType::UI_IMAGE:
			{
				auto sprite = (UIImage*)element;
				spriteBatch->Draw(sprite->img, XMLoadFloat3(&sprite->GetPosition()), nullptr, XMLoadFloat4(&sprite->tint), 0, g_XMZero, XMLoadFloat2(&sprite->scale));
				break;
			}
			default:
				break;
		}
	}
	spriteBatch->End();

	// Reset render states, since sprite batch changes these.
	core->GetDeviceContext()->OMSetBlendState(0, 0, 0xFFFFFFFF);
	core->GetDeviceContext()->OMSetDepthStencilState(0, 0);
}

UICanvas::UICanvas(SystemCore *core)
{
	this->core = core;
	spriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(core->GetDeviceContext()));
}


UICanvas::~UICanvas()
{
	for (std::map<std::string, UIComponent*>::iterator itr = mUIElements.begin(); itr != mUIElements.end(); itr++) // https://stackoverflow.com/questions/19970531/properly-destroying-pointers-in-an-stdmap
	{
		delete (itr->second);
	}
	mUIElements.clear();
}
