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
**    Mark Page
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
#include "API/GUI/Components/radiobutton.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// RadioButton_Impl Class:

class RadioButton_Impl
{
public:
	RadioButton_Impl() : id(-1)
	{
	}

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_enablemode_changed();
	void create_parts();

	RadioButton *radio;
	Callback_v0 func_selected;
	Callback_v0 func_unselected;
	Callback_v1<RadioButton*> func_group_selection_changed;
	std::string text;
	int id;

	
	GUIThemePart part_checker;
	GUIThemePart part_focus;

	// Returns the group changed callback, if it is set on one radio button in the group.
	Callback_v1<RadioButton*> uncheck_radio_buttons(GUIComponent *parent);
};

/////////////////////////////////////////////////////////////////////////////
// RadioButton Construction:

RadioButton::RadioButton(GUIComponent *parent)
: GUIComponent(parent, CssStr::RadioButton::type_name), impl(new RadioButton_Impl)
{
	set_focus_policy(focus_group);

	impl->radio = this;

	impl->create_parts();

	func_process_message().set(impl.get(), &RadioButton_Impl::on_process_message);
	func_render().set(impl.get(), &RadioButton_Impl::on_render);
	func_enablemode_changed().set(impl.get(), &RadioButton_Impl::on_enablemode_changed);
}

RadioButton::~RadioButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// RadioButton Attributes:

RadioButton *RadioButton::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	RadioButton *object = NULL;
	if (reference_component)
		object = dynamic_cast<RadioButton*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find RadioButton named item: %1", id));

	return object;
}

std::string RadioButton::get_text() const
{
	return impl->text;
}

int RadioButton::get_id() const
{
	return impl->id;
}

std::string RadioButton::get_group_name() const
{
	return get_component_group_name();
}

bool RadioButton::is_selected() const
{
	return impl->part_checker.get_pseudo_class(CssStr::checked);
}

float RadioButton::get_preferred_content_width()
{
	return impl->part_checker.get_css_width();
}

float RadioButton::get_preferred_content_height(float width)
{
	return impl->part_checker.get_css_height();
}

/////////////////////////////////////////////////////////////////////////////
// RadioButton Operations:

void RadioButton::set_text(const std::string &text)
{
	impl->text = text;
}

void RadioButton::set_id(int id)
{
	impl->id = id;
}

void RadioButton::set_selected(bool selected)
{
	impl->uncheck_radio_buttons(get_parent_component());

	if (selected)
	{
		impl->part_checker.set_pseudo_class(CssStr::checked, true);
		impl->part_checker.set_pseudo_class(CssStr::unchecked, false);
		set_selected_in_component_group(true);
	}

	request_repaint();
}

void RadioButton::set_group_name(const std::string &name)
{
	set_component_group_name(name);
}

/////////////////////////////////////////////////////////////////////////////
// RadioButton Events:

Callback_v0 &RadioButton::func_selected()
{
	return impl->func_selected;
}

Callback_v0 &RadioButton::func_unselected()
{
	return impl->func_unselected;
}

Callback_v1<RadioButton*> &RadioButton::func_group_selection_changed()
{
	return impl->func_group_selection_changed;
}

/////////////////////////////////////////////////////////////////////////////
// RadioButton Implementation:

void RadioButton_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!radio->is_enabled())
		return;

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		
		const InputEvent &e = input_msg->input_event;

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			part_checker.set_pseudo_class(CssStr::pressed, true);
			radio->request_repaint();
			msg->consumed = true;
		}
		else if (e.type == InputEvent::released && e.id == mouse_left)
		{
			if ((part_checker.get_pseudo_class(CssStr::checked) == false))
			{
				Callback_v1<RadioButton*> cb = uncheck_radio_buttons(radio->get_parent_component());

				part_checker.set_pseudo_class(CssStr::pressed, false);
				radio->set_selected(true);

				if (!cb.is_null())
					cb.invoke(radio);
				if (!func_selected.is_null())
					func_selected.invoke();
			}
			else
			{
				part_checker.set_pseudo_class(CssStr::pressed, false);
				radio->request_repaint();
			}

			radio->set_focus(true);
			msg->consumed = true;
		}
		else if (e.type == InputEvent::pressed && (e.id == keycode_left || e.id == keycode_up))
		{
			std::vector<GUIComponent*> group = radio->get_parent_component()->get_child_component_group(radio->get_component_group_name());

			// find previous visible & enabled radiobutton in the same group:
			GUIComponent *comp = radio->get_previous_sibling();
			if (comp == 0)
				comp = group.back();

			while (comp != radio)
			{
				if (comp->is_visible() && comp->is_enabled() && comp->get_component_group_name() == radio->get_component_group_name())
				{
					RadioButton *focus_comp = dynamic_cast<RadioButton*>(comp);
					if (focus_comp)
					{
						Callback_v1<RadioButton*> cb = uncheck_radio_buttons(radio->get_parent_component());
						focus_comp->set_selected(true);
						if (!cb.is_null())
							cb.invoke(focus_comp);
						if (!focus_comp->func_selected().is_null())
							focus_comp->func_selected().invoke();
						focus_comp->set_focus();
						break;
					}
				}

				if (!comp->get_previous_sibling() || comp->get_previous_sibling()->get_component_group_name() != radio->get_component_group_name())
				{
					// reach first item in group, loop around from the last:
					comp = group.back();
				}
				else
					comp = comp->get_previous_sibling();
			}

			msg->consumed = true;
		}
		else if (e.type == InputEvent::pressed && (e.id == keycode_right || e.id == keycode_down))
		{
			std::vector<GUIComponent*> group = radio->get_parent_component()->get_child_component_group(radio->get_component_group_name());

			// find next visible & enabled radiobutton in the same group:
			GUIComponent *comp = radio->get_next_sibling();
			if (comp == 0 || comp->get_component_group_name() != radio->get_component_group_name())
				comp = group.front();

			while (comp != radio)
			{
				if (comp->is_visible() && comp->is_enabled() && comp->get_component_group_name() == radio->get_component_group_name())
				{
					RadioButton *focus_comp = dynamic_cast<RadioButton*>(comp);
					if (focus_comp)
					{
						Callback_v1<RadioButton*> cb = uncheck_radio_buttons(radio->get_parent_component());
						focus_comp->set_selected(true);
						if (!cb.is_null())
							cb.invoke(focus_comp);
						if (!focus_comp->func_selected().is_null())
							focus_comp->func_selected().invoke();
						focus_comp->set_focus();
						break;
					}
				}

				if (!comp->get_previous_sibling() || comp->get_previous_sibling()->get_component_group_name() != radio->get_component_group_name())
				{
					// reach first item in group, loop around from the last:
					comp = group.back();
				}
				else
					comp = comp->get_next_sibling();
			}
			msg->consumed = true;
		}

	}
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
		{
			part_checker.set_pseudo_class(CssStr::hot, true);
			radio->request_repaint();
		}
		else
		{
			part_checker.set_pseudo_class(CssStr::hot, false);
			radio->request_repaint();
		}
	}
	std::shared_ptr<GUIMessage_FocusChange> focus_change_msg = std::dynamic_pointer_cast<GUIMessage_FocusChange>(msg);
	if (focus_change_msg)
	{
		
		if (focus_change_msg->focus_type == GUIMessage_FocusChange::gained_focus)
		{
			radio->set_pseudo_class(CssStr::focused, true);
			if (!radio->is_selected())
			{
				Callback_v1<RadioButton*> cb = uncheck_radio_buttons(radio->get_parent_component());
				radio->set_selected(true);
				if (!cb.is_null())
					cb.invoke(radio);
				if (!func_selected.is_null())
					func_selected.invoke();
			}
			radio->request_repaint();
		}
		else 
		{
			radio->set_pseudo_class(CssStr::focused, false);
			radio->request_repaint();
		}
		msg->consumed = true;
	}
}

void RadioButton_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = radio->get_size();

	Size pref_size = part_checker.get_css_size();
	Rect content_rect = radio->get_content_box();
	int ypos = content_rect.top + content_rect.get_height()/2 - pref_size.height/2;
	Rect checker_rect(content_rect.left, ypos, content_rect.left + pref_size.width, ypos + pref_size.height);
	part_checker.render_box(canvas, checker_rect);

	int text_gap = radio->get_property_int(CssStr::text_gap, "2");

	Size text_size = radio->render_text(canvas, text, checker_rect.right + text_gap, radio->get_vertical_text_align(canvas).baseline).get_size();

	if (radio->has_focus())
	{
		int focus_left = checker_rect.right + text_gap - 2; // todo: remove magic number hacks
		Rect focus_rect = RectPS(focus_left, content_rect.top, text_size.width+4, content_rect.bottom);
		part_focus.render_box(canvas, focus_rect);
	}
}

Callback_v1<RadioButton*> RadioButton_Impl::uncheck_radio_buttons(GUIComponent *parent)
{
	Callback_v1<RadioButton*> callback;

	std::vector<GUIComponent*> group = parent->get_child_component_group(radio->get_group_name());

	std::vector<GUIComponent*>::size_type i;
	for (i=0; i < group.size(); i++)
	{
		RadioButton *rb = dynamic_cast<RadioButton*>(group[i]);

		if (rb && rb->get_group_name() == radio->get_group_name())
		{
			if (callback.is_null() && !rb->func_group_selection_changed().is_null())
			{
				callback = rb->func_group_selection_changed();
			}

			if (rb->impl->part_checker.get_pseudo_class(CssStr::checked))
			{
				if (!rb->impl->func_unselected.is_null())
					rb->impl->func_unselected.invoke();

				rb->impl->part_checker.set_pseudo_class(CssStr::checked, false);
				rb->impl->part_checker.set_pseudo_class(CssStr::unchecked, true);
				rb->set_selected_in_component_group(false);
				rb->request_repaint();
			}
		}
	}

	return callback;
}

void RadioButton_Impl::create_parts()
{
	part_checker = GUIThemePart(radio, CssStr::RadioButton::part_checker);
	part_checker.set_pseudo_class(CssStr::hot, false);
	part_checker.set_pseudo_class(CssStr::normal, true);
	part_checker.set_pseudo_class(CssStr::pressed, false);
	part_checker.set_pseudo_class(CssStr::checked, false);
	part_checker.set_pseudo_class(CssStr::indeterminated, false);
	part_checker.set_pseudo_class(CssStr::unchecked, true);
	part_checker.set_pseudo_class(CssStr::disabled, false);

	part_focus = GUIThemePart(radio, CssStr::RadioButton::part_focus);
}

void RadioButton_Impl::on_enablemode_changed()
{
	radio->set_pseudo_class(CssStr::disabled, !radio->is_enabled());
	radio->set_pseudo_class(CssStr::normal, radio->is_enabled());
	part_checker.set_pseudo_class(CssStr::disabled, !radio->is_enabled());
	part_checker.set_pseudo_class(CssStr::normal, radio->is_enabled());

	radio->request_repaint();
}

}
