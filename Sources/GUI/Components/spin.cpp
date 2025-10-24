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

#ifndef WIN32
#include <climits>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Spin_Impl Class:

class CL_Spin_Impl
{
public:
	CL_Spin_Impl() :
	  component(0),
	  lineedit(0),
	  value_i(0),
	  value_d(0.0f),
	  min_value_i(INT_MIN), 
	  max_value_i(INT_MAX),
	  min_value_d(-1000.0f),
	  max_value_d(1000.0f),
	  num_decimal_places(2),
	  floating_point_mode(false)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();
	void on_style_changed();
	void on_lineedit_modified(CL_InputEvent &event);
	void on_enablemode_changed();

	void create_components();
	
	void clamp_value();
	void update_lineedit();
	void decrement_value();
	void increment_value();
	CL_Spin *component;
	CL_LineEdit *lineedit;
	
	CL_GUIThemePart part_component;
	CL_GUIThemePart part_button_up;
	CL_GUIThemePart part_button_down;
	CL_GUIThemePart part_arrow_up;
	CL_GUIThemePart part_arrow_down;

	CL_Rect button_up_rect;
	CL_Rect button_down_rect;

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

	CL_Callback_v0 func_value_changed;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Spin Construction:

CL_Spin::CL_Spin(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_Spin_Impl)
{
	set_type_name(CssStr::Spin::type_name);
	impl->component = this;
	func_process_message().set(impl.get(), &CL_Spin_Impl::on_process_message);
	func_render().set(impl.get(), &CL_Spin_Impl::on_render);
	func_resized().set(impl.get(), &CL_Spin_Impl::on_resized);
	func_enablemode_changed().set(impl.get(), &CL_Spin_Impl::on_enablemode_changed);

	impl->create_components();
}

CL_Spin::~CL_Spin()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Spin Attributes:

CL_Spin *CL_Spin::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_Spin *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_Spin*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_Spin named item: %1", id));

	return object;
}

int CL_Spin::get_value() const
{
	return impl->value_i;
}

int CL_Spin::get_min() const
{
	return impl->min_value_i;
}

int CL_Spin::get_max() const
{
	return impl->max_value_i;
}

bool CL_Spin::get_floating_point_mode() const
{
	return impl->floating_point_mode;
}

float CL_Spin::get_value_float() const
{
	return impl->value_d;
}

float CL_Spin::get_min_float() const
{
	return impl->min_value_d;
}

float CL_Spin::get_max_float() const
{
	return impl->max_value_d;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Spin Operations:

void CL_Spin::set_value(int value)
{
	impl->value_i = value;
	impl->clamp_value();
	impl->update_lineedit();
}

void CL_Spin::set_ranges(int minv, int maxv)
{
	impl->min_value_i = minv;
	impl->max_value_i = maxv;
	impl->clamp_value();
	impl->update_lineedit();
}

void CL_Spin::set_step_size(int step_size)
{
	impl->step_size_i = step_size;
}

void CL_Spin::set_value_float(float value)
{
	impl->value_d = value;
	impl->clamp_value();
	impl->update_lineedit();
}

void CL_Spin::set_ranges_float(float minv, float maxv)
{
	impl->min_value_d = minv;
	impl->max_value_d = maxv;
	impl->clamp_value();
	impl->update_lineedit();
}

void CL_Spin::set_step_size_float(float step_size)
{
	impl->step_size_d = step_size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Spin Events:

CL_Callback_v0 &CL_Spin::func_value_changed()
{
	return impl->func_value_changed;
}

void CL_Spin::set_number_of_decimal_places(int decimal_places)
{
	impl->num_decimal_places = decimal_places;
}

void CL_Spin::set_floating_point_mode(bool use_floating_point)
{
	impl->lineedit->set_numeric_mode(true, true);
	impl->floating_point_mode = use_floating_point;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Spin Implementation:

void CL_Spin_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (component->is_enabled())
	{
		if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
		{
			CL_GUIMessage_Input input_msg = msg;
			CL_InputEvent e = input_msg.get_event();

			if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
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
			else if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
			{
				part_button_down.set_state(CssStr::pressed, false);
				part_button_up.set_state(CssStr::pressed, false);
				part_arrow_down.set_state(CssStr::pressed, false);
				part_arrow_up.set_state(CssStr::pressed, false);
				part_component.set_state(CssStr::pressed, false);
				component->request_repaint();
				msg.set_consumed();
			}
			else if (e.type == CL_InputEvent::pressed && e.id == CL_KEY_UP)
			{
				increment_value();
				msg.set_consumed();
			}
			else if (e.type == CL_InputEvent::pressed && e.id == CL_KEY_DOWN)
			{
				decrement_value();
				msg.set_consumed();
			}
			else if (e.type == CL_InputEvent::pointer_moved)
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
		else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
		{
			CL_GUIMessage_Pointer pointer = msg;
			if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
			{
				part_component.set_state(CssStr::hot, true);
				component->request_repaint();
			}
			else if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
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
		else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name()))
		{
			CL_GUIMessage_FocusChange fmsg(msg);

			if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus)
			{
				// Give focus to lineedit
				lineedit->set_focus();
			}
		}
	}
}

void CL_Spin_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = component->get_geometry().get_size();
	part_component.render_box(gc, rect, update_rect);
	part_button_up.render_box(gc, button_up_rect, update_rect);
	part_button_down.render_box(gc, button_down_rect, update_rect);
	part_arrow_up.render_box(gc, button_up_rect, update_rect);
	part_arrow_down.render_box(gc, button_down_rect, update_rect);
}

void CL_Spin_Impl::create_components()
{
	lineedit = new CL_LineEdit(component);
	lineedit->set_class_name("spin");
	lineedit->func_after_edit_changed().set(this, &CL_Spin_Impl::on_lineedit_modified);
	lineedit->set_numeric_mode(true);

	part_component = CL_GUIThemePart(component);
	part_button_down = CL_GUIThemePart(component, CssStr::Spin::part_button_down);
	part_button_up = CL_GUIThemePart(component, CssStr::Spin::part_button_up);
	part_arrow_down = CL_GUIThemePart(component, CssStr::Spin::part_arrow_down);
	part_arrow_up = CL_GUIThemePart(component, CssStr::Spin::part_arrow_up);

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


void CL_Spin_Impl::on_resized()
{
	CL_Rect rect = component->get_geometry().get_size();
	CL_Rect content_rect = part_component.get_content_box(rect);
	CL_Rect lineedit_rect(content_rect.left, content_rect.top, content_rect.right-part_button_down.get_preferred_width(), content_rect.bottom);
	lineedit->set_geometry(lineedit_rect);

	if (floating_point_mode)
		lineedit->set_text(CL_StringHelp::float_to_text(value_d, num_decimal_places));
	else
		lineedit->set_text(value_i);

	button_up_rect = CL_Rect(content_rect.right-part_button_up.get_preferred_width(), content_rect.top, content_rect.right, content_rect.get_center().y);
	button_down_rect = CL_Rect(content_rect.right-part_button_up.get_preferred_width(), button_up_rect.bottom, content_rect.right, content_rect.bottom);
}

void CL_Spin_Impl::on_style_changed()
{
	delete lineedit;
	create_components();
	if (floating_point_mode)
		lineedit->set_text(CL_StringHelp::float_to_text(value_d, num_decimal_places));
	else
    	lineedit->set_text(value_i);
}

void CL_Spin_Impl::on_lineedit_modified(CL_InputEvent &event)
{
	if (floating_point_mode)
	{
		float original_value = value_d;
		value_d = lineedit->get_text_float();

		if (value_d != original_value && !lineedit->get_text().empty()) 
		{
			clamp_value();
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
			clamp_value();
			if (!func_value_changed.is_null())
				func_value_changed.invoke();
		}
	}
}

void CL_Spin_Impl::clamp_value()
{
	bool changed = false;
	if (value_i > max_value_i) { changed=true; value_i = max_value_i; }
	if (value_i < min_value_i) { changed=true; value_i = min_value_i; }
	if (value_d > max_value_d) { changed=true; value_d = max_value_d; }
	if (value_d < min_value_d) { changed=true; value_d = min_value_d; }
}

void CL_Spin_Impl::on_enablemode_changed()
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

void CL_Spin_Impl::increment_value()
{
	if (floating_point_mode)
		value_d += step_size_d;
	else
		value_i += step_size_i;

	clamp_value();
	update_lineedit();
}

void CL_Spin_Impl::decrement_value()
{
	if (floating_point_mode)
		value_d -= step_size_d;
	else
		value_i -= step_size_i;

	clamp_value();
	update_lineedit();
}

void CL_Spin_Impl::update_lineedit()
{
	if (floating_point_mode)
		lineedit->set_text(CL_StringHelp::float_to_text(value_d, num_decimal_places));
	else
		lineedit->set_text(value_i);
}