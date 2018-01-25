#include "Keyboard.h"

bool Keyboard::IsKeyPressed(Keys key)
{
	if (GetAsyncKeyState(KeyMap[key])) 
	{
		return true;
	}
}

Keyboard::Keyboard()
{
	KeyMap.insert(std::pair<wchar_t, Keys>(VK_UP, Up));
}

Keyboard::~Keyboard()
{

}