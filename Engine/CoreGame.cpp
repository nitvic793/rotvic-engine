#include "CoreGame.h"

bool CoreGame::Initialize(HINSTANCE hInstance)
{

}

CoreGame::CoreGame()
{
}

CoreGame::CoreGame(int height, int width, std::string title)
{
	screenHeight = height;
	screenWidth = width;
	screenTitle = title;
}


CoreGame::~CoreGame()
{
}
