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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
*/

#include "GUI/precomp.h"
#include "gui_manager_impl.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_resize.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Font/font.h"
#include "API/Core/Text/string_format.h"
#include "gui_component_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIManager_Impl Construction:

GUIManager_Impl::GUIManager_Impl()
: mouse_capture_component(0), mouse_over_component(0), theme(0), exit_flag(false), exit_code(0), window_manager(NULL)
{
	func_focus_lost.set(this, &GUIManager_Impl::on_focus_lost);
	func_focus_gained.set(this, &GUIManager_Impl::on_focus_gained);
	func_resize.set(this, &GUIManager_Impl::on_resize);
	func_paint.set(this, &GUIManager_Impl::on_paint);
	func_close.set(this, &GUIManager_Impl::on_close);
	func_destroy.set(this, &GUIManager_Impl::on_destroy);
	func_input_received.set(this, &GUIManager_Impl::on_input_received);

	wm_site.func_focus_lost = &func_focus_lost;
	wm_site.func_focus_gained = &func_focus_gained;
	wm_site.func_resize = &func_resize;
	wm_site.func_paint = &func_paint;
	wm_site.func_close = &func_close;
	wm_site.func_destroy = &func_destroy;
	wm_site.func_input_received = &func_input_received;
}

GUIManager_Impl::~GUIManager_Impl()
{
	while (!root_components.empty())
	{
		GUITopLevelWindow *window = root_components.front();

		// The following line deletes the GUIComponent, which should cause it to clean up every aspect of the window.
		// Do not add additional window clean up code here, since that would cause it not to be called if the
		// application deletes a component by itself. -- mbn 22. Oct 2008
		delete window->component;
	}
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager_Impl Attributes:

Font GUIManager_Impl::get_registered_font(const FontDescription &desc)
{
	// Perform exact matches
	std::vector<NamedFontCacheEntry>::iterator it;
	for (it = named_font_cache.begin(); it != named_font_cache.end(); ++it)
	{
		if ((*it).desc == desc)
			return (*it).font;
	}

	FontDescription desc_copy;
	// Match font where the registered font details have not been set
	for (it = named_font_cache.begin(); it != named_font_cache.end(); ++it)
	{
		desc_copy.clone(desc);
		const FontDescription &current_desc = (*it).desc;

		// Copy the unset details
		if (current_desc.get_height() == 0)
			desc_copy.set_height(0);

		if (current_desc.get_average_width() == 0)
			desc_copy.set_average_width(0);

		if (current_desc.get_weight() == 0)
			desc_copy.set_weight(0);

		if (current_desc == desc_copy)
			return (*it).font;
	}

	return Font();
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager_Impl Operations:

void GUIManager_Impl::add_component(GUIComponent *component, GUIComponent *owner, GUITopLevelDescription desc)
{
	GUITopLevelWindow *top_level_window = new GUITopLevelWindow();
	top_level_window->component = component;
	top_level_window->focused_component = component;
	top_level_window->owner = 0;
	top_level_window->proximity_component = 0;

	GUITopLevelWindow *owner_window = 0;

	if (owner)
	{
		top_level_window->owner = owner;

		GUIComponent *root_owner = top_level_window->owner->get_top_level_component();
		std::vector<GUITopLevelWindow *>::size_type pos, size;
		size = root_components.size();
		for (pos = 0; pos < size; pos++)
		{
			if (root_components[pos]->component == root_owner)
			{
				owner_window = root_components[pos];
				break;
			}
		}
	}

	try
	{
		window_manager.create_window(top_level_window, owner_window, component, desc);
	}
	catch (const Exception &e)
	{
		delete top_level_window;
		throw;
	}

	root_components.push_back(top_level_window);
}

void GUIManager_Impl::remove_component(GUIComponent_Impl *component_impl)
{
	if (mouse_capture_component && mouse_capture_component->impl.get() == component_impl)
	{
		window_manager.capture_mouse(get_toplevel_window(mouse_capture_component), false);
		mouse_capture_component = 0;
	}

	if (mouse_over_component && mouse_over_component->impl.get() == component_impl)
		mouse_over_component = 0;

	std::vector<GUITopLevelWindow>::size_type index, size;

	// Remove the components that this component owns
	bool owner_component_deleted;
	do
	{
		owner_component_deleted = false;
		size = root_components.size();

		for (index = 0; index < size; index++)
		{
			GUITopLevelWindow *cur = root_components[index];

			if (cur->owner)
			{
				if (cur->owner->impl.get() == component_impl)
				{
					// Found owned component
					delete (cur->component);

					// Be VERY carefuly at this point, as this is a recursive function, and root_components will have changed
					owner_component_deleted = true;
					break;
				}
			}
		}
	}while(owner_component_deleted);

	// Remove this component
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		GUITopLevelWindow *cur = root_components[index];

		// Clear focus pointer if needed:
		if (cur->focused_component && cur->focused_component->impl.get() == component_impl)
			cur->focused_component = 0;

		// Remove display window if needed:
		if (cur->component->impl.get() == component_impl)
		{
			window_manager.destroy_window(cur);
			root_components.erase(root_components.begin() + index);
			delete cur;
			break;
		}
	}
}

void GUIManager_Impl::gain_focus(GUIComponent *component)
{
	GUITopLevelWindow *toplevel_window = get_toplevel_window(component);
	if (toplevel_window == NULL)
	{
		throw Exception("No top level window is set");
	}

	if (toplevel_window->focused_component == component)
		return;

	if (window_manager.has_focus(toplevel_window))
	{
		if (toplevel_window->focused_component)
		{
			GUITopLevelWindow_Alive toplevel_window_alive(toplevel_window);

			GUIMessage_FocusChange message;
			message.set_target(toplevel_window->focused_component);
			message.set_focus_type(GUIMessage_FocusChange::losing_focus);
			dispatch_message(message);

			if (toplevel_window_alive.is_null())	// Top level window was destroyed
				return;
		}

		toplevel_window->focused_component = component;

		// The defaulted component (usually a PushButton in dialogs) needs to update its "default" GUIThemePart state depending on 
		// whether the focused component consumes certain input events.
		// This should be done before sending the gained_focus message
		GUIComponent *default_comp = get_default_component(toplevel_window->component);
		if (default_comp && !default_comp->func_style_changed().is_null())
			default_comp->func_style_changed().invoke();

		if (toplevel_window->focused_component)
		{
			GUIMessage_FocusChange message;
			message.set_target(toplevel_window->focused_component);
			message.set_focus_type(GUIMessage_FocusChange::gained_focus);
			dispatch_message(message);
		}
	}
	else
	{
		// Don't need to send messages if this window is not in focus - it this the correct behaviour?
		toplevel_window->focused_component = component;
	}
}

void GUIManager_Impl::loose_focus(GUIComponent *component)
{

	GUITopLevelWindow *toplevel_window = get_toplevel_window(component);
	if (toplevel_window == NULL)
		return;

	if (component != toplevel_window->focused_component)
	{
		return;	// Not in focus
	}

	if (window_manager.has_focus(toplevel_window))
	{
		if (toplevel_window->focused_component)
		{
			GUITopLevelWindow_Alive toplevel_window_alive(toplevel_window);

			GUIMessage_FocusChange message;
			message.set_target(toplevel_window->focused_component);
			message.set_focus_type(GUIMessage_FocusChange::losing_focus);
			dispatch_message(message);

			if (toplevel_window_alive.is_null())	// Top level window was destroyed
				return;
		}

		toplevel_window->focused_component = NULL;

		// The defaulted component (usually a PushButton in dialogs) needs to update its "default" GUIThemePart state depending on 
		// whether the focused component consumes certain input events.
		// This should be done before sending the gained_focus message
		GUIComponent *default_comp = get_default_component(toplevel_window->component);
		if (default_comp && !default_comp->func_style_changed().is_null())
			default_comp->func_style_changed().invoke();
	}
	else
	{
		// Don't need to send messages if this window is not in focus - it this the correct behaviour?
		toplevel_window->focused_component = NULL;
	}
}

void GUIManager_Impl::set_enabled(GUIComponent *component, bool enable)
{
	window_manager.enable_window(get_toplevel_window(component), enable);
}

bool GUIManager_Impl::has_focus(const GUIComponent *component) const
{
	GUITopLevelWindow *toplevel_window = get_toplevel_window(component);
	if (toplevel_window->focused_component == component)
		return window_manager.has_focus(toplevel_window);
	else
		return false;
}

GUITopLevelWindow *GUIManager_Impl::get_toplevel_window(const GUIComponent * const component) const
{
	GUIComponent *root = (GUIComponent *) component;
	while (root->get_parent_component())
		root = root->get_parent_component();

	std::vector<GUITopLevelWindow>::size_type index, size;
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		GUITopLevelWindow *cur = root_components[index];
		if (cur->component == root)
			return cur;
	}
	return 0;
}

GUIComponent *GUIManager_Impl::get_owner_component(const GUIComponent *component)
{
	std::vector<GUITopLevelWindow>::size_type index, size;
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		GUITopLevelWindow *cur = root_components[index];
		if (cur->component == component)
			return cur->owner;
	}
	return 0;
}

const GUIComponent *GUIManager_Impl::get_owner_component(const GUIComponent *component) const
{
	std::vector<GUITopLevelWindow>::size_type index, size;
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		GUITopLevelWindow *cur = root_components[index];
		if (cur->component == component)
			return cur->owner;
	}
	return 0;
}

void GUIManager_Impl::deliver_message(GUIMessage &m)
{
	sig_filter_message.invoke(m);

	if (!m.is_consumed())
	{
		GUIComponent *target = m.get_target();
		if (target)
		{
			bool double_click_changed_to_single_click = false;
			if (m.get_type() == GUIMessage_Input::get_type_name() && !target->is_double_click_enabled())
			{
				GUIMessage_Input m_input(m);
				InputEvent input_event = m_input.get_event();
				if (input_event.type == InputEvent::doubleclick)
				{
					input_event.type = InputEvent::pressed;
					double_click_changed_to_single_click = true;
					m_input.set_event(input_event);
				}
			}

			if (!target->func_filter_message().is_null())
			{
				target->func_filter_message().invoke(m);
			}

			if (!m.is_consumed())
			{
				if (!target->func_process_message().is_null())
					target->func_process_message().invoke(m);
			}

			if (!m.is_consumed())
			{
				if (m.get_type() == GUIMessage_ActivationChange::get_type_name())
				{
					switch (GUIMessage_ActivationChange(m).get_activation_type())
					{
					case GUIMessage_ActivationChange::activation_lost:
						if (!target->func_deactivated().is_null() && target->func_deactivated().invoke())
							m.set_consumed();
						break;
					case GUIMessage_ActivationChange::activation_gained:
						if (!target->func_activated().is_null() && target->func_activated().invoke())
							m.set_consumed();
						break;
					}
				}
				else if (m.get_type() == GUIMessage_Close::get_type_name())
				{
					if (!target->func_close().is_null() && target->func_close().invoke())
						m.set_consumed();
				}
				else if (m.get_type() == GUIMessage_FocusChange::get_type_name())
				{
					switch (GUIMessage_FocusChange(m).get_focus_type())
					{
					case GUIMessage_FocusChange::losing_focus:
						if (!target->func_focus_lost().is_null() && target->func_focus_lost().invoke())
							m.set_consumed();
						break;
					case GUIMessage_FocusChange::gained_focus:
						if (!target->func_focus_gained().is_null() && target->func_focus_gained().invoke())
							m.set_consumed();
						break;
					}
				}
				else if (m.get_type() == GUIMessage_Input::get_type_name())
				{
					GUIMessage_Input m_input(m);
					InputEvent e = m_input.get_event();

					if (!target->func_input().is_null() && target->func_input().invoke(e))
						m.set_consumed();

					switch (e.type)
					{
					case InputEvent::pressed:
						if (!target->func_input_pressed().is_null() && target->func_input_pressed().invoke(e))
							m.set_consumed();
						break;
					case InputEvent::released:
						if (!target->func_input_released().is_null() && target->func_input_released().invoke(e))
							m.set_consumed();
						break;
					case InputEvent::doubleclick:
						if (!target->func_input_doubleclick().is_null() && target->func_input_doubleclick().invoke(e))
							m.set_consumed();
						break;
					case InputEvent::pointer_moved:
						if (!target->func_input_pointer_moved().is_null() && target->func_input_pointer_moved().invoke(e))
							m.set_consumed();
						break;
					default:
						break;
					}
				}
				else if (m.get_type() == GUIMessage_Pointer::get_type_name())
				{
					switch (GUIMessage_Pointer(m).get_pointer_type())
					{
					case GUIMessage_Pointer::pointer_enter:
						if (!target->func_pointer_enter().is_null() && target->func_pointer_enter().invoke())
							m.set_consumed();
						break;
					case GUIMessage_Pointer::pointer_leave:
						if (!target->func_pointer_exit().is_null() && target->func_pointer_exit().invoke())
							m.set_consumed();
						break;
					}
				}
				else if (m.get_type() == GUIMessage_Resize::get_type_name())
				{
					// No need to do anything here. Already handled elsewhere.
					m.set_consumed();
				}
			}

			if (double_click_changed_to_single_click)
			{
				GUIMessage_Input m_input(m);
				InputEvent input_event = m_input.get_event();
				input_event.type = InputEvent::doubleclick;
				m_input.set_event(input_event);
			}
		}
	}
}

void GUIManager_Impl::dispatch_message(GUIMessage &message)
{
	GUIMessage m = message;
	deliver_message(m);

	GUIComponent *message_original_target = m.get_target();

	// Unconsumed input message are sent up the component tree until a null parent is encountered or the message gets consumed.
	if (!m.is_consumed() && m.get_target() && m.is_type(GUIMessage_Input::get_type_name()))
	{
		while (m.get_target() && !m.is_consumed())
		{
			// Translate mouse pos to parent coordinates.
			GUIMessage_Input input(m);
			InputEvent event = input.get_event();
			event.mouse_pos.x += m.get_target()->get_geometry().left;
			event.mouse_pos.y += m.get_target()->get_geometry().top;
			input.set_event(event);

			m.set_target(m.get_target()->get_parent_component());
			if (m.get_target())
				deliver_message(m);
		}
	}

	if (m.get_target() == 0 && !m.is_consumed())
		m.set_target(message_original_target);
}

GUIComponent *GUIManager_Impl::get_focus_component()
{
	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = root_components.size();

	for( pos = 0; pos < size; pos++ )
	{
		if( window_manager.has_focus(root_components[pos]) )
		{
			return root_components[pos]->focused_component;
		}
	}

	return 0;
}

std::vector<CSSProperty> &GUIManager_Impl::get_properties(const std::string &element_name) const
{
	std::map< std::string, std::vector<CSSProperty> >::iterator it = properties_cache.find(element_name);
	if (it != properties_cache.end())
		return it->second;
	properties_cache[element_name] = css_document.select(element_name);
	return properties_cache[element_name];
}

void GUIManager_Impl::reset_properties()
{
	properties_cache.clear();
}

void GUIManager_Impl::register_font(const Font &font, const FontDescription &desc)
{
	if (get_registered_font(desc).is_null())
	{
		NamedFontCacheEntry entry;
		entry.desc = desc;
		entry.font = font;
		named_font_cache.push_back(entry);
	}
}

bool GUIManager_Impl::is_constant_repaint_enabled() const
{
	for (std::vector<GUITopLevelWindow *>::size_type i = 0; i < root_components.size(); i++)
	{
		if (is_constant_repaint_enabled(root_components[i]->component))
			return true;
	}
	return false;
}

bool GUIManager_Impl::is_constant_repaint_enabled(GUIComponent *component) const
{
	if (component->get_constant_repaint())
	{
		return true;
	}
	else
	{
		std::vector<GUIComponent *> components = component->get_child_components();
		for (std::vector<GUIComponent *>::size_type i = 0; i < components.size(); i++)
		{
			if (is_constant_repaint_enabled(components[i]))
				return true;
		}
		return false;
	}
}

void GUIManager_Impl::invalidate_constant_repaint_components()
{
	for (std::vector<GUITopLevelWindow *>::size_type i = 0; i < root_components.size(); i++)
	{
		if (is_constant_repaint_enabled(root_components[i]->component))
		{
			GUITopLevelWindow *cur = root_components[i];
			cur->component->request_repaint();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager_Impl Implementation:

void GUIManager_Impl::on_focus_lost(GUITopLevelWindow *toplevel_window)
{
	if (toplevel_window->focused_component)
	{
		GUITopLevelWindow_Alive toplevel_window_alive(toplevel_window);

		GUIMessage_FocusChange message;
		message.set_target(toplevel_window->focused_component);
		message.set_focus_type(GUIMessage_FocusChange::losing_focus);
		dispatch_message(message);

		if (toplevel_window_alive.is_null())	// Top level window was destroyed
			return;

	}

	// toplevel_window->focused_component = 0;

	GUIMessage_ActivationChange message;
	message.set_target(toplevel_window->component);
	message.set_activation_type(GUIMessage_ActivationChange::activation_lost);
	dispatch_message(message);
}

void GUIManager_Impl::on_focus_gained(GUITopLevelWindow *toplevel_window)
{
	if (toplevel_window->focused_component == 0)
		toplevel_window->focused_component = toplevel_window->component;

	if (toplevel_window->focused_component)
	{
		GUITopLevelWindow_Alive toplevel_window_alive(toplevel_window);

		GUIMessage_FocusChange message;
		message.set_target(toplevel_window->focused_component);
		message.set_focus_type(GUIMessage_FocusChange::gained_focus);
		dispatch_message(message);

		if (toplevel_window_alive.is_null())	// Top level window was destroyed
			return;
	}

	GUIMessage_ActivationChange message;
	message.set_target(toplevel_window->component);
	message.set_activation_type(GUIMessage_ActivationChange::activation_gained);
	dispatch_message(message);
}

void GUIManager_Impl::on_resize(GUITopLevelWindow *toplevel_window, const Size &new_size)
{
	GUIComponent *component = toplevel_window->component;
	component->impl->geometry = component->get_geometry();
	component->impl->geometry_updated();

	GUIMessage_Resize message;
	message.set_target(component);
	message.set_geometry(component->get_geometry());
	dispatch_message(message);
}

void GUIManager_Impl::on_paint(GUITopLevelWindow *toplevel_window, const Rect &update_rect)
{
	toplevel_window->component->paint(update_rect);
}

void GUIManager_Impl::on_close(GUITopLevelWindow *toplevel_window)
{
	GUIMessage_Close message;
	message.set_target(toplevel_window->component);
	dispatch_message(message);
}

void GUIManager_Impl::on_destroy(GUITopLevelWindow *toplevel_window)
{
//	delete toplevel_window->component;
}

void GUIManager_Impl::on_input_received(
	GUITopLevelWindow *toplevel_window,
	const InputEvent &input_event)
{
	// Find target for input message:
	GUIComponent *target = 0;
	if (input_event.device.get_type() == InputDevice::pointer ||
		input_event.device.get_type() == InputDevice::tablet)
	{
		target = mouse_capture_component;

		if (input_event.type == InputEvent::proximity_change && toplevel_window->proximity_component)
			target = toplevel_window->proximity_component;

		if (target == 0)
			target = toplevel_window->component->get_component_at(
				toplevel_window->component->window_to_component_coords(input_event.mouse_pos));
	}
	else
	{
		if (toplevel_window->focused_component)
			target = toplevel_window->focused_component;
		else
			target = toplevel_window->component; // All keyboard messages are redirected to the toplevel component if no component is focused.
	}

	if (target)
	{
		// Localize mouse position:
		InputEvent local_input_event = input_event;
		local_input_event.mouse_pos = target->window_to_component_coords(input_event.mouse_pos);

		// For pointer movements we may need to generate enter and leave messages:
		if (local_input_event.type == InputEvent::pointer_moved &&
			target != mouse_over_component)
		{
			if (mouse_over_component)
			{
				GUIMessage_Pointer msg;
				msg.set_pointer_type(GUIMessage_Pointer::pointer_leave);
				msg.set_target(mouse_over_component);
				dispatch_message(msg);
			}
			mouse_over_component = target;
			if (mouse_over_component)
			{
				GUIMessage_Pointer msg;
				msg.set_pointer_type(GUIMessage_Pointer::pointer_enter);
				msg.set_target(mouse_over_component);
				dispatch_message(msg);
			}
		}

		GUIMessage_Input message;
		message.set_target(target);
		message.set_event(local_input_event);
		dispatch_message(message);

		if (!message.is_consumed())
			accel_table.process_message(message);

		if (!message.is_consumed())
			process_standard_gui_keys(message);
	}

}

GUIComponent *GUIManager_Impl::get_cancel_component(GUIComponent *comp)
{
	if (comp && comp->is_cancel())
		return comp;

	GUIComponent *child = comp->get_first_child();

	while (child)
	{
		if (child->is_cancel())
			return child;

		if (child->has_child_components())
		{
			GUIComponent *def = get_cancel_component(child);
			if (def != 0)
			{
				return def;
			}
		}

		child = child->get_next_sibling();
	}

	return 0;
}

GUIComponent *GUIManager_Impl::get_default_component(GUIComponent *comp)
{
	if (comp && comp->is_default())
		return comp;

	GUIComponent *child = comp->get_first_child();

	while (child)
	{
		if (child->is_default())
			return child;

		if (child->has_child_components())
		{
			GUIComponent *def = get_default_component(child);
			if (def != 0)
			{
				return def;
			}
		}

		child = child->get_next_sibling();
	}

	return 0;
}

void GUIManager_Impl::process_standard_gui_keys(GUIMessage &message)
{
	if (message.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input = message;
		InputEvent e = input.get_event();

		if (e.type == InputEvent::pressed && 
			(e.id == keycode_tab || e.id == keycode_left || e.id == keycode_right || e.id == keycode_up || e.id == keycode_down))
		{
			GUIComponent *focus_comp = input.get_target();

			if (e.id == keycode_tab)
			{
				if (!e.shift)
					focus_comp->focus_next();
				else
					focus_comp->focus_previous();
			}
			else if (e.id == keycode_left || e.id == keycode_up)
			{
				focus_comp->focus_next();
			}
			else if (e.id == keycode_right || e.id == keycode_down)
			{
				focus_comp->focus_previous();
			}
		}
		else if ((e.id == keycode_enter || e.id == keycode_return || e.id == keycode_numpad_enter) && e.type == InputEvent::released)
		{
			GUIComponent *comp = input.get_target();
			if (comp)
			{
				GUITopLevelWindow *win = get_toplevel_window(comp);
				GUIComponent *defaulted_component = get_default_component(win->component);

				if (defaulted_component)
				{
					input.set_target(defaulted_component);
					dispatch_message(input);
				}
			}
		}
		else if (e.id == keycode_escape)
		{
			GUIComponent *comp = input.get_target();
			if (comp)
			{
				GUITopLevelWindow *win = get_toplevel_window(comp);
				GUIComponent *cancel_comp = get_cancel_component(win->component);
				if (cancel_comp)
				{
					input.set_target(cancel_comp);
					dispatch_message(input);
				}
			}
		}
	}
}


GUITopLevelWindow_Alive::GUITopLevelWindow_Alive(GUITopLevelWindow *window)
{
	if (window)
		window_alive = window->alive;
}

bool GUITopLevelWindow_Alive::is_null() const
{
	return window_alive.expired();
}

}
