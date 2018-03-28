#include "Keyboard.h"
#include "Console.h"

Keyboard* Keyboard::instance = nullptr;

bool Keyboard::IsKeyPressed(wchar_t key)
{
	return (GetAsyncKeyState(key) & 0x8000) != 0;
}

bool Keyboard::IsActionPressed(std::string action)
{
	for (auto key : actionMap[action])
	{
		if (IsKeyPressed(key))
		{
			return true;
		}
	}

	return false;
}

void Keyboard::AddAction(std::string action, std::vector<Keys> keys)
{
	if (actionMap.find(action) != actionMap.end()) 
		actionMap[action] = keys;
	else actionMap.insert(std::pair<std::string, std::vector<Keys>>(action, keys));
}


/// <summary>
/// Checks if given key is pressed or not. 
/// </summary>
/// <param name="key">The enum representing the key</param>
/// <returns>True if key is pressed, else false</returns>
/// <remarks>Code referenced from SFML project https://github.com/SFML/SFML/. 
/// Source: https://github.com/SFML/SFML/blob/5d7843c58a850b4941d7d9794d6fede2bbc02833/src/SFML/Window/Win32/InputImpl.cpp 
/// </remarks>
bool Keyboard::IsKeyPressed(Keys key)
{
	int vkey = 0;
	switch (key)
	{
	default:                   vkey = 0;             break;
	case Keys::A:          vkey = 'A';           break;
	case Keys::B:          vkey = 'B';           break;
	case Keys::C:          vkey = 'C';           break;
	case Keys::D:          vkey = 'D';           break;
	case Keys::E:          vkey = 'E';           break;
	case Keys::F:          vkey = 'F';           break;
	case Keys::G:          vkey = 'G';           break;
	case Keys::H:          vkey = 'H';           break;
	case Keys::I:          vkey = 'I';           break;
	case Keys::J:          vkey = 'J';           break;
	case Keys::K:          vkey = 'K';           break;
	case Keys::L:          vkey = 'L';           break;
	case Keys::M:          vkey = 'M';           break;
	case Keys::N:          vkey = 'N';           break;
	case Keys::O:          vkey = 'O';           break;
	case Keys::P:          vkey = 'P';           break;
	case Keys::Q:          vkey = 'Q';           break;
	case Keys::R:          vkey = 'R';           break;
	case Keys::S:          vkey = 'S';           break;
	case Keys::T:          vkey = 'T';           break;
	case Keys::U:          vkey = 'U';           break;
	case Keys::V:          vkey = 'V';           break;
	case Keys::W:          vkey = 'W';           break;
	case Keys::X:          vkey = 'X';           break;
	case Keys::Y:          vkey = 'Y';           break;
	case Keys::Z:          vkey = 'Z';           break;
	case Keys::Num0:       vkey = '0';           break;
	case Keys::Num1:       vkey = '1';           break;
	case Keys::Num2:       vkey = '2';           break;
	case Keys::Num3:       vkey = '3';           break;
	case Keys::Num4:       vkey = '4';           break;
	case Keys::Num5:       vkey = '5';           break;
	case Keys::Num6:       vkey = '6';           break;
	case Keys::Num7:       vkey = '7';           break;
	case Keys::Num8:       vkey = '8';           break;
	case Keys::Num9:       vkey = '9';           break;
	case Keys::Esc:		   vkey = VK_ESCAPE;     break;
	case Keys::LControl:   vkey = VK_LCONTROL;   break;
	case Keys::LShift:     vkey = VK_LSHIFT;     break;
	case Keys::LAlt:       vkey = VK_LMENU;      break;
	case Keys::LSystem:    vkey = VK_LWIN;       break;
	case Keys::RControl:   vkey = VK_RCONTROL;   break;
	case Keys::RShift:     vkey = VK_RSHIFT;     break;
	case Keys::RAlt:       vkey = VK_RMENU;      break;
	case Keys::RSystem:    vkey = VK_RWIN;       break;
	case Keys::Menu:       vkey = VK_APPS;       break;
	case Keys::LBracket:   vkey = VK_OEM_4;      break;
	case Keys::RBracket:   vkey = VK_OEM_6;      break;
	case Keys::SemiColon:  vkey = VK_OEM_1;      break;
	case Keys::Comma:      vkey = VK_OEM_COMMA;  break;
	case Keys::Period:     vkey = VK_OEM_PERIOD; break;
	case Keys::Quote:      vkey = VK_OEM_7;      break;
	case Keys::Slash:      vkey = VK_OEM_2;      break;
	case Keys::BackSlash:  vkey = VK_OEM_5;      break;
	case Keys::Tilde:      vkey = VK_OEM_3;      break;
	case Keys::Equal:      vkey = VK_OEM_PLUS;   break;
	case Keys::Dash:       vkey = VK_OEM_MINUS;  break;
	case Keys::Space:      vkey = VK_SPACE;      break;
	case Keys::Return:     vkey = VK_RETURN;     break;
	case Keys::BackSpace:  vkey = VK_BACK;       break;
	case Keys::Tab:        vkey = VK_TAB;        break;
	case Keys::PageUp:     vkey = VK_PRIOR;      break;
	case Keys::PageDown:   vkey = VK_NEXT;       break;
	case Keys::End:        vkey = VK_END;        break;
	case Keys::Home:       vkey = VK_HOME;       break;
	case Keys::Insert:     vkey = VK_INSERT;     break;
	case Keys::Delete:     vkey = VK_DELETE;     break;
	case Keys::Add:        vkey = VK_ADD;        break;
	case Keys::Subtract:   vkey = VK_SUBTRACT;   break;
	case Keys::Multiply:   vkey = VK_MULTIPLY;   break;
	case Keys::Divide:     vkey = VK_DIVIDE;     break;
	case Keys::Left:       vkey = VK_LEFT;       break;
	case Keys::Right:      vkey = VK_RIGHT;      break;
	case Keys::Up:         vkey = VK_UP;         break;
	case Keys::Down:       vkey = VK_DOWN;       break;
	case Keys::Numpad0:    vkey = VK_NUMPAD0;    break;
	case Keys::Numpad1:    vkey = VK_NUMPAD1;    break;
	case Keys::Numpad2:    vkey = VK_NUMPAD2;    break;
	case Keys::Numpad3:    vkey = VK_NUMPAD3;    break;
	case Keys::Numpad4:    vkey = VK_NUMPAD4;    break;
	case Keys::Numpad5:    vkey = VK_NUMPAD5;    break;
	case Keys::Numpad6:    vkey = VK_NUMPAD6;    break;
	case Keys::Numpad7:    vkey = VK_NUMPAD7;    break;
	case Keys::Numpad8:    vkey = VK_NUMPAD8;    break;
	case Keys::Numpad9:    vkey = VK_NUMPAD9;    break;
	case Keys::F1:         vkey = VK_F1;         break;
	case Keys::F2:         vkey = VK_F2;         break;
	case Keys::F3:         vkey = VK_F3;         break;
	case Keys::F4:         vkey = VK_F4;         break;
	case Keys::F5:         vkey = VK_F5;         break;
	case Keys::F6:         vkey = VK_F6;         break;
	case Keys::F7:         vkey = VK_F7;         break;
	case Keys::F8:         vkey = VK_F8;         break;
	case Keys::F9:         vkey = VK_F9;         break;
	case Keys::F10:        vkey = VK_F10;        break;
	case Keys::F11:        vkey = VK_F11;        break;
	case Keys::F12:        vkey = VK_F12;        break;
	case Keys::F13:        vkey = VK_F13;        break;
	case Keys::F14:        vkey = VK_F14;        break;
	case Keys::F15:        vkey = VK_F15;        break;
	case Keys::Pause:      vkey = VK_PAUSE;      break;
	}

	return (GetAsyncKeyState(vkey) & 0x8000) != 0;
}

Keyboard::Keyboard()
{
	instance = this;
	//Incomplete. Need to finish all keys
	inputEnumMap.insert(KeyEnumMap("A", Keys::A));
	inputEnumMap.insert(KeyEnumMap("B", Keys::B));
	inputEnumMap.insert(KeyEnumMap("C", Keys::C));
	inputEnumMap.insert(KeyEnumMap("D", Keys::D));
	inputEnumMap.insert(KeyEnumMap("E", Keys::E));
	inputEnumMap.insert(KeyEnumMap("F", Keys::F));
	inputEnumMap.insert(KeyEnumMap("G", Keys::G));
	inputEnumMap.insert(KeyEnumMap("H", Keys::H));
	inputEnumMap.insert(KeyEnumMap("I", Keys::I));
	inputEnumMap.insert(KeyEnumMap("J", Keys::J));
	inputEnumMap.insert(KeyEnumMap("K", Keys::K));
	inputEnumMap.insert(KeyEnumMap("L", Keys::L));
	inputEnumMap.insert(KeyEnumMap("M", Keys::M));
	inputEnumMap.insert(KeyEnumMap("N", Keys::N));
	inputEnumMap.insert(KeyEnumMap("O", Keys::O));
	inputEnumMap.insert(KeyEnumMap("P", Keys::P));
	inputEnumMap.insert(KeyEnumMap("Q", Keys::Q));
	inputEnumMap.insert(KeyEnumMap("R", Keys::R));
	inputEnumMap.insert(KeyEnumMap("S", Keys::S));
	inputEnumMap.insert(KeyEnumMap("T", Keys::T));
	inputEnumMap.insert(KeyEnumMap("U", Keys::U));
	inputEnumMap.insert(KeyEnumMap("V", Keys::V));
	inputEnumMap.insert(KeyEnumMap("W", Keys::W));
	inputEnumMap.insert(KeyEnumMap("X", Keys::X));
	inputEnumMap.insert(KeyEnumMap("Y", Keys::Y));
	inputEnumMap.insert(KeyEnumMap("Z", Keys::Z));
	inputEnumMap.insert(KeyEnumMap("F1", Keys::F1));
	inputEnumMap.insert(KeyEnumMap("F2", Keys::F2));
	inputEnumMap.insert(KeyEnumMap("ArrowUp", Keys::Up));
	inputEnumMap.insert(KeyEnumMap("ArrowDown", Keys::Down));
	inputEnumMap.insert(KeyEnumMap("ArrowLeft", Keys::Left));
	inputEnumMap.insert(KeyEnumMap("ArrowRight", Keys::Right));
	inputEnumMap.insert(KeyEnumMap("Delete", Keys::Delete));
	inputEnumMap.insert(KeyEnumMap("Space", Keys::Space));
	inputEnumMap.insert(KeyEnumMap("LShift", Keys::LShift));
	inputEnumMap.insert(KeyEnumMap("RShift", Keys::RShift));
	inputEnumMap.insert(KeyEnumMap("RCtrl", Keys::RControl));
	inputEnumMap.insert(KeyEnumMap("LCtrl", Keys::LControl));
	inputEnumMap.insert(KeyEnumMap("Backspace", Keys::BackSpace));
	inputEnumMap.insert(KeyEnumMap("Backslash", Keys::BackSlash));
}

Keyboard::~Keyboard()
{

}

Keyboard * Keyboard::GetInstance()
{
	return instance;
}

const Keys Keyboard::GetKeyEnumValue(std::string key)
{
	if (inputEnumMap.find(key) == inputEnumMap.end())
		return Undefined;
	return inputEnumMap[key];
}

std::vector<Keys> Keyboard::GetKeyEnumValue(std::vector<std::string> keys)
{
	std::vector<Keys> enumKeys;
	for (auto key : keys) 
	{
		enumKeys.push_back(GetKeyEnumValue(key));
	}
	return enumKeys;
}
