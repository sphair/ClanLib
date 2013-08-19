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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview_header.h"
#include "API/GUI/Components/listview_column_header.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/keys.h"
#include "listview_column_header_impl.h"
#include "../../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListViewHeader_Impl Class:

class ListViewHeader_Impl
{
public:
	ListViewHeader_Impl()
	: listview_header(0), display_mode(listview_mode_details), text_height(0),
	  visible(true), current_mouse_in_rect(0,0,0,0)
	{
	}

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void update_geometry(const Rect &content_rect);
	void on_mouse_lbutton_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_lbutton_up(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_move(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_leave();
	void on_mouse_enter();
	void on_column_size_changed(ListViewColumnHeader col);
	ListViewColumnHeader create_column(const std::string &column_id, const std::string &caption);

	ListViewHeader *listview_header;
	
	ListViewDisplayMode display_mode;
	int text_height;
	bool visible;
	Rect current_mouse_in_rect;
	std::shared_ptr<ListViewColumnHeader_Impl> first_column;
	std::weak_ptr<ListViewColumnHeader_Impl> last_column;
	Callback_v1<ListViewColumnHeader> func_column_added;
	Callback_v1<ListViewColumnHeader> func_column_removed;
	Callback_v1<ListViewColumnHeader> func_column_size_changed;
};

/////////////////////////////////////////////////////////////////////////////
// ListViewHeader Construction:

ListViewHeader::ListViewHeader(GUIComponent *parent)
: GUIComponent(parent, CssStr::ListView::Header::type_name), impl(new ListViewHeader_Impl)
{
	impl->listview_header = this;
	func_process_message().set(impl.get(), &ListViewHeader_Impl::on_process_message);
	func_render().set(impl.get(), &ListViewHeader_Impl::on_render);
	// todo: resize (?)
	// todo: disabled
}

ListViewHeader::~ListViewHeader()
{
}

/////////////////////////////////////////////////////////////////////////////
// ListViewHeader Attributes:

ListViewColumnHeader ListViewHeader::get_first_column()
{
	return impl->first_column;
}

ListViewColumnHeader ListViewHeader::get_column(const std::string &column_id)
{	
	ListViewColumnHeader	col = impl->first_column;
	while (!col.is_null())
	{
		if (col.get_column_id() == column_id)
			return col;
		col = col.get_next_sibling();
	}

	return ListViewColumnHeader();
}

bool ListViewHeader::get_visible() const
{
	return impl->visible;	
}

Size ListViewHeader::get_css_size() const
{
	return Size(50, 50);	//FIXME: impl->listview_header->get_css_size();
}


/////////////////////////////////////////////////////////////////////////////
// ListViewHeader Operations:

ListViewColumnHeader ListViewHeader::create_column(const std::string &column_id, const std::string &caption)
{
	return impl->create_column(column_id, caption);
}

ListViewColumnHeader ListViewHeader::append(ListViewColumnHeader column)
{
	if (impl->first_column)
	{
		impl->last_column.lock()->next_sibling = column.impl;
		column.impl->prev_sibling = impl->last_column;
		impl->last_column = column.impl;
	}
	else
	{
		impl->first_column = column.impl;
		impl->last_column = column.impl;
	}

	if (!impl->func_column_added.is_null())
		impl->func_column_added.invoke(ListViewColumnHeader(impl->last_column.lock()));

	return ListViewColumnHeader(impl->last_column.lock());
}

ListViewColumnHeader ListViewHeader::remove(const std::string &column_id)
{
	std::shared_ptr<ListViewColumnHeader_Impl> cur = impl->first_column;
	while (cur)
	{
		if (cur->column_id == column_id)
		{
			ListViewColumnHeader column(cur);
			if (!impl->func_column_removed.is_null())
				impl->func_column_removed.invoke(column);
			if (!cur->prev_sibling.expired())
				cur->prev_sibling.lock()->next_sibling = cur->next_sibling;
			if (cur->next_sibling)
				cur->next_sibling->prev_sibling = cur->prev_sibling;
			if (impl->first_column == cur)
				impl->first_column = cur->next_sibling;
			if (impl->last_column.lock() == cur)
				impl->last_column = cur->prev_sibling;

			return column;
		}
		cur = cur->next_sibling;
	}
	throw Exception(string_format("No column found with column id: %1", column_id));
}

ListViewColumnHeader ListViewHeader::insert_after(const std::string &column_id, ListViewColumnHeader new_column)
{
	throw Exception("ListViewHeader::insert_after not implemented yet!");
	return ListViewColumnHeader();
}

ListViewColumnHeader ListViewHeader::insert_before(const std::string &column_id, ListViewColumnHeader new_column)
{
	throw Exception("ListViewHeader::insert_before not implemented yet!");
	return ListViewColumnHeader();
}

void ListViewHeader::set_display_mode(ListViewDisplayMode mode)
{
	impl->display_mode = mode;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewHeader Events:

Callback_v1<ListViewColumnHeader> &ListViewHeader::func_column_added()
{
	return impl->func_column_added; 
}

Callback_v1<ListViewColumnHeader> &ListViewHeader::func_column_removed()
{
	return impl->func_column_removed;
}

Callback_v1<ListViewColumnHeader> &ListViewHeader::func_column_size_changed()
{
	return impl->func_column_size_changed;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewHeader Implementation:

void ListViewHeader::update_geometry(const Rect &parent_content_rect)
{
	impl->update_geometry(parent_content_rect);
}

void ListViewHeader_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		
		InputEvent &input_event = input_msg->input_event;
		if (input_event.type == InputEvent::pointer_moved)
			on_mouse_move(msg, input_event);
		else if (input_event.type == InputEvent::pressed && input_event.id == mouse_left)
			on_mouse_lbutton_down(msg, input_event);
		else if (input_event.type == InputEvent::released && input_event.id == mouse_left)
			on_mouse_lbutton_up(msg, input_event);
	}
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
		else if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
			on_mouse_enter();
	}
}

void ListViewHeader_Impl::on_mouse_lbutton_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	Point pos = input_event.mouse_pos;

	ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		bool inside = col.impl->rect.contains(pos);
		col.impl->part.set_pseudo_class(CssStr::pressed, inside);
		col.impl->part.set_pseudo_class(CssStr::normal, !inside);
		col = col.get_next_sibling();
	}

	listview_header->request_repaint();
	listview_header->capture_mouse(true);
}

void ListViewHeader_Impl::on_mouse_lbutton_up(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		col.impl->part.set_pseudo_class(CssStr::pressed, false);
		col.impl->part.set_pseudo_class(CssStr::normal, true);
		col = col.get_next_sibling();
	}

	listview_header->request_repaint();
	listview_header->capture_mouse(false);
}


void ListViewHeader_Impl::on_mouse_move(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	Point pos = input_event.mouse_pos;
	bool current_rect_set = false;

	if (!current_mouse_in_rect.contains(pos))
	{
		ListViewColumnHeader col = first_column;
		while (!col.is_null())
		{
			bool inside = col.impl->rect.contains(pos);
			col.impl->part.set_pseudo_class(CssStr::hover, inside);
			col.impl->part.set_pseudo_class(CssStr::normal, !inside);
			if (inside)
			{
				current_rect_set = true;
				current_mouse_in_rect = col.impl->rect;
			}
			col = col.get_next_sibling();
		}
	}

	if (!current_rect_set)
	{
		current_mouse_in_rect = Rect(0,0,0,0);
	}

	listview_header->request_repaint();
}

void ListViewHeader_Impl::on_mouse_leave()
{
	ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		col.impl->part.set_pseudo_class(CssStr::hover, false);
		col.impl->part.set_pseudo_class(CssStr::normal, true);
		col = col.get_next_sibling();
	}

	listview_header->request_repaint();
}

void ListViewHeader_Impl::on_mouse_enter()
{
}

void ListViewHeader_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = listview_header->get_geometry();

	Font font = listview_header->get_font();
	// draw listview column headers and backgrounds
	text_height = font.get_text_size(canvas, "l").height;
	ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		GUIThemePart &part = col.impl->part;
		Rect &crect = col.impl->rect;
		if (display_mode != listview_mode_details)
			crect.right = rect.right;
		part.render_box(canvas, crect);
		Rect content_rect = part.get_content_box(crect);

		part.render_text(canvas, col.get_caption(), content_rect);

		col = col.get_next_sibling();

		if (display_mode != listview_mode_details)
			break;
	}
}


void ListViewHeader_Impl::update_geometry(const Rect &parent_content_rect)
{
	int header_height = listview_header->get_content_box().get_height();

	Rect rect_header = Rect(
		parent_content_rect.left,
		parent_content_rect.top,
		parent_content_rect.right-1,
		parent_content_rect.top+header_height);
	Rect rect(rect_header.get_size());

	listview_header->set_geometry(rect_header);

	Rect content_rect = listview_header->get_content_box();

	int xpos = content_rect.left;
	ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		Rect &R = col.impl->rect;
		R.left   = xpos;
		R.top    = 0;
		R.right  = xpos + col.get_width();
		R.bottom = header_height;

		if (col.get_next_sibling().is_null())
			R.right = content_rect.right;

		xpos += col.get_width();
		col = col.get_next_sibling();
	}
}

ListViewColumnHeader ListViewHeader_Impl::create_column(const std::string &column_id, const std::string &caption)
{
	std::shared_ptr<ListViewColumnHeader_Impl> col_impl(new ListViewColumnHeader_Impl());
	col_impl->part = GUIThemePart(listview_header, "columnheader");
	col_impl->part.set_pseudo_class(CssStr::normal, true);
	ListViewColumnHeader new_column = ListViewColumnHeader(col_impl);
	new_column.set_caption(caption);
	new_column.set_column_id(column_id);
	new_column.set_alignment(ListViewColumnHeader::align_left);
	new_column.func_size_changed().set(this, &ListViewHeader_Impl::on_column_size_changed);

	return new_column;
}

void ListViewHeader_Impl::on_column_size_changed(ListViewColumnHeader col)
{
	if (!func_column_size_changed.is_null())
		func_column_size_changed.invoke(col);
}

}
