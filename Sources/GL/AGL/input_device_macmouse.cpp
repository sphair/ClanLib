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
#include "input_device_macmouse.h"

#include "../../GL/AGL/display_window_opengl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacMouse construction:

CL_InputDevice_MacMouse::CL_InputDevice_MacMouse(CL_DisplayWindow_OpenGL *owner) :
	owner(owner)
{
	type = CL_InputDevice::mouse;
	
	for( int i=0; i<5; i++)
		owner->mouse_states[i] = false;
}

CL_InputDevice_MacMouse::~CL_InputDevice_MacMouse()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacMouse attributes:

int CL_InputDevice_MacMouse::get_x() const
{
	return owner->mouse_pos.x;
}

int CL_InputDevice_MacMouse::get_y() const
{
	return owner->mouse_pos.y;
}

bool CL_InputDevice_MacMouse::get_keycode(int keycode) const
{
	if( keycode >= 0 && keycode < 5 )
		return owner->mouse_states[keycode];
	return false;
}

float CL_InputDevice_MacMouse::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_MacMouse::get_name() const
{
	return "System Mouse";
}

int CL_InputDevice_MacMouse::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_MacMouse::get_button_count() const
{
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacMouse operations:

std::string CL_InputDevice_MacMouse::get_key_name(int id) const
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

void CL_InputDevice_MacMouse::set_position(int x, int y)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_MacMouse implementation:
