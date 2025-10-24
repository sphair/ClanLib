/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include <cstdio>
#include "API/Display/input_device.h"
#include "API/Display/input_event.h"
#include "API/Display/keys.h"
#include "API/Core/System/clanstring.h"
#include "input_device_mackeyboard.h"
#include "display_window_opengl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacKeyboard construction:

CL_InputDevice_MacKeyboard::CL_InputDevice_MacKeyboard(CL_DisplayWindow_OpenGL *owner) :
	owner(owner)
{
	type = CL_InputDevice::keyboard;

//	slot_xevent = owner->sig_xevent.connect(
//		this, &CL_InputDevice_MacKeyboard::on_xevent);
}

CL_InputDevice_MacKeyboard::~CL_InputDevice_MacKeyboard()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacKeyboard attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacKeyboard operations:

std::string CL_InputDevice_MacKeyboard::get_key_name(int virtual_key) const
{
	switch (virtual_key)
	{
	case CL_KEY_A:				return "A";
	case CL_KEY_S:				return "S";
	case CL_KEY_D:				return "D";
	case CL_KEY_F:				return "F";
	case CL_KEY_H:				return "H";
	case CL_KEY_G:				return "G";
	case CL_KEY_Z:				return "Z";
	case CL_KEY_X:				return "X";
	case CL_KEY_C:				return "C";
	case CL_KEY_V:				return "V";
	case CL_KEY_B:				return "B";
	case CL_KEY_Q:				return "Q";
	case CL_KEY_W:				return "W";
	case CL_KEY_E:				return "E";
	case CL_KEY_R:				return "R";
	case CL_KEY_Y:				return "Y";
	case CL_KEY_T:				return "T";
	case CL_KEY_1:				return "1";
	case CL_KEY_2:				return "2";
	case CL_KEY_3:				return "3";
	case CL_KEY_4:				return "4";
	case CL_KEY_6:				return "6";
	case CL_KEY_5:				return "5";
	case CL_KEY_EQUALS:			return "=";
	case CL_KEY_9:				return "9";
	case CL_KEY_7:				return "7";
	case CL_KEY_SUBTRACT:		return "-";
	case CL_KEY_8:				return "8";
	case CL_KEY_0:				return "0";
	case CL_KEY_O:				return "O";
	case CL_KEY_U:				return "U";
	case CL_KEY_I:				return "I";
	case CL_KEY_P:				return "P";
	case CL_KEY_ENTER:			return "Return";
	case CL_KEY_L:				return "L";
	case CL_KEY_J:				return "J";
	case CL_KEY_QUOTE:			return "\'";
	case CL_KEY_K:				return "K";
	case CL_KEY_SEMICOLON:		return ";";
	case CL_KEY_COMMA:			return ",";
	case CL_KEY_DIVIDE:			return "/";
	case CL_KEY_N:				return "N";
	case CL_KEY_M:				return "M";
	case CL_KEY_PERIOD:			return ".";
	case CL_KEY_TAB:			return "Tab";
	case CL_KEY_SPACE:			return "Space";
	case CL_KEY_TILDE:			return "~";
	case CL_KEY_BACKSPACE:		return "Backspace";
	case CL_KEY_ESCAPE:			return "Escape";
	case CL_KEY_SHIFT:			return "Shift";
	case CL_KEY_MENU:			return "Option";
	case CL_KEY_CONTROL:		return "Control";
	case CL_KEY_NUMPAD_DECIMAL:	return "Numpad .";
	case CL_KEY_NUMPAD_MULTIPLY:return "Numpad *";
	case CL_KEY_NUMPAD_ADD:		return "Numpad +";
	case CL_KEY_NUMPAD_DIVIDE:	return "Numpad /";
	case CL_KEY_NUMPAD_ENTER:	return "Numpad Enter";
	case CL_KEY_NUMPAD_SUBTRACT:return "Numpad -";
	case CL_KEY_NUMPAD_EQUALS:	return "Numpad =";
	case CL_KEY_NUMPAD_0:		return "Numpad 0";
	case CL_KEY_NUMPAD_1:		return "Numpad 1";
	case CL_KEY_NUMPAD_2:		return "Numpad 2";
	case CL_KEY_NUMPAD_3:		return "Numpad 3";
	case CL_KEY_NUMPAD_4:		return "Numpad 4";
	case CL_KEY_NUMPAD_5:		return "Numpad 5";
	case CL_KEY_NUMPAD_6:		return "Numpad 6";
	case CL_KEY_NUMPAD_7:		return "Numpad 7";
	case CL_KEY_NUMPAD_8:		return "Numpad 8";
	case CL_KEY_NUMPAD_9:		return "Numpad 9";
	case CL_KEY_F5:				return "F5";
	case CL_KEY_F6:				return "F6";
	case CL_KEY_F7:				return "F7";
	case CL_KEY_F3:				return "F3";
	case CL_KEY_F8:				return "F8";
	case CL_KEY_F9:				return "F9";
	case CL_KEY_F10:			return "F10";
	case CL_KEY_F12:			return "F12";
	case CL_KEY_HOME:			return "Home";
	case CL_KEY_PAGE_UP:		return "Page Up";
	case CL_KEY_DELETE:			return "Delete";
	case CL_KEY_F4:				return "F4";
	case CL_KEY_END:			return "End";
	case CL_KEY_F2:				return "F2";
	case CL_KEY_PAGE_DOWN:		return "Page Down";
	case CL_KEY_F1:				return "F1";
	case CL_KEY_LEFT:			return "Left";
	case CL_KEY_RIGHT:			return "Right";
	case CL_KEY_DOWN:			return "Down";
	case CL_KEY_UP:				return "Up";
	case CL_KEY_RSHIFT:			return "RShift";
	case CL_KEY_RCONTROL:		return "RControl";
	case CL_KEY_COMMAND:		return "Command";
	case CL_KEY_CAPSLOCK:		return "Caps Lock";
	default:					return CL_String::format("Unknown %1", virtual_key);
	}
}

bool CL_InputDevice_MacKeyboard::get_keycode(int clkey) const
{
	// Ignore all key events when we don't have focus
	if(!owner->has_focus())	
		return false;
 
	//a few special cases for keys that are not handled below
	switch (clkey)
	{
	case CL_KEY_COMMAND:
		return (GetCurrentKeyModifiers() & cmdKey) != 0;
		break;
	}

	int code = CL_DisplayWindow_OpenGL::clkey_to_keycode(clkey);
	if (code < 0 || code >= 128)
		return false;

	KeyMap key_map;
	GetKeys(key_map);
	unsigned char *k = (unsigned char *) key_map;
	return k[code/8] & (1 << (code%8));
}

float CL_InputDevice_MacKeyboard::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_MacKeyboard::get_name() const
{
	return "System Keyboard";
}

int CL_InputDevice_MacKeyboard::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_MacKeyboard::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacKeyboard implementation:
/*
void CL_InputDevice_MacKeyboard::on_xevent(XEvent &event)
{
	// Only handle keyboard events.
	if (event.type != KeyPress && event.type != KeyRelease) return;

	// Figure out what key it was:
	KeySym sym = XLookupKeysym(&event.xkey, 0);

	// set state of modifier keys
	bool key_state = false;
	if (event.type == KeyPress)
		key_state = true;
	
	switch(sym)
	{
		case CL_KEY_LCONTROL: owner->left_ctrl_down = key_state; break;
		case CL_KEY_LMENU: owner->left_alt_down = key_state; break;
		case CL_KEY_LSHIFT: owner->left_shift_down = key_state; break;
		case CL_KEY_RCONTROL: owner->right_ctrl_down = key_state; break;
		case CL_KEY_RMENU: owner->right_alt_down = key_state; break;
		case CL_KEY_RSHIFT: owner->right_shift_down = key_state; break;
	}
	
	// Setup event structure for ClanLib:
	CL_InputEvent key;
	key.type = CL_InputEvent::released;
	if (event.type == KeyPress) key.type = CL_InputEvent::pressed;
	key.id = sym;
	key.device = owner->keyboard;
	key.repeat_count = 0;
	key.left_alt = owner->left_alt_down;
	key.left_ctrl = owner->left_ctrl_down;
	key.left_shift = owner->left_shift_down;
	key.right_alt = owner->right_alt_down;
	key.right_ctrl = owner->right_ctrl_down;
	key.right_shift = owner->right_shift_down;
	
	// Need to somehow get proper character sequence from X11. This code
	// wont keep proper track of deadkeys:
	char buf[11];
	buf[10] = 0;
	XLookupString(&event.xkey, buf, 10, 0, 0);
	if (strlen(buf) > 0) key.str = std::string(buf, 1);

	// Emit it.
	if (key.type == CL_InputEvent::pressed)
		owner->keyboard.sig_key_down().call(key);
	else
		owner->keyboard.sig_key_up().call(key);
}
*/
