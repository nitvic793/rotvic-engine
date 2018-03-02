#include "Keyboard.h"

/// <summary>
/// Checks if given key is pressed or not.
/// </summary>
/// <param name="key">The enum representing the key</param>
/// <returns>True if key is pressed, else false</returns>
/// <remarks>Need to expand key map.</remarks>
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
	//Manually setting the key-value map.
	KeyMap.insert(std::pair<Keys, wchar_t>(Up, VK_UP));
	KeyMap.insert(std::pair<Keys, wchar_t>(Down, VK_DOWN));
	KeyMap.insert(std::pair<Keys, wchar_t>(Left, VK_LEFT));
	KeyMap.insert(std::pair<Keys, wchar_t>(Right, VK_RIGHT));
	KeyMap.insert(std::pair<Keys, wchar_t>(F5, VK_F5));
	KeyMap.insert(std::pair<Keys, wchar_t>(F6, VK_F6));
	KeyMap.insert(std::pair<Keys, wchar_t>(Tab, VK_TAB));
}

Keyboard::~Keyboard()
{

}