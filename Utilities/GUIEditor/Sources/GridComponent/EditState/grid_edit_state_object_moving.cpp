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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "grid_edit_state_object_moving.h"
#include "GridComponent/grid_component.h"
#include "GridComponent/grid_object.h"
#include "MainWindow/gui_editor_window.h"

GridEditStateObjectMoving::GridEditStateObjectMoving()
: object(0)
{
}

bool GridEditStateObjectMoving::on_input_pressed(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		object = grid->find_object_at(e.mouse_pos);
		if (object)
		{
			start = e.mouse_pos;
			start_geometry = object->get_geometry();
			if (!grid->main_window->get_selection()->is_selected(object))
			{
				grid->main_window->get_selection()->clear();
				grid->main_window->get_selection()->add_object(object);
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
	else if(e.id == keycode_left || e.id == keycode_right || e.id == keycode_up || e.id == keycode_down)
	{
		Point delta;

		if(e.id == keycode_left)
			delta.x = -1;
		else if(e.id == keycode_right)
			delta.x = 1;
		else if(e.id == keycode_up)
			delta.y = -1;
		else if(e.id == keycode_down)
			delta.y = 1;

		std::vector<GridObject*> selection = grid->main_window->get_selection()->get_selection();
		for (size_t i = 0; i < selection.size(); i++)
		{
			Rect geometry = selection[i]->get_geometry().translate(delta);
			selection[i]->set_geometry(geometry);
		}

		grid->request_repaint();

		return true;
	}
	else 
	{
		return false;
	}
}

bool GridEditStateObjectMoving::on_input_released(const InputEvent &e)
{
	if (e.id == mouse_left && object)
	{
		object = 0;
		grid->capture_mouse(false);
		grid->edit_state.set_pseudo_class(GridEditState::state_none);
		grid->main_window->get_selection()->sig_selection_changed().invoke();
		return true;
	}
	else if(e.id == keycode_left || e.id == keycode_right || e.id == keycode_up || e.id == keycode_down)
	{
		grid->edit_state.set_pseudo_class(GridEditState::state_none);
		grid->main_window->get_selection()->sig_selection_changed().invoke();

		return true;
	}
	else
	{
		return false;
	}
}

bool GridEditStateObjectMoving::on_input_doubleclick(const InputEvent &e)
{
	return false;
}

bool GridEditStateObjectMoving::on_input_pointer_moved(const InputEvent &e)
{
	if(object)
	{
		bool perform_snap = e.alt == false;
		move_to(e.mouse_pos, perform_snap);
		return true;
	}
	else
	{
		return false;
	}
}

void GridEditStateObjectMoving::move_to(const Point &mouse_pos, bool perform_snap)
{
	Vec2i delta = mouse_pos - start;

	Rect source_rect = start_geometry;
	source_rect.translate(delta);

	if(perform_snap)
		source_rect.translate(grid->snap(object, object->get_snaplines(), source_rect));

	delta.x = source_rect.left - object->get_geometry().left;
	delta.y = source_rect.top - object->get_geometry().top;

	std::vector<GridObject *> selection = grid->main_window->get_selection()->get_selection();
	for (size_t i = 0; i < selection.size(); i++)
	{
		Rect geometry = selection[i]->get_geometry();
		geometry.translate(delta.x, delta.y);
		selection[i]->set_geometry(geometry);
	}
	grid->request_repaint();
}
