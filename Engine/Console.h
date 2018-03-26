#pragma once

#include <d3d11.h>
#include "SystemCore.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Keyboard.h"
#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <locale>
#include <codecvt>
#include <stack>

using namespace DirectX;

typedef std::function<void(std::vector<std::string>)> CommandCallback;
typedef std::unordered_map<std::string, CommandCallback> CommandMap;

/// <summary>
/// In-game console
/// </summary>
class Console
{
	static Console* instance;
	SystemCore *core;
	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;
	std::mutex consoleMutex;
	int consoleWidth;
	int maxLines;
	int currentLine;
	int height;
	int caretPosition;
	float delayTime;
	std::vector<std::wstring> buffer;
	std::wstringstream currentCommand;
	std::stack<std::wstring> commandHistory;
	std::stack<std::wstring> commandHistoryUp;
	std::stack<char> spaceStack;
	CommandMap commandMap;
	Keyboard* keyboard;

	/// <summary>
	/// Calculates where the caret should be
	/// </summary>
	/// <remarks>Backspace doesn't work as intended when there are "spaces". No arrow key support yet.</remarks>
	/// <returns></returns>
	float CalculateCaretX();

	/// <summary>
	/// Invokes the command with given command name if it was registered with the console.
	/// </summary>
	/// <param name="commandName">Name of the command</param>
	/// <param name="params">Params to pass to command</param>
	void ProcessCommand(std::string commandName, std::vector<std::string> params);
public:
	/// <summary>
	/// Get current console instance
	/// </summary>
	/// <returns></returns>
	static Console* GetInstance();

	/// <summary>
	/// Flag which enables or disables the console rendering and command processing.
	/// </summary>
	bool enabled;

	/// <summary>
	/// Called every frame when console is enabled.
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime);

	/// <summary>
	/// Invoked by core game whenever a key is pressed and console is enabled.
	/// </summary>
	/// <param name="key"></param>
	void OnKeyPress(char key);

	/// <summary>
	/// Write to the console.
	/// </summary>
	/// <param name="line"></param>
	void WriteLine(std::wstring line);

	/// <summary>
	/// Renders the console on-screen
	/// </summary>
	void Render();

	/// <summary>
	/// Register a command to the console. This command will be invoke-able from the console.
	/// </summary>
	/// <param name="commandName">Name of the command. This will be used to invoke the command</param>
	/// <param name="command">The function to call when the command is invoked</param>
	void RegisterCommand(std::string commandName, CommandCallback command);

	/// <summary>
	/// Console constructor
	/// </summary>
	/// <param name="sysCore">System core is required to instantiate the console.</param>
	Console(SystemCore *sysCore);

	~Console();
};

