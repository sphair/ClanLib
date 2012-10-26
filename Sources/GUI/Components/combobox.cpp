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
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_window_manager.h"
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ComboBox_Impl Class:

class ComboBox_Impl
{
public:
	ComboBox_Impl() : dropdown_height_items(0), editable(false), selected_item(-1), button_width(17), item_height(17), minimum_width(-1)
	{
	}

	void on_process_message(GUIMessage &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_resized();
	void on_style_changed();
	void on_lineedit_message(GUIMessage &msg);
	void on_lineedit_enter_pressed();
	void on_btn_arrow_clicked();
	void on_lineedit_text_edited(InputEvent &event);
	bool on_lineedit_unhandled_event(InputEvent &event);
	void on_popup_item_selected(PopupMenuItem item);
	void on_popup_menu_closed();
	void on_enablemode_changed();
	void create_components();

	Callback_v0 func_dropdown_opened;
	Callback_v0 func_dropdown_closed;
	Callback_v0 func_before_edit_changed;
	Callback_v0 func_after_edit_changed;
	Callback_v0 func_enter_pressed;
	Callback_v1<int> func_item_selected;
	Callback_v1<int> func_selection_changed;
	Callback_1<bool, InputEvent> func_lineedit_unhandled_event;

	ComboBox *component;
	LineEdit *lineedit;
	PopupMenu popup_menu;

	GUIThemePart part_component;
	GUIThemePart part_opener;
	GUIThemePart part_opener_glyph;

	int dropdown_height_items;
	bool editable;
	int selected_item;
	int button_width;
	int item_height;
	int minimum_width;
	Rect opener_rect;

	class NoLoopHack : public GUIMessageData
	{
	};
};

/////////////////////////////////////////////////////////////////////////////
// ComboBox Construction:

ComboBox::ComboBox(GUIComponent *parent)
: GUIComponent(parent), impl(new ComboBox_Impl)
{
	set_tag_name(CssStr::ComboBox::type_name);
	set_focus_policy(focus_local);
	impl->component = this;
	func_process_message().set(impl.get(), &ComboBox_Impl::on_process_message);
	func_render().set(impl.get(), &ComboBox_Impl::on_render);
	func_resized().set(impl.get(), &ComboBox_Impl::on_resized);
	func_style_changed().set(impl.get(), &ComboBox_Impl::on_style_changed);
	// todo: enablemode
	func_enablemode_changed().set(impl.get(), &ComboBox_Impl::on_enablemode_changed);

	impl->create_components();
}

ComboBox::~ComboBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// ComboBox Attributes:

ComboBox *ComboBox::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	ComboBox *object = NULL;
	if (reference_component)
		object = dynamic_cast<ComboBox*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find ComboBox named item: %1", id));

	return object;
}

bool ComboBox::is_editable() const
{
	return impl->editable;
}

int ComboBox::get_dropdown_height() const
{
	return impl->dropdown_height_items;
}

int ComboBox::get_selected_item() const
{
	return impl->selected_item;
}

std::string ComboBox::get_text() const
{
	return impl->lineedit->get_text();
}

std::string ComboBox::get_item(int index) const
{
	PopupMenu const_hack = impl->popup_menu;
	PopupMenuItem pmi = const_hack.get_item(index);
	return pmi.get_text();
}

/////////////////////////////////////////////////////////////////////////////
// ComboBox Operations:

void ComboBox::set_editable(bool editable)
{
	impl->editable = editable;
	request_repaint();
}

void ComboBox::set_dropdown_height(int height)
{
	impl->dropdown_height_items = height;
}

void ComboBox::set_text(const std::string &text)
{
	impl->lineedit->set_text(text);
	if (impl->lineedit->has_focus())
		impl->lineedit->select_all();
}

void ComboBox::set_selected_item(int index)
{
	impl->selected_item = index;
	std::string str = get_item(index);
	impl->lineedit->set_text(str);
	if (impl->lineedit->has_focus() || has_focus())
		impl->lineedit->select_all();
}

void ComboBox::set_popup_menu(PopupMenu &menu)
{
	impl->popup_menu = menu;

	menu.func_close().set(impl.get(), &ComboBox_Impl::on_popup_menu_closed);

	// Assumes the popup menu only has one level. If you want submenu's, feel free to extend this function :)
	int max = menu.get_item_count();
	for (int cnt=0; cnt < max; cnt++)
	{
		PopupMenuItem item = menu.get_item_at(cnt);
		if (!item.is_null())
		{
			item.func_clicked().set(impl.get(), &ComboBox_Impl::on_popup_item_selected, item);
		}
	}
}

void ComboBox::set_dropdown_minimum_width(int min_width)
{
	impl->minimum_width = min_width;
}

/////////////////////////////////////////////////////////////////////////////
// ComboBox Events:

Callback_v0 &ComboBox::func_dropdown_opened()
{
	return impl->func_dropdown_opened;
}

Callback_v0 &ComboBox::func_dropdown_closed()
{
	return impl->func_dropdown_closed;
}

Callback_v0 &ComboBox::func_enter_pressed()
{
	return impl->func_enter_pressed;
}

Callback_v0 &ComboBox::func_before_edit_changed()
{
	return impl->func_before_edit_changed;
}

Callback_v0 &ComboBox::func_after_edit_changed()
{
	return impl->func_after_edit_changed;
}

Callback_v1<int> &ComboBox::func_selection_changed()
{
	return impl->func_selection_changed;
}

Callback_v1<int> & ComboBox::func_item_selected()
{
	return impl->func_item_selected;
}

/////////////////////////////////////////////////////////////////////////////
// ComboBox Implementation:

void ComboBox_Impl::on_enablemode_changed()
{
	lineedit->set_enabled(component->is_enabled());
	lineedit->request_repaint();
	component->request_repaint();
}

void ComboBox_Impl::on_process_message(GUIMessage &msg)
{
	if(!component->is_enabled())
		return;

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::pressed)
		{
			if (e.id == mouse_left)
			{
				bool in_rect = Rect(component->get_size()).contains(e.mouse_pos);
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
			if (e.id == keycode_down && popup_menu.get_item_count() > 0)
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
			else if (e.id == keycode_up && popup_menu.get_item_count() > 0)
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
			else if (e.id == keycode_enter || e.id == keycode_return || e.id == keycode_numpad_enter)
			{
				if (!func_enter_pressed.is_null())
					func_enter_pressed.invoke();
				msg.set_consumed();
			}
			else if (editable && 
				!msg.is_consumed() &&
				e.id != keycode_tab && 
				e.id != keycode_enter && 
				e.id != keycode_numpad_enter &&
				e.id != keycode_escape &&
				!msg.get_data("No Loop Hack"))
			{
				GUIMessage_Input input_msg;
				input_msg.set_target(lineedit);
				input_msg.set_event(e);
				input_msg.set_data("No Loop Hack", std::shared_ptr<NoLoopHack>(new NoLoopHack()));
				component->get_gui_manager().dispatch_message(input_msg);
				msg.set_consumed();
			}
		}
		else if (e.type == InputEvent::released)
		{
			part_component.set_state(CssStr::pressed, false);
			part_opener.set_state(CssStr::pressed, false);
			part_opener_glyph.set_state(CssStr::pressed, false);
			component->request_repaint();
		}
		else if (e.type == InputEvent::pointer_moved)
		{
			bool state_changed = false;
			bool in_opener_rect = opener_rect.contains(e.mouse_pos);
			state_changed |= part_opener.set_state(CssStr::hot, in_opener_rect);
			state_changed |= part_opener_glyph.set_state(CssStr::hot, in_opener_rect);

			if (state_changed)
				component->request_repaint();
		}
	}
	else if (msg.is_type(GUIMessage_Pointer::get_type_name()))
	{
		GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == GUIMessage_Pointer::pointer_enter)
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
	else if (msg.is_type(GUIMessage_FocusChange::get_type_name()))
	{
		GUIMessage_FocusChange fmsg(msg);

		if (fmsg.get_focus_type() == GUIMessage_FocusChange::gained_focus)
		{
			lineedit->select_all();
			lineedit->set_cursor_drawing_enabled(editable);
		}
		else if (fmsg.get_focus_type() == GUIMessage_FocusChange::losing_focus)
		{
			lineedit->clear_selection();

			fmsg.set_target(lineedit);
			component->get_gui_manager().dispatch_message(fmsg);
		}
	}
}
	
void ComboBox_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = component->get_size();
	part_component.render_box(canvas, rect, update_rect);
	part_opener.render_box(canvas, opener_rect, update_rect);
	part_opener_glyph.render_box(canvas, opener_rect, update_rect);
}

void ComboBox_Impl::on_resized()
{
	Rect g = component->get_size();
	Rect content_rect = part_component.get_content_box(g);

	int opener_width = part_opener.get_preferred_width();
	opener_rect = Rect(content_rect.right-opener_width, content_rect.top, content_rect.right, content_rect.bottom);
	lineedit->set_geometry(Rect(content_rect.top, content_rect.left, content_rect.right-opener_width,content_rect.bottom));
}

void ComboBox_Impl::on_style_changed()
{
}

void ComboBox_Impl::create_components()
{
	part_component = GUIThemePart(component);
	part_opener = GUIThemePart(component, "opener");
	part_opener_glyph = GUIThemePart(component, "opener_glyph");

	part_component.set_state(CssStr::normal, true);
	part_opener.set_state(CssStr::normal, true);
	part_opener_glyph.set_state(CssStr::normal, true);
	part_component.set_state(CssStr::hot, false);
	part_opener.set_state(CssStr::hot, false);
	part_opener_glyph.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::pressed, false);
	part_opener.set_state(CssStr::pressed, false);
	part_opener_glyph.set_state(CssStr::pressed, false);

	lineedit = new LineEdit(component);
	lineedit->set_focus_policy(GUIComponent::focus_parent);
	
	lineedit->func_after_edit_changed().set(this, &ComboBox_Impl::on_lineedit_text_edited);
	lineedit->func_enter_pressed().set(this, &ComboBox_Impl::on_lineedit_enter_pressed);
	lineedit->func_filter_message().set(this, &ComboBox_Impl::on_lineedit_message);
}

void ComboBox_Impl::on_btn_arrow_clicked()
{
	popup_menu.set_maximum_height(dropdown_height_items*item_height); 
	Rect g = component->get_geometry().get_size();
	if (minimum_width != -1)
		popup_menu.set_minimum_width(minimum_width);
	else
		popup_menu.set_minimum_width(g.get_width());

	popup_menu.start(component, component->component_to_screen_coords(g.get_bottom_left()));
}

void ComboBox_Impl::on_lineedit_enter_pressed()
{
	if (!func_enter_pressed.is_null())
		func_enter_pressed.invoke();
}

void ComboBox_Impl::on_lineedit_text_edited(InputEvent &event)
{
	if (!func_after_edit_changed.is_null())
		func_after_edit_changed.invoke();
}

void ComboBox_Impl::on_popup_menu_closed()
{
	part_component.set_state(CssStr::pressed, false);
	part_opener.set_state(CssStr::pressed, false);
	part_opener_glyph.set_state(CssStr::pressed, false);
	part_component.set_state(CssStr::hot, false);
	part_opener.set_state(CssStr::hot, false);
	part_opener_glyph.set_state(CssStr::hot, false);

	component->request_repaint();
}

void ComboBox_Impl::on_popup_item_selected(PopupMenuItem item)
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

void ComboBox_Impl::on_lineedit_message(GUIMessage &msg)
{
	if(!component->is_enabled())
		return;

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::pressed)
		{
			if (e.id == mouse_left)
			{
				bool in_rect = Rect(component->get_size()).contains(e.mouse_pos);
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

}
