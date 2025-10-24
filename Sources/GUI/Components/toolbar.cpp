/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBar_Impl Class:

enum CL_ToolBarMouseMode
{
	cl_mouse_mode_normal,
	cl_mouse_mode_pressed
};

class CL_ToolBar_Impl
{
public:
	CL_ToolBar_Impl()
	: toolbar(0), need_layout_update(true), horizontal(false), single_select_mode(false), index_hot_item(-1), index_pressed_item(-1),
	  mouse_mode(cl_mouse_mode_normal), size_icon(16,16), next_id(1)
	{
		prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, "black");
		prop_icon_width = CL_GUIThemePartProperty(CssStr::icon_width);
		prop_icon_height = CL_GUIThemePartProperty(CssStr::icon_height);
		prop_layout = CL_GUIThemePartProperty(CssStr::layout, "left");
		prop_text_gap = CL_GUIThemePartProperty(CssStr::text_gap, "3");
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();
	void create_parts();
	void update_layout(CL_GraphicContext &gc);
	void unselect_all(CL_ToolBarItem_Impl *ignore);
	int find_item_at(const CL_Point &pos);

	CL_ToolBar *toolbar;
	std::vector<CL_ToolBarItem> items;
	CL_GUIThemePart part_component;
	CL_GUIThemePart part_item_normal;
	CL_GUIThemePart part_item_disabled;
	CL_GUIThemePart part_item_pressed;
	CL_GUIThemePart part_item_hot;
	CL_GUIThemePart part_item_on;
	CL_GUIThemePartProperty prop_text_color;
	CL_GUIThemePartProperty prop_icon_width;
	CL_GUIThemePartProperty prop_icon_height;
	CL_GUIThemePartProperty prop_layout;
	CL_GUIThemePartProperty prop_text_gap;

	bool need_layout_update;
	bool horizontal;
	bool single_select_mode;

	int index_hot_item;

	int	index_pressed_item;

	CL_ToolBarMouseMode mouse_mode;

	CL_Font font;
	CL_Font font_disabled;
	CL_Colorf text_color;
	CL_Size size_icon;
	enum Layout {layout_left, layout_center} layout;

	CL_Callback_v1<CL_ToolBarItem> func_item_clicked;
	CL_Callback_v1<CL_ToolBarItem> func_item_selected;
	CL_Callback_v1<CL_ToolBarItem> func_item_unselected;
	CL_Callback_v2<CL_Point, int> func_mouse_right_up; // click position, clicked item index.

	int next_id;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBar Construction:

CL_ToolBar::CL_ToolBar(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_ToolBar_Impl)
{
	set_type_name("toolbar");
	set_class_name("horizontal");
	impl->toolbar = this;
	func_process_message().set(impl.get(), &CL_ToolBar_Impl::on_process_message);
	func_render().set(impl.get(), &CL_ToolBar_Impl::on_render);
	func_resized().set(impl.get(), &CL_ToolBar_Impl::on_resized);
	func_style_changed().set(impl.get(), &CL_ToolBar_Impl::create_parts);
	impl->create_parts();
}

CL_ToolBar::~CL_ToolBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBar Attributes:

CL_ToolBar *CL_ToolBar::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_ToolBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_ToolBar*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_ToolBar named item: %1", id));

	return object;
}

CL_Size CL_ToolBar::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

CL_ToolBarItem CL_ToolBar::get_item(int index)
{
	return impl->items[index];
}

CL_ToolBarItem CL_ToolBar::get_item_by_id(int id)
{
	int index = get_item_index(id);
	return impl->items[index];
}

int CL_ToolBar::get_item_index(int id)
{
	int index = 0;
	std::vector<CL_ToolBarItem>::iterator it;
	for (it = impl->items.begin(); it != impl->items.end(); ++it)
	{
		if ((*it).get_id() == id)
			return index;
		index++;
	}
	return -1;
}

int CL_ToolBar::get_item_count() const
{
	return impl->items.size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBar Operations:

CL_ToolBarItem CL_ToolBar::add_item(const CL_Sprite &icon, int frame, const CL_StringRef &text, int id)
{
	CL_ToolBarItem item = insert_item(icon, frame, text, id);
	return item;
}

CL_ToolBarItem CL_ToolBar::insert_item(const CL_Sprite &icon, int frame, const CL_StringRef &text, int id, int index)
{
	if (id == -1)
		id = impl->next_id++;

	CL_ToolBarItem item(id);
	item.impl->icon = icon;
	item.impl->frame = frame;
	item.impl->text = text;
	impl->items.push_back(item);
	impl->need_layout_update = true;
	request_repaint();
	return item;
}

CL_ToolBarItem CL_ToolBar::insert_separator(int index)
{
	return CL_ToolBarItem();
}

void CL_ToolBar::set_single_selection(bool value)
{
	impl->single_select_mode = value;
	clear_selection();
}

void CL_ToolBar::clear_selection()
{
	impl->unselect_all(0);
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolBar Events:

CL_Callback_v1<CL_ToolBarItem> & CL_ToolBar::func_item_clicked()
{
	return impl->func_item_clicked;
}

CL_Callback_v1<CL_ToolBarItem> & CL_ToolBar::func_item_selected()
{
	return impl->func_item_selected;
}

CL_Callback_v1<CL_ToolBarItem> & CL_ToolBar::func_item_unselected()
{
	return impl->func_item_unselected;
}

CL_Callback_v2<CL_Point, int> & CL_ToolBar::func_mouse_right_up()
{
	return impl->func_mouse_right_up;
}

void CL_ToolBar::delete_item(int index)
{
	impl->items.erase(impl->items.begin() + index);
	impl->need_layout_update = true;
	request_repaint();
}

////////////////////////////////////////////////////////////////////////////
// CL_ToolBar Implementation:

void CL_ToolBar_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!toolbar->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_RIGHT)
		{
			int index = find_item_at(e.mouse_pos);
			if (!func_mouse_right_up.is_null())
				func_mouse_right_up.invoke(e.mouse_pos, index);
		}
		else if (mouse_mode == cl_mouse_mode_normal)
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

			if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
			{
				index_hot_item = -1;
				index_pressed_item = index;
				mouse_mode = cl_mouse_mode_pressed;
				toolbar->request_repaint();
				toolbar->capture_mouse(true);
			}
			else
			{
				index_hot_item = index;
				toolbar->request_repaint();
			}
		}
		else if (mouse_mode == cl_mouse_mode_pressed)
		{
			if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
			{
				toolbar->capture_mouse(false);
				mouse_mode = cl_mouse_mode_normal;

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
						unselect_all(items[index].impl);

						if (!func_item_selected.is_null())
							func_item_selected.invoke(items[index]);
					}
				}
			}
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
		{
			if (index_hot_item != -1)
			{
				index_hot_item = -1;
				toolbar->request_repaint();
			}
		}
	}
}

void CL_ToolBar_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	update_layout(gc);

	CL_Rect rect = toolbar->get_size();
	part_component.render_box(gc, rect, update_rect);

	std::vector<CL_ToolBarItem>::size_type index, size;
	size = items.size();
	for (index = 0; index < size; index++)
	{
		CL_ToolBarItem &item = items[index];

		CL_GUIThemePart part_item = part_item_normal;
		if (index == index_hot_item)
			part_item = part_item_hot;
		if (index == index_pressed_item || items[index].impl->pressed)
			part_item = part_item_pressed;

		CL_Rect item_content = part_item.get_content_box(item.impl->position);
		part_item.render_box(gc, item.impl->position, update_rect);

		CL_Rect icon_pos = item.impl->icon_pos;
		icon_pos.translate(item_content.left, item_content.top);

		CL_Point pressed_offset(0,0);
		if (index == index_pressed_item)
			pressed_offset = CL_Point(0,0); // To do: Read this from css properties or remove feature totally

		if (!item.impl->icon.is_null())
		{
			icon_pos.translate(pressed_offset);
			item.impl->icon.set_frame(item.impl->frame);
			item.impl->icon.draw(gc, icon_pos);
		}

		toolbar->push_cliprect(gc, item_content);

		font.draw_text(gc,
			item_content.left + item.impl->text_pos.x + pressed_offset.x,
			item_content.top + item.impl->text_pos.y + pressed_offset.y,
			item.impl->text,
			text_color);

		toolbar->pop_cliprect(gc);
	}
}


void CL_ToolBar_Impl::on_resized()
{
	CL_Rect rect = toolbar->get_geometry();

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

void CL_ToolBar_Impl::unselect_all(CL_ToolBarItem_Impl *ignore)
{
	std::vector<CL_ToolBarItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		CL_ToolBarItem &item = *it;
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

void CL_ToolBar_Impl::create_parts()
{
	part_component = CL_GUIThemePart(toolbar);
	part_component.set_state(CssStr::horizontal, true);
	part_item_normal = CL_GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_normal.set_state(CssStr::normal, true);
	part_item_disabled = CL_GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_disabled.set_state(CssStr::disabled, true);
	part_item_pressed = CL_GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_pressed.set_state(CssStr::pressed, true);
	part_item_hot = CL_GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_hot.set_state(CssStr::hot, true);
	part_item_on = CL_GUIThemePart(toolbar, CssStr::ToolBar::part_item);
	part_item_on.set_state(CssStr::on, true);

	font = part_component.get_font();
	text_color = part_component.get_property(prop_text_color);
	size_icon.width = part_item_normal.get_property_int(prop_icon_width);
	size_icon.height = part_item_normal.get_property_int(prop_icon_height);
	CL_String str_alignment = part_component.get_property(prop_layout);
	if (str_alignment == "center")
		layout = layout_center;
	else
		layout = layout_left;

	need_layout_update = true;
}

void CL_ToolBar_Impl::update_layout(CL_GraphicContext &gc)
{
	if (need_layout_update == false)
		return;
	need_layout_update = false;

	CL_Rect rect = toolbar->get_size();
	CL_Rect component_content = part_component.get_content_box(rect);

	CL_Rect item_content = part_item_normal.get_content_box(component_content);
	int original_text_gap = part_item_normal.get_property_int(prop_text_gap);

	if (horizontal)
	{
		int x = component_content.left;
		int center_y = item_content.get_center().y;
		int item_size = part_item_normal.get_preferred_width();

		std::vector<CL_ToolBarItem>::size_type index, size;
		size = items.size();
		for (index = 0; index < size; index++)
		{
			CL_ToolBarItem &item = items[index];
			item_content.left = x;
			CL_Size text_size = font.get_text_size(gc, item.impl->text);
			int text_gap = original_text_gap;
			if (text_size.width == 0)
				text_gap = 0;

			if (layout == layout_left)
			{
				item.impl->icon_pos = CL_Rect(CL_Point(0, center_y-size_icon.height/2-item_content.top), size_icon);
				item_content.right = item_content.left + item.impl->icon_pos.get_width() + text_gap + text_size.width;
				item.impl->text_pos = CL_Point(item.impl->icon_pos.right + text_gap, part_item_normal.get_vertical_text_align(gc, font, item_content).baseline-item_content.top);
			}
			else if (layout == layout_center)
			{
				item.impl->icon_pos = CL_Rect(CL_Point(item_size/2-size_icon.width/2, 0), size_icon);
				item.impl->text_pos = CL_Point(item_size/2-text_size.width/2, item.impl->icon_pos.bottom + text_gap + text_size.height);
				item_content.right = item_content.left + item_size;
			}

			CL_Rect item_render = part_item_normal.get_render_box(item_content);
			CL_Rect shrink_box = part_item_normal.get_content_shrink_box();
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

		std::vector<CL_ToolBarItem>::size_type index, size;
		size = items.size();
		for (index = 0; index < size; index++)
		{
			CL_ToolBarItem &item = items[index];

			item_content.top = y;
			CL_Size text_size = font.get_text_size(gc, item.impl->text);

			int text_gap = original_text_gap;
			if (text_size.width == 0)
				text_gap = 0;

			if (layout == layout_left)
			{
				item.impl->icon_pos = CL_Rect(CL_Point(0,0), size_icon);
				item.impl->text_pos = CL_Point(item.impl->icon_pos.right + text_gap, size_item/2+text_size.height/2);
				item_content.bottom = item_content.top + size_item;
			}
			else if (layout == layout_center)
			{
				item.impl->icon_pos = CL_Rect(CL_Point(center_x-size_icon.width/2, 0), size_icon);
				item.impl->text_pos = CL_Point(center_x-text_size.width/2, item.impl->icon_pos.bottom + text_gap + text_size.height);
				item_content.bottom = item_content.top + item.impl->icon_pos.get_height() + text_gap + text_size.height;
			}

			CL_Rect item_render = part_item_normal.get_render_box(item_content);
			CL_Rect shrink_box = part_item_normal.get_content_shrink_box();
			item_render.translate(0, shrink_box.top);
			item.impl->position = item_render;

			y = item_render.bottom;
		}
	}

//	toolbar->request_repaint();
}

int CL_ToolBar_Impl::find_item_at(const CL_Point &pos)
{
	std::vector<CL_ToolBarItem>::size_type index, size;
	size = items.size();
	for (index = 0; index < size; index++)
	{
		CL_ToolBarItem &item = items[index];
		if (item.impl->position.contains(pos))
			return index;
	}
	return -1;
}
