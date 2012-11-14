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
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/checkbox.h"
#include "API/GUI/Components/label.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"
#include "API/GUI/Components/imageview.h"

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

	void update_pseudo_classes(bool checked = false, bool indeterminated = false);

	CheckBox *checkbox;

	Callback_v0 func_checked;
	Callback_v0 func_unchecked;
	Callback_v0 func_indeterminated;
	Callback_v0 func_state_changed;

	bool three_state;

	Label *part_label;
	GUIComponent *part_checker;
};

/////////////////////////////////////////////////////////////////////////////
// CheckBox Construction:

CheckBox::CheckBox(GUIComponent *parent)
: GUIComponent(parent), impl(new CheckBox_Impl)
{
	set_tag_name(CssStr::CheckBox::type_name);
	impl->checkbox = this;

	impl->part_label = new Label(this);
	impl->part_checker = new GUIComponent(this);
	impl->part_checker->set_tag_name(CssStr::CheckBox::part_checker);

	set_focus_policy(focus_local);
	set_double_click_enabled(false);

	func_process_message().set(impl.get(), &CheckBox_Impl::on_process_message);
	func_enablemode_changed().set(impl.get(), &CheckBox_Impl::on_enablemode_changed);

	impl->update_pseudo_classes();
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
	return impl->checkbox->get_pseudo_class(CssStr::indeterminated);
}

bool CheckBox::is_checked() const
{
	return impl->checkbox->get_pseudo_class(CssStr::checked);
}

bool CheckBox::is_3state() const
{
	return impl->three_state;
}

std::string CheckBox::get_text() const
{
	return impl->part_label->get_text();
}

/////////////////////////////////////////////////////////////////////////////
// CheckBox Operations:

void CheckBox::set_indeterminated()
{
	impl->checkbox->set_pseudo_class(CssStr::checked, false);
	impl->checkbox->set_pseudo_class(CssStr::unchecked, false);
	impl->checkbox->set_pseudo_class(CssStr::indeterminated, true);
}

void CheckBox::set_checked(bool enable)
{
	impl->checkbox->set_pseudo_class(CssStr::checked, enable);
	impl->checkbox->set_pseudo_class(CssStr::unchecked, !enable);
	impl->checkbox->set_pseudo_class(CssStr::indeterminated, false);
}

void CheckBox::set_3state(bool enable)
{
	impl->three_state = enable;
}

void CheckBox::set_text(const std::string &text)
{
	impl->part_label->set_text(text);
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

void CheckBox_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!checkbox->is_enabled())
		return;

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		if (input_msg->input_event.type == InputEvent::pressed && (input_msg->input_event.id == mouse_left || input_msg->input_event.id == keycode_space))
		{
			checkbox->set_pseudo_class(CssStr::pressed, true);
			input_msg->consumed = true;
		}
		else if (input_msg->input_event.type == InputEvent::released && 
			(input_msg->input_event.id == mouse_left || input_msg->input_event.id == keycode_space) &&
			checkbox->get_pseudo_class(CssStr::pressed))
		{
			if (checkbox->get_focus_policy() == GUIComponent::focus_local)
				checkbox->set_focus(true);

			checkbox->set_pseudo_class(CssStr::pressed, false);
			if (checkbox->get_pseudo_class(CssStr::checked))
			{
				checkbox->set_pseudo_class(CssStr::unchecked, true);
				checkbox->set_pseudo_class(CssStr::checked, false);
			}
			else if (checkbox->get_pseudo_class(CssStr::unchecked))
			{
				if (three_state)
				{
					checkbox->set_pseudo_class(CssStr::indeterminated, true);
					checkbox->set_pseudo_class(CssStr::unchecked, false);
				}
				else
				{
					checkbox->set_pseudo_class(CssStr::checked, true);
					checkbox->set_pseudo_class(CssStr::unchecked, false);
				}
			}
			else if (checkbox->get_pseudo_class(CssStr::indeterminated))
			{
				checkbox->set_pseudo_class(CssStr::checked, true);
				checkbox->set_pseudo_class(CssStr::indeterminated, false);
			}

			if (checkbox->get_pseudo_class(CssStr::checked))
			{
				if (!func_checked.is_null())
					func_checked.invoke();
			}
			else if (checkbox->get_pseudo_class(CssStr::indeterminated))
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

			input_msg->consumed = true;
		}
		else if (input_msg->input_event.type == InputEvent::pressed && (input_msg->input_event.id == keycode_left || input_msg->input_event.id == keycode_up))
		{
			checkbox->focus_previous();
			input_msg->consumed = true;
		}
		else if (input_msg->input_event.type == InputEvent::pressed && (input_msg->input_event.id == keycode_right || input_msg->input_event.id == keycode_down))
		{
			checkbox->focus_next();
			input_msg->consumed = true;
		}
	}
	//std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	//if (pointer)
	//{
	//	if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
	//	{
	//		checkbox->set_pseudo_class(CssStr::hot, true);
	//	}
	//	else
	//	{
	//		checkbox->set_pseudo_class(CssStr::hot, false);
	//		checkbox->set_pseudo_class(CssStr::pressed, false);
	//	}
	//	pointer->consumed = true;
	//}
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
	}
}

void CheckBox_Impl::update_pseudo_classes(bool checked, bool indeterminated)
{
	//checkbox->set_pseudo_class(CssStr::hot, false);
	checkbox->set_pseudo_class(CssStr::normal, checkbox->is_enabled());
	checkbox->set_pseudo_class(CssStr::disabled, !checkbox->is_enabled());

	checkbox->set_pseudo_class(CssStr::pressed, false);
	checkbox->set_pseudo_class(CssStr::indeterminated, indeterminated);
	checkbox->set_pseudo_class(CssStr::checked, indeterminated ? false : checked);
	checkbox->set_pseudo_class(CssStr::unchecked, indeterminated ? false : !checked);
}

void CheckBox_Impl::on_enablemode_changed()
{
	update_pseudo_classes(checkbox->is_checked(), checkbox->is_indeterminated());
}

}
