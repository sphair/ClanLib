/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "x11_window.h"
#include "input_device_provider_x11keyboard.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Core/Text/string_help.h"
#include <cstdio>
#include <X11/XKBlib.h>

namespace clan
{
	InputDeviceProvider_X11Keyboard::InputDeviceProvider_X11Keyboard(X11Window *window)
		: window(window), ctrl_down(false), shift_down(false), alt_down(false)
	{
		current_keys_down.clear();
	}

	InputDeviceProvider_X11Keyboard::~InputDeviceProvider_X11Keyboard()
	{
		dispose();
	}

	void InputDeviceProvider_X11Keyboard::on_dispose()
	{
	}

	bool InputDeviceProvider_X11Keyboard::get_keycode(int keycode) const
	{
		// Ignore all key events when we don't have focus
		if (!window->has_focus())
			return false;

		char keyboard_state[32];

		InputCode code = (InputCode)XKeysymToKeycode(window->get_display(), keycode);
		XQueryKeymap(window->get_display(), keyboard_state);

		return keyboard_state[code / 8] & (1 << code % 8);
	}

	std::string InputDeviceProvider_X11Keyboard::get_key_name(int virtual_key) const
	{
		// Look up key name:

		char *name = XKeysymToString(virtual_key);
		if (name && name[0] != 0) return name;

		// Unknown. Return something at least :)
		char buffer[256];
		snprintf(buffer, 256, "Unknown %d", virtual_key);
		return buffer;
	}

	std::string InputDeviceProvider_X11Keyboard::get_name() const
	{
		return "System Keyboard";
	}

	std::string InputDeviceProvider_X11Keyboard::get_device_name() const
	{
		return "System Keyboard";
	}

	void InputDeviceProvider_X11Keyboard::get_keyboard_modifiers(bool &key_shift, bool &key_alt, bool &key_ctrl) const
	{
		key_shift = shift_down;
		key_alt = alt_down;
		key_ctrl = ctrl_down;
	}

	void InputDeviceProvider_X11Keyboard::received_keyboard_input(InputDevice &keyboard, XKeyEvent &event)
	{
		// Is message a down or up event?
		bool keydown = (event.type == KeyPress);

		InputCode key_code = (InputCode)event.keycode;

		// Prepare event to be emitted:
		InputEvent key;
		if (keydown)
			key.type = InputEvent::pressed;
		else
			key.type = InputEvent::released;

		key.mouse_pos = Pointf(window->get_mouse_position()) / window->get_pixel_ratio();
		key.mouse_device_pos = window->get_mouse_position();

		KeySym key_symbol = XkbKeycodeToKeysym(window->get_display(), key_code, 0, 0);

		bool keypressed = get_keycode(key_symbol);

		// Add to repeat count
		if (keydown && keypressed)
		{
			if (repeat_count.find(key_symbol) == repeat_count.end())
				repeat_count[key_symbol] = 0;
			else
				repeat_count[key_symbol]++;
		}

		key.repeat_count = repeat_count[key_symbol];

		if (!keydown && !keypressed)
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
				key_symbol = keycode_numpad7;
				break;
			case XK_KP_Up:
				key_symbol = keycode_numpad8;
				break;
			case XK_KP_Page_Up:
				key_symbol = keycode_numpad9;
				break;
			case XK_KP_Left:
				key_symbol = keycode_numpad4;
				break;
			case XK_KP_Begin:
				key_symbol = keycode_numpad5;
				break;
			case XK_KP_Right:
				key_symbol = keycode_numpad6;
				break;
			case XK_KP_End:
				key_symbol = keycode_numpad1;
				break;
			case XK_KP_Down:
				key_symbol = keycode_numpad2;
				break;
			case XK_KP_Page_Down:
				key_symbol = keycode_numpad3;
				break;
			case XK_KP_Insert:
				key_symbol = keycode_numpad0;
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
				key_symbol = keycode_home;
				break;
			case XK_KP_Up:
				key_symbol = keycode_up;
				break;
			case XK_KP_Page_Up:
				key_symbol = keycode_prior;
				break;
			case XK_KP_Left:
				key_symbol = keycode_left;
				break;
			case XK_KP_Begin:
				key_symbol = keycode_clear;
				break;
			case XK_KP_Right:
				key_symbol = keycode_right;
				break;
			case XK_KP_End:
				key_symbol = keycode_end;
				break;
			case XK_KP_Down:
				key_symbol = keycode_down;
				break;
			case XK_KP_Page_Down:
				key_symbol = keycode_next;
				break;
			case XK_KP_Insert:
				key_symbol = keycode_insert;
				break;
			case XK_KP_Delete:
				key_symbol = keycode_delete;
				break;
			}
		}

		key.id = (InputCode)key_symbol;

		key.shift = shift_down;
		key.alt = alt_down;
		key.ctrl = ctrl_down;

		const int buff_size = 16;
		char buff[buff_size];
		int result = XLookupString(&event, buff, buff_size - 1, nullptr, nullptr);
		if (result < 0) result = 0;
		if (result > (buff_size - 1)) result = buff_size - 1;
		buff[result] = 0;

		key.str = StringHelp::local8_to_text(std::string(buff, result));

		// Emit message:
		if (keydown)
			keyboard.sig_key_down()(key);
		else
			keyboard.sig_key_up()(key);
	}
}
