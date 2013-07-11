/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#include "precomp.h"
#include "grid_edit_state_boundary_sizing.h"
#include "GridComponent/grid_component.h"

GridEditStateBoundarySizing::GridEditStateBoundarySizing()
: dir(0, 0)
{
}

bool GridEditStateBoundarySizing::on_input_pressed(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		start = e.mouse_pos;
		dir = Vec2i(0,0);
		if (grid->get_boundary_grabber_se().contains(e.mouse_pos))
			dir = Vec2i(1,1);
		else if (grid->get_boundary_grabber_e().contains(e.mouse_pos))
			dir = Vec2i(1,0);
		else if (grid->get_boundary_grabber_s().contains(e.mouse_pos))
			dir = Vec2i(0,1);

		if (dir != Vec2i(0,0))
		{
			grid->capture_mouse(true);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool GridEditStateBoundarySizing::on_input_released(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		grid->capture_mouse(false);
		grid->edit_state.set_pseudo_class(GridEditState::state_none);
		return true;
	}
	else
	{
		return false;
	}
}

bool GridEditStateBoundarySizing::on_input_doubleclick(const InputEvent &e)
{
	return false;
}

bool GridEditStateBoundarySizing::on_input_pointer_moved(const InputEvent &e)
{
	Size boundary_size = grid->boundary;

	if (dir.x > 0)
		boundary_size.width = clan::max(e.mouse_pos.x, 10);

	if (dir.y > 0)
		boundary_size.height = clan::max(e.mouse_pos.y, 10);

	if ( (dir.x > 0) || (dir.y > 0) )
	{
		grid->set_boundary_size(boundary_size);
	}
	return true;
}
