#include "Console.h"

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
		spriteFont->DrawString(spriteBatch.get(), line.c_str(), XMVectorSet(10, height * counter, 0, 0));
		counter++;
	}

	spriteBatch->End();

	// Reset render states, since sprite batch changes these!
	core->GetDeviceContext()->OMSetBlendState(0, 0, 0xFFFFFFFF);
	core->GetDeviceContext()->OMSetDepthStencilState(0, 0);
}

Console::Console(SystemCore* sysCore)
{
	enabled = false;
	maxLines = 10;
	height = 20;
	currentLine = 0;
	core = sysCore;
	spriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(core->GetDeviceContext()));
	spriteFont = std::unique_ptr<SpriteFont>(new SpriteFont(core->GetDevice(), L"../../Assets/Fonts/arial.spritefont"));
	//buffer = std::vector<std::wstring>(maxLines);
}


Console::~Console()
{
}
