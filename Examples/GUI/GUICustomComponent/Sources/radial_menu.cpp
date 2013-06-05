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
#include "radial_menu.h"

RadialMenu::RadialMenu(GUIComponent *parent)
: GUIComponent(parent, "radialmenu")
{
	func_render().set(this, &RadialMenu::on_render);
	func_resized().set(this, &RadialMenu::on_resized);
	func_process_message().set(this, &RadialMenu::on_process_message);

	create_parts();
	
	clear_items();

	set_visible(false);
}

RadialMenu::~RadialMenu()
{
	items.clear();
}


std::string RadialMenu::get_item(int index) const
{
	if(index >= 0 && index < (int)items.size())
		return items[index].text;
	else
		return std::string();
}

void RadialMenu::show(Point &position)
{
	set_geometry(Rect(position.x - 120, position.y - 120, position.x + 120, position.y + 120));
	set_visible(true);
	capture_mouse(true);
}

void RadialMenu::clear_items()
{
	selected_index = -1;
	max_angle = Angle::from_degrees(0);
	items.clear();
	request_repaint();
}

void RadialMenu::add_item(const std::string &text)
{
	items.push_back(RadialMenuItem(text));

	max_angle = Angle::from_degrees((items.size() - 1) * 45.0f + 22.5f);

	request_repaint();
}

void RadialMenu::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		InputEvent &input_event = input_msg->input_event;
		if (input_event.type == InputEvent::pointer_moved)
			on_mouse_move(input_event);
		else if (input_event.type == InputEvent::released && input_event.id == mouse_left)
			on_mouse_lbutton_up(input_event);
	}
}

void RadialMenu::on_mouse_move(InputEvent &input_event)
{
	Vec2f mouse_offset((float)input_event.mouse_pos.x - center_x, (float)input_event.mouse_pos.y - center_y);
	if(mouse_offset.length() > 25.0f)
	{
		Vec2f origin(Vec2f(0, -1));
		Angle mouse_angle = origin.angle(mouse_offset);

		int degrees = ((int)(mouse_angle.to_degrees() + 22.5f) / 45) * 45;
		selected_angle = Angle::from_degrees((float)degrees);
		if(mouse_offset.x < 0)
			selected_angle = Angle::from_degrees(360.0f) - selected_angle;
		if(selected_angle.to_degrees() >= 360.0f)
			selected_angle = Angle::from_degrees(360.0f) - selected_angle;

		image_pointer.set_angle(selected_angle);

		if(selected_angle >= max_angle)
			selected_index = -1;
		else
			selected_index = (int)selected_angle.to_degrees() / 45;
	}
	else
		selected_index = -1;
}

void RadialMenu::on_mouse_lbutton_up(InputEvent &input_event)
{
	if (!func_selected.is_null())
		func_selected.invoke(selected_index);

	set_visible(false);	
	capture_mouse(false);
}

void RadialMenu::on_render(Canvas &canvas, const Rect &update_rect)
{
	if(selected_index != -1)
		image_pointer.draw(canvas, (float)center_x, (float)center_y);

	GraphicContext &gc = canvas.get_gc();

	Angle text_angle = Angle::from_degrees(90);
	for(unsigned int i = 0; i < items.size(); ++i)
	{
		float rads = text_angle.to_radians();

		if(i == selected_index)
		{
			int x = (int)((-cosf(rads)) * selected_text_distance);
			int y = (int)((-sinf(rads)) * selected_text_distance);
			int font_center_x = selected_font.get_text_size(gc, items[i].text).width / 2;
			int font_center_y = (int)selected_font_height / 2;
			selected_font.draw_text(canvas, center_x + x - font_center_x, center_y + y + font_center_y, items[i].text, selected_text_color);
		}
		else
		{
			int x = (int)((-cosf(rads)) * normal_text_distance);
			int y = (int)((-sinf(rads)) * normal_text_distance);
			int fontx = normal_font.get_text_size(gc, items[i].text).width / 2;
			int fonty = (int)normal_font_height / 2;
			normal_font.draw_text(canvas, center_x + x - fontx, center_y + y + fonty, items[i].text, normal_text_color);
		}

		text_angle += Angle::from_degrees(45.0f);
	}
}

void RadialMenu::on_resized()
{
	Rect rect = get_geometry();
	center_x = rect.get_width() / 2;
	center_y = rect.get_height() / 2;
}

void RadialMenu::create_parts()
{
	Canvas canvas = get_canvas();
	ResourceManager resources = get_resources();

	GUIThemePart part_selected_item(this, "selected");

	normal_text_color = get_property("text-color", "");
	normal_font = get_font();
	normal_text_distance = StringHelp::text_to_float(get_property("text-distance", ""));
	normal_font_height = normal_font.get_font_metrics().get_height();

	selected_text_color = part_selected_item.get_property("text-color", "");
	selected_font = part_selected_item.get_font();
	selected_text_distance = StringHelp::text_to_float(part_selected_item.get_property("text-distance", ""));
	selected_font_height = selected_font.get_font_metrics().get_height();

	std::string pointer_image_name = get_property("pointer-image", "");
	image_pointer = Sprite::resource(canvas, pointer_image_name, resources);
	image_pointer.set_alignment(origin_center);
}
