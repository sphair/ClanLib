#pragma once

#include <ClanLib/gui.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>

class Toolbar : public CL_GUIComponent
{
public:
	Toolbar(const CL_Rect &position, CL_GUIComponent* owner);
	virtual ~Toolbar();

// Attributes
public:
	CL_Size get_preferred_size() const;

// Operations:
public:
	void clear_items();

	void add_item(CL_Sprite icon, CL_Sprite icon_selected, CL_Sprite icon_clicked);

// Signals and callbacks:
public:
	CL_Callback_v1<int> func_selected;

// Implementation:
private:
	struct ToolbarItem
	{
		ToolbarItem(CL_Sprite &icon, CL_Sprite &icon_selected, CL_Sprite &icon_clicked) : icon(icon), icon_selected(icon_selected), icon_clicked(icon_clicked) {}
		CL_Sprite icon;
		CL_Sprite icon_selected;
		CL_Sprite icon_clicked;
	};

	void create_parts();
	CL_GUITopLevelDescription get_toplevel_description(const CL_Rect &position);

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_resized();
	void on_process_message(CL_GUIMessage &msg);
	void on_mouse_move(CL_InputEvent &input_event);
	void on_mouse_lbutton_down(CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_InputEvent &input_event);

	int start_x;
	int start_y;
	int offset_x;

	CL_GUIThemePart part_component;

	int selected_index;
	int clicked_index;

	std::vector<ToolbarItem> items;
};
