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

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <iostream>
#include <errno.h>
#include <cstring>

#include "API/Core/Math/point.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "API/Display/input_event.h"

#include "input_device_linuxjoystick.h"

CL_InputDevice_LinuxJoystick::CL_InputDevice_LinuxJoystick(const std::string& device_)
	: device(device_)
{
	type = CL_InputDevice::joystick;

	fd = open(device.c_str(), O_RDONLY | O_NONBLOCK);

	if (fd == -1)
	{
		throw CL_Error(strerror(errno));
	}

	char number_of_axes;
	char number_of_buttons;

	ioctl(fd, JSIOCGBUTTONS, &number_of_buttons );
	ioctl(fd, JSIOCGAXES,    &number_of_axes );
  
	char name_cstr[256];
	if (ioctl(fd, JSIOCGNAME(sizeof(name_cstr)), name_cstr) < 0)
		strncpy(name_cstr, "Unknown", sizeof(name_cstr));

	name = name_cstr;

	axes_states.resize(number_of_axes);
	button_states.resize(number_of_buttons);
}

CL_InputDevice_LinuxJoystick::~CL_InputDevice_LinuxJoystick()
{
	close(fd);
}

void
CL_InputDevice_LinuxJoystick::process_event(struct js_event event)
{
	// We don't threat JS_EVENT_INIT special, so this should do
	if (event.type & JS_EVENT_BUTTON) 
	{
		button_states[event.number] = event.value;
		send_button_event(event);
	}
	else if (event.type & JS_EVENT_AXIS) 
	{
		axes_states[event.number] = float(event.value)/32767;
		send_axis_event(event);
	}
}

void
CL_InputDevice_LinuxJoystick::send_button_event(const struct js_event& event)
{
	CL_InputEvent e; 
			
	e.device = CL_InputDevice(this);
	e.id     = event.number;
	e.repeat_count = 0;

	if (event.value)
	{
		e.type = CL_InputEvent::pressed;
		sig_key_down(e);
	}
	else
	{
		e.type = CL_InputEvent::released;
		sig_key_up(e);
	}
}

void
CL_InputDevice_LinuxJoystick::send_axis_event(const struct js_event& event)
{
	CL_InputEvent e; 
		
	e.device   = CL_InputDevice(this);
	e.type     = CL_InputEvent::axis_moved;
	e.id       = event.number;
	e.axis_pos = float(event.value)/32767;
	e.repeat_count = 0;

	sig_axis_move(e);
}

void
CL_InputDevice_LinuxJoystick::keep_alive()
{
	struct js_event event;

	while (read(fd, &event, sizeof(struct js_event)) != -1)
	{
		process_event(event);
	}
	/* EAGAIN is returned when the queue is empty */
	if (errno != EAGAIN) {
		throw CL_Error(strerror(errno));
	}
}

std::string
CL_InputDevice_LinuxJoystick::get_key_name(int num) const
{
	return CL_String::from_int(num);
}

// Local Variables: ***
// mode: clanlib ***
// End: ***

