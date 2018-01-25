#include "Keyboard.h"

bool Keyboard::IsKeyPressed(Keys key)
{
	if (GetAsyncKeyState(KeyMap[key])) 
	{
		return true;
	}
	return false;
}

Keyboard::Keyboard()
{
	KeyMap.insert(std::pair<Keys, wchar_t>(Up, VK_UP));
	KeyMap.insert(std::pair<Keys, wchar_t>(Down, VK_DOWN));
	KeyMap.insert(std::pair<Keys, wchar_t>(Left, VK_LEFT));
	KeyMap.insert(std::pair<Keys, wchar_t>(Right, VK_RIGHT));
}

Keyboard::~Keyboard()
{

}