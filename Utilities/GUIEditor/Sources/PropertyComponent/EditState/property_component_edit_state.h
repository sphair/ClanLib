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

#include "property_component_edit_state_none.h"
#include "property_component_edit_state_header_sizing.h"
#include "property_component_edit_state_node_opener.h"
#include "property_component_edit_state_revert_button.h"

class PropertyComponentEditState
{
public:
	PropertyComponentEditState();

	enum State
	{
		state_none,
		state_header_size,
		state_node_opener,
		state_revert_button
	};
	
	enum { num_edit_states = state_revert_button+1 };

	void set_component(PropertyComponent *property_component);
	void set_pseudo_class(State state);

	bool on_input_pressed(const InputEvent &input_event);
	bool on_input_released(const InputEvent &input_event);
	bool on_input_doubleclick(const InputEvent &input_event);
	bool on_input_pointer_moved(const InputEvent &input_event);

private:
	State state;
	PropertyComponentEditStateNone edit_state_none;
	PropertyComponentEditStateHeaderSizing edit_state_header_size;
	PropertyComponentEditStateNodeOpener edit_state_node_opener;
	PropertyComponentEditStateRevertButton edit_state_revert_button;
	PropertyComponentEditStateHandler *edit_state_handlers[num_edit_states];
};
