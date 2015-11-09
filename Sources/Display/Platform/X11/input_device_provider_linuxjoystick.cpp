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

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include "Display/precomp.h"
#include "API/Core/Text/string_format.h"
#include "x11_window.h"
#include "input_device_provider_linuxjoystick.h"
#include "API/Display/Window/input_event.h"

namespace clan
{
	InputDeviceProvider_LinuxJoystick::InputDeviceProvider_LinuxJoystick(X11Window *window, const std::string &device)
		: window(window), device(device), fd(-1), new_event(false)
	{
		fd = open(device.c_str(), O_RDONLY | O_NONBLOCK);

		if (fd == -1)
		{
			throw Exception("Cannot Open Joystick");
		}

		char number_of_axes;
		char number_of_buttons;

		ioctl(fd, JSIOCGBUTTONS, &number_of_buttons);
		ioctl(fd, JSIOCGAXES, &number_of_axes);

		char name_cstr[256] = { '\0' };
		if (ioctl(fd, JSIOCGNAME(sizeof(name_cstr)), name_cstr) < 0)
			strncpy(name_cstr, "Unknown", sizeof(name_cstr));

		name = name_cstr;

		axis_states.resize(number_of_axes);
		button_states.resize(number_of_buttons);
	}

	InputDeviceProvider_LinuxJoystick::~InputDeviceProvider_LinuxJoystick()
	{
		dispose();
	}

	void InputDeviceProvider_LinuxJoystick::on_dispose()
	{
		if (fd)
		{
			close(fd);
		}
	}

	int InputDeviceProvider_LinuxJoystick::get_fd() const
	{
		throw_if_disposed();
		return fd;
	}

	bool InputDeviceProvider_LinuxJoystick::get_keycode(int keycode) const
	{
		int size = button_states.size();
		if (keycode < 0 || keycode >= size) return false;
		return button_states[keycode];
	}

	std::string InputDeviceProvider_LinuxJoystick::get_key_name(int id) const
	{
		return string_format("Joystick button %1", id);
	}

	float InputDeviceProvider_LinuxJoystick::get_axis(int index) const
	{
		int size = axis_states.size();
		if (index < 0 || index >= size) return 0.0f;
		return axis_states[index];
	}

	std::string InputDeviceProvider_LinuxJoystick::get_name() const
	{
		return name;
	}

	std::string InputDeviceProvider_LinuxJoystick::get_device_name() const
	{
		return device;
	}

	std::vector<int> InputDeviceProvider_LinuxJoystick::get_axis_ids() const
	{
		std::vector<int> ids;
		for (size_t i = 0; i < axis_states.size(); i++)
			ids.push_back(i);
		return ids;
	}

	int InputDeviceProvider_LinuxJoystick::get_button_count() const
	{
		return button_states.size();
	}

	void InputDeviceProvider_LinuxJoystick::process_event(InputDevice &joystick, js_event event) const
	{
		InputEvent input_event;
		input_event.mouse_pos = Pointf(window->get_mouse_position()) / window->get_pixel_ratio();
		input_event.mouse_device_pos = window->get_mouse_position();
		input_event.repeat_count = 0;

		// We don't threat JS_EVENT_INIT special, so this should do
		if (event.type & JS_EVENT_BUTTON)
		{
			// Validate event
			if (event.number >= button_states.size())
			{
				return;
			}
			new_event = true;
			bool value = event.value;
			button_states[event.number] = value;

			if (value)
			{
				input_event.type = InputEvent::pressed;
			}
			else
			{
				input_event.type = InputEvent::released;
			}

			input_event.id = (InputCode)event.number;

			if (value)
				joystick.sig_key_down()(input_event);
			else
				joystick.sig_key_up()(input_event);
		}
		else if (event.type & JS_EVENT_AXIS)
		{
			// Validate event
			if (event.number >= axis_states.size())
			{
				return;
			}
			new_event = true;
			float value = float(event.value) / 32767;
			axis_states[event.number] = value;

			input_event.type = InputEvent::axis_moved;
			input_event.id = (InputCode)event.number;
			input_event.axis_pos = value;

			joystick.sig_axis_move()(input_event);
		}
	}


	bool InputDeviceProvider_LinuxJoystick::poll(InputDevice &joystick, bool peek_only)
	{
		struct js_event event;

		while (read(fd, &event, sizeof(struct js_event)) != -1)
		{
			process_event(joystick, event);
		}

		bool joy_event = new_event;
		if (!peek_only)
		{
			new_event = false;
		}
		return joy_event;
	}
}
