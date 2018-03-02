#pragma once

#include <d3d11.h>
#include "SystemCore.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include <mutex>
#include <string>
#include <vector>

using namespace DirectX;

class Console
{
	SystemCore *core;
	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;
	std::mutex consoleMutex;
	int consoleWidth;
	int maxLines;
	int currentLine;
	int height;
	std::vector<std::wstring> buffer;
public:
	bool enabled;
	void WriteLine(std::wstring line);
	void Render();
	Console(SystemCore *sysCore);
	~Console();
};

