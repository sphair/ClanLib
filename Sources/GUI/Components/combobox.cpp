/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/gui_theme.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/combobox.h"
#include "API/GUI/Components/lineedit.h"
#include "API/GUI/Components/push_button.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/Display/2D/image.h"
#include "API/Display/Window/keys.h"
#include "Menus/popupmenu_window.h"
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
	void on_lineedit_clicked();
	void on_btn_arrow_clicked();
	void on_lineedit_text_edited(CL_InputEvent event);
	void on_lineedit_unhandled_event(CL_InputEvent event);
	void on_popup_item_selected(CL_PopupMenuItem item);
	void on_popup_keyboard_event(CL_InputEvent event);

	void create_components();

	CL_Callback_v0 func_dropdown_opened;
	CL_Callback_v0 func_dropdown_closed;
	CL_Callback_v0 func_before_edit_changed;
	CL_Callback_v0 func_after_edit_changed;
	CL_Callback_v1<int> func_item_selected;
	CL_Callback_v1<int> func_selection_changed;
	CL_Callback_v1<CL_Rect> func_display_popup;

	CL_ComboBox *component;
	CL_LineEdit *lineedit;
	CL_PopupMenuWindow *menu_window;
	CL_ScrollBar *scrollbar;
	CL_PushButton *btn_arrow;
	CL_PopupMenu popup_menu;

	CL_GUIThemePart part_component;

	int dropdown_height_items;
	bool editable;
	int selected_item;
	int button_width;
	int item_height;
	int minimum_width;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox Construction:

CL_ComboBox::CL_ComboBox(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_ComboBox_Impl)
{
	set_type_name(CssStr::ComboBox::type_name);
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

void CL_ComboBox::set_editable(bool enable)
{
	impl->editable = enable;
}

void CL_ComboBox::set_dropdown_height(int height)
{
	impl->dropdown_height_items = height;
}

void CL_ComboBox::set_text(const CL_StringRef &text)
{
	impl->lineedit->set_text(text);
}

void CL_ComboBox::set_selected_item(int index)
{
	impl->selected_item = index;
	CL_StringRef str = get_item(index);
	impl->lineedit->set_text(str);
}

void CL_ComboBox::set_popup_menu( CL_PopupMenu &menu )
{
	impl->popup_menu = menu;
	menu.func_item_selected().set(impl.get(), &CL_ComboBox_Impl::on_popup_item_selected);
	menu.func_keyboard_event().set(impl.get(), &CL_ComboBox_Impl::on_popup_keyboard_event);
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

CL_Callback_v1<CL_Rect> & CL_ComboBox::func_display_popup()
{
	return impl->func_display_popup;
}

void CL_ComboBox::set_dropdown_minimum_width( int min_width )
{
	impl->minimum_width = min_width;
}

CL_Callback_1<bool, CL_InputEvent> &CL_ComboBox::func_lineedit_unhandled_event()
{
	return impl->lineedit->func_unhandled_event();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComboBox Implementation:

void CL_ComboBox_Impl::on_process_message(CL_GUIMessage &msg)
{
}

void CL_ComboBox_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_GUIThemePart part_component(component);
	CL_Rect rect = component->get_geometry();
	part_component.render_box(gc, rect, update_rect);
}

void CL_ComboBox_Impl::on_resized()
{
	CL_Rect g = component->get_geometry().get_size();
	
	btn_arrow->set_geometry(CL_Rect(g.right-g.get_height()+1, 1, g.right-1, g.bottom-1));
	lineedit->set_geometry(g);
}

void CL_ComboBox_Impl::on_style_changed()
{
}

void CL_ComboBox_Impl::create_components()
{
	lineedit = new CL_LineEdit(component);
	btn_arrow = new CL_PushButton(component);

	CL_GraphicContext gc = component->get_gc();
	CL_ResourceManager resources = component->get_resources();
	CL_Image icon(gc, cl_text("ComboBoxDownArrow"), &resources);

	btn_arrow->set_icon(icon);
	btn_arrow->func_clicked().set(this, &CL_ComboBox_Impl::on_btn_arrow_clicked);
	lineedit->func_after_edit_changed().set(this, &CL_ComboBox_Impl::on_lineedit_text_edited);
	lineedit->func_focus_gained().set(this, &CL_ComboBox_Impl::on_lineedit_clicked);
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

	//TODO: If this required, should be exec handling be handled by the main window manager
	if (component->get_gui_manager().get_window_manager()->get_window_manager_type() == CL_GUIWindowManager::cl_wm_type_system)
	{
		popup_menu.exec(component, g.get_bottom_left());
		popup_menu.set_minimum_width(old_width);
	}
	else
	{
		popup_menu.start(component, g.get_bottom_left());
	}
}

void CL_ComboBox_Impl::on_lineedit_text_edited(CL_InputEvent event)
{
	CL_TempString text = lineedit->get_text();	
	int index = popup_menu.find_item(text);
	selected_item = index;
	if (selected_item != -1 && event.id != CL_KEY_BACKSPACE && event.id != CL_KEY_DELETE)
	{
		CL_StringRef str = popup_menu.get_item_at(index).get_text();
		CL_String old_lineedit_text = lineedit->get_text();
		int selection_length = str.length() - old_lineedit_text.length();
		lineedit->set_text(str);
		lineedit->set_selection(old_lineedit_text.length(), selection_length);
	}

	if (!func_after_edit_changed.is_null())
		func_after_edit_changed.invoke();
}

void CL_ComboBox_Impl::on_popup_item_selected(CL_PopupMenuItem item)
{
	if (!item.is_null())
		component->set_text(item.get_text());

	if (!func_item_selected.is_null())
		func_item_selected.invoke(item.get_id());
}

void CL_ComboBox_Impl::on_lineedit_clicked()
{
	on_btn_arrow_clicked();
}

void CL_ComboBox_Impl::on_lineedit_unhandled_event(CL_InputEvent event)
{
	if (event.id == CL_KEY_DOWN)
	{
		on_btn_arrow_clicked();
	}
}

void CL_ComboBox_Impl::on_popup_keyboard_event(CL_InputEvent event)
{
	CL_GUIMessage_Input input_msg;
	input_msg.set_target(lineedit);
	input_msg.set_event(event);
	component->get_gui_manager().send_message(input_msg);
}
