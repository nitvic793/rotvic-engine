#include <Windows.h>
#include "../Core.Game/Game.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	{
		// Get the real, full path to this executable, end the string before
		// the filename itself and then set that as the current directory
		char currentDir[1024] = {};
		GetModuleFileName(0, currentDir, 1024);
		char* lastSlash = strrchr(currentDir, '\\');
		if (lastSlash)
		{
			*lastSlash = 0; // End the string at the last slash character
			SetCurrentDirectory(currentDir);
		}
	}

	CoreGame *game = new CoreGame(1280, 720, "DXGame");
	game->Initialize(hInstance, nCmdShow);
	auto gInstance = Game::CreateInstance();
	game->Bind(gInstance);
	gInstance->Initialize();
	game->Run();
	delete game;
	return 0;
}