/*
Author: Nitish Victor
Contains reference code from source https://github.com/SFML/SFML/blob/5d7843c58a850b4941d7d9794d6fede2bbc02833/include/SFML/Window/Keyboard.hpp
from the SFML project on https://github.com/SFML/SFML/
*/

#pragma once
#define NOMINMAX
#include <Windows.h>
#include <map>
#include <vector>

enum Keys
{
	Undefined = -1,
	A = 0,        ///< The A key
	B,            ///< The B key
	C,            ///< The C key
	D,            ///< The D key
	E,            ///< The E key
	F,            ///< The F key
	G,            ///< The G key
	H,            ///< The H key
	I,            ///< The I key
	J,            ///< The J key
	K,            ///< The K key
	L,            ///< The L key
	M,            ///< The M key
	N,            ///< The N key
	O,            ///< The O key
	P,            ///< The P key
	Q,            ///< The Q key
	R,            ///< The R key
	S,            ///< The S key
	T,            ///< The T key
	U,            ///< The U key
	V,            ///< The V key
	W,            ///< The W key
	X,            ///< The X key
	Y,            ///< The Y key
	Z,            ///< The Z key
	Num0,         ///< The 0 key
	Num1,         ///< The 1 key
	Num2,         ///< The 2 key
	Num3,         ///< The 3 key
	Num4,         ///< The 4 key
	Num5,         ///< The 5 key
	Num6,         ///< The 6 key
	Num7,         ///< The 7 key
	Num8,         ///< The 8 key
	Num9,         ///< The 9 key
	Esc,       ///< The Escape key
	LControl,     ///< The left Control key
	LShift,       ///< The left Shift key
	LAlt,         ///< The left Alt key
	LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
	RControl,     ///< The right Control key
	RShift,       ///< The right Shift key
	RAlt,         ///< The right Alt key
	RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
	Menu,         ///< The Menu key
	LBracket,     ///< The [ key
	RBracket,     ///< The ] key
	SemiColon,    ///< The ; key
	Comma,        ///< The , key
	Period,       ///< The . key
	Quote,        ///< The ' key
	Slash,        ///< The / key
	BackSlash,    ///< The \ key
	Tilde,        ///< The ~ key
	Equal,        ///< The = key
	Dash,         ///< The - key
	Space,        ///< The Space key
	Return,       ///< The Return key
	BackSpace,    ///< The Backspace key
	Tab,          ///< The Tabulation key
	PageUp,       ///< The Page up key
	PageDown,     ///< The Page down key
	End,          ///< The End key
	Home,         ///< The Home key
	Insert,       ///< The Insert key
	Delete,       ///< The Delete key
	Add,          ///< The + key
	Subtract,     ///< The - key
	Multiply,     ///< The * key
	Divide,       ///< The / key
	Left,         ///< Left arrow
	Right,        ///< Right arrow
	Up,           ///< Up arrow
	Down,         ///< Down arrow
	Numpad0,      ///< The numpad 0 key
	Numpad1,      ///< The numpad 1 key
	Numpad2,      ///< The numpad 2 key
	Numpad3,      ///< The numpad 3 key
	Numpad4,      ///< The numpad 4 key
	Numpad5,      ///< The numpad 5 key
	Numpad6,      ///< The numpad 6 key
	Numpad7,      ///< The numpad 7 key
	Numpad8,      ///< The numpad 8 key
	Numpad9,      ///< The numpad 9 key
	F1,           ///< The F1 key
	F2,           ///< The F2 key
	F3,           ///< The F3 key
	F4,           ///< The F4 key
	F5,           ///< The F5 key
	F6,           ///< The F6 key
	F7,           ///< The F7 key
	F8,           ///< The F8 key
	F9,           ///< The F9 key
	F10,          ///< The F10 key
	F11,          ///< The F11 key
	F12,          ///< The F12 key
	F13,          ///< The F13 key
	F14,          ///< The F14 key
	F15,          ///< The F15 key
	Pause,        ///< The Pause key
};

typedef std::pair<std::string, Keys> KeyEnumMap;

/// <summary>
/// Keyboard class for all keyboard related input data. 
/// </summary>
class Keyboard
{
protected:
	std::map<std::string, std::vector<Keys>> actionMap;
	std::map<std::string, Keys> inputEnumMap;
	static Keyboard* instance;
public:
	Keyboard();
	~Keyboard();

	/// <summary>
	/// Get instance of keyboard.
	/// </summary>
	/// <returns></returns>
	static Keyboard* GetInstance();

	/// <summary>
	/// Get enum of given key string
	/// </summary>
	/// <param name="key">A string representing one key value</param>
	/// <returns></returns>
	const Keys GetKeyEnumValue(std::string key);

	/// <summary>
	/// Get enum list of given keys.
	/// </summary>
	/// <param name="keys">Vector of keys</param>
	/// <returns></returns>
	std::vector<Keys> GetKeyEnumValue(std::vector<std::string> keys);

	/// <summary>
	/// Returns true if given key is pressed on the keyboard.
	/// </summary>
	/// <param name="key">The key enum</param>
	/// <returns></returns>
	virtual bool IsKeyPressed(Keys key);

	/// <summary>
	/// Returns true if given key is pressed on the keyboard.
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	virtual bool IsKeyPressed(wchar_t key);

	/// <summary>
	/// Returns true if any of the keys associated to this action is pressed.
	/// </summary>
	/// <param name="action">Name of action</param>
	/// <returns>True if action is pressed.</returns>
	virtual bool IsActionPressed(std::string action);

	/// <summary>
	/// Associate given keys with given action.
	/// </summary>
	/// <param name="action">Action name</param>
	/// <param name="keys">List of keys</param>
	void AddAction(std::string action, std::vector<Keys> keys);
};