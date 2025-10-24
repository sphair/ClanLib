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
#include "API/Display/mouse.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/input_context.h"
#include "API/Display/input_device.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Mouse construction:

/////////////////////////////////////////////////////////////////////////////
// CL_Mouse attributes:

#define get_ic() CL_Display::get_current_window()->get_ic()

int CL_Mouse::get_device_count()
{
	return get_ic()->get_mouse_count();
}

CL_InputDevice &CL_Mouse::get_device(int mouse)
{
	return get_ic()->get_mouse(mouse);
}

int CL_Mouse::get_x(int mouse)
{
	return get_ic()->get_mouse(mouse).get_x();
}
	
int CL_Mouse::get_y(int mouse)
{
	return get_ic()->get_mouse(mouse).get_y();
}
	
bool CL_Mouse::get_keycode(int keycode, int mouse)
{
	return get_ic()->get_mouse(mouse).get_keycode(keycode);
}

std::string CL_Mouse::get_key_name(int id)
{
	return get_ic()->get_mouse().get_key_name(id);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mouse operations:

void CL_Mouse::set_position(int x, int y, int mouse)
{
	get_ic()->get_mouse(mouse).set_position(x, y);
}

void CL_Mouse::show()
{
	CL_Display::get_current_window()->show_cursor();
}

void CL_Mouse::hide()
{
	CL_Display::get_current_window()->hide_cursor();
}

void CL_Mouse::set_cursor(const CL_Sprite &sprite)
{
	CL_Display::get_current_window()->set_cursor(sprite);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mouse signals:

CL_Signal_v1<const CL_InputEvent &> &CL_Mouse::sig_key_down(int mouse)
{
	return get_ic()->get_mouse(mouse).sig_key_down();
}

CL_Signal_v1<const CL_InputEvent &> &CL_Mouse::sig_key_up(int mouse)
{
	return get_ic()->get_mouse(mouse).sig_key_up();
}

CL_Signal_v1<const CL_InputEvent &> &CL_Mouse::sig_key_dblclk(int mouse)
{
	return get_ic()->get_mouse(mouse).sig_key_dblclk();
}

CL_Signal_v1<const CL_InputEvent &> &CL_Mouse::sig_move(int mouse)
{
	return get_ic()->get_mouse(mouse).sig_move();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mouse implementation:
