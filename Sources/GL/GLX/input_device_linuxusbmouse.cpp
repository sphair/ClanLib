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
#include <iostream>
#include <errno.h>
#include <cstring>

#include "API/Core/System/error.h"
#include "API/Display/keys.h"
#include "API/Display/input_event.h"
#include "API/Display/input_device.h"
#include "display_window_opengl.h"
#include "input_device_linuxusbmouse.h"

CL_InputDevice_LinuxUSBMouse::CL_InputDevice_LinuxUSBMouse(CL_DisplayWindow_Generic* parent, 
																			  const std::string& dev)
	: parent(parent),
	  device(dev),
	  buttons(5)
{
	type = CL_InputDevice::mouse;

	fd = open (device.c_str (), O_RDWR | O_NONBLOCK);

	if (fd == -1)
	{
		throw CL_Error(strerror(errno));
	}

	init_explorer_ps2();

	char data[4];
	read(fd, data, sizeof (data));
	read(fd, data, sizeof (data));
	read(fd, data, sizeof (data));
}

CL_InputDevice_LinuxUSBMouse::~CL_InputDevice_LinuxUSBMouse()
{
	close(fd);
}

void CL_InputDevice_LinuxUSBMouse::init_explorer_ps2()
{
	// Microsoft init sequence for Explorer mouse (wheel + 5 buttons)
	static unsigned char data[] = { 0xF3, 0xC8, 
											  0xF3, 0xC8,
											  0xF3, 0x50 };
  	write(fd, data, sizeof(data));
}

bool
CL_InputDevice_LinuxUSBMouse::get_keycode(int keycode) const
{
	if (keycode > 0 && keycode < int(buttons.size()))
		return buttons[keycode];
	else
		return false;
}

void
CL_InputDevice_LinuxUSBMouse::keep_alive()
{
	unsigned char data[4];
	while (read(fd, data, sizeof (data)) > 0)
	{		
		// Mouse Move:
		int delta_x = (data[0] & 0x10) ? data[1]-256 : data[1];
		int delta_y = (data[0] & 0x20) ? data[2]-256 : data[2];

		if (delta_x != 0 || delta_y != 0)
		{
			mouse_pos.x += delta_x;
			mouse_pos.y -= delta_y; // y-axis is reversed on-screen

			if (mouse_pos.x < 0) 
				mouse_pos.x = 0;
			else if (mouse_pos.x > parent->get_width())
				mouse_pos.x =  parent->get_width() - 1;

			if (mouse_pos.y < 0) 
				mouse_pos.y = 0;
			else if (mouse_pos.y > parent->get_height()) 
				mouse_pos.y = parent->get_height() - 1;

			send_ball_move(delta_x, delta_y);
			send_pointer_move(mouse_pos);
		}

		// Scrollwheel move
		int delta_z = (data[3] & 0x08) ? (data[3] & 0x0F)-16 : (data[3] & 0x0F);

		if (delta_z > 0)
		{
			while (delta_z != 0)
			{
				--delta_z;
				send_key_event(CL_MOUSE_WHEEL_DOWN, true);
				send_key_event(CL_MOUSE_WHEEL_DOWN, false);
			}
		} 
		else if (delta_z < 0)
		{
			while (delta_z != 0)
			{
				++delta_z;
				send_key_event(CL_MOUSE_WHEEL_UP, true);
				send_key_event(CL_MOUSE_WHEEL_UP, false);
			}
		}

		// Button event
		std::vector<bool> new_state(5);

		new_state[0] = ((data[0] &  1)>0);
		new_state[1] = ((data[0] &  2)>0);
		new_state[2] = ((data[0] &  4)>0);
		new_state[3] = ((data[3] & 16)>0);
		new_state[4] = ((data[3] & 32)>0);

		for (int i = 0; i < 5; ++i)
		{
			if (new_state[i] != buttons[i])
			{
				send_key_event(i<3?i:i+2, new_state[i]);
			}
		}

		buttons = new_state;
	}
}

void
CL_InputDevice_LinuxUSBMouse::send_pointer_move(const CL_Point& pos)
{
	CL_InputEvent event;
	event.type      = CL_InputEvent::pointer_moved;
	event.device    = CL_InputDevice(this);
	event.mouse_pos = pos;
	sig_pointer_move(event);
}

void
CL_InputDevice_LinuxUSBMouse::send_ball_move(int delta_x, int delta_y)
{
	CL_InputEvent event;
	event.type      = CL_InputEvent::ball_moved;
	event.device    = CL_InputDevice(this);
	event.mouse_pos = CL_Point(delta_x, delta_y);
	sig_ball_move(event);
}

void
CL_InputDevice_LinuxUSBMouse::send_key_event(int key, bool press)
{
	CL_InputEvent event;
	event.type      = press ? CL_InputEvent::pressed : CL_InputEvent::released;
	event.id        = key;
	event.device    = CL_InputDevice(this);
	event.mouse_pos = mouse_pos;

	if (press)
		sig_key_down(event);
	else
		sig_key_up(event);
}

// Local Variables: ***
// mode: clanlib ***
// End: ***
