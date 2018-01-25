#pragma once
#include "Keyboard.h"

class IGame
{
protected:
	Keyboard *keyboard;
public:
	void BindKeyboard(Keyboard* keyboard);
	virtual void Update() = 0;
	IGame();
	~IGame();
};

