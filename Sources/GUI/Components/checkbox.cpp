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
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/Components/checkbox.h"
#include "API/GUI/Components/label.h"
#include "API/GUI/Components/imageview.h"
#include "API/GUI/gui_theme_part.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/canvas.h"
#include "../gui_css_strings.h"

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
	}

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_enablemode_changed();

	void update_classes(bool checked = false, bool indeterminated = false);

	CheckBox *checkbox;

	std::function<void()> func_checked;
	std::function<void()> func_unchecked;
	std::function<void()> func_indeterminated;
	std::function<void()> func_state_changed;

	bool three_state;

	GUIComponent *checker;
	Label *label;
};

/////////////////////////////////////////////////////////////////////////////
// CheckBox Construction:

CheckBox::CheckBox(GUIComponent *parent)
: GUIComponent(parent, CssStr::CheckBox::type_name), impl(new CheckBox_Impl)
{
	impl->checkbox = this;

	impl->checker = new GUIComponent(this, CssStr::CheckBox::part_checker);
	impl->label = new Label(this);

	set_focus_policy(focus_local);
	set_double_click_enabled(false);

	func_process_message() = bind_member(impl.get(), &CheckBox_Impl::on_process_message);
	func_enablemode_changed() = bind_member(impl.get(), &CheckBox_Impl::on_enablemode_changed);

	impl->update_classes();
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
	return impl->checker->get_pseudo_class(CssStr::indeterminated);
}

bool CheckBox::is_checked() const
{
	return impl->checker->get_pseudo_class(CssStr::checked);
}

bool CheckBox::is_3state() const
{
	return impl->three_state;
}

const std::string &CheckBox::get_text() const
{
	return impl->label->get_text();
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Operations:

void CheckBox::set_indeterminated()
{
	impl->checker->set_pseudo_class(CssStr::checked, false);
	impl->checker->set_pseudo_class(CssStr::unchecked, false);
	impl->checker->set_pseudo_class(CssStr::indeterminated, true);
}

void CheckBox::set_checked(bool enable)
{
	impl->checker->set_pseudo_class(CssStr::checked, enable);
	impl->checker->set_pseudo_class(CssStr::unchecked, !enable);
	impl->checker->set_pseudo_class(CssStr::indeterminated, false);
}

void CheckBox::set_3state(bool enable)
{
	impl->three_state = enable;
}

void CheckBox::set_text(const std::string &text)
{
	impl->label->set_text(text);
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Events:

std::function<void()> &CheckBox::func_checked()
{
	return impl->func_checked;
}

std::function<void()> &CheckBox::func_unchecked()
{
	return impl->func_unchecked;
}

std::function<void()> &CheckBox::func_indeterminated()
{
	return impl->func_indeterminated;
}

std::function<void()> &CheckBox::func_state_changed()
{
	return impl->func_state_changed;
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Implementation:

void CheckBox_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!checkbox->is_enabled())
		return;

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		const InputEvent &e = input_msg->input_event;

		if (e.type == InputEvent::pressed && (e.id == mouse_left || e.id == keycode_space))
		{
			checkbox->set_pseudo_class(CssStr::pressed, true);
			checker->set_pseudo_class(CssStr::pressed, true);
			msg->consumed = true;
		}
		else if (e.type == InputEvent::released && (e.id == mouse_left || e.id == keycode_space) && checker->get_pseudo_class(CssStr::pressed))
		{
			if (checkbox->get_focus_policy() == GUIComponent::focus_local)
				checkbox->set_focus(true);

			checker->set_pseudo_class(CssStr::pressed, false);
			if (checker->get_pseudo_class(CssStr::checked))
			{
				checker->set_pseudo_class(CssStr::unchecked, true);
				checker->set_pseudo_class(CssStr::checked, false);
			}
			else if (checker->get_pseudo_class(CssStr::unchecked))
			{
				if (three_state)
				{
					checker->set_pseudo_class(CssStr::indeterminated, true);
					checker->set_pseudo_class(CssStr::unchecked, false);
				}
				else
				{
					checker->set_pseudo_class(CssStr::checked, true);
					checker->set_pseudo_class(CssStr::unchecked, false);
				}
			}
			else if (checker->get_pseudo_class(CssStr::indeterminated))
			{
				checker->set_pseudo_class(CssStr::checked, true);
				checker->set_pseudo_class(CssStr::indeterminated, false);
			}

			if (checker->get_pseudo_class(CssStr::checked))
			{
				if (func_checked)
					func_checked();
			}
			else if (checker->get_pseudo_class(CssStr::indeterminated))
			{
				if (func_indeterminated)
					func_indeterminated();
			}
			else 
			{
				if (func_unchecked)
					func_unchecked();
			}
			if (func_state_changed)
				func_state_changed();

			msg->consumed = true;
		}
		else if (e.type == InputEvent::pressed && (e.id == keycode_left || e.id == keycode_up))
		{
			checkbox->focus_previous();
			msg->consumed = true;
		}
		else if (e.type == InputEvent::pressed && (e.id == keycode_right || e.id == keycode_down))
		{
			checkbox->focus_next();
			msg->consumed = true;
		}
	}

	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
		{
			checker->set_pseudo_class(CssStr::hover, true);
		}
		else
		{
			checker->set_pseudo_class(CssStr::hover, false);
			checker->set_pseudo_class(CssStr::pressed, false);
		}
		msg->consumed = true;
	}

	std::shared_ptr<GUIMessage_FocusChange> focus_change_msg = std::dynamic_pointer_cast<GUIMessage_FocusChange>(msg);
	if (focus_change_msg)
	{
		if (focus_change_msg->focus_type == GUIMessage_FocusChange::gained_focus)
		{
			checkbox->set_pseudo_class(CssStr::focused, true);
		}
		else 
		{
			checkbox->set_pseudo_class(CssStr::focused, false);
		}
		msg->consumed = true;
	}
}

void CheckBox_Impl::update_classes(bool checked, bool indeterminated)
{
	checkbox->set_pseudo_class(CssStr::normal, checkbox->is_enabled());
	checkbox->set_pseudo_class(CssStr::disabled, !checkbox->is_enabled());

	checker->set_pseudo_class(CssStr::normal, checkbox->is_enabled());
	checker->set_pseudo_class(CssStr::pressed, false);
	checker->set_pseudo_class(CssStr::indeterminated, indeterminated);
	checker->set_pseudo_class(CssStr::checked, indeterminated ? false : checked);
	checker->set_pseudo_class(CssStr::unchecked, indeterminated ? false : !checked);
	checker->set_pseudo_class(CssStr::disabled, !checkbox->is_enabled());
}

void CheckBox_Impl::on_enablemode_changed()
{
	update_classes(checkbox->is_checked(), checkbox->is_indeterminated());
}

}
