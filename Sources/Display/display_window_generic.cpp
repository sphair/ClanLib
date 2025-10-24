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
#include "display_window_generic.h"
#include "API/Display/input_device.h"
#include "API/Display/input_context.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Generic construction:

CL_DisplayWindow_Generic::CL_DisplayWindow_Generic() : cursor_visible(true), ref_count(0)
{
}

CL_DisplayWindow_Generic::~CL_DisplayWindow_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Generic attributes:

CL_DisplayWindow *CL_DisplayWindow_Generic::current = 0;

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Generic operations:

void CL_DisplayWindow_Generic::add_ref()
{
	ref_count++;
}

void CL_DisplayWindow_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0)
	{
		cursor = CL_Sprite();
		delete this;
	}
}

void CL_DisplayWindow_Generic::do_flip(int interval)
{
	CL_InputDevice &mouse = get_ic()->get_mouse();
	if (cursor_visible && !cursor.is_null())
	{
		cursor.update();
		cursor.draw(float(mouse.get_x()), float(mouse.get_y()), get_gc());
	}
	flip(interval);
}

void CL_DisplayWindow_Generic::set_cursor(const CL_Sprite &sprite)
{
	cursor = sprite;
	cursor_changed();
	if (cursor_visible)
	{
		if (!cursor.is_null()) hide_system_cursor();
		else show_system_cursor();
	}
}

void CL_DisplayWindow_Generic::show_cursor()
{
	if (cursor.is_null()) show_system_cursor();
	cursor_visible = true;
}

void CL_DisplayWindow_Generic::hide_cursor()
{
	if (cursor.is_null()) hide_system_cursor();
	cursor_visible = false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindow_Generic implementation:
