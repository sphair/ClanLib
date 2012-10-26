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
*/

#include "precomp.h"
#include "toolbar.h"

Toolbar::Toolbar(const Rect &position, GUIComponent* owner)
: GUIComponent(owner, get_toplevel_description(position)), selected_index(-1), clicked_index(-1)
{
	set_type_name("toolbar");

	func_render().set(this, &Toolbar::on_render);
	func_style_changed().set(this, &Toolbar::on_style_changed);
	func_process_message().set(this, &Toolbar::on_process_message);

	create_parts();
}

Toolbar::~Toolbar()
{
	items.clear();
}

GUITopLevelDescription Toolbar::get_toplevel_description(const Rect &position)
{
	GUITopLevelDescription desc;
	desc.set_position(position, false);
	return desc;
}

Size Toolbar::get_preferred_size() const
{
	return part_component.get_preferred_size();
}

void Toolbar::on_process_message(GUIMessage &msg)
{
	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input = msg;
		InputEvent input_event = input.get_event();
		if (input_event.type == InputEvent::pointer_moved)
			on_mouse_move(input_event);
		else if (input_event.type == InputEvent::pressed && input_event.id == mouse_left)
			on_mouse_lbutton_down(input_event);
		else if (input_event.type == InputEvent::released && input_event.id == mouse_left)
			on_mouse_lbutton_up(input_event);
	}
}

void Toolbar::on_mouse_move(InputEvent &input_event)
{
	int pos_x = start_x;
	int pos_y = start_y;
	for(unsigned int i = 0; i < items.size(); ++i)
	{
		Rect icon_rect(pos_x, pos_y, pos_x + items[i].icon.get_width(), pos_y + items[i].icon.get_height());
		if(icon_rect.contains(input_event.mouse_pos))
		{
			selected_index = i;
			request_repaint();
			return;
		}

		pos_x += offset_x;
	}

	clicked_index = -1;
	selected_index = -1;
	request_repaint();
}

void Toolbar::on_mouse_lbutton_down(InputEvent &input_event)
{
	clicked_index = selected_index;
	request_repaint();
}

void Toolbar::on_mouse_lbutton_up(InputEvent &input_event)
{
	if(clicked_index != -1)
		if (!func_selected.is_null())
			func_selected.invoke(clicked_index);

	clicked_index = -1;
	request_repaint();
}

void Toolbar::on_render(Canvas &canvas, const Rect &update_rect)
{
	part_component.render_box(canvas, get_geometry().get_size(), update_rect);

	int pos_x = start_x;
	int pos_y = start_y;

	for(unsigned int i = 0; i < items.size(); ++i)
	{
		if(i == clicked_index)
			items[i].icon_clicked.draw(canvas, (float)pos_x, (float)pos_y);
		else if(i == selected_index)
			items[i].icon_selected.draw(canvas, (float)pos_x, (float)pos_y);
		else
			items[i].icon.draw(canvas, (float)pos_x, (float)pos_y);

		pos_x += offset_x;
	}
}

void Toolbar::on_style_changed()
{
	create_parts();
}

void Toolbar::create_parts()
{
	part_component = GUIThemePart(this);

	GUIThemePartProperty prop_start_x("start-x");
	start_x = StringHelp::text_to_int(part_component.get_property(prop_start_x));
	GUIThemePartProperty prop_start_y("start-y");
	start_y = StringHelp::text_to_int(part_component.get_property(prop_start_y));
	GUIThemePartProperty prop_offset_x("offset-x");
	offset_x = StringHelp::text_to_int(part_component.get_property(prop_offset_x));
}

void Toolbar::add_item(Sprite icon, Sprite icon_selected, Sprite icon_clicked)
{
	items.push_back(ToolbarItem(icon, icon_selected, icon_clicked));

	request_repaint();
}

void Toolbar::clear_items()
{
	selected_index = -1;
	items.clear();
	request_repaint();
}
