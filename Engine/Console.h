#pragma once

#include <d3d11.h>
#include "SystemCore.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <locale>
#include <codecvt>

using namespace DirectX;

typedef std::function<void(std::vector<std::string>)> CommandCallback;
typedef std::unordered_map<std::string, CommandCallback> CommandMap;

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
	std::wstringstream currentCommand;
	CommandMap commandMap;
	void ProcessCommand(std::string commandName, std::vector<std::string> params);
public:
	bool enabled;
	void Update(float deltaTime);
	void OnKeyPress(char key);
	void WriteLine(std::wstring line);
	void Render();
	void RegisterCommand(std::string commandName, CommandCallback command);
	Console(SystemCore *sysCore);
	~Console();
};

