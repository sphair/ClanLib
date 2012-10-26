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
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/checkbox.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// CheckBox_Impl Class:

class CheckBox_Impl
{
public:
	CheckBox_Impl()
	: three_state(false)
	{
		prop_text_gap = GUIThemePartProperty(CssStr::text_gap, "2");
		prop_text_color = GUIThemePartProperty(CssStr::text_color, "black");
	}

	void on_process_message(GUIMessage &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_style_changed();
	void on_enablemode_changed();

	void create_parts(bool checked = false, bool indeterminated = false);

	CheckBox *checkbox;

	Callback_v0 func_checked;
	Callback_v0 func_unchecked;
	Callback_v0 func_indeterminated;
	Callback_v0 func_state_changed;

	std::string text;

	bool three_state;

	GUIThemePart part_component;
	GUIThemePart part_checker;
	GUIThemePart part_focus;
	GUIThemePartProperty prop_text_gap;
	GUIThemePartProperty prop_text_color;
};

/////////////////////////////////////////////////////////////////////////////
// CheckBox Construction:

CheckBox::CheckBox(GUIComponent *parent)
: GUIComponent(parent), impl(new CheckBox_Impl)
{
	set_tag_name(CssStr::CheckBox::type_name);
	impl->checkbox = this;
	set_focus_policy(focus_local);
	set_double_click_enabled(false);

	func_process_message().set(impl.get(), &CheckBox_Impl::on_process_message);
	func_render().set(impl.get(), &CheckBox_Impl::on_render);
	func_style_changed().set(impl.get(), &CheckBox_Impl::on_style_changed);
	func_enablemode_changed().set(impl.get(), &CheckBox_Impl::on_enablemode_changed);

	impl->create_parts();
}

CheckBox::~CheckBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Attributes:

CheckBox *CheckBox::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	CheckBox *object = NULL;
	if (reference_component)
		object = dynamic_cast<CheckBox*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find CheckBox named item: %1", id));

	return object;
}

bool CheckBox::is_indeterminated() const
{
	return impl->part_checker.get_state(CssStr::indeterminated);
}

bool CheckBox::is_checked() const
{
	return impl->part_checker.get_state(CssStr::checked);
}

bool CheckBox::is_3state() const
{
	return impl->three_state;
}

const std::string &CheckBox::get_text() const
{
	return impl->text;
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Operations:

void CheckBox::set_indeterminated()
{
	impl->part_checker.set_state(CssStr::checked, false);
	impl->part_checker.set_state(CssStr::unchecked, false);
	impl->part_checker.set_state(CssStr::indeterminated, true);

	request_repaint();
}

void CheckBox::set_checked(bool enable)
{
	impl->part_checker.set_state(CssStr::checked, enable);
	impl->part_checker.set_state(CssStr::unchecked, !enable);
	impl->part_checker.set_state(CssStr::indeterminated, false);

	request_repaint();
}

void CheckBox::set_3state(bool enable)
{
	impl->three_state = enable;
}

void CheckBox::set_text(const std::string &text)
{
	impl->text = text;
}

Size CheckBox::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Events:

Callback_v0 &CheckBox::func_checked()
{
	return impl->func_checked;
}

Callback_v0 &CheckBox::func_unchecked()
{
	return impl->func_unchecked;
}

Callback_v0 &CheckBox::func_indeterminated()
{
	return impl->func_indeterminated;
}

Callback_v0 &CheckBox::func_state_changed()
{
	return impl->func_state_changed;
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Implementation:

void CheckBox_Impl::on_process_message(GUIMessage &msg)
{
	if (!checkbox->is_enabled())
		return;

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::pressed && (e.id == mouse_left || e.id == keycode_space))
		{
			part_component.set_state(CssStr::pressed, true);
			part_checker.set_state(CssStr::pressed, true);
			checkbox->request_repaint();
			msg.set_consumed();
		}
		else if (e.type == InputEvent::released && 
			(e.id == mouse_left || e.id == keycode_space) &&
			part_checker.get_state(CssStr::pressed))
		{
			if (checkbox->get_focus_policy() == GUIComponent::focus_local)
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
		else if (e.type == InputEvent::pressed && (e.id == keycode_left || e.id == keycode_up))
		{
			checkbox->focus_previous();
			msg.set_consumed();
		}
		else if (e.type == InputEvent::pressed && (e.id == keycode_right || e.id == keycode_down))
		{
			checkbox->focus_next();
			msg.set_consumed();
		}
	}
	else if (msg.is_type(GUIMessage_Pointer::get_type_name()))
	{
		GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == GUIMessage_Pointer::pointer_enter)
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
	else if (msg.is_type(GUIMessage_FocusChange::get_type_name()))
	{
		GUIMessage_FocusChange focus_msg = msg;
		if (focus_msg.get_focus_type() == GUIMessage_FocusChange::gained_focus)
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

void CheckBox_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = checkbox->get_geometry();
	part_component.render_box(canvas, rect.get_size(), update_rect);

	Size pref_size = part_checker.get_preferred_size();
	Rect content_rect = part_component.get_content_box(rect.get_size());

	int ypos = content_rect.top + content_rect.get_height()/2 - pref_size.height/2;

	Rect checker_rect(content_rect.left, ypos, content_rect.left + pref_size.width, ypos + pref_size.height);

	part_checker.render_box(canvas, checker_rect, update_rect);

	int text_gap = part_component.get_property_int(prop_text_gap);

	Rect text_rect;
	text_rect.left = checker_rect.right + text_gap;
	text_rect.right = content_rect.right;
	text_rect.top = content_rect.top;
	text_rect.bottom = content_rect.bottom;

	part_component.render_text(canvas, text, text_rect, update_rect);

	Size text_size = part_component.get_text_size(canvas, text);
	int focus_left = checker_rect.right + text_gap - 2; // todo: remove magic number hacks
	Rect focus_rect = RectPS(focus_left, content_rect.top, text_size.width+4, content_rect.bottom);

	if (checkbox->has_focus())
		part_focus.render_box(canvas, focus_rect, update_rect);
}

void CheckBox_Impl::create_parts(bool checked, bool indeterminated)
{
	part_component = GUIThemePart(checkbox);
	part_checker = GUIThemePart(checkbox, CssStr::CheckBox::part_checker);
	part_focus = GUIThemePart(checkbox, CssStr::CheckBox::part_focus);

	part_component.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::normal, checkbox->is_enabled());
	part_component.set_state(CssStr::disabled, !checkbox->is_enabled());

	part_checker.set_state(CssStr::normal, checkbox->is_enabled());
	part_checker.set_state(CssStr::pressed, false);
	part_checker.set_state(CssStr::indeterminated, indeterminated);
	part_checker.set_state(CssStr::checked, indeterminated ? false : checked);
	part_checker.set_state(CssStr::unchecked, indeterminated ? false : !checked);
	part_checker.set_state(CssStr::disabled, !checkbox->is_enabled());
}

void CheckBox_Impl::on_style_changed()
{
	create_parts(checkbox->is_checked(), checkbox->is_indeterminated());
	checkbox->request_repaint();
}

void CheckBox_Impl::on_enablemode_changed()
{
	create_parts(checkbox->is_checked(), checkbox->is_indeterminated());
	checkbox->request_repaint();
}

}
