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
*/

#include "precomp.h"
#include "toolbar.h"

Toolbar::Toolbar(const Rect &position, GUIComponent* owner)
: GUIComponent(owner, get_toplevel_description(position), "toolbar"), selected_index(-1), clicked_index(-1)
{
	func_render().set(this, &Toolbar::on_render);
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

void Toolbar::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		if (input_msg->input_event.type == InputEvent::pointer_moved)
			on_mouse_move(input_msg->input_event);
		else if (input_msg->input_event.type == InputEvent::pressed && input_msg->input_event.id == mouse_left)
			on_mouse_lbutton_down(input_msg->input_event);
		else if (input_msg->input_event.type == InputEvent::released && input_msg->input_event.id == mouse_left)
			on_mouse_lbutton_up(input_msg->input_event);
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

void Toolbar::create_parts()
{
	start_x = StringHelp::text_to_int(get_property("start-x", ""));
	start_y = StringHelp::text_to_int(get_property("start-y", ""));
	offset_x = StringHelp::text_to_int(get_property("offset-x", ""));
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
