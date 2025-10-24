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
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/checkbox.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CheckBox_Impl Class:

class CL_CheckBox_Impl
{
public:
	CL_CheckBox_Impl()
	: three_state(false)
	{
		prop_text_gap = CL_GUIThemePartProperty(CssStr::text_gap, "2");
		prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, "black");
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_enablemode_changed();

	void create_parts();

	CL_CheckBox *checkbox;

	CL_Callback_v0 func_checked;
	CL_Callback_v0 func_unchecked;
	CL_Callback_v0 func_indeterminated;
	CL_Callback_v0 func_state_changed;

	CL_String text;

	bool three_state;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_checker;
	CL_GUIThemePart part_focus;
	CL_GUIThemePartProperty prop_text_gap;
	CL_GUIThemePartProperty prop_text_color;
};

/////////////////////////////////////////////////////////////////////////////
// CL_CheckBox Construction:

CL_CheckBox::CL_CheckBox(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_CheckBox_Impl)
{
	set_type_name(CssStr::CheckBox::type_name);
	impl->checkbox = this;
	set_focus_policy(focus_local);

	func_process_message().set(impl.get(), &CL_CheckBox_Impl::on_process_message);
	func_render().set(impl.get(), &CL_CheckBox_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_CheckBox_Impl::on_style_changed);
	func_enablemode_changed().set(impl.get(), &CL_CheckBox_Impl::on_enablemode_changed);

	impl->create_parts();
}

CL_CheckBox::~CL_CheckBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CheckBox Attributes:

CL_CheckBox *CL_CheckBox::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_CheckBox *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_CheckBox*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_CheckBox named item: %1", id));

	return object;
}

bool CL_CheckBox::is_indeterminated() const
{
	return impl->part_checker.get_state(CssStr::indeterminated);
}

bool CL_CheckBox::is_checked() const
{
	return impl->part_checker.get_state(CssStr::checked);
}

bool CL_CheckBox::is_3state() const
{
	return impl->three_state;
}

const CL_String &CL_CheckBox::get_text() const
{
	return impl->text;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CheckBox Operations:

void CL_CheckBox::set_indeterminated()
{
	impl->part_checker.set_state(CssStr::checked, false);
	impl->part_checker.set_state(CssStr::unchecked, false);
	impl->part_checker.set_state(CssStr::indeterminated, true);

	request_repaint();
}

void CL_CheckBox::set_checked(bool enable)
{
	impl->part_checker.set_state(CssStr::checked, enable);
	impl->part_checker.set_state(CssStr::unchecked, !enable);
	impl->part_checker.set_state(CssStr::indeterminated, false);

	request_repaint();
}

void CL_CheckBox::set_3state(bool enable)
{
	impl->three_state = enable;
}

void CL_CheckBox::set_text(const CL_StringRef &text)
{
	impl->text = text;
}

CL_Size CL_CheckBox::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_CheckBox Events:

CL_Callback_v0 &CL_CheckBox::func_checked()
{
	return impl->func_checked;
}

CL_Callback_v0 &CL_CheckBox::func_unchecked()
{
	return impl->func_unchecked;
}

CL_Callback_v0 &CL_CheckBox::func_indeterminated()
{
	return impl->func_indeterminated;
}

CL_Callback_v0 &CL_CheckBox::func_state_changed()
{
	return impl->func_state_changed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CheckBox Implementation:

void CL_CheckBox_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!checkbox->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && (e.id == CL_MOUSE_LEFT || e.id == CL_KEY_SPACE))
		{
			part_component.set_state(CssStr::pressed, true);
			part_checker.set_state(CssStr::pressed, true);
			checkbox->request_repaint();
			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::released && 
			(e.id == CL_MOUSE_LEFT || e.id == CL_KEY_SPACE) &&
			part_checker.get_state(CssStr::pressed))
		{
			if (checkbox->get_focus_policy() == CL_GUIComponent::focus_local)
				checkbox->set_focus(true);

			part_checker.set_state(CssStr::pressed, false);
			if (part_checker.get_state(CssStr::checked))
			{
				part_checker.set_state(CssStr::unchecked, true);
				part_checker.set_state(CssStr::checked, false);
			}
			else if (part_checker.get_state(CssStr::unchecked))
			{
				if (three_state)
				{
					part_checker.set_state(CssStr::indeterminated, true);
					part_checker.set_state(CssStr::unchecked, false);
				}
				else
				{
					part_checker.set_state(CssStr::checked, true);
					part_checker.set_state(CssStr::unchecked, false);
				}
			}
			else if (part_checker.get_state(CssStr::indeterminated))
			{
				part_checker.set_state(CssStr::checked, true);
				part_checker.set_state(CssStr::indeterminated, false);
			}
			checkbox->request_repaint();

			if (part_checker.get_state(CssStr::checked))
			{
				if (!func_checked.is_null())
					func_checked.invoke();
			}
			else if (part_checker.get_state(CssStr::indeterminated))
			{
				if (!func_indeterminated.is_null())
					func_indeterminated.invoke();
			}
			else 
			{
				if (!func_unchecked.is_null())
					func_unchecked.invoke();
			}
			if (!func_state_changed.is_null())
				func_state_changed.invoke();

			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::pressed && (e.id == CL_KEY_LEFT || e.id == CL_KEY_UP))
		{
			checkbox->focus_previous();
			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::pressed && (e.id == CL_KEY_RIGHT || e.id == CL_KEY_DOWN))
		{
			checkbox->focus_next();
			msg.set_consumed();
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
		{
			part_checker.set_state(CssStr::hot, true);
			part_component.set_state(CssStr::hot, true);
			checkbox->request_repaint();
		}
		else
		{
			part_checker.set_state(CssStr::hot, false);
			part_checker.set_state(CssStr::pressed, false);
			part_component.set_state(CssStr::hot, false);
			checkbox->request_repaint();
		}
		msg.set_consumed();
	}
	else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name()))
	{
		CL_GUIMessage_FocusChange focus_msg = msg;
		if (focus_msg.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus)
		{
			part_component.set_state(CssStr::focused, true);
			checkbox->request_repaint();
		}
		else 
		{
			part_component.set_state(CssStr::focused, false);
			checkbox->request_repaint();
		}
		msg.set_consumed();
	}
}

void CL_CheckBox_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = checkbox->get_geometry();
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

	CL_Size text_size = part_component.get_text_size(gc, text);
	int focus_left = checker_rect.right + text_gap - 2; // todo: remove magic number hacks
	CL_Rect focus_rect = CL_RectPS(focus_left, content_rect.top, text_size.width+4, content_rect.bottom);

	if (checkbox->has_focus())
		part_focus.render_box(gc, focus_rect, update_rect);
}

void CL_CheckBox_Impl::create_parts()
{
	part_component = CL_GUIThemePart(checkbox);
	part_checker = CL_GUIThemePart(checkbox, CssStr::CheckBox::part_checker);
	part_focus = CL_GUIThemePart(checkbox, CssStr::CheckBox::part_focus);

	part_component.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::normal, checkbox->is_enabled());
	part_component.set_state(CssStr::disabled, !checkbox->is_enabled());

	part_checker.set_state(CssStr::normal, checkbox->is_enabled());
	part_checker.set_state(CssStr::pressed, false);
	part_checker.set_state(CssStr::indeterminated, checkbox->is_indeterminated());
	part_checker.set_state(CssStr::checked, checkbox->is_checked());
	part_checker.set_state(CssStr::unchecked, !checkbox->is_checked());
	part_checker.set_state(CssStr::disabled, !checkbox->is_enabled());
}

void CL_CheckBox_Impl::on_style_changed()
{
	create_parts();
	checkbox->request_repaint();
}

void CL_CheckBox_Impl::on_enablemode_changed()
{
	create_parts();
	checkbox->request_repaint();
}
