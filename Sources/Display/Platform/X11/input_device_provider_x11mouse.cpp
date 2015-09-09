/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Core/Text/string_format.h"
#include "input_device_provider_x11mouse.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "x11_window.h"

namespace clan
{
	InputDeviceProvider_X11Mouse::InputDeviceProvider_X11Mouse(X11Window *window)
		: window(window), mouse_pos(-1, -1), time_at_last_press(0), last_press_id(-1)
	{
		for (auto & elem : key_states) elem = false;
	}

	InputDeviceProvider_X11Mouse::~InputDeviceProvider_X11Mouse()
	{
		dispose();
	}

	void InputDeviceProvider_X11Mouse::on_dispose()
	{
	}

	Pointf InputDeviceProvider_X11Mouse::get_position() const
	{
		return Pointf{ get_device_position() } / window->get_pixel_ratio();
	}

	Point InputDeviceProvider_X11Mouse::get_device_position() const
	{
		Window root_return;
		Window child_return;
		int root_x_return;
		int root_y_return;
		int win_x_return = 0;
		int win_y_return = 0;
		unsigned int mask_return;

		XQueryPointer(window->get_display(), window->get_window(), &root_return, &child_return,
			&root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);
		return Point{ win_x_return, win_y_return };
	}


	bool InputDeviceProvider_X11Mouse::get_keycode(int keycode) const
	{
		if (keycode < 0 || keycode >= 32) return false;
		return key_states[keycode];
	}

	std::string InputDeviceProvider_X11Mouse::get_key_name(int id) const
	{
		switch (id)
		{
		case 0: return "Mouse left";
		case 1: return "Mouse right";
		case 2: return "Mouse middle";
		case 3: return "Mouse wheel up";
		case 4: return "Mouse wheel down";
		}

		return string_format("Mouse button %1", id);
	}

	std::string InputDeviceProvider_X11Mouse::get_name() const
	{
		return "System Mouse";
	}

	std::string InputDeviceProvider_X11Mouse::get_device_name() const
	{
		return "System Mouse";
	}

	int InputDeviceProvider_X11Mouse::get_button_count() const
	{
		return -1;
	}

	void InputDeviceProvider_X11Mouse::set_position(float x, float y)
	{
		x *= window->get_pixel_ratio();
		y *= window->get_pixel_ratio();
		set_device_position(x, y);
	}

	void InputDeviceProvider_X11Mouse::set_device_position(int x, int y)
	{
		XWarpPointer(window->get_display(), None, window->get_window(), 0, 0, 0, 0, x, y);
	}

	void InputDeviceProvider_X11Mouse::received_mouse_input(InputDevice &mouse, XButtonEvent &event)
	{
		int id;

		switch (event.button)
		{
		case 1: id = mouse_left; break;	// Left
		case 3: id = mouse_right; break;	// Right
		case 2: id = mouse_middle; break;	// Middle
		case 4: id = mouse_wheel_up; break;	// Scroll up
		case 5: id = mouse_wheel_down; break;	// Scroll down
		case 6: id = mouse_xbutton1; break;
		case 7: id = mouse_xbutton2; break;
		default: id = -1;	// Unknown press
		}

		bool is_a_double_click_event = false;

		// Handle double click timing
		if (event.type == ButtonPress)
		{
			Time time_change = event.time - time_at_last_press;
			time_at_last_press = event.time;

			if (last_press_id == id)	// Same key pressed
			{
				if (time_change < 500)	// 500 ms is the default in Windows
				{
					is_a_double_click_event = true;
					last_press_id = -1;	// Reset to avoid "tripple clicks"
				}
			}
			else
			{
				last_press_id = id;
			}
		}

		if (id == -1)
			return;	// Ignore unknown mouse clicks

		mouse_pos.x = event.x;
		mouse_pos.y = event.y;

		// Prepare event to be emitted:
		InputEvent key;
		key.mouse_pos = Pointf(mouse_pos) / window->get_pixel_ratio();
		key.mouse_device_pos = mouse_pos;

		key.id = (InputCode)id;
		if (event.type == ButtonPress)
		{
			if (is_a_double_click_event)
			{
				key.type = InputEvent::doubleclick;
			}
			else
			{
				key.type = InputEvent::pressed;
			}
			key_states[id] = 1;
		}
		else
		{
			key.type = InputEvent::released;
			key_states[id] = 0;
		}

		window->get_keyboard_modifiers(key.shift, key.alt, key.ctrl);

		if (event.type == ButtonPress)
			mouse.sig_key_down()(key);
		else
			mouse.sig_key_up()(key);
	}

	void InputDeviceProvider_X11Mouse::received_mouse_move(InputDevice &mouse, XMotionEvent &event)
	{
		int x = event.x;
		int y = event.y;

		if (mouse_pos.x != x || mouse_pos.y != y)
		{
			mouse_pos.x = x;
			mouse_pos.y = y;

			InputEvent key;
			key.type = InputEvent::pointer_moved;
			key.mouse_pos = Pointf(mouse_pos) / window->get_pixel_ratio();
			key.mouse_device_pos = mouse_pos;
			window->get_keyboard_modifiers(key.shift, key.alt, key.ctrl);

			mouse.sig_pointer_moved()(key);
		}
	}
}
