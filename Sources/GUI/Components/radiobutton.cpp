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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/radiobutton.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_RadioButton_Impl Class:

class CL_RadioButton_Impl
{
public:
	CL_RadioButton_Impl() : id(-1)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_enablemode_changed();
	void create_parts();

	CL_RadioButton *radio;
	CL_Callback_v0 func_selected;
	CL_Callback_v0 func_unselected;
	CL_Callback_v1<CL_RadioButton*> func_group_selection_changed;
	CL_String text;
	CL_String group_name;
	int id;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_checker;
	CL_GUIThemePartProperty prop_text_gap;
	CL_GUIThemePartProperty prop_text_color;

	// Returns the group changed callback, if it is set on one radio button in the group.
	CL_Callback_v1<CL_RadioButton*> uncheck_radio_buttons(CL_GUIComponent *parent);
};

/////////////////////////////////////////////////////////////////////////////
// CL_RadioButton Construction:

CL_RadioButton::CL_RadioButton(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_RadioButton_Impl)
{
	set_type_name(CssStr::RadioButton::type_name);

	impl->radio = this;
	impl->prop_text_gap = CL_GUIThemePartProperty(CssStr::text_gap, cl_text("2"));
	impl->prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, cl_text("black"));

	impl->create_parts();

	func_process_message().set(impl.get(), &CL_RadioButton_Impl::on_process_message);
	func_render().set(impl.get(), &CL_RadioButton_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_RadioButton_Impl::on_style_changed);
	func_enablemode_changed().set(impl.get(), &CL_RadioButton_Impl::on_enablemode_changed);
}

CL_RadioButton::~CL_RadioButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_RadioButton Attributes:

CL_StringRef CL_RadioButton::get_text() const
{
	return impl->text;
}

int CL_RadioButton::get_id() const
{
	return impl->id;
}

CL_StringRef CL_RadioButton::get_group_name() const
{
	return impl->group_name;
}

bool CL_RadioButton::is_selected() const
{
	return impl->part_checker.get_state(CssStr::checked);
}

/////////////////////////////////////////////////////////////////////////////
// CL_RadioButton Operations:

void CL_RadioButton::set_text(const CL_StringRef &text)
{
	impl->text = text;
}

void CL_RadioButton::set_id(int id)
{
	impl->id = id;
}

void CL_RadioButton::set_selected(bool selected)
{
	impl->uncheck_radio_buttons(get_parent_component());

	if (selected)
	{
		impl->part_checker.set_state(CssStr::checked, true);
		impl->part_checker.set_state(CssStr::unchecked, false);
	}
}

void CL_RadioButton::set_group_name(const CL_StringRef &name)
{
	impl->group_name = name;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RadioButton Events:

CL_Callback_v0 &CL_RadioButton::func_selected()
{
	return impl->func_selected;
}

CL_Callback_v0 &CL_RadioButton::func_unselected()
{
	return impl->func_unselected;
}

CL_Callback_v1<CL_RadioButton*> &CL_RadioButton::func_group_selection_changed()
{
	return impl->func_group_selection_changed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RadioButton Implementation:

void CL_RadioButton_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!radio->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
			part_checker.set_state(CssStr::pressed, true);
			radio->request_repaint();
		}
		else if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
		{
			if ((part_checker.get_state(CssStr::checked) == false))
			{
				CL_Callback_v1<CL_RadioButton*> cb = uncheck_radio_buttons(radio->get_parent_component());
				part_checker.set_state(CssStr::pressed, false);
				part_checker.set_state(CssStr::checked, true);
				part_checker.set_state(CssStr::unchecked, false);
				radio->request_repaint();
				if (!cb.is_null())
					cb.invoke(radio);
				if (!func_selected.is_null())
					func_selected.invoke();
			}
			else
			{
				part_checker.set_state(CssStr::pressed, false);
				radio->request_repaint();
			}
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
		{
			part_checker.set_state(CssStr::hot, true);
			radio->request_repaint();
		}
		else
		{
			part_checker.set_state(CssStr::hot, false);
			radio->request_repaint();
		}
	}
}

void CL_RadioButton_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = radio->get_geometry();
	part_component.render_box(gc, rect.get_size(), update_rect);

	CL_Size pref_size = part_checker.get_preferred_size();
	CL_Rect content_rect = part_component.get_content_box(rect.get_size());
	int ypos = content_rect.top + content_rect.get_height()/2 - pref_size.height/2;
	CL_Rect checker_rect(content_rect.left, ypos, content_rect.left + pref_size.width, ypos + pref_size.height);
	part_checker.render_box(gc, checker_rect, update_rect);

	int text_gap = part_component.get_property_int(prop_text_gap);

	CL_Rect text_rect;
	text_rect.left = checker_rect.right + text_gap;
	text_rect.right = content_rect.right;
	text_rect.top = content_rect.top;
	text_rect.bottom = content_rect.bottom;

	part_component.render_text(gc, text, text_rect, update_rect);
}

CL_Callback_v1<CL_RadioButton*> CL_RadioButton_Impl::uncheck_radio_buttons(CL_GUIComponent *parent)
{
	CL_Callback_v1<CL_RadioButton*> callback;

	std::vector<CL_GUIComponent*> children = parent->get_child_components();
	std::vector<CL_GUIComponent*>::iterator it;
	for (it=children.begin(); it != children.end(); ++it)
	{
		if ((*it)->has_child_components())
			uncheck_radio_buttons(*it);
		CL_RadioButton *rb = dynamic_cast<CL_RadioButton*>(*it);
		if (rb && rb->get_group_name() == group_name)
		{
			if (callback.is_null() && !rb->func_group_selection_changed().is_null())
			{
				callback = rb->func_group_selection_changed();
			}

			if (rb->impl->part_checker.get_state(CssStr::checked))
			{
				if (!rb->impl->func_unselected.is_null())
					rb->impl->func_unselected.invoke();

				rb->impl->part_checker.set_state(CssStr::checked, false);
				rb->impl->part_checker.set_state(CssStr::unchecked, true);
				rb->request_repaint();
			}
		}
	}

	return callback;
}

void CL_RadioButton_Impl::on_style_changed()
{
	create_parts();
}

void CL_RadioButton_Impl::create_parts()
{
	part_component = CL_GUIThemePart(radio);
	part_checker = CL_GUIThemePart(radio, CssStr::RadioButton::part_checker);
	part_checker.set_state(CssStr::hot, false);
	part_checker.set_state(CssStr::normal, true);
	part_checker.set_state(CssStr::pressed, false);
	part_checker.set_state(CssStr::checked, false);
	part_checker.set_state(CssStr::indeterminated, false);
	part_checker.set_state(CssStr::unchecked, true);
	part_checker.set_state(CssStr::disabled, false);
}

void CL_RadioButton_Impl::on_enablemode_changed()
{
	part_checker.set_state(CssStr::disabled, !radio->is_enabled());
	part_checker.set_state(CssStr::normal, radio->is_enabled());

	radio->request_repaint();
}
