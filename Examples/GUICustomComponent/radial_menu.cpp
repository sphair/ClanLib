#include "radial_menu.h"

RadialMenu::RadialMenu(CL_GUIComponent *parent)
: CL_GUIComponent(parent)
{
	set_type_name("radialmenu");

	func_render().set(this, &RadialMenu::on_render);
	func_style_changed().set(this, &RadialMenu::on_style_changed);
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

CL_Size RadialMenu::get_preferred_size() const
{
	return part_component.get_preferred_size();
}

CL_String RadialMenu::get_item(int index) const
{
	if(index >= 0 && index < (int)items.size())
		return items[index].text;
	else
		return CL_String();
}

void RadialMenu::show(CL_Point &position)
{
	set_geometry(CL_Rect(position.x - 120, position.y - 120, position.x + 120, position.y + 120));
	set_visible(true);
	capture_mouse(true);
}

void RadialMenu::clear_items()
{
	selected_index = -1;
	max_angle = CL_Angle::from_degrees(0);
	items.clear();
	invalidate_rect();
}

void RadialMenu::add_item(const CL_String &text)
{
	items.push_back(RadialMenuItem(text));

	max_angle = CL_Angle::from_degrees((items.size() - 1) * 45.0f + 22.5f);

	invalidate_rect();
}

void RadialMenu::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input = msg;
		CL_InputEvent input_event = input.get_event();
		if (input_event.type == CL_InputEvent::pointer_moved)
			on_mouse_move(input_event);
		else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_LEFT)
			on_mouse_lbutton_up(input_event);
	}
}

void RadialMenu::on_mouse_move(CL_InputEvent &input_event)
{
	CL_Vec2f mouse_offset((float)input_event.mouse_pos.x - center_x, (float)input_event.mouse_pos.y - center_y);
	if(mouse_offset.length() > 25.0f)
	{
		CL_Vec2f origin(CL_Vec2f(0, -1));
		CL_Angle mouse_angle = origin.angle(mouse_offset);

		int degrees = ((int)(mouse_angle.to_degrees() + 22.5f) / 45) * 45;
		selected_angle = CL_Angle::from_degrees((float)degrees);
		if(mouse_offset.x < 0)
			selected_angle = CL_Angle::from_degrees(360.0f) - selected_angle;
		if(selected_angle.to_degrees() >= 360.0f)
			selected_angle = CL_Angle::from_degrees(360.0f) - selected_angle;

		image_pointer.set_angle(selected_angle);

		if(selected_angle >= max_angle)
			selected_index = -1;
		else
			selected_index = (int)selected_angle.to_degrees() / 45;
	}
	else
		selected_index = -1;
}

void RadialMenu::on_mouse_lbutton_up(CL_InputEvent &input_event)
{
	if (!func_selected.is_null())
		func_selected.invoke(selected_index);

	set_visible(false);	
	capture_mouse(false);
}

void RadialMenu::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	if(selected_index != -1)
		image_pointer.draw(gc, (float)center_x, (float)center_y);

	part_component.render_box(gc, get_geometry().get_size(), update_rect);

	CL_Angle text_angle = CL_Angle::from_degrees(90);
	for(unsigned int i = 0; i < items.size(); ++i)
	{
		float rads = text_angle.to_radians();

		if(i == selected_index)
		{
			int x = (int)((-cosf(rads)) * text_distance_selected);
			int y = (int)((-sinf(rads)) * text_distance_selected);
			int font_center_x = font_selected.get_text_size(gc, items[i].text).width / 2;
			int font_center_y = (int)font_selected_height / 2;
			font_selected.draw_text(gc, center_x + x - font_center_x, center_y + y + font_center_y, items[i].text, text_color_selected);
		}
		else
		{
			int x = (int)((-cosf(rads)) * text_distance);
			int y = (int)((-sinf(rads)) * text_distance);
			int fontx = font_normal.get_text_size(gc, items[i].text).width / 2;
			int fonty = (int)font_normal_height / 2;
			font_normal.draw_text(gc, center_x + x - fontx, center_y + y + fonty, items[i].text, text_color_normal);
		}

		text_angle += CL_Angle::from_degrees(45.0f);
	}
}

void RadialMenu::on_style_changed()
{
	create_parts();
}

void RadialMenu::on_resized()
{
	CL_Rect rect = get_geometry();
	center_x = rect.get_width() / 2;
	center_y = rect.get_height() / 2;
}

void RadialMenu::create_parts()
{
	part_component = CL_GUIThemePart(this);
	CL_GUIThemePart part_selected_item(this, "selected");

	CL_GraphicContext gc = get_gc();

	CL_GUIThemePartProperty prop_text_color("text-color");
	text_color_normal = part_component.get_property(prop_text_color);
	font_normal = part_component.get_font();
	CL_FontMetrics font_normal_metrics = font_normal.get_font_metrics(gc);
	font_normal_height = font_normal_metrics.get_height();

	CL_GUIThemePartProperty prop_text_color_selected("text-color");
	text_color_selected = part_selected_item.get_property(prop_text_color_selected);
	font_selected = part_selected_item.get_font();
	CL_FontMetrics font_selected_metrics = font_selected.get_font_metrics(gc);
	font_selected_height = font_selected_metrics.get_height();

	CL_GUIThemePartProperty prop_pointer_image("pointer-image");
	CL_String pointer_image_name = part_component.get_property(prop_pointer_image);
	CL_ResourceManager resources = get_resources();
	image_pointer = CL_Sprite(gc, pointer_image_name, &resources);
	image_pointer.set_alignment(origin_center);

	CL_GUIThemePartProperty prop_text_distance("text-distance");
	text_distance = CL_StringHelp::text_to_float(part_component.get_property(prop_text_distance));

	CL_GUIThemePartProperty prop_text_distance_selected("text-distance");
	text_distance_selected = CL_StringHelp::text_to_float(part_selected_item.get_property(prop_text_distance));
}
