#pragma once

#include <ClanLib/gui.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>

class Toolbar : public GUIComponent
{
public:
	Toolbar(const Rect &position, GUIComponent* owner);
	virtual ~Toolbar();

// Attributes
public:
	Size get_preferred_size() const;

// Operations:
public:
	void clear_items();

	void add_item(Sprite icon, Sprite icon_selected, Sprite icon_clicked);

// Signals and callbacks:
public:
	Callback<void(int)> func_selected;

// Implementation:
private:
	struct ToolbarItem
	{
		ToolbarItem(Sprite &icon, Sprite &icon_selected, Sprite &icon_clicked) : icon(icon), icon_selected(icon_selected), icon_clicked(icon_clicked) {}
		Sprite icon;
		Sprite icon_selected;
		Sprite icon_clicked;
	};

	void create_parts();
	GUITopLevelDescription get_toplevel_description(const Rect &position);

	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_resized();
	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_mouse_move(InputEvent &input_event);
	void on_mouse_lbutton_down(InputEvent &input_event);
	void on_mouse_lbutton_up(InputEvent &input_event);

	int start_x;
	int start_y;
	int offset_x;

	int selected_index;
	int clicked_index;

	std::vector<ToolbarItem> items;
};
