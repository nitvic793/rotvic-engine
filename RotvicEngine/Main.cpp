#include <Windows.h>
#include "../Core.Game/Game.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	CoreGame *game = new CoreGame(1280, 720, "DXGame");
	game->Initialize(hInstance, nCmdShow);
	game->Bind(Game::CreateInstance());
	game->Run();
	delete game;
	return 0;
}