#include "IGame.h"



IGame::IGame()
{
}


IGame::~IGame()
{
}

void IGame::BindKeyboard(Keyboard* keyboard)
{
	this->keyboard = keyboard;
}

void IGame::SetCore(SystemCore* core)
{
	this->core = core;
}