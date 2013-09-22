/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "property_component_edit_state_none.h"
#include "PropertyComponent/property_component.h"

PropertyComponentEditStateNone::PropertyComponentEditStateNone()
{
}

bool PropertyComponentEditStateNone::on_input_pressed(const clan::InputEvent &e)
{
	if (e.type == clan::InputEvent::pressed && e.id == clan::mouse_left)
	{
		int item = property_component->hit_test(e.mouse_pos);
		if (item != -1)
			property_component->activate_item(item);
		return true;
	}
	return false;
}

bool PropertyComponentEditStateNone::on_input_released(const clan::InputEvent &e)
{
	return false;
}

bool PropertyComponentEditStateNone::on_input_doubleclick(const clan::InputEvent &e)
{
	return false;
}

bool PropertyComponentEditStateNone::on_input_pointer_moved(const clan::InputEvent &e)
{
	clan::StandardCursor cursor = clan::cursor_arrow;
	if (property_component->get_header_resize_grabber().contains(e.mouse_pos))
		cursor = clan::cursor_size_we;

	property_component->set_cursor(cursor);
	return true;
}
