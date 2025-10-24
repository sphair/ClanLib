#pragma once

#include <ClanLib/gui.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>

class RadialMenu : public CL_GUIComponent
{
public:
	RadialMenu(CL_GUIComponent *parent);
	virtual ~RadialMenu();

// Attributes
public:
	CL_Size get_preferred_size() const;

	CL_String get_item(int index) const;

// Operations:
public:
	void show(CL_Point &position);

	void clear_items();

	void add_item(const CL_String &text);

// Signals and callbacks:
public:
	CL_Callback_v1<int> func_selected;

// Implementation:
private:
	struct RadialMenuItem
	{
		RadialMenuItem(const CL_String &text) : text(text) {}
		CL_String text;
	};

	void create_parts();

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_resized();
	void on_process_message(CL_GUIMessage &msg);
	void on_mouse_move(CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_InputEvent &input_event);

	CL_GUIThemePart part_component;

	CL_Sprite image_pointer;

	CL_Font font_normal;
	CL_Colorf text_color_normal;
	float font_normal_height;

	CL_Font font_selected;
	CL_Colorf text_color_selected;
	float font_selected_height;

	float text_distance;
	float text_distance_selected;

	int selected_index;
	CL_Angle selected_angle;
	CL_Angle max_angle;

	int center_x, center_y;

	std::vector<RadialMenuItem> items;
};
