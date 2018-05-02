/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "Console.h"

Console* Console::instance = nullptr;

float Console::CalculateCaretX()
{
	float xLength = DirectX::XMVectorGetX(spriteFont->MeasureString(L"_"));
	float stringLength = 0;
	int currentCommandLength = (int)currentCommand.str().length();
	if (!spaceStack.empty())
	{
		int extraLength = (int)spaceStack.size();
		stringLength = DirectX::XMVectorGetX(spriteFont->MeasureString(currentCommand.str().c_str())) + extraLength * xLength;
	}
	else
	{
		stringLength = DirectX::XMVectorGetX(spriteFont->MeasureString(currentCommand.str().c_str()));
	}
	return 20 + stringLength;
}

void Console::ProcessCommand(std::string commandName, std::vector<std::string> params)
{
	auto command = commandMap[commandName];
	if (command)
	{
		command(params);
	}
	else
	{
		WriteLine(L"Command not found");
	}
}

Console * Console::GetInstance()
{
	return instance;
}

void Console::Update(float deltaTime)
{
	delayTime += deltaTime;
	if (keyboard->IsKeyPressed(Up) && delayTime > 0.1f && !commandHistory.empty())
	{
		currentCommand.str(std::wstring());
		currentCommand.str(commandHistory.top());
		caretPosition = (int)commandHistory.top().length();
		currentCommand.seekp(commandHistory.top().length());
		commandHistoryUp.push(commandHistory.top());
		commandHistory.pop();
		while (!spaceStack.empty())spaceStack.pop();
		delayTime = 0;
	}

	if (keyboard->IsKeyPressed(Down) && delayTime > 0.1f && !commandHistoryUp.empty())
	{
		currentCommand.str(std::wstring());
		currentCommand.str(commandHistoryUp.top());
		caretPosition = (int)commandHistoryUp.top().length();
		currentCommand.seekp(commandHistoryUp.top().length());
		commandHistory.push(commandHistoryUp.top());
		commandHistoryUp.pop();
		while (!spaceStack.empty())spaceStack.pop();
		delayTime = 0;
	}
}

void Console::OnKeyPress(char key)
{
	if (!enabled)
	{
		return;
	}

	if (key == VK_ESCAPE)
	{
		enabled = false;
	}
	else if (key == VK_TAB)
	{
		//Do nothing
	}
	else if (key == '\b') //Backspace
	{
		if (caretPosition == 0) return;
		if(currentCommand.str()[caretPosition-1]==L' ' && !spaceStack.empty())spaceStack.pop();
		caretPosition--;
		auto cmd = currentCommand.str();		
		if (caretPosition != 0)
		{
			cmd.pop_back();
			currentCommand.str(cmd);
			currentCommand.seekp(caretPosition); //Clear command. No caret support yet. 
		}
		else
			currentCommand.str(std::wstring());
		
	}
	else if (key == '\r') //Enter
	{
		caretPosition = 0;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string commandStr = converter.to_bytes(currentCommand.str().c_str());
		std::istringstream commandArgs(commandStr);
		std::string input;
		std::string command;
		std::vector<std::string> args;
		bool gotCommandName = false;
		while (std::getline(commandArgs, input, ' '))
		{
			if (!gotCommandName)
			{
				command = input;
				gotCommandName = true;
			}
			else args.push_back(input);
		}

		WriteLine(L"Command " + currentCommand.str());
		ProcessCommand(command, args);

		while (!commandHistoryUp.empty())
		{
			commandHistory.push(commandHistoryUp.top());
			commandHistoryUp.pop();
		}
		commandHistory.push(currentCommand.str());
		currentCommand.str(std::wstring());
		while (!spaceStack.empty())spaceStack.pop();
	}
	else if (key == '`')return;
	else
	{
		caretPosition++;
		currentCommand << key;
		if (key == ' ')spaceStack.push(' ');
		else while (!spaceStack.empty())spaceStack.pop();
	}
}

void Console::WriteLine(std::wstring line)
{
	if (currentLine < maxLines)
	{
		buffer.push_back(line);
		currentLine++;
	}
	else
	{
		buffer.erase(buffer.begin());
		buffer.push_back(line);
	}
}

void Console::Render()
{
	spriteBatch->Begin();
	int counter = 1;
	for (auto line : buffer)
	{
		if (counter > currentLine)break;
		spriteFont->DrawString(spriteBatch.get(), line.c_str(), XMVectorSet(10, (float)height * counter, 0, 0));
		counter++;
	}

	float xLength = DirectX::XMVectorGetX(spriteFont->MeasureString(L"_"));
	spriteFont->DrawString(spriteBatch.get(), L":", XMVectorSet(10, (float)height * counter, 0, 0));
	spriteFont->DrawString(spriteBatch.get(), currentCommand.str().c_str(), XMVectorSet(15 + DirectX::XMVectorGetX(spriteFont->MeasureString(L":")), (float)height * counter, 0, 0));
	spriteFont->DrawString(spriteBatch.get(), L"_", XMVectorSet(CalculateCaretX(), (float)height * counter, 0, 0));
	spriteBatch->End();

	// Reset render states, since sprite batch changes these!
	core->GetDeviceContext()->OMSetBlendState(0, 0, 0xFFFFFFFF);
	core->GetDeviceContext()->OMSetDepthStencilState(0, 0);
}

void Console::RegisterCommand(std::string commandName, CommandCallback command)
{
	commandMap.insert(std::pair<std::string, CommandCallback>(commandName, command));
}

Console::Console(SystemCore* sysCore)
{
	enabled = false;
	maxLines = 10;
	height = 20;
	currentLine = 0;
	core = sysCore;
	spriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(core->GetDeviceContext()));
	spriteFont = std::unique_ptr<SpriteFont>(new SpriteFont(core->GetDevice(), L"../../Assets/Fonts/lucida_small.spritefont"));
	keyboard = Keyboard::GetInstance();
	delayTime = 0;
	caretPosition = 0;
	//buffer = std::vector<std::wstring>(maxLines);
	instance = this;
}


Console::~Console()
{
}
