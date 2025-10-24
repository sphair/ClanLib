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

#include "Display/precomp.h"
#include "API/Core/Text/string_format.h"
#include "input_device_provider_x11mouse.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "x11_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Mouse construction:

CL_InputDeviceProvider_X11Mouse::CL_InputDeviceProvider_X11Mouse(CL_X11Window *window)
: sig_provider_event(0), window(window), mouse_pos(-1,-1)
{
	for (int i=0; i<32; i++) key_states[i] = false;
}

CL_InputDeviceProvider_X11Mouse::~CL_InputDeviceProvider_X11Mouse()
{
	dispose();
}

void CL_InputDeviceProvider_X11Mouse::on_dispose()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Mouse attributes:

int CL_InputDeviceProvider_X11Mouse::get_x() const
{
	Window root_return;
	Window child_return;
	int root_x_return;
	int root_y_return;
	int win_x_return=0;
	int win_y_return=0;
	unsigned int mask_return;

	XQueryPointer(window->get_display(), window->get_window(), &root_return, &child_return,
		&root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);
	return win_x_return;

}

int CL_InputDeviceProvider_X11Mouse::get_y() const
{
	Window root_return;
	Window child_return;
	int root_x_return;
	int root_y_return;
	int win_x_return=0;
	int win_y_return=0;
	unsigned int mask_return;

	XQueryPointer(window->get_display(), window->get_window(), &root_return, &child_return,
		&root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);
	return win_y_return;
}

CL_Point CL_InputDeviceProvider_X11Mouse::get_position() const
{
	Window root_return;
	Window child_return;
	int root_x_return;
	int root_y_return;
	int win_x_return=0;
	int win_y_return=0;
	unsigned int mask_return;

	XQueryPointer(window->get_display(), window->get_window(), &root_return, &child_return,
		&root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);
	return (CL_Point(win_x_return, win_y_return));
}

bool CL_InputDeviceProvider_X11Mouse::get_keycode(int keycode) const
{
	if (keycode < 0 || keycode >= 32) return false;
	return key_states[keycode];
}

CL_String CL_InputDeviceProvider_X11Mouse::get_key_name(int id) const
{
	switch (id)
	{
	case 0: return "Mouse left";
	case 1: return "Mouse right";
	case 2: return "Mouse middle";
	case 3: return "Mouse wheel up";
	case 4: return "Mouse wheel down";
	}

	return cl_format("Mouse button %1", id);
}

float CL_InputDeviceProvider_X11Mouse::get_axis(int index) const
{
	return 0.0f;
}

CL_String CL_InputDeviceProvider_X11Mouse::get_name() const
{
	return "System Mouse";
}

CL_String CL_InputDeviceProvider_X11Mouse::get_device_name() const
{
	return "System Mouse";
}

int CL_InputDeviceProvider_X11Mouse::get_axis_count() const
{
	return 0;
}

int CL_InputDeviceProvider_X11Mouse::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Mouse operations:

void CL_InputDeviceProvider_X11Mouse::set_position(int x, int y)
{
	XWarpPointer(window->get_display(), None, window->get_window(), 0,0, 0,0, x,y);
}


void CL_InputDeviceProvider_X11Mouse::received_mouse_input(XButtonEvent &event)
{

	int id;

	switch(event.button)
	{
		case 1: id = CL_MOUSE_LEFT; break;	// Left
		case 3: id = CL_MOUSE_RIGHT; break;	// Right
		case 2: id = CL_MOUSE_MIDDLE; break;	// Middle
		case 4: id = CL_MOUSE_WHEEL_UP; break;	// Scroll up
		case 5: id = CL_MOUSE_WHEEL_DOWN; break;	// Scroll down
		case 6: id = CL_MOUSE_XBUTTON1; break;
		case 7: id = CL_MOUSE_XBUTTON2; break;
		default: return;	// Unknown press
	}
	mouse_pos.x = event.x;
	mouse_pos.y = event.y;

	// Prepare event to be emitted:
	CL_InputEvent key;
	key.mouse_pos = mouse_pos;
	key.id = id;
	if (event.type == ButtonPress)
	{
		key.type = CL_InputEvent::pressed;
		key_states[id] = 1;
	}
	else
	{
		key.type = CL_InputEvent::released;
		key_states[id] = 0;
	}

	window->get_keyboard_modifiers(key.shift, key.alt, key.ctrl);

	// Emit message:
	sig_provider_event->invoke(key);
}

void CL_InputDeviceProvider_X11Mouse::received_mouse_move(XMotionEvent &event)
{
	// Fetch coordinates
	int x = event.x;
	int y = event.y;

	if(mouse_pos.x != x || mouse_pos.y != y)
	{
		mouse_pos.x = x;
		mouse_pos.y = y;

		// Prepare event to be emitted:
		CL_InputEvent key;
		key.type = CL_InputEvent::pointer_moved;
		key.mouse_pos = mouse_pos;
		window->get_keyboard_modifiers(key.shift, key.alt, key.ctrl);

		// Fire off signal
		sig_provider_event->invoke(key);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_X11Mouse implementation:
