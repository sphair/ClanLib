/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
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

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewHeader_Impl Class:

class CL_ListViewHeader_Impl
{
public:
	CL_ListViewHeader_Impl()
	: listview_header(0), display_mode(listview_mode_details), text_height(0),
	  visible(true), current_mouse_in_rect(0,0,0,0)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void update_geometry(const CL_Rect &content_rect);
	void create_parts();
	void on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_leave();
	void on_mouse_enter();
	void on_column_size_changed(CL_ListViewColumnHeader col);
	CL_ListViewColumnHeader create_column(const CL_StringRef &column_id, const CL_StringRef &caption);

	CL_ListViewHeader *listview_header;
	CL_Font font;
	CL_GUIThemePart part_component;
	CL_ListViewDisplayMode display_mode;
	int text_height;
	bool visible;
	CL_Rect current_mouse_in_rect;
	CL_SharedPtr<CL_ListViewColumnHeader_Impl> first_column;
	CL_WeakPtr<CL_ListViewColumnHeader_Impl> last_column;
	CL_Callback_v1<CL_ListViewColumnHeader> func_column_added;
	CL_Callback_v1<CL_ListViewColumnHeader> func_column_removed;
	CL_Callback_v1<CL_ListViewColumnHeader> func_column_size_changed;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewHeader Construction:

CL_ListViewHeader::CL_ListViewHeader(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_ListViewHeader_Impl)
{
	set_type_name(CssStr::ListView::Header::type_name);
	impl->listview_header = this;
	func_process_message().set(impl.get(), &CL_ListViewHeader_Impl::on_process_message);
	func_render().set(impl.get(), &CL_ListViewHeader_Impl::on_render);
	// todo: resize (?)
	// todo: disabled
	impl->create_parts();
}

CL_ListViewHeader::~CL_ListViewHeader()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewHeader Attributes:

CL_ListViewColumnHeader CL_ListViewHeader::get_first_column()
{
	return impl->first_column;
}

CL_ListViewColumnHeader CL_ListViewHeader::get_column(const CL_StringRef &column_id)
{	
	CL_ListViewColumnHeader	col = impl->first_column;
	while (!col.is_null())
	{
		if (col.get_column_id() == column_id)
			return col;
		col = col.get_next_sibling();
	}

	return CL_ListViewColumnHeader();
}

bool CL_ListViewHeader::get_visible() const
{
	return impl->visible;	
}

CL_Size CL_ListViewHeader::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}


/////////////////////////////////////////////////////////////////////////////
// CL_ListViewHeader Operations:

CL_ListViewColumnHeader CL_ListViewHeader::create_column(const CL_StringRef &column_id, const CL_StringRef &caption)
{
	return impl->create_column(column_id, caption);
}

CL_ListViewColumnHeader CL_ListViewHeader::append(CL_ListViewColumnHeader column)
{
	if (!impl->first_column.is_null())
	{
		impl->last_column->next_sibling = column.impl;
		column.impl->prev_sibling = impl->last_column;
		impl->last_column = column.impl;
	}
	else
	{
		impl->first_column = column.impl;
		impl->last_column = column.impl;
	}

	if (!impl->func_column_added.is_null())
		impl->func_column_added.invoke(CL_ListViewColumnHeader(impl->last_column));

	return CL_ListViewColumnHeader(impl->last_column);
}

CL_ListViewColumnHeader CL_ListViewHeader::remove(const CL_StringRef &column_id)
{
	throw CL_Exception("CL_ListViewHeader::remove not implemented yet!");
	return CL_ListViewColumnHeader();
}

CL_ListViewColumnHeader CL_ListViewHeader::insert_after(const CL_StringRef &column_id, CL_ListViewColumnHeader new_column)
{
	throw CL_Exception("CL_ListViewHeader::insert_after not implemented yet!");
	return CL_ListViewColumnHeader();
}

CL_ListViewColumnHeader CL_ListViewHeader::insert_before(const CL_StringRef &column_id, CL_ListViewColumnHeader new_column)
{
	throw CL_Exception("CL_ListViewHeader::insert_before not implemented yet!");
	return CL_ListViewColumnHeader();
}

void CL_ListViewHeader::set_display_mode(CL_ListViewDisplayMode mode)
{
	impl->display_mode = mode;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewHeader Events:

CL_Callback_v1<CL_ListViewColumnHeader> &CL_ListViewHeader::func_column_added()
{
	return impl->func_column_added; 
}

CL_Callback_v1<CL_ListViewColumnHeader> &CL_ListViewHeader::func_column_removed()
{
	return impl->func_column_removed;
}

CL_Callback_v1<CL_ListViewColumnHeader> &CL_ListViewHeader::func_column_size_changed()
{
	return impl->func_column_size_changed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewHeader Implementation:

void CL_ListViewHeader::update_geometry(const CL_Rect &parent_content_rect)
{
	impl->update_geometry(parent_content_rect);
}

void CL_ListViewHeader_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input = msg;
		CL_InputEvent input_event = input.get_event();
		if (input_event.type == CL_InputEvent::pointer_moved)
			on_mouse_move(input, input_event);
		else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_LEFT)
			on_mouse_lbutton_down(input, input_event);
		else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_LEFT)
			on_mouse_lbutton_up(input, input_event);
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
		else if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
			on_mouse_enter();
	}
}

void CL_ListViewHeader_Impl::on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	CL_Point pos = input_event.mouse_pos;

	CL_ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		bool inside = col.impl->rect.contains(pos);
		col.impl->part.set_state(CssStr::pressed, inside);
		col.impl->part.set_state(CssStr::normal, !inside);
		col = col.get_next_sibling();
	}

	listview_header->request_repaint();
	listview_header->capture_mouse(true);
}

void CL_ListViewHeader_Impl::on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	CL_ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		col.impl->part.set_state(CssStr::pressed, false);
		col.impl->part.set_state(CssStr::normal, true);
		col = col.get_next_sibling();
	}

	listview_header->request_repaint();
	listview_header->capture_mouse(false);
}


void CL_ListViewHeader_Impl::on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	CL_Point pos = input_event.mouse_pos;
	bool current_rect_set = false;

	if (!current_mouse_in_rect.contains(pos))
	{
		CL_ListViewColumnHeader col = first_column;
		while (!col.is_null())
		{
			bool inside = col.impl->rect.contains(pos);
			col.impl->part.set_state(CssStr::hot, inside);
			col.impl->part.set_state(CssStr::normal, !inside);
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
		current_mouse_in_rect = CL_Rect(0,0,0,0);
	}

	listview_header->request_repaint();
}

void CL_ListViewHeader_Impl::on_mouse_leave()
{
	part_component.set_state(CssStr::hot, false);
	CL_ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		col.impl->part.set_state(CssStr::hot, false);
		col.impl->part.set_state(CssStr::normal, true);
		col = col.get_next_sibling();
	}

	listview_header->request_repaint();
}

void CL_ListViewHeader_Impl::on_mouse_enter()
{
	part_component.set_state(CssStr::hot, true);
}

void CL_ListViewHeader_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = listview_header->get_geometry();
	part_component.render_box(gc, rect.get_size(), update_rect);
	
	// draw listview column headers and backgrounds
	text_height = font.get_text_size(gc, "l").height;
	int xpos = 0;
	CL_ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		CL_GUIThemePart &part = col.impl->part;
		CL_Rect &crect = col.impl->rect;
		if (display_mode != listview_mode_details)
			crect.right = rect.right;
		part.render_box(gc, crect, update_rect);
		CL_Rect content_rect = part.get_content_box(crect);

		part.render_text(gc, col.get_caption(), content_rect, update_rect);

		col = col.get_next_sibling();

		if (display_mode != listview_mode_details)
			break;
	}
}


void CL_ListViewHeader_Impl::update_geometry(const CL_Rect &parent_content_rect)
{
	int header_height = part_component.get_preferred_height();

	CL_Rect rect_header = CL_Rect(
		parent_content_rect.left,
		parent_content_rect.top,
		parent_content_rect.right-1,
		parent_content_rect.top+header_height);
	CL_Rect rect(rect_header.get_size());

	listview_header->set_geometry(rect_header);

	CL_Rect content_rect = part_component.get_content_box(listview_header->get_geometry().get_size());

	int xpos = content_rect.left;
	CL_ListViewColumnHeader col = first_column;
	while (!col.is_null())
	{
		CL_Rect &R = col.impl->rect;
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

void CL_ListViewHeader_Impl::create_parts()
{
	part_component = CL_GUIThemePart(listview_header);
	font = part_component.get_font();
}

CL_ListViewColumnHeader CL_ListViewHeader_Impl::create_column(const CL_StringRef &column_id, const CL_StringRef &caption)
{
	CL_SharedPtr<CL_ListViewColumnHeader_Impl> col_impl(new CL_ListViewColumnHeader_Impl());
	col_impl->part = CL_GUIThemePart(listview_header, "columnheader");
	col_impl->part.set_state(CssStr::normal, true);
	CL_ListViewColumnHeader new_column = CL_ListViewColumnHeader(col_impl);
	new_column.set_caption(caption);
	new_column.set_column_id(column_id);
	new_column.set_alignment(CL_ListViewColumnHeader::align_left);
	new_column.func_size_changed().set(this, &CL_ListViewHeader_Impl::on_column_size_changed);

	return new_column;
}

void CL_ListViewHeader_Impl::on_column_size_changed(CL_ListViewColumnHeader col)
{
	if (!func_column_size_changed.is_null())
		func_column_size_changed.invoke(col);
}
