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

#include "Display/display_precomp.h"
#include "API/Display/input_event.h"
#include "API/Core/System/clanstring.h"
#include "input_device_win32mouse.h"
#include "display_window_win32.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Mouse construction:

CL_InputDevice_Win32Mouse::CL_InputDevice_Win32Mouse(CL_DisplayWindow_Win32 *window) : window(window)
{
	type = CL_InputDevice::mouse;
	for (int i=0; i<32; i++) key_states[i] = false;
	slots.connect(sig_key_down, this, &CL_InputDevice_Win32Mouse::on_key_down);
	slots.connect(sig_key_up, this, &CL_InputDevice_Win32Mouse::on_key_up);
}

CL_InputDevice_Win32Mouse::~CL_InputDevice_Win32Mouse()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Mouse attributes:

int CL_InputDevice_Win32Mouse::get_x() const
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	BOOL res = ScreenToClient(window->get_hwnd(), &cursor_pos);
	if (res == FALSE) return 0;

	return cursor_pos.x;
}

int CL_InputDevice_Win32Mouse::get_y() const
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	BOOL res = ScreenToClient(window->get_hwnd(), &cursor_pos);
	if (res == FALSE) return 0;

	return cursor_pos.y;
}

bool CL_InputDevice_Win32Mouse::get_keycode(int keycode) const
{
	if (keycode < 0 || keycode >= 32) return false;
	return key_states[keycode];
}

std::string CL_InputDevice_Win32Mouse::get_key_name(int id) const
{
	switch (id)
	{
	case 0: return "Mouse left";
	case 1: return "Mouse right";
	case 2: return "Mouse middle";
	case 3: return "Mouse wheel up";
	case 4: return "Mouse wheel down";
	}

	return CL_String::format("Mouse button %1", id);
}

float CL_InputDevice_Win32Mouse::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_Win32Mouse::get_name() const
{
	return "System Mouse";
}

int CL_InputDevice_Win32Mouse::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_Win32Mouse::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Mouse operations:

void CL_InputDevice_Win32Mouse::set_position(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	
	ClientToScreen(window->get_hwnd(), &pt);
	SetCursorPos(pt.x, pt.y);
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_Win32Mouse implementation:

void CL_InputDevice_Win32Mouse::on_key_down(const CL_InputEvent &event)
{
	key_states[event.id] = true;
}

void CL_InputDevice_Win32Mouse::on_key_up(const CL_InputEvent &event)
{
	key_states[event.id] = false;
}
