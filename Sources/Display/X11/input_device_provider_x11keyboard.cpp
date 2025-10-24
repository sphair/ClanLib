/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#include "Display/precomp.h"
#include "input_device_provider_x11keyboard.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Core/Text/string_help.h"
#include "x11_window.h"
#include <cstdio>

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard construction:

CL_InputDeviceProvider_X11Keyboard::CL_InputDeviceProvider_X11Keyboard(CL_X11Window *window)
: sig_provider_event(0), window(window), ctrl_down(false), shift_down(false), alt_down(false)
{
	current_keys_down.clear();
}

CL_InputDeviceProvider_X11Keyboard::~CL_InputDeviceProvider_X11Keyboard()
{
	dispose();
}

void CL_InputDeviceProvider_X11Keyboard::on_dispose()
{
}


/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard attributes:

bool CL_InputDeviceProvider_X11Keyboard::get_keycode(int keycode) const
{
	// Ignore all key events when we don't have focus
	if(!window->has_focus())
		return false;

	char keyboard_state[32];

	KeyCode code = XKeysymToKeycode(window->get_display(), keycode);
	XQueryKeymap(window->get_display(), keyboard_state);

	return keyboard_state[code/8] & (1 << code%8);
}

CL_String CL_InputDeviceProvider_X11Keyboard::get_key_name(int virtual_key) const
{
	// Look up key name:

	char *name = XKeysymToString(virtual_key);
	if (name && name[0] != 0) return name;

	// Unknown. Return something at least :)
	char buffer[256];
	snprintf(buffer, 256, "Unknown %d", virtual_key);
	return buffer;
}

float CL_InputDeviceProvider_X11Keyboard::get_axis(int index) const
{
	return 0.0f;
}

CL_String CL_InputDeviceProvider_X11Keyboard::get_name() const
{
	return "System Keyboard";
}

CL_String CL_InputDeviceProvider_X11Keyboard::get_device_name() const
{
	return "System Keyboard";
}

int CL_InputDeviceProvider_X11Keyboard::get_axis_count() const
{
	return 0;
}

int CL_InputDeviceProvider_X11Keyboard::get_button_count() const
{
	return -1;
}

void CL_InputDeviceProvider_X11Keyboard::get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const
{
	key_shift = shift_down;
	key_alt = alt_down;
	key_ctrl = ctrl_down;
}


/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard operations:

void CL_InputDeviceProvider_X11Keyboard::received_keyboard_input(XKeyEvent &event)
{
	// Is message a down or up event?
	bool keydown;
	if (event.type == KeyPress)
	{
		keydown = true;
	}else	keydown = false;


	KeyCode key_code = event.keycode;

	// Prepare event to be emitted:
	CL_InputEvent key;
	if (keydown)
		key.type = CL_InputEvent::pressed;
	else
		key.type = CL_InputEvent::released;
	key.mouse_pos = window->get_mouse_position();

	KeySym key_symbol = XKeycodeToKeysym(window->get_display(), key_code, 0);

	bool keypressed = get_keycode(key_symbol);

        // Add to repeat count                                                                                               
        if(keydown && keypressed)
	  {
	    if( repeat_count.find(key_symbol) == repeat_count.end() )
	      repeat_count[key_symbol] = 0;
	    else
	      repeat_count[key_symbol]++;
	  }

	key.repeat_count = repeat_count[key_symbol];

	if( !keydown && !keypressed )
	  {
	    repeat_count[key_symbol] = -1;

	  }

	switch (key_symbol)
	{
		case XK_Control_L:
		case XK_Control_R:
			ctrl_down = keydown;
			break;
		case XK_Shift_L:
		case XK_Shift_R:
			shift_down = keydown;
			break;
		case XK_Alt_L:
		case XK_Alt_R:
			alt_down = keydown;
			break;
	}

	if (event.state & Mod2Mask)	// Num Lock pressed
	{
		switch (key_symbol)
		{
			case XK_KP_Home:
				key_symbol = CL_KEY_NUMPAD7;
				break;
			case XK_KP_Up:
				key_symbol = CL_KEY_NUMPAD8;
				break;
			case XK_KP_Page_Up:
				key_symbol = CL_KEY_NUMPAD9;
				break;
			case XK_KP_Left:
				key_symbol = CL_KEY_NUMPAD4;
				break;
			case XK_KP_Begin:
				key_symbol = CL_KEY_NUMPAD5;
				break;
			case XK_KP_Right:
				key_symbol = CL_KEY_NUMPAD6;
				break;
			case XK_KP_End:
				key_symbol = CL_KEY_NUMPAD1;
				break;
			case XK_KP_Down:
				key_symbol = CL_KEY_NUMPAD2;
				break;
			case XK_KP_Page_Down:
				key_symbol = CL_KEY_NUMPAD3;
				break;
			case XK_KP_Insert:
				key_symbol = CL_KEY_NUMPAD0;
				break;
			case XK_KP_Delete:
				key_symbol = (int) '.';
				break;
		}
	}
	else
	{
		switch (key_symbol)
		{
			case XK_KP_Home:
				key_symbol = CL_KEY_HOME;
				break;
			case XK_KP_Up:
				key_symbol = CL_KEY_UP;
				break;
			case XK_KP_Page_Up:
				key_symbol = CL_KEY_PRIOR;
				break;
			case XK_KP_Left:
				key_symbol = CL_KEY_LEFT;
				break;
			case XK_KP_Begin:
				key_symbol = CL_KEY_CLEAR;
				break;
			case XK_KP_Right:
				key_symbol = CL_KEY_RIGHT;
				break;
			case XK_KP_End:
				key_symbol = CL_KEY_END;
				break;
			case XK_KP_Down:
				key_symbol = CL_KEY_DOWN;
				break;
			case XK_KP_Page_Down:
				key_symbol = CL_KEY_NEXT;
				break;
			case XK_KP_Insert:
				key_symbol = CL_KEY_INSERT;
				break;
			case XK_KP_Delete:
				key_symbol = CL_KEY_DELETE;
				break;
		}
	}

	key.id = key_symbol;

	key.shift = shift_down;
	key.alt = alt_down;
	key.ctrl = ctrl_down;

	const int buff_size = 16;
	char buff[buff_size];
	int result = XLookupString(&event, buff, buff_size - 1, NULL, NULL);
	if (result < 0) result = 0;
	if (result > (buff_size-1)) result = buff_size - 1;
	buff[result] = 0;

	key.str = CL_StringHelp::local8_to_text(CL_String8(buff, result));

	// Emit message:
	sig_provider_event->invoke(key);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Keyboard implementation:
