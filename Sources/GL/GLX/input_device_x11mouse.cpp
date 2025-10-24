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
#include "input_device_x11mouse.h"

#include "../../GL/GLX/display_window_opengl.h"



/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Mouse construction:

CL_InputDevice_X11Mouse::CL_InputDevice_X11Mouse(CL_DisplayWindow_OpenGL *owner) :
	mouse_pos(-1,-1), time_at_last_press(0), last_press_id(-1), owner(owner)
{
	type = CL_InputDevice::mouse;
	
	for( int i=0; i<5; i++)
		key_states[i] = false;
	
	slot_xevent = owner->sig_xevent.connect(
		this, &CL_InputDevice_X11Mouse::on_xevent);
}

CL_InputDevice_X11Mouse::~CL_InputDevice_X11Mouse()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Mouse attributes:

int CL_InputDevice_X11Mouse::get_x() const
{
	return mouse_pos.x;
}

int CL_InputDevice_X11Mouse::get_y() const
{
	return mouse_pos.y;
}

bool CL_InputDevice_X11Mouse::get_keycode(int keycode) const
{
	if( keycode >= 0 && keycode < 5 )
		return key_states[keycode];
	return false;
}

float CL_InputDevice_X11Mouse::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_X11Mouse::get_name() const
{
	return "System Mouse";
}

std::string CL_InputDevice_X11Mouse::get_device_name() const
{
  return "x11:CorePointer";
}

int CL_InputDevice_X11Mouse::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_X11Mouse::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Mouse operations:

std::string CL_InputDevice_X11Mouse::get_key_name(int id) const
{
	switch (id)
	{
	case 0: return "Mouse left";
	case 1: return "Mouse right";
	case 2: return "Mouse middle";
	case 3: return "Mouse wheel up";
	case 4: return "Mouse wheel down";
	}

	char buf[256];
	sprintf(buf, "Mouse button %d", id);
	return buf;
}

void CL_InputDevice_X11Mouse::set_position(int x, int y)
{
	XWarpPointer(
		owner->get_display(),
		None,
		owner->get_window(),
		0, 0,
		0, 0,
		x, y);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_X11Mouse implementation:

void CL_InputDevice_X11Mouse::on_xevent(XEvent &event)
{
	// Only handle mouse events.
	if (event.type != ButtonPress
		&& event.type != ButtonRelease
		&& event.type != MotionNotify )
		return;
	
	if (event.type == ButtonPress || event.type == ButtonRelease )
		received_mouse_input(event);
	else
		received_mouse_move(event);
}

void CL_InputDevice_X11Mouse::received_mouse_input(XEvent &e)
{
	XButtonEvent event = (XButtonEvent&)e;
	
	int id = 0;
	bool down = false;
	int repeat_count = 0;
	
	mouse_pos.x = event.x;
	mouse_pos.y = event.y;

	Time time_change = event.time - time_at_last_press;
	
	switch(event.button)
	{
		case 1: id = 0; break; // left
		case 3: id = 1; break; // right
		case 2: id = 2; break; // middle
		default: id = event.button-1;
	}
	
	if (event.type == ButtonPress)
	{
		time_at_last_press = event.time;
		last_press_id = id;
	}
	
	CL_InputEvent::Type event_type = CL_InputEvent::released;
	
	if (event.type == ButtonPress)
	{
		event_type = CL_InputEvent::pressed;
		down = true;
		if(time_change < 500)
			repeat_count = 2;
	}
	
	if( id >= 0 && id < 5 )
		key_states[id] = down;

	// Prepare event to be emitted:
	CL_InputEvent key;
	key.id        = id;
	key.type      = event_type;
	key.device    = owner->mouse;
	key.mouse_pos = mouse_pos;
	key.repeat_count = repeat_count;

	// Emit message:
	if(repeat_count == 2)
	{
		key.repeat_count = 1; //we need to issue a normal mouse click too, for compatibility with older clanlibs we
		//ask it to pretend it's non-repeater, in case they are using the same callback for double clicks too
		owner->mouse.sig_key_down().call(key);

		key.repeat_count = repeat_count;
		owner->mouse.sig_key_dblclk().call(key);
	}
	else if (down)
	{
		owner->mouse.sig_key_down().call(key);
	}
	else
		owner->mouse.sig_key_up().call(key);
}

void CL_InputDevice_X11Mouse::received_mouse_move(XEvent &e)
{
	XMotionEvent event = (XMotionEvent&)e;
	
	mouse_pos.x = event.x;
	mouse_pos.y = event.y;
	
	// Prepare event to be emitted:
	CL_InputEvent key;
	key.type      = CL_InputEvent::moved;
	key.device    = owner->mouse;
	key.mouse_pos = mouse_pos;
	
	// Fire off signal
	owner->mouse.sig_move().call(key);
}
