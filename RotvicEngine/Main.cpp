/// <summary>
/// Author: Nitish Victor
/// </summary>

#define NOMINMAX

#include <Windows.h>
#include "../Core.Game/Game.h"
//#include "vld.h" // - Include if Visual Leak Detector is installed.

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
	CoreGame *game = nullptr;
	try
	{
		game = new CoreGame("config.json");
		game->Initialize(hInstance, nCmdShow);
		auto gInstance = Game::CreateInstance();
		game->Bind(gInstance);
		game->InitializeInstance();
		game->Run();
	}
	catch (std::exception e)
	{
		if (game) game->HandleError(&e);
		else MessageBox(NULL, e.what(), "Error", MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(NULL, "Unexpected error. Aborting.", "Error", MB_ICONERROR);
	}

	if(game)delete game;
	return 0;
}