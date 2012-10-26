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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/Components/toolbar.h"
#include "API/GUI/Components/toolbar_item.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_theme.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Core/Text/string_help.h"
#include "toolbar_item_impl.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ToolBar_Impl Class:

enum ToolBarMouseMode
{
	mouse_mode_normal,
	mouse_mode_pressed
};

class ToolBar_Impl
{
public:
	ToolBar_Impl()
	: toolbar(0), need_layout_update(true), horizontal(false), single_select_mode(false), index_hot_item(-1), index_pressed_item(-1),
	  mouse_mode(mouse_mode_normal), size_icon(16,16), next_id(1)
	{
		prop_text_color = GUIThemePartProperty(CssStr::text_color, "black");
		prop_icon_width = GUIThemePartProperty(CssStr::icon_width);
		prop_icon_height = GUIThemePartProperty(CssStr::icon_height);
		prop_layout = GUIThemePartProperty(CssStr::layout, "left");
		prop_text_gap = GUIThemePartProperty(CssStr::text_gap, "3");
	}

	void on_process_message(GUIMessage &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_resized();
	void create_parts();
	void update_layout(Canvas &canvas);
	void unselect_all(ToolBarItem_Impl *ignore);
	int find_item_at(const Point &pos);

	ToolBar *toolbar;
	std::vector<ToolBarItem> items;
	GUIThemePart part_component;
	GUIThemePart part_item_normal;
	GUIThemePart part_item_disabled;
	GUIThemePart part_item_pressed;
	GUIThemePart part_item_hot;
	GUIThemePart part_item_on;
	GUIThemePartProperty prop_text_color;
	GUIThemePartProperty prop_icon_width;
	GUIThemePartProperty prop_icon_height;
	GUIThemePartProperty prop_layout;
	GUIThemePartProperty prop_text_gap;

	bool need_layout_update;
	bool horizontal;
	bool single_select_mode;

	int index_hot_item;

	int	index_pressed_item;

	ToolBarMouseMode mouse_mode;

	Font font;
	Font font_disabled;
	Colorf text_color;
	Size size_icon;
	enum Layout {layout_left, layout_center} layout;

	Callback_v1<ToolBarItem> func_item_clicked;
	Callback_v1<ToolBarItem> func_item_selected;
	Callback_v1<ToolBarItem> func_item_unselected;
	Callback_v2<Point, int> func_mouse_right_up; // click position, clicked item index.

	int next_id;
};

/////////////////////////////////////////////////////////////////////////////
// ToolBar Construction:

ToolBar::ToolBar(GUIComponent *parent)
: GUIComponent(parent), impl(new ToolBar_Impl)
{
	set_type_name("toolbar");
	set_class_name("horizontal");
	impl->toolbar = this;
	func_process_message().set(impl.get(), &ToolBar_Impl::on_process_message);
	func_render().set(impl.get(), &ToolBar_Impl::on_render);
	func_resized().set(impl.get(), &ToolBar_Impl::on_resized);
	func_style_changed().set(impl.get(), &ToolBar_Impl::create_parts);
	impl->create_parts();
}

ToolBar::~ToolBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// ToolBar Attributes:

ToolBar *ToolBar::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	ToolBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<ToolBar*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find ToolBar named item: %1", id));

	return object;
}

Size ToolBar::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

ToolBarItem ToolBar::get_item(int index)
{
	return impl->items[index];
}

ToolBarItem ToolBar::get_item_by_id(int id)
{
	int index = get_item_index(id);
	return impl->items[index];
}

int ToolBar::get_item_index(int id)
{
	int index = 0;
	std::vector<ToolBarItem>::iterator it;
	for (it = impl->items.begin(); it != impl->items.end(); ++it)
	{
		if ((*it).get_id() == id)
			return index;
		index++;
	}
	return -1;
}

int ToolBar::get_item_count() const
{
	return impl->items.size();
}

/////////////////////////////////////////////////////////////////////////////
// ToolBar Operations:

ToolBarItem ToolBar::add_item(const Sprite &icon, int frame, const std::string &text, int id)
{
	ToolBarItem item = insert_item(icon, frame, text, id);
	return item;
}

ToolBarItem ToolBar::insert_item(const Sprite &icon, int frame, const std::string &text, int id, int index)
{
	if (id == -1)
		id = impl->next_id++;

	ToolBarItem item(id);
	item.impl->icon = icon;
	item.impl->frame = frame;
	item.impl->text = text;
	impl->items.push_back(item);
	impl->need_layout_update = true;
	request_repaint();
	return item;
}

ToolBarItem ToolBar::insert_separator(int index)
{
	return ToolBarItem();
}

void ToolBar::set_single_selection(bool value)
{
	impl->single_select_mode = value;
	clear_selection();
}

void ToolBar::clear_selection()
{
	impl->unselect_all(0);
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// ToolBar Events:

Callback_v1<ToolBarItem> & ToolBar::func_item_clicked()
{
	return impl->func_item_clicked;
}

Callback_v1<ToolBarItem> & ToolBar::func_item_selected()
{
	return impl->func_item_selected;
}

Callback_v1<ToolBarItem> & ToolBar::func_item_unselected()
{
	return impl->func_item_unselected;
}

Callback_v2<Point, int> & ToolBar::func_mouse_right_up()
{
	return impl->func_mouse_right_up;
}

void ToolBar::delete_item(int index)
{
	impl->items.erase(impl->items.begin() + index);
	impl->need_layout_update = true;
	request_repaint();
}

////////////////////////////////////////////////////////////////////////////
// ToolBar Implementation:

void ToolBar_Impl::on_process_message(GUIMessage &msg)
{
	if (!toolbar->is_enabled())
		return;

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::released && e.id == mouse_right)
		{
			int index = find_item_at(e.mouse_pos);
			if (!func_mouse_right_up.is_null())
				func_mouse_right_up.invoke(e.mouse_pos, index);
		}
		else if (mouse_mode == mouse_mode_normal)
		{
			int index = find_item_at(e.mouse_pos);
			if (index == -1)
			{
				if (index_hot_item != -1)
				{
					index_hot_item = -1;
					toolbar->request_repaint();
				}
				return;
			}

			if (e.type == InputEvent::pressed && e.id == mouse_left)
			{
				index_hot_item = -1;
				index_pressed_item = index;
				mouse_mode = mouse_mode_pressed;
				toolbar->request_repaint();
				toolbar->capture_mouse(true);
			}
			else
			{
				index_hot_item = index;
				toolbar->request_repaint();
			}
		}
		else if (mouse_mode == mouse_mode_pressed)
		{
			if (e.type == InputEvent::released && e.id == mouse_left)
			{
				toolbar->capture_mouse(false);
				mouse_mode = mouse_mode_normal;

				int index = find_item_at(e.mouse_pos);
				bool perform_click = (index == index_pressed_item);
				bool pressed_state = items[index_pressed_item].is_pressed();

				if (pressed_state == true && single_select_mode)
				{
					return;
				}

				if (items[index_pressed_item].impl->toggling && perform_click)
					items[index_pressed_item].impl->pressed = !items[index_pressed_item].impl->pressed;

				index_pressed_item = -1;
				index_hot_item = index;
				toolbar->request_repaint();

				if (perform_click)
				{
					if (!func_item_clicked.is_null())
						func_item_clicked.invoke(items[index]);

					if (items[index].is_toggling() && single_select_mode)
					{
						unselect_all(items[index].impl.get());

						if (!func_item_selected.is_null())
							func_item_selected.invoke(items[index]);
					}
				}
			}
		}
	}
	else if (msg.is_type(GUIMessage_Pointer::get_type_name()))
	{
		GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == GUIMessage_Pointer::pointer_leave)
		{
			if (index_hot_item != -1)
			{
				index_hot_item = -1;
				toolbar->request_repaint();
			}
		}
	}
}

void ToolBar_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	update_layout(canvas);

	Rect rect = toolbar->get_size();
	part_component.render_box(canvas, rect, update_rect);

	std::vector<ToolBarItem>::size_type index, size;
	size = items.size();
	for (index = 0; index < size; index++)
	{
		ToolBarItem &item = items[index];

		GUIThemePart part_item = part_item_normal;
		if (index == index_hot_item)
			part_item = part_item_hot;
		if (index == index_pressed_item || items[index].impl->pressed)
			part_item = part_item_pressed;

		Rect item_content = part_item.get_content_box(item.impl->position);
		part_item.render_box(canvas, item.impl->position, update_rect);

		Rect icon_pos = item.impl->icon_pos;
		icon_pos.translate(item_content.left, item_content.top);

		Point pressed_offset(0,0);
		if (index == index_pressed_item)
			pressed_offset = Point(0,0); // To do: Read this from css properties or remove feature totally

		if (!item.impl->icon.is_null())
		{
			icon_pos.translate(pressed_offset);
			item.impl->icon.set_frame(item.impl->frame);
			item.impl->icon.draw(canvas, icon_pos);
		}

		toolbar->push_cliprect(canvas, item_content);

		font.draw_text(canvas,
			item_content.left + item.impl->text_pos.x + pressed_offset.x,
			item_content.top + item.impl->text_pos.y + pressed_offset.y,
			item.impl->text,
			text_color);

		toolbar->pop_cliprect(canvas);
	}
}


void ToolBar_Impl::on_resized()
{
	Rect rect = toolbar->get_geometry();

	int rect_width = rect.get_width();
	int rect_height = rect.get_height();

	if ( (rect_width > 0) && (rect_height > 0) )	// Only update if the geometry is valid
	{
		bool old_horiz = part_component.get_state("horizontal");

		horizontal = (rect_width >= rect_height);

		if (old_horiz != horizontal)
		{
			if (horizontal)
				toolbar->set_class_name("horizontal");
			else 
				toolbar->set_class_name("vertical");
		}

		need_layout_update = true;
	}
}

void ToolBar_Impl::unselect_all(ToolBarItem_Impl *ignore)
{
	std::vector<ToolBarItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		ToolBarItem &item = *it;
		if (item.impl.get() == ignore)
			continue;

		if (item.is_pressed())
		{
			item.impl->pressed = false;
			if (!func_item_unselected.is_null())
			{
				func_item_unselected.invoke(item);
			}
		}
	}
}

void ToolBar_Impl::create_parts()
{
	part_component = GUIThemePart(toolbar);
	part_component.set_state(CssStr::horizontal, true);
	part_item_normal = GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_normal.set_state(CssStr::normal, true);
	part_item_disabled = GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_disabled.set_state(CssStr::disabled, true);
	part_item_pressed = GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_pressed.set_state(CssStr::pressed, true);
	part_item_hot = GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_hot.set_state(CssStr::hot, true);
	part_item_on = GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_on.set_state(CssStr::on, true);

	font = part_component.get_font();
	text_color = part_component.get_property(prop_text_color);
	size_icon.width = part_item_normal.get_property_int(prop_icon_width);
	size_icon.height = part_item_normal.get_property_int(prop_icon_height);
	std::string str_alignment = part_component.get_property(prop_layout);
	if (str_alignment == "center")
		layout = layout_center;
	else
		layout = layout_left;

	need_layout_update = true;
}

void ToolBar_Impl::update_layout(Canvas &canvas)
{
	if (need_layout_update == false)
		return;
	need_layout_update = false;

	Rect rect = toolbar->get_size();
	Rect component_content = part_component.get_content_box(rect);

	Rect item_content = part_item_normal.get_content_box(component_content);
	int original_text_gap = part_item_normal.get_property_int(prop_text_gap);

	if (horizontal)
	{
		int x = component_content.left;
		int center_y = item_content.get_center().y;
		int item_size = part_item_normal.get_preferred_width();

		std::vector<ToolBarItem>::size_type index, size;
		size = items.size();
		for (index = 0; index < size; index++)
		{
			ToolBarItem &item = items[index];
			item_content.left = x;
			Size text_size = font.get_text_size(canvas, item.impl->text);
			int text_gap = original_text_gap;
			if (text_size.width == 0)
				text_gap = 0;

			if (layout == layout_left)
			{
				item.impl->icon_pos = Rect(Point(0, center_y-size_icon.height/2-item_content.top), size_icon);
				item_content.right = item_content.left + item.impl->icon_pos.get_width() + text_gap + text_size.width;
				item.impl->text_pos = Point(item.impl->icon_pos.right + text_gap, part_item_normal.get_vertical_text_align(canvas, font, item_content).baseline-item_content.top);
			}
			else if (layout == layout_center)
			{
				item.impl->icon_pos = Rect(Point(item_size/2-size_icon.width/2, 0), size_icon);
				item.impl->text_pos = Point(item_size/2-text_size.width/2, item.impl->icon_pos.bottom + text_gap + text_size.height);
				item_content.right = item_content.left + item_size;
			}

			Rect item_render = part_item_normal.get_render_box(item_content);
			Rect shrink_box = part_item_normal.get_content_shrink_box();
			item_render.translate(shrink_box.left,0);
			item.impl->position = item_render;

			x = item_render.right;
		}
	}
	else
	{
		int y = component_content.top;
		int center_x = item_content.get_center().x;
		int size_item = part_item_normal.get_preferred_height();

		std::vector<ToolBarItem>::size_type index, size;
		size = items.size();
		for (index = 0; index < size; index++)
		{
			ToolBarItem &item = items[index];

			item_content.top = y;
			Size text_size = font.get_text_size(canvas, item.impl->text);

			int text_gap = original_text_gap;
			if (text_size.width == 0)
				text_gap = 0;

			if (layout == layout_left)
			{
				item.impl->icon_pos = Rect(Point(0,0), size_icon);
				item.impl->text_pos = Point(item.impl->icon_pos.right + text_gap, size_item/2+text_size.height/2);
				item_content.bottom = item_content.top + size_item;
			}
			else if (layout == layout_center)
			{
				item.impl->icon_pos = Rect(Point(center_x-size_icon.width/2, 0), size_icon);
				item.impl->text_pos = Point(center_x-text_size.width/2, item.impl->icon_pos.bottom + text_gap + text_size.height);
				item_content.bottom = item_content.top + item.impl->icon_pos.get_height() + text_gap + text_size.height;
			}

			Rect item_render = part_item_normal.get_render_box(item_content);
			Rect shrink_box = part_item_normal.get_content_shrink_box();
			item_render.translate(0, shrink_box.top);
			item.impl->position = item_render;

			y = item_render.bottom;
		}
	}

//	toolbar->request_repaint();
}

int ToolBar_Impl::find_item_at(const Point &pos)
{
	std::vector<ToolBarItem>::size_type index, size;
	size = items.size();
	for (index = 0; index < size; index++)
	{
		ToolBarItem &item = items[index];
		if (item.impl->position.contains(pos))
			return index;
	}
	return -1;
}

}
