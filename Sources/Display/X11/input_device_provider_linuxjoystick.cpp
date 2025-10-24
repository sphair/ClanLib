/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_LinuxJoystick construction:

CL_InputDeviceProvider_LinuxJoystick::CL_InputDeviceProvider_LinuxJoystick(CL_X11Window *window, const CL_StringRef &device)
: sig_provider_event(0), window(window), device(device), fd(-1), new_event(false)
{
	fd = open(device.c_str(), O_RDONLY | O_NONBLOCK);

	if (fd == -1)
	{
		throw CL_Exception("Cannot Open Joystick");
	}

	char number_of_axes;
	char number_of_buttons;

	ioctl(fd, JSIOCGBUTTONS, &number_of_buttons );
	ioctl(fd, JSIOCGAXES,    &number_of_axes );
  
	char name_cstr[256];
	if (ioctl(fd, JSIOCGNAME(sizeof(name_cstr)), name_cstr) < 0)
		strncpy(name_cstr, "Unknown", sizeof(name_cstr));

	name = name_cstr;

	axis_states.resize(number_of_axes);
	button_states.resize(number_of_buttons);
}

CL_InputDeviceProvider_LinuxJoystick::~CL_InputDeviceProvider_LinuxJoystick()
{
	dispose();
}

void CL_InputDeviceProvider_LinuxJoystick::on_dispose()
{
	if (fd)
	{
		close(fd);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_LinuxJoystick attributes:

int CL_InputDeviceProvider_LinuxJoystick::get_fd() const
{
	throw_if_disposed();
	return fd;
}

int CL_InputDeviceProvider_LinuxJoystick::get_x() const
{
	return 0;
}

int CL_InputDeviceProvider_LinuxJoystick::get_y() const
{
	return 0;
}

bool CL_InputDeviceProvider_LinuxJoystick::get_keycode(int keycode) const
{
	int size = button_states.size();
	if (keycode < 0 || keycode >= size) return false;
	update_states();
	return button_states[keycode];
}

CL_String CL_InputDeviceProvider_LinuxJoystick::get_key_name(int id) const
{
	return cl_format("Joystick button %1", id);
}

float CL_InputDeviceProvider_LinuxJoystick::get_axis(int index) const
{
	int size = axis_states.size();
	if (index < 0 || index >= size) return 0.0f;
	update_states();
	return axis_states[index];
}

CL_String CL_InputDeviceProvider_LinuxJoystick::get_name() const
{
	return name;
}

CL_String CL_InputDeviceProvider_LinuxJoystick::get_device_name() const
{
	return device;
}

int CL_InputDeviceProvider_LinuxJoystick::get_axis_count() const
{
	return axis_states.size();
}

int CL_InputDeviceProvider_LinuxJoystick::get_button_count() const
{
	return button_states.size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_LinuxJoystick operations:

void CL_InputDeviceProvider_LinuxJoystick::set_position(int x, int y)
{
	// Force feedback?
}

void CL_InputDeviceProvider_LinuxJoystick::process_event(struct js_event event) const
{
	CL_InputEvent input_event;
	input_event.mouse_pos = window->get_mouse_position();
	input_event.repeat_count = 0;

	// We don't threat JS_EVENT_INIT special, so this should do
	if (event.type & JS_EVENT_BUTTON) 
	{
		// Validate event
		if (event.number >=  button_states.size())
		{
			return;
		}
		new_event = true;
		bool value = event.value;
		button_states[event.number] = value;

		if (sig_provider_event)
		{
			if (value)
			{
				input_event.type = CL_InputEvent::pressed;
			}else
			{
				input_event.type = CL_InputEvent::released;
			}

			input_event.id = event.number;

			sig_provider_event->invoke(input_event);
		}
	}
	else if (event.type & JS_EVENT_AXIS) 
	{
		// Validate event
		if (event.number >=  axis_states.size())
		{
			return;
		}
		new_event = true;
		float value = float(event.value)/32767;
		axis_states[event.number] = value;

		if (sig_provider_event)
		{
			input_event.type = CL_InputEvent::axis_moved;
			input_event.id = event.number;
			input_event.axis_pos = value;

			sig_provider_event->invoke(input_event);
		}
	}
}

void CL_InputDeviceProvider_LinuxJoystick::update_states() const
{
	throw_if_disposed();

	struct js_event event;

	while (read(fd, &event, sizeof(struct js_event)) != -1)
	{
		process_event(event);
	}

	/* EAGAIN is returned when the queue is empty */
	// Ignore errors
	//if (errno != EAGAIN) {
	//	throw CL_Error(strerror(errno));
	//}
}

bool CL_InputDeviceProvider_LinuxJoystick::poll(bool peek_only)
{
	update_states();

	bool joy_event = new_event;
	if (!peek_only)
	{
		new_event = false;
	}
	return joy_event;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_LinuxJoystick implementation:

