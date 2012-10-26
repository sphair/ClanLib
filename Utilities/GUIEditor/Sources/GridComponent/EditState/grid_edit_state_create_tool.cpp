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
#include "grid_edit_state_create_tool.h"
#include "GridComponent/grid_component.h"
#include "MainWindow/gui_editor_window.h"

GridEditStateCreateTool::GridEditStateCreateTool()
{
}

bool GridEditStateCreateTool::on_input_pressed(const InputEvent &e)
{
	if (e.id == MOUSE_LEFT)
	{
		int selected_tool = grid->main_window->get_selected_tool();
		if (selected_tool != 1337) // OK, maybe pick a less silly value for select tool?
		{
			grid->capture_mouse(true);
			grid->main_window->get_selection()->clear();
			GridObject *object = grid->on_add_component(selected_tool, e.mouse_pos);
			grid->main_window->get_selection()->add_object(object);
			grid->main_window->use_select_tool();
			grid->request_repaint();
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

bool GridEditStateCreateTool::on_input_released(const InputEvent &e)
{
	if (e.id == MOUSE_LEFT)
	{
		grid->capture_mouse(false);
		grid->edit_state.set_state(GridEditState::state_none);
		return true;
	}
	else
	{
		return false;
	}
}

bool GridEditStateCreateTool::on_input_doubleclick(const InputEvent &e)
{
	return false;
}

bool GridEditStateCreateTool::on_input_pointer_moved(const InputEvent &e)
{
	return true;
}
