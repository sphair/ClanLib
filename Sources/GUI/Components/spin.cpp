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
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/spin.h"
#include "API/GUI/Components/lineedit.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "../gui_css_strings.h"
#include "API/Core/Math/rect.h"

#ifndef _MSC_VER
#include <climits>
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Spin_Impl Class:

class Spin_Impl
{
public:
	Spin_Impl() :
	  component(0),
	  lineedit(0),
	  value_i(0),
	  value_d(0.0f),
	  min_value_i(INT_MIN), 
	  max_value_i(INT_MAX),
	  min_value_d(-1000.0f),
	  max_value_d(1000.0f),
	  num_decimal_places(2),
	  floating_point_mode(false),
	  step_size_i(1),
	  step_size_d(1.0f)
	{
	}

	void on_process_message(GUIMessage &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_resized();
	void on_style_changed();
	void on_lineedit_modified(InputEvent &event);
	void on_enablemode_changed();

	void create_components();
	
	bool clamp_value();
	void update_lineedit();
	void decrement_value();
	void increment_value();
	Spin *component;
	LineEdit *lineedit;
	
	GUIThemePart part_component;
	GUIThemePart part_button_up;
	GUIThemePart part_button_down;
	GUIThemePart part_arrow_up;
	GUIThemePart part_arrow_down;

	Rect button_up_rect;
	Rect button_down_rect;

	int num_decimal_places;
	int value_i;
	int min_value_i;
	int max_value_i;
	int step_size_i;
	bool floating_point_mode;
	float value_d;
	float min_value_d;
	float max_value_d;
	float step_size_d;

	Callback_v0 func_value_changed;
};

/////////////////////////////////////////////////////////////////////////////
// Spin Construction:

Spin::Spin(GUIComponent *parent)
: GUIComponent(parent), impl(new Spin_Impl)
{
	set_type_name(CssStr::Spin::type_name);
	impl->component = this;
	set_double_click_enabled(false);
	func_process_message().set(impl.get(), &Spin_Impl::on_process_message);
	func_render().set(impl.get(), &Spin_Impl::on_render);
	func_resized().set(impl.get(), &Spin_Impl::on_resized);
	func_enablemode_changed().set(impl.get(), &Spin_Impl::on_enablemode_changed);

	impl->create_components();
}

Spin::~Spin()
{
}

/////////////////////////////////////////////////////////////////////////////
// Spin Attributes:

Spin *Spin::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	Spin *object = NULL;
	if (reference_component)
		object = dynamic_cast<Spin*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find Spin named item: %1", id));

	return object;
}

int Spin::get_value() const
{
	return impl->value_i;
}

int Spin::get_min() const
{
	return impl->min_value_i;
}

int Spin::get_max() const
{
	return impl->max_value_i;
}

bool Spin::get_floating_point_mode() const
{
	return impl->floating_point_mode;
}

float Spin::get_value_float() const
{
	return impl->value_d;
}

float Spin::get_min_float() const
{
	return impl->min_value_d;
}

float Spin::get_max_float() const
{
	return impl->max_value_d;
}

/////////////////////////////////////////////////////////////////////////////
// Spin Operations:

void Spin::set_value(int value)
{
	set_floating_point_mode(false);
	impl->value_i = value;
	impl->clamp_value();
	impl->update_lineedit();
}

void Spin::set_ranges(int minv, int maxv)
{
	set_floating_point_mode(false);
	impl->min_value_i = minv;
	impl->max_value_i = maxv;
	impl->clamp_value();
	impl->update_lineedit();
}

void Spin::set_step_size(int step_size)
{
	set_floating_point_mode(false);
	impl->step_size_i = step_size;
}

void Spin::set_value_float(float value)
{
	set_floating_point_mode(true);
	impl->value_d = value;
	impl->clamp_value();
	impl->update_lineedit();
}

void Spin::set_ranges_float(float minv, float maxv)
{
	set_floating_point_mode(true);
	impl->min_value_d = minv;
	impl->max_value_d = maxv;
	impl->clamp_value();
	impl->update_lineedit();
}

void Spin::set_step_size_float(float step_size)
{
	set_floating_point_mode(true);
	impl->step_size_d = step_size;
}

/////////////////////////////////////////////////////////////////////////////
// Spin Events:

Callback_v0 &Spin::func_value_changed()
{
	return impl->func_value_changed;
}

void Spin::set_number_of_decimal_places(int decimal_places)
{
	impl->num_decimal_places = decimal_places;
}

void Spin::set_floating_point_mode(bool use_floating_point)
{
	if (use_floating_point != impl->floating_point_mode)
	{
		impl->lineedit->set_numeric_mode(true, use_floating_point);
		impl->floating_point_mode = use_floating_point;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Spin Implementation:

void Spin_Impl::on_process_message(GUIMessage &msg)
{
	if (component->is_enabled())
	{
		if (msg.is_type(GUIMessage_Input::get_type_name()))
		{
			GUIMessage_Input input_msg = msg;
			InputEvent e = input_msg.get_event();

			if (e.type == InputEvent::pressed && e.id == mouse_left)
			{
				bool down_pressed = button_down_rect.contains(e.mouse_pos);
				bool up_pressed = button_up_rect.contains(e.mouse_pos);
				part_button_down.set_state(CssStr::pressed, down_pressed);
				part_button_up.set_state(CssStr::pressed, up_pressed);
				part_arrow_down.set_state(CssStr::pressed, down_pressed);
				part_arrow_up.set_state(CssStr::pressed, up_pressed);

				if (up_pressed || down_pressed)
				{
					if (up_pressed)
						increment_value();
					else if (down_pressed)
						decrement_value();

					if (!func_value_changed.is_null())
						func_value_changed.invoke();
					component->request_repaint();
				}
				msg.set_consumed();
			}
			else if (e.type == InputEvent::released && e.id == mouse_left)
			{
				part_button_down.set_state(CssStr::pressed, false);
				part_button_up.set_state(CssStr::pressed, false);
				part_arrow_down.set_state(CssStr::pressed, false);
				part_arrow_up.set_state(CssStr::pressed, false);
				part_component.set_state(CssStr::pressed, false);
				component->request_repaint();
				msg.set_consumed();
			}
			else if (e.type == InputEvent::pressed && e.id == keycode_up)
			{
				increment_value();
				msg.set_consumed();
			}
			else if (e.type == InputEvent::pressed && e.id == keycode_down)
			{
				decrement_value();
				msg.set_consumed();
			}
			else if (e.type == InputEvent::pointer_moved)
			{
				bool new_hot_state_arrow_down = button_down_rect.contains(e.mouse_pos);
				bool new_hot_state_arrow_up = button_up_rect.contains(e.mouse_pos);

				if (part_arrow_down.get_state(CssStr::hot) != new_hot_state_arrow_down)
				{
					part_button_down.set_state(CssStr::hot, new_hot_state_arrow_down);
					part_arrow_down.set_state(CssStr::hot, new_hot_state_arrow_down);
					component->request_repaint();
				}
				if (part_arrow_up.get_state(CssStr::hot) != new_hot_state_arrow_up)
				{
					part_button_up.set_state(CssStr::hot, new_hot_state_arrow_up);
					part_arrow_up.set_state(CssStr::hot, new_hot_state_arrow_up);
					component->request_repaint();
				}
				msg.set_consumed();
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
			else if (pointer.get_pointer_type() == GUIMessage_Pointer::pointer_leave)
			{
				part_component.set_state(CssStr::hot, false);

				part_button_up.set_state(CssStr::pressed, false);
				part_button_down.set_state(CssStr::pressed, false);
				part_arrow_up.set_state(CssStr::pressed, false);
				part_arrow_down.set_state(CssStr::pressed, false);

				part_button_up.set_state(CssStr::hot, false);
				part_button_down.set_state(CssStr::hot, false);
				part_arrow_up.set_state(CssStr::hot, false);
				part_arrow_down.set_state(CssStr::hot, false);

				component->request_repaint();
			}
			msg.set_consumed();
		}
		else if (msg.is_type(GUIMessage_FocusChange::get_type_name()))
		{
			GUIMessage_FocusChange fmsg(msg);

			if (fmsg.get_focus_type() == GUIMessage_FocusChange::gained_focus)
			{
				// Give focus to lineedit
				lineedit->set_focus();
			}
		}
	}
}

void Spin_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = component->get_geometry().get_size();
	part_component.render_box(canvas, rect, update_rect);
	part_button_up.render_box(canvas, button_up_rect, update_rect);
	part_button_down.render_box(canvas, button_down_rect, update_rect);
	part_arrow_up.render_box(canvas, button_up_rect, update_rect);
	part_arrow_down.render_box(canvas, button_down_rect, update_rect);
}

void Spin_Impl::create_components()
{
	lineedit = new LineEdit(component);
	lineedit->set_class_name("spin");
	lineedit->func_after_edit_changed().set(this, &Spin_Impl::on_lineedit_modified);
	lineedit->set_numeric_mode(true);

	part_component = GUIThemePart(component);
	part_button_down = GUIThemePart(component, CssStr::Spin::part_button_down);
	part_button_up = GUIThemePart(component, CssStr::Spin::part_button_up);
	part_arrow_down = GUIThemePart(component, CssStr::Spin::part_arrow_down);
	part_arrow_up = GUIThemePart(component, CssStr::Spin::part_arrow_up);

	bool enabled = component->is_enabled();

	part_component.set_state(CssStr::normal, enabled);
	part_button_up.set_state(CssStr::normal, enabled);
	part_button_down.set_state(CssStr::normal, enabled);
	part_arrow_up.set_state(CssStr::normal, enabled);
	part_arrow_down.set_state(CssStr::normal, enabled);

	part_component.set_state(CssStr::disabled, !enabled);
	part_button_up.set_state(CssStr::disabled, !enabled);
	part_button_down.set_state(CssStr::disabled, !enabled);
	part_arrow_up.set_state(CssStr::disabled, !enabled);
	part_arrow_down.set_state(CssStr::disabled, !enabled);
}


void Spin_Impl::on_resized()
{
	Rect rect = component->get_geometry().get_size();
	Rect content_rect = part_component.get_content_box(rect);
	Rect lineedit_rect(content_rect.left, content_rect.top, content_rect.right-part_button_down.get_preferred_width(), content_rect.bottom);
	lineedit->set_geometry(lineedit_rect);

	if (floating_point_mode)
		lineedit->set_text(StringHelp::float_to_text(value_d, num_decimal_places));
	else
		lineedit->set_text(value_i);

	button_up_rect = Rect(content_rect.right-part_button_up.get_preferred_width(), content_rect.top, content_rect.right, content_rect.get_center().y);
	button_down_rect = Rect(content_rect.right-part_button_up.get_preferred_width(), button_up_rect.bottom, content_rect.right, content_rect.bottom);
}

void Spin_Impl::on_style_changed()
{
	delete lineedit;
	create_components();
	if (floating_point_mode)
		lineedit->set_text(StringHelp::float_to_text(value_d, num_decimal_places));
	else
    	lineedit->set_text(value_i);
}

void Spin_Impl::on_lineedit_modified(InputEvent &event)
{
	if (floating_point_mode)
	{
		float original_value = value_d;
		value_d = lineedit->get_text_float();

		if (value_d != original_value && !lineedit->get_text().empty()) 
		{
			if (clamp_value())
				update_lineedit();

			if (!func_value_changed.is_null())
				func_value_changed.invoke();
		}
	}
	else
	{
		int original_value = value_i;
		value_i = lineedit->get_text_int();
		if (value_i != original_value && !lineedit->get_text().empty()) 
		{
			if (clamp_value())
				update_lineedit();

			if (!func_value_changed.is_null())
				func_value_changed.invoke();
		}
	}
}

bool Spin_Impl::clamp_value()
{
	bool changed = false;
	if (value_i > max_value_i) { changed=true; value_i = max_value_i; }
	if (value_i < min_value_i) { changed=true; value_i = min_value_i; }
	if (value_d > max_value_d) { changed=true; value_d = max_value_d; }
	if (value_d < min_value_d) { changed=true; value_d = min_value_d; }
	return changed;
}

void Spin_Impl::on_enablemode_changed()
{
	bool enabled = component->is_enabled();

	part_component.set_state(CssStr::normal, enabled);
	part_button_up.set_state(CssStr::normal, enabled);
	part_button_down.set_state(CssStr::normal, enabled);
	part_arrow_up.set_state(CssStr::normal, enabled);
	part_arrow_down.set_state(CssStr::normal, enabled);

	part_component.set_state(CssStr::disabled, !enabled);
	part_button_up.set_state(CssStr::disabled, !enabled);
	part_button_down.set_state(CssStr::disabled, !enabled);
	part_arrow_up.set_state(CssStr::disabled, !enabled);
	part_arrow_down.set_state(CssStr::disabled, !enabled);

	lineedit->set_enabled(enabled);

	component->request_repaint();
}

void Spin_Impl::increment_value()
{
	if (floating_point_mode)
		value_d += step_size_d;
	else
		value_i += step_size_i;

	clamp_value();
	update_lineedit();
}

void Spin_Impl::decrement_value()
{
	if (floating_point_mode)
		value_d -= step_size_d;
	else
		value_i -= step_size_i;

	clamp_value();
	update_lineedit();
}

void Spin_Impl::update_lineedit()
{
	if (floating_point_mode)
		lineedit->set_text(value_d, num_decimal_places);
	else
		lineedit->set_text(value_i);
}

}
