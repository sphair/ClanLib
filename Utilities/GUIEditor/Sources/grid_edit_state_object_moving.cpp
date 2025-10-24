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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "grid_edit_state_object_moving.h"
#include "grid_component.h"
#include "holder_component.h"
#include "main_window.h"

GridEditStateObjectMoving::GridEditStateObjectMoving()
: holder(0)
{
}

bool GridEditStateObjectMoving::on_input_pressed(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT)
	{
		holder = grid->find_holder_at(e.mouse_pos);
		if (holder)
		{
			start = e.mouse_pos;
			start_geometry = holder->get_geometry();
			if (!grid->main_window->get_selection()->is_selected(holder))
			{
				grid->main_window->get_selection()->clear();
				grid->main_window->get_selection()->add_holder(holder);
			}
			grid->capture_mouse(true);
			grid->request_repaint();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(e.id == CL_KEY_LEFT || e.id == CL_KEY_RIGHT || e.id == CL_KEY_UP || e.id == CL_KEY_DOWN)
	{
		// TODO: Move stuffz
		return true;
	}
	else 
	{
		return false;
	}
}

bool GridEditStateObjectMoving::on_input_released(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT)
	{
		bool perform_snap = e.alt == false;
		move_to(e.mouse_pos, perform_snap);
		holder = 0;
		grid->capture_mouse(false);
		grid->edit_state.set_state(GridEditState::state_none);
		grid->main_window->get_selection()->sig_selection_changed().invoke();
		return true;
	}
	else
	{
		return false;
	}
}

bool GridEditStateObjectMoving::on_input_doubleclick(const CL_InputEvent &e)
{
	return false;
}

bool GridEditStateObjectMoving::on_input_pointer_moved(const CL_InputEvent &e)
{
	bool perform_snap = e.alt == false;
	move_to(e.mouse_pos, perform_snap);
	return true;
}

void GridEditStateObjectMoving::move_to(const CL_Point &mouse_pos, bool perform_snap)
{
	CL_Vec2i delta = mouse_pos - start;

	CL_Rect source_rect = start_geometry;
	source_rect.translate(delta);

	if(perform_snap)
		source_rect.translate(grid->snap(holder, holder->get_snaplines(), source_rect));

	delta.x = source_rect.left - holder->get_geometry().left;
	delta.y = source_rect.top - holder->get_geometry().top;

	std::vector<HolderComponent *> selection = grid->main_window->get_selection()->get_selection();
	for (size_t i = 0; i < selection.size(); i++)
	{
		CL_Rect geometry = selection[i]->get_geometry();
		geometry.translate(delta.x, delta.y);
		selection[i]->set_geometry(geometry);
	}
	grid->request_repaint();
}
