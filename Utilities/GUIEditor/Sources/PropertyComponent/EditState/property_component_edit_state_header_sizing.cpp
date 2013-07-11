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
#include "property_component_edit_state_header_sizing.h"
#include "PropertyComponent/property_component.h"

PropertyComponentEditStateHeaderSizing::PropertyComponentEditStateHeaderSizing()
: start_column_width(0)
{
}

bool PropertyComponentEditStateHeaderSizing::on_input_pressed(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		start = e.mouse_pos;
		start_column_width = property_component->name_column_width;
		if (property_component->get_header_resize_grabber().contains(e.mouse_pos))
		{
			property_component->capture_mouse(true);
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

bool PropertyComponentEditStateHeaderSizing::on_input_released(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		property_component->capture_mouse(false);
		property_component->edit_state.set_pseudo_class(PropertyComponentEditState::state_none);
		return true;
	}
	else
	{
		return false;
	}
}

bool PropertyComponentEditStateHeaderSizing::on_input_doubleclick(const InputEvent &e)
{
	return false;
}

bool PropertyComponentEditStateHeaderSizing::on_input_pointer_moved(const InputEvent &e)
{
	int delta = e.mouse_pos.x - start.x;

	int name_column_width = start_column_width + delta;
	if(name_column_width < 55)
		name_column_width = 55; // Should prolly be configurable or calculated :)
	property_component->name_column_width = name_column_width;
	property_component->request_repaint();
	return true;
}
