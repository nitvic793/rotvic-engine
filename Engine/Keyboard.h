#pragma once

#include <Windows.h>
#include <map>

enum Keys
{
	Up, Down, Left, Right
};

class Keyboard
{
protected:
	std::map<Keys, wchar_t> KeyMap;
public:
	Keyboard();
	~Keyboard();
	virtual bool IsKeyPressed(Keys key);
};