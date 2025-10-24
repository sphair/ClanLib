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
*/

#include "precomp.h"
#include "grid_edit_state_none.h"
#include "grid_component.h"
#include "main_window.h"
#include "holder_component.h"

GridEditStateNone::GridEditStateNone()
{
}

bool GridEditStateNone::on_input_pressed(const CL_InputEvent &e)
{
	return false;
}

bool GridEditStateNone::on_input_released(const CL_InputEvent &e)
{
	return false;
}

bool GridEditStateNone::on_input_doubleclick(const CL_InputEvent &e)
{
	return false;
}

bool GridEditStateNone::on_input_pointer_moved(const CL_InputEvent &e)
{
	CL_StandardCursor cursor = cl_cursor_arrow;
	if (grid->get_boundary_grabber_se().contains(e.mouse_pos))
		cursor = cl_cursor_size_nwse;
	else if (grid->get_boundary_grabber_s().contains(e.mouse_pos))
		cursor = cl_cursor_size_ns;
	else if (grid->get_boundary_grabber_e().contains(e.mouse_pos))
		cursor = cl_cursor_size_we;

	HolderComponent *holder = grid->find_holder_at(e.mouse_pos);
	if (holder)
		cursor = cl_cursor_size_all;

	std::vector<HolderComponent*> selection = grid->main_window->get_selection()->get_selection();
	for (size_t i = 0; i < selection.size(); i++)
	{
		HolderComponent *h = selection[i];
		CL_Point h_mouse_pos = grid->grid_to_holder_coords(h, e.mouse_pos);

		if (h->get_grabber_e().contains(h_mouse_pos))
			cursor = cl_cursor_size_we;
		else if (h->get_grabber_se().contains(h_mouse_pos))
			cursor = cl_cursor_size_nwse;
		else if (h->get_grabber_s().contains(h_mouse_pos))
			cursor = cl_cursor_size_ns;
		else if (h->get_grabber_sw().contains(h_mouse_pos))
			cursor = cl_cursor_size_nesw;
		else if (h->get_grabber_w().contains(h_mouse_pos))
			cursor = cl_cursor_size_we;
		else if (h->get_grabber_nw().contains(h_mouse_pos))
			cursor = cl_cursor_size_nwse;
		else if (h->get_grabber_n().contains(h_mouse_pos))
			cursor = cl_cursor_size_ns;
		else if (h->get_grabber_ne().contains(h_mouse_pos))
			cursor = cl_cursor_size_nesw;
	}

	grid->set_cursor(cursor);

	return true;
}
