#pragma once

#include <Windows.h>
#include <map>

enum Keys
{
	Up, Down, Left, Right, F5, F6, Tab, Tilde
};

/// <summary>
/// Keyboard class for all keyboard related input data. 
/// </summary>
class Keyboard
{
protected:
	std::map<Keys, wchar_t> KeyMap;
	static Keyboard* instance;
public:
	Keyboard();
	~Keyboard();
	static Keyboard* GetInstance();
	virtual bool IsKeyPressed(Keys key);
	virtual bool IsKeyPressed(wchar_t key);
};