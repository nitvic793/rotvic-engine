#include "Console.h"

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

void Console::Update(float deltaTime)
{

}

void Console::OnKeyPress(char key)
{
	if (!enabled) 
	{
		return;
	}

	if (key == '\b') //Backspace
	{
		currentCommand.str(std::wstring()); //Clear command. No caret support yet. 
	}
	else if (key == '\r') //Enter
	{
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
		currentCommand.str(std::wstring());
	}
	else if (key == '`')return;
	else currentCommand << key;
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
	
	spriteFont->DrawString(spriteBatch.get(), L":", XMVectorSet(10, (float)height * counter, 0, 0));
	spriteFont->DrawString(spriteBatch.get(), currentCommand.str().c_str(), XMVectorSet(15 + DirectX::XMVectorGetX(spriteFont->MeasureString(L":")), (float)height * counter, 0, 0));
	spriteBatch->End();

	// Reset render states, since sprite batch changes these!
	core->GetDeviceContext()->OMSetBlendState(0, 0, 0xFFFFFFFF);
	core->GetDeviceContext()->OMSetDepthStencilState(0, 0);
}

void Console::RegisterCommand(std::string commandName, CommandCallback command)
{
	commandMap.insert(std::pair<std::string, CommandCallback>(commandName,command));
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
	//buffer = std::vector<std::wstring>(maxLines);
}


Console::~Console()
{
}
