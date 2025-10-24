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
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/gui_theme.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/combobox.h"
#include "API/GUI/Components/lineedit.h"
#include "API/GUI/Components/push_button.h"
#include "API/GUI/Components/popupmenu_item.h"
#include "API/GUI/Components/popupmenu.h"
#include "API/Display/2D/image.h"
#include "API/Display/Window/keys.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox_Impl Class:

class CL_ComboBox_Impl
{
public:
	CL_ComboBox_Impl() : dropdown_height_items(0), editable(false), selected_item(-1), button_width(17), item_height(17), minimum_width(-1)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();
	void on_style_changed();
	void on_lineedit_message(CL_GUIMessage &msg);
	void on_btn_arrow_clicked();
	void on_lineedit_text_edited(CL_InputEvent &event);
	bool on_lineedit_unhandled_event(CL_InputEvent &event);
	void on_popup_item_selected(CL_PopupMenuItem item);
	void on_popup_menu_closed();

	void create_components();

	CL_Callback_v0 func_dropdown_opened;
	CL_Callback_v0 func_dropdown_closed;
	CL_Callback_v0 func_before_edit_changed;
	CL_Callback_v0 func_after_edit_changed;
	CL_Callback_v1<int> func_item_selected;
	CL_Callback_v1<int> func_selection_changed;
	CL_Callback_1<bool, CL_InputEvent> func_lineedit_unhandled_event;

	CL_ComboBox *component;
	CL_LineEdit *lineedit;
	CL_PopupMenu popup_menu;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_opener;
	CL_GUIThemePart part_opener_glyph;

	int dropdown_height_items;
	bool editable;
	int selected_item;
	int button_width;
	int item_height;
	int minimum_width;
	CL_Rect opener_rect;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox Construction:

CL_ComboBox::CL_ComboBox(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_ComboBox_Impl)
{
	set_type_name(CssStr::ComboBox::type_name);
	set_focus_policy(focus_local);
	impl->component = this;
	func_process_message().set(impl.get(), &CL_ComboBox_Impl::on_process_message);
	func_render().set(impl.get(), &CL_ComboBox_Impl::on_render);
	func_resized().set(impl.get(), &CL_ComboBox_Impl::on_resized);
	func_style_changed().set(impl.get(), &CL_ComboBox_Impl::on_style_changed);
	// todo: enablemode

	impl->create_components();
}

CL_ComboBox::~CL_ComboBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox Attributes:

CL_ComboBox *CL_ComboBox::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_ComboBox *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_ComboBox*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_ComboBox named item: %1", id));

	return object;
}

bool CL_ComboBox::is_editable() const
{
	return impl->editable;
}

int CL_ComboBox::get_dropdown_height() const
{
	return impl->dropdown_height_items;
}

int CL_ComboBox::get_selected_item() const
{
	return impl->selected_item;
}

CL_StringRef CL_ComboBox::get_text() const
{
	return impl->lineedit->get_text();
}

CL_StringRef CL_ComboBox::get_item(int index) const
{
	CL_PopupMenu const_hack = impl->popup_menu;
	CL_PopupMenuItem pmi = const_hack.get_item(index);
	return pmi.get_text();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox Operations:

void CL_ComboBox::set_editable(bool editable)
{
	impl->editable = editable;
	request_repaint();
}

void CL_ComboBox::set_dropdown_height(int height)
{
	impl->dropdown_height_items = height;
}

void CL_ComboBox::set_text(const CL_StringRef &text)
{
	impl->lineedit->set_text(text);
	if (impl->lineedit->has_focus())
		impl->lineedit->select_all();
}

void CL_ComboBox::set_selected_item(int index)
{
	impl->selected_item = index;
	CL_StringRef str = get_item(index);
	impl->lineedit->set_text(str);
	if (impl->lineedit->has_focus() || has_focus())
		impl->lineedit->select_all();
}

void CL_ComboBox::set_popup_menu(CL_PopupMenu &menu)
{
	impl->popup_menu = menu;

	menu.func_close().set(impl.get(), &CL_ComboBox_Impl::on_popup_menu_closed);

	// Assumes the popup menu only has one level. If you want submenu's, feel free to extend this function :)
	int max = menu.get_item_count();
	for (int cnt=0; cnt < max; cnt++)
	{
		CL_PopupMenuItem item = menu.get_item_at(cnt);
		if (!item.is_null())
		{
			item.func_clicked().set(impl.get(), &CL_ComboBox_Impl::on_popup_item_selected, item);
		}
	}
}

void CL_ComboBox::set_dropdown_minimum_width(int min_width)
{
	impl->minimum_width = min_width;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox Events:

CL_Callback_v0 &CL_ComboBox::func_dropdown_opened()
{
	return impl->func_dropdown_opened;
}

CL_Callback_v0 &CL_ComboBox::func_dropdown_closed()
{
	return impl->func_dropdown_closed;
}

CL_Callback_v0 &CL_ComboBox::func_before_edit_changed()
{
	return impl->func_before_edit_changed;
}

CL_Callback_v0 &CL_ComboBox::func_after_edit_changed()
{
	return impl->func_after_edit_changed;
}

CL_Callback_v1<int> &CL_ComboBox::func_selection_changed()
{
	return impl->func_selection_changed;
}

CL_Callback_v1<int> & CL_ComboBox::func_item_selected()
{
	return impl->func_item_selected;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox Implementation:

void CL_ComboBox_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed)
		{
			if (e.id == CL_MOUSE_LEFT)
			{
				bool in_rect = CL_Rect(component->get_size()).contains(e.mouse_pos);
				bool in_opener_rect = opener_rect.contains(e.mouse_pos);

				if (in_opener_rect)
				{
					part_opener.set_state(CssStr::pressed, true);
					part_opener_glyph.set_state(CssStr::pressed, true);
					on_btn_arrow_clicked();
				}
				else if (in_rect)
				{
					part_component.set_state(CssStr::pressed, true);
					on_btn_arrow_clicked();
				}

				component->request_repaint();
				msg.set_consumed();
			}
			if (e.id == CL_KEY_DOWN && popup_menu.get_item_count() > 0)
			{
				int old_selected_item = selected_item;
				selected_item++;
				if (selected_item >= popup_menu.get_item_count())
					selected_item = popup_menu.get_item_count()-1;
				if (selected_item != old_selected_item)
				{
					component->set_selected_item(selected_item);
					func_item_selected.invoke(selected_item);
				}
				msg.set_consumed();
			}
			else if (e.id == CL_KEY_UP && popup_menu.get_item_count() > 0)
			{
				int old_selected_item = selected_item;
				selected_item--;
				if (selected_item < 0)
					selected_item = 0;
				if (selected_item != old_selected_item)
				{
					component->set_selected_item(selected_item);
					func_item_selected.invoke(selected_item);
				}
				msg.set_consumed();
			}
			else if (editable && 
				!msg.is_consumed() &&
				e.id != CL_KEY_TAB && 
				e.id != CL_KEY_ENTER && 
				e.id != CL_KEY_ESCAPE &&
				msg.get_data("No Loop Hack").is_null())
			{
				CL_GUIMessage_Input input_msg;
				input_msg.set_target(lineedit);
				input_msg.set_event(e);
				input_msg.set_data("No Loop Hack", CL_SharedPtr<int>(new int(1337)));
				component->get_gui_manager().dispatch_message(input_msg);
				msg.set_consumed();
			}
		}
		else if (e.type == CL_InputEvent::released)
		{
			part_component.set_state(CssStr::pressed, false);
			part_opener.set_state(CssStr::pressed, false);
			part_opener_glyph.set_state(CssStr::pressed, false);
			component->request_repaint();
		}
		else if (e.type == CL_InputEvent::pointer_moved)
		{
			bool state_changed = false;
			bool in_opener_rect = opener_rect.contains(e.mouse_pos);
			state_changed |= part_opener.set_state(CssStr::hot, in_opener_rect);
			state_changed |= part_opener_glyph.set_state(CssStr::hot, in_opener_rect);

			if (state_changed)
				component->request_repaint();
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
		{
			part_component.set_state(CssStr::hot, true);
			component->request_repaint();
		}
		else
		{
			part_component.set_state(CssStr::hot, false);
			part_opener.set_state(CssStr::hot, false);
			part_opener_glyph.set_state(CssStr::hot, false);
			component->request_repaint();
		}
	}
	else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name()))
	{
		CL_GUIMessage_FocusChange fmsg(msg);

		if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus)
		{
			lineedit->select_all();
			lineedit->set_cursor_drawing_enabled(editable);
		}
		else if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::losing_focus)
		{
			lineedit->clear_selection();

			fmsg.set_target(lineedit);
			component->get_gui_manager().dispatch_message(fmsg);
		}
	}
}
	
void CL_ComboBox_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = component->get_size();
	part_component.render_box(gc, rect, update_rect);
	part_opener.render_box(gc, opener_rect, update_rect);
	part_opener_glyph.render_box(gc, opener_rect, update_rect);
}

void CL_ComboBox_Impl::on_resized()
{
	CL_Rect g = component->get_size();
	CL_Rect content_rect = part_component.get_content_box(g);

	int opener_width = part_opener.get_preferred_width();
	opener_rect = CL_Rect(content_rect.right-opener_width, content_rect.top, content_rect.right, content_rect.bottom);
	lineedit->set_geometry(CL_Rect(content_rect.top, content_rect.left, content_rect.right-opener_width,content_rect.bottom));
}

void CL_ComboBox_Impl::on_style_changed()
{
}

void CL_ComboBox_Impl::create_components()
{
	part_component = CL_GUIThemePart(component);
	part_opener = CL_GUIThemePart(component, "opener");
	part_opener_glyph = CL_GUIThemePart(component, "opener_glyph");

	part_component.set_state(CssStr::normal, true);
	part_opener.set_state(CssStr::normal, true);
	part_opener_glyph.set_state(CssStr::normal, true);
	part_component.set_state(CssStr::hot, false);
	part_opener.set_state(CssStr::hot, false);
	part_opener_glyph.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::pressed, false);
	part_opener.set_state(CssStr::pressed, false);
	part_opener_glyph.set_state(CssStr::pressed, false);

	lineedit = new CL_LineEdit(component);
	lineedit->set_focus_policy(CL_GUIComponent::focus_parent);
	
	CL_GraphicContext &gc = component->get_gc();

	lineedit->func_after_edit_changed().set(this, &CL_ComboBox_Impl::on_lineedit_text_edited);
	lineedit->func_filter_message().set(this, &CL_ComboBox_Impl::on_lineedit_message);
}

void CL_ComboBox_Impl::on_btn_arrow_clicked()
{
	popup_menu.set_maximum_height(dropdown_height_items*item_height); 
	CL_Rect g = component->get_geometry().get_size();
	int old_width = popup_menu.get_minimum_width();
	if (minimum_width != -1)
		popup_menu.set_minimum_width(minimum_width);
	else
		popup_menu.set_minimum_width(g.get_width());

	popup_menu.start(component, component->component_to_screen_coords(g.get_bottom_left()));
}

void CL_ComboBox_Impl::on_lineedit_text_edited(CL_InputEvent &event)
{
/*  What is this supposed to do?? - Harry 2009-07-29

	CL_String text = lineedit->get_text();	
	int index = popup_menu.find_item(text);
	selected_item = index;
	if (selected_item != -1 && event.id != CL_KEY_BACKSPACE && 
		event.id != CL_KEY_DELETE && 
		event.id != CL_KEY_LEFT && 
		event.id != CL_KEY_RIGHT)
	{
		CL_StringRef str = popup_menu.get_item_at(index).get_text();
		CL_String old_lineedit_text = lineedit->get_text();
		int selection_length = str.length() - old_lineedit_text.length();
		lineedit->set_text(str);
		lineedit->set_selection(old_lineedit_text.length(), selection_length);
	}
*/
	if (!func_after_edit_changed.is_null())
		func_after_edit_changed.invoke();
}

void CL_ComboBox_Impl::on_popup_menu_closed()
{
	part_component.set_state(CssStr::pressed, false);
	part_opener.set_state(CssStr::pressed, false);
	part_opener_glyph.set_state(CssStr::pressed, false);
	part_component.set_state(CssStr::hot, false);
	part_opener.set_state(CssStr::hot, false);
	part_opener_glyph.set_state(CssStr::hot, false);

	component->request_repaint();
}

void CL_ComboBox_Impl::on_popup_item_selected(CL_PopupMenuItem item)
{
	if (!item.is_null())
	{
		selected_item = item.get_id();
		component->set_text(item.get_text());

		part_component.set_state(CssStr::pressed, false);
		part_opener.set_state(CssStr::pressed, false);
		part_opener_glyph.set_state(CssStr::pressed, false);

		if (!func_item_selected.is_null())
			func_item_selected.invoke(selected_item);
	}
}

void CL_ComboBox_Impl::on_lineedit_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed)
		{
			if (e.id == CL_MOUSE_LEFT)
			{
				bool in_rect = CL_Rect(component->get_size()).contains(e.mouse_pos);
				if (in_rect)
				{
					part_component.set_state(CssStr::pressed, true);
					if (editable)
						lineedit->set_focus();
					else
						on_btn_arrow_clicked();

					component->request_repaint();
				}

				msg.set_consumed();
			}
		}
	}
}
