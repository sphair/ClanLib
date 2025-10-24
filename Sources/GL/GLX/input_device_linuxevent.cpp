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

#include <assert.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <cstdio>

#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/input_event.h"
#include "input_device_linuxevent.h"

// FIXME: These three should be in linux/input.h
#define MSC_RAW			0x03
#define MSC_SCAN		0x04
#define MSC_MAX			0x07


#ifndef EV_SYN
#define EV_SYN 0
#endif

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

static char *events[EV_MAX + 1] = { NULL };
static char **names[EV_MAX + 1] = { NULL }; 

CL_InputDevice_LinuxEvent::CL_InputDevice_LinuxEvent(CL_DisplayWindow_Generic* parent_, 
																	  const std::string& device_)
  : parent(parent_),
	 device(device_) 
{
	fd = open(device.c_str(), O_RDONLY | O_NONBLOCK);

	if (fd == -1)
	{
		throw CL_Error(strerror(errno));
	}

	if (ioctl(fd, EVIOCGVERSION, &version)) 
	{
		throw CL_Error("CL_InputDevice_LinuxEvent: can't get version");
	}

#if 0
	if (0)
	{ // FIXME: Some versions of linux don't have these structs, use arrays there
		struct input_id id;
		ioctl(fd, EVIOCGID, &id);
		printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
				 id.bustype, id.vendor, id.product, id.vendor);
	}
#endif

	{  // Get the human readable name
		char c_name[256] = "Unknown";
		ioctl(fd, EVIOCGNAME(sizeof(c_name)), c_name);
		name = c_name;
	}

	{
		unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
		memset(bit, 0, sizeof(bit));
		ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

		for (int i = 0; i < EV_MAX; i++)
		{
			if (test_bit(i, bit[0])) 
			{
				//printf("  Event type %d (%s)\n", i, events[i] ? events[i] : "?");
				
				if (!i) continue;

				ioctl(fd, EVIOCGBIT(i, KEY_MAX), bit[i]);
				for (int j = 0; j < KEY_MAX; j++) 
				{
					if (test_bit(j, bit[i])) 
					{
						if (i == EV_KEY) 
						{ // Found something that might be a button
							buttons.push_back(Button(j));
						} 
						else if (i == EV_ABS) 
						{ // Found something that might be an axis
#if 0
							// FIXME: Some Linuxes don't have these struct
							struct input_absinfo absinfo;
							ioctl(fd, EVIOCGABS(j), &absinfo);
							// FIXME: we are ignoring absinfo.fuzz and absinfo.flat, not sure what they are good for
							axes.push_back(Axis(j, absinfo.minimum, absinfo.maximum, absinfo.value));
#else
							int absinfo[5];
							ioctl(fd, EVIOCGABS(j), &absinfo);
							// FIXME: we are ignoring absinfo.fuzz and absinfo.flat, not sure what they are good for
							axes.push_back(Axis(j, absinfo[1], absinfo[2], absinfo[0]));
#endif
						}
						else if (i == EV_REL) 
						{ // Found something mouse/ball like
							balls.push_back(Ball(j));
						}
					}
				}
			}
		}
	}
}

int
CL_InputDevice_LinuxEvent::get_ball_index_by_code(int code)
{
	for(std::vector<Ball>::size_type i = 0; i != balls.size(); ++i)
	{
		if (balls[i].code == code)
		{
			return i;
		}
	}
	return -1;
}

int
CL_InputDevice_LinuxEvent::get_button_index_by_code(int code)
{
	for(std::vector<Button>::size_type i = 0; i != buttons.size(); ++i)
	{
		if (buttons[i].code == code)
		{
			return i;
		}
	}
	return -1;
}

int
CL_InputDevice_LinuxEvent::get_axis_index_by_code(int code)
{
	for(std::vector<Axis>::size_type i = 0; i != axes.size(); ++i)
	{
		if (axes[i].code == code)
		{
			return i;
		}
	}
	return -1;
}

void
CL_InputDevice_LinuxEvent::keep_alive()
{
	struct input_event ev[64];
	
	int rd = read(fd, ev, sizeof(struct input_event) * 64);

	if (rd >= (int) sizeof(struct input_event))
	{
		for (int i = 0; i < rd / (int)sizeof(struct input_event); ++i)
		{
			switch (ev[i].type)
			{
			case EV_ABS:
			{
				int axis_index = get_axis_index_by_code(ev[i].code);
				if (axis_index != -1)
				{
					axes[axis_index].pos = ev[i].value;

					CL_InputEvent e; 
					
					e.device   = CL_InputDevice(this);
					e.type     = CL_InputEvent::axis_moved;
					e.id       = axis_index;
					e.axis_pos = axes[axis_index].get_posf();
					e.repeat_count = 0;

					sig_axis_move(e);
				}
			}
			break;

			case EV_REL:
			{
				int ball_index = get_ball_index_by_code(ev[i].code);

				balls[ball_index].pos += ev[i].value;

				CL_InputEvent e; 
				
				e.device       = CL_InputDevice(this);
				e.type         = CL_InputEvent::ball_moved;
				e.id           = ball_index;
				e.mouse_pos.x  = ev[i].value;
				e.mouse_pos.y  = 0;
				e.axis_pos     = 0;
				e.repeat_count = 0;

				sig_ball_move(e);
			}
			break;

			case EV_KEY:
			{
				int button_index = get_button_index_by_code(ev[i].code);

				buttons[button_index].pressed = ev[i].value;

				CL_InputEvent e; 
			
				e.device = CL_InputDevice(this);
				e.id     = button_index;
				e.repeat_count = 0;

				if (ev[i].value)
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
			break;

			default:
				if (0) 
				{
					if (ev[i].type == EV_SYN) 
					{
						printf("Event: time %ld.%06ld, -------------- %s ------------\n",
								 ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code ? "Config Sync" : "Report Sync" );
					}
					else if (ev[i].type == EV_MSC && (ev[i].code == MSC_RAW || ev[i].code == MSC_SCAN)) 
					{
						printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %02x\n",
								 ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
								 events[ev[i].type] ? events[ev[i].type] : "?",
								 ev[i].code,
								 names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
								 ev[i].value);
					} 
					else 
					{
						printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %d\n",
								 ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
								 events[ev[i].type] ? events[ev[i].type] : "?",
								 ev[i].code,
								 names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
								 ev[i].value);
					}	
				}
				break;
			}
		}
	}
}

CL_InputDevice_LinuxEvent::~CL_InputDevice_LinuxEvent()
{
	close(fd);
}

std::string
CL_InputDevice_LinuxEvent::get_key_name(int num) const
{
	// FIXME: copy enum2string code from evtest here
	return CL_String::from_int(num);
}

float
CL_InputDevice_LinuxEvent::get_axis(int num)   const
{
	assert(num >= 0 && num < int(axes.size()));
	return axes[num].pos;
}

bool
CL_InputDevice_LinuxEvent::get_keycode(int num) const
{
	if (num >= 0 && num < int(buttons.size()) )
	{
		return buttons[num].pressed;
	} 
	else 
	{
		return false;
	}
}

int
CL_InputDevice_LinuxEvent::get_axis_count() const
{
	return axes.size();
}

int
CL_InputDevice_LinuxEvent::get_button_count() const
{
	return buttons.size();
}

std::string
CL_InputDevice_LinuxEvent::get_name() const 
{
	return name; 
}

std::string
CL_InputDevice_LinuxEvent::get_device_name() const 
{ 
	return "linuxevent:" + device; 
}

// Local Variables: ***
// mode: clanlib ***
// End: ***
