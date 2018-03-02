#pragma once

#include <Windows.h>
#include <map>

enum Keys
{
	Up, Down, Left, Right, F5, F6, Tab
};

/// <summary>
/// Keyboard class for all keyboard related input data. 
/// </summary>
class Keyboard
{
protected:
	std::map<Keys, wchar_t> KeyMap;
public:
	Keyboard();
	~Keyboard();
	virtual bool IsKeyPressed(Keys key);
};