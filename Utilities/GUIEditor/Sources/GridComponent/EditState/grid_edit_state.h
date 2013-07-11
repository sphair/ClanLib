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

#pragma once

#include "grid_edit_state_none.h"
#include "grid_edit_state_boundary_sizing.h"
#include "grid_edit_state_net_selecting.h"
#include "grid_edit_state_object_moving.h"
#include "grid_edit_state_object_sizing.h"
#include "grid_edit_state_create_tool.h"

class GridEditState
{
public:
	GridEditState();

	enum State
	{
		state_none,
		state_net_select,
		state_boundary_size,
		state_object_move,
		state_object_size,
		state_create_tool
	};
	
	enum { num_edit_states = state_create_tool+1 };

	void set_grid_component(GridComponent *grid);
	void set_pseudo_class(State state);

	bool on_input_pressed(const InputEvent &input_event);
	bool on_input_released(const InputEvent &input_event);
	bool on_input_doubleclick(const InputEvent &input_event);
	bool on_input_pointer_moved(const InputEvent &input_event);

private:
	State state;
	GridEditStateNone edit_state_none;
	GridEditStateBoundarySizing edit_state_boundary_size;
	GridEditStateNetSelecting edit_state_net_select;
	GridEditStateObjectMoving edit_state_object_move;
	GridEditStateObjectSizing edit_state_object_size;
	GridEditStateCreateTool edit_state_create_tool;
	GridEditStateHandler *edit_state_handlers[num_edit_states];
};
