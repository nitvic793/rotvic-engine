#pragma once
#include "Keyboard.h"
#include <vector>
#include "Entity.h"

class IGame
{
protected:
	Keyboard *keyboard;
	SystemCore *core;
public:
	void BindKeyboard(Keyboard* keyboard);
	void SetCore(SystemCore* core);
	virtual void Update() = 0;
	virtual std::vector<Entity*> GetEntities() = 0;
	IGame();
	~IGame();
};

