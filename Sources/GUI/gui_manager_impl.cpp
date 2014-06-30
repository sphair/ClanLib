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
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/2D/image.h"
#include "API/Display/Font/font.h"
#include "API/Core/Text/string_format.h"
#include "gui_component_impl.h"
#include "API/Core/Resources/xml_resource_manager.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIManager_Impl Construction:

GUIManager_Impl::GUIManager_Impl()
: mouse_capture_component(0), exit_flag(false), exit_code(0), window_manager(NULL)
{
	resources = XMLResourceManager::create(XMLResourceDocument());

	func_focus_lost = bind_member(this, &GUIManager_Impl::on_focus_lost);
	func_focus_gained = bind_member(this, &GUIManager_Impl::on_focus_gained);
	func_resize = bind_member(this, &GUIManager_Impl::on_resize);
	func_paint = bind_member(this, &GUIManager_Impl::on_paint);
	func_close = bind_member(this, &GUIManager_Impl::on_close);
	func_destroy = bind_member(this, &GUIManager_Impl::on_destroy);
	func_input_received = bind_member(this, &GUIManager_Impl::on_input_received);

	wm_site.func_focus_lost = &func_focus_lost;
	wm_site.func_focus_gained = &func_focus_gained;
	wm_site.func_resize = &func_resize;
	wm_site.func_paint = &func_paint;
	wm_site.func_close = &func_close;
	wm_site.func_destroy = &func_destroy;
	wm_site.func_input_received = &func_input_received;


	resource_cache.cb_get_image = bind_member(this, &GUIManager_Impl::on_resource_cache_get_image);
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

/////////////////////////////////////////////////////////////////////////////
// GUIManager_Impl Operations:

void GUIManager_Impl::add_component(GUIComponent *component, GUIComponent *owner, const GUITopLevelDescription &desc)
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

			std::shared_ptr<GUIMessage_FocusChange> message(new GUIMessage_FocusChange());
			message->target = toplevel_window->focused_component;
			message->focus_type = GUIMessage_FocusChange::losing_focus;
			dispatch_message(message);

			if (toplevel_window_alive.is_null())	// Top level window was destroyed
				return;
		}

		toplevel_window->focused_component = component;

		if (toplevel_window->focused_component)
		{
			std::shared_ptr<GUIMessage_FocusChange> message(new GUIMessage_FocusChange());
			message->target = toplevel_window->focused_component;
			message->focus_type = GUIMessage_FocusChange::gained_focus;
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

			std::shared_ptr<GUIMessage_FocusChange> message(new GUIMessage_FocusChange());
			message->target = toplevel_window->focused_component;
			message->focus_type = GUIMessage_FocusChange::losing_focus;
			dispatch_message(message);

			if (toplevel_window_alive.is_null())	// Top level window was destroyed
				return;
		}

		toplevel_window->focused_component = NULL;
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

void GUIManager_Impl::deliver_message(std::shared_ptr<GUIMessage> &m)
{
	sig_filter_message(m);

	if (!m->consumed)
	{
		GUIComponent *target = m->target;
		if (target)
		{
			bool double_click_changed_to_single_click = false;
			std::shared_ptr<GUIMessage_Input> m_input = std::dynamic_pointer_cast<GUIMessage_Input>(m);
			if (m_input && !target->is_double_click_enabled())
			{
				if (m_input->input_event.type == InputEvent::doubleclick)
				{
					m_input->input_event.type = InputEvent::pressed;
					double_click_changed_to_single_click = true;
				}
			}

			if (target->func_filter_message())
			{
				target->func_filter_message()(m);
			}

			// Since we use a callback on func_process_message, we just call process_message directly on guicomponent here 
			// in case the specific components themselves want to use func_process_message.
			// If process_message later is changed into a signal, this should be removed, and a normal signal slot should be used in guicomponent.
			target->impl->on_process_message(m);

			if (!m->consumed)
			{
				if (target->func_process_message())
					target->func_process_message()(m);
			}

			if (!m->consumed)
			{
				std::shared_ptr<GUIMessage_ActivationChange> m_activation = std::dynamic_pointer_cast<GUIMessage_ActivationChange>(m);
				if (m_activation)
				{
					switch (m_activation->activation_type)
					{
					case GUIMessage_ActivationChange::activation_lost:
						if (target->func_deactivated() && target->func_deactivated()())
							m->consumed = true;
						break;
					case GUIMessage_ActivationChange::activation_gained:
						if (target->func_activated() && target->func_activated()())
							m->consumed = true;
						break;
					}
				}
				else if (std::dynamic_pointer_cast<GUIMessage_Close>(m))
				{
					if (target->func_close() && target->func_close()())
						m->consumed = true;
				}
				else if (std::dynamic_pointer_cast<GUIMessage_FocusChange>(m))
				{
					switch (std::dynamic_pointer_cast<GUIMessage_FocusChange>(m)->focus_type)
					{
					case GUIMessage_FocusChange::losing_focus:
						if (target->func_focus_lost() && target->func_focus_lost()())
							m->consumed = true;
						break;
					case GUIMessage_FocusChange::gained_focus:
						if (target->func_focus_gained() && target->func_focus_gained()())
							m->consumed = true;
						break;
					}
				}
				else if (std::dynamic_pointer_cast<GUIMessage_Input>(m))
				{
					std::shared_ptr<GUIMessage_Input> m_input = std::dynamic_pointer_cast<GUIMessage_Input>(m);
					InputEvent &e = m_input->input_event;

					if (target->func_input() && target->func_input()(e))
						m->consumed = true;

					switch (e.type)
					{
					case InputEvent::pressed:
						if (target->func_input_pressed() && target->func_input_pressed()(e))
							m->consumed = true;
						break;
					case InputEvent::released:
						if (target->func_input_released() && target->func_input_released()(e))
							m->consumed = true;
						break;
					case InputEvent::doubleclick:
						if (target->func_input_doubleclick() && target->func_input_doubleclick()(e))
							m->consumed = true;
						break;
					case InputEvent::pointer_moved:
						if (target->func_input_pointer_moved() && target->func_input_pointer_moved()(e))
							m->consumed = true;
						break;
					default:
						break;
					}
				}
				else if (std::dynamic_pointer_cast<GUIMessage_Pointer>(m))
				{
					switch (std::dynamic_pointer_cast<GUIMessage_Pointer>(m)->pointer_type)
					{
					case GUIMessage_Pointer::pointer_enter:
						if (target->func_pointer_enter() && target->func_pointer_enter()())
							m->consumed = true;
						break;
					case GUIMessage_Pointer::pointer_leave:
						if (target->func_pointer_exit() && target->func_pointer_exit()())
							m->consumed = true;
						break;
					}
				}
				else if (std::dynamic_pointer_cast<GUIMessage_Resize>(m))
				{
					// No need to do anything here. Already handled elsewhere.
					m->consumed = true;
				}
			}

			if (double_click_changed_to_single_click)
			{
				m_input->input_event.type = InputEvent::doubleclick;
			}
		}
	}
}

void GUIManager_Impl::dispatch_message(std::shared_ptr<GUIMessage> message)
{
	deliver_message(message);

	GUIComponent *message_original_target = message->target;

	// Unconsumed input message are sent up the component tree until a null parent is encountered or the message gets consumed.
	std::shared_ptr<GUIMessage_Input> m_input = std::dynamic_pointer_cast<GUIMessage_Input>(message);
	if (m_input && !m_input->consumed && m_input->target)
	{
		while (m_input->target && !m_input->consumed)
		{
			// Translate mouse pos to parent coordinates.
			m_input->input_event.mouse_pos.x += m_input->target->get_geometry().left;
			m_input->input_event.mouse_pos.y += m_input->target->get_geometry().top;

			m_input->target = m_input->target->get_parent_component();
			if (m_input->target)
				deliver_message(message);
		}
	}

	if (!message->target && !message->consumed)
		message->target = message_original_target;
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

		std::shared_ptr<GUIMessage_FocusChange> message(new GUIMessage_FocusChange());
		message->target = toplevel_window->focused_component;
		message->focus_type = GUIMessage_FocusChange::losing_focus;
		dispatch_message(message);

		if (toplevel_window_alive.is_null())	// Top level window was destroyed
			return;

	}

	// toplevel_window->focused_component = 0;

	std::shared_ptr<GUIMessage_ActivationChange> message(new GUIMessage_ActivationChange());
	message->target = toplevel_window->component;
	message->activation_type = GUIMessage_ActivationChange::activation_lost;
	dispatch_message(message);
}

void GUIManager_Impl::on_focus_gained(GUITopLevelWindow *toplevel_window)
{
	if (toplevel_window->focused_component == 0)
		toplevel_window->focused_component = toplevel_window->component;

	if (toplevel_window->focused_component)
	{
		GUITopLevelWindow_Alive toplevel_window_alive(toplevel_window);

		std::shared_ptr<GUIMessage_FocusChange> message(new GUIMessage_FocusChange());
		message->target = toplevel_window->focused_component;
		message->focus_type = GUIMessage_FocusChange::gained_focus;
		dispatch_message(message);

		if (toplevel_window_alive.is_null())	// Top level window was destroyed
			return;
	}

	std::shared_ptr<GUIMessage_ActivationChange> message(new GUIMessage_ActivationChange());
	message->target = toplevel_window->component;
	message->activation_type = GUIMessage_ActivationChange::activation_gained;
	dispatch_message(message);
}

void GUIManager_Impl::on_resize(GUITopLevelWindow *toplevel_window, const Size &new_size)
{
	GUIComponent *component = toplevel_window->component;
	component->impl->window_resized();

	std::shared_ptr<GUIMessage_Resize> message(new GUIMessage_Resize());
	message->target = component;
	message->geometry = component->get_geometry();
	dispatch_message(message);
}

void GUIManager_Impl::on_paint(GUITopLevelWindow *toplevel_window, const Rect &update_rect)
{
	toplevel_window->component->paint(update_rect);
}

void GUIManager_Impl::on_close(GUITopLevelWindow *toplevel_window)
{
	std::shared_ptr<GUIMessage_Close> message(new GUIMessage_Close());
	message->target = toplevel_window->component;
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

		if (input_event.type == InputEvent::pointer_moved)
		{
			if (target)
			{
				Point point = target->window_to_component_coords(input_event.mouse_pos);
				process_pointer_moved_at(target, point, true);
			}
			else
			{
				Point point = toplevel_window->component->window_to_component_coords(input_event.mouse_pos);
				process_pointer_moved_at(toplevel_window->component, point, false);
			}
		}

		if (target == 0)
		{
			target = toplevel_window->component->get_component_at(toplevel_window->component->window_to_component_coords(input_event.mouse_pos));
		}

	}
	else
	{
		if (toplevel_window->focused_component)
			target = toplevel_window->focused_component;
		else
			target = toplevel_window->component; // All keyboard messages are redirected to the toplevel component if no component is focused.
	}

	dispatch_message_to_component(target, input_event);

}

void GUIManager_Impl::process_pointer_moved_at(GUIComponent *this_component, const Point &point, bool force_pointer_enter)
{
	if (this_component->is_visible())
	{
		Rect parent_rect = this_component->get_size();
		if( force_pointer_enter || parent_rect.contains(point) )  
		{
			if (!this_component->impl->pointer_inside_component)
			{
				this_component->impl->pointer_inside_component = true;
				std::shared_ptr<GUIMessage_Pointer> msg(new GUIMessage_Pointer());
				msg->pointer_type = GUIMessage_Pointer::pointer_enter;
				msg->target = this_component;
				dispatch_message(msg);
			}
		}
		else
		{
			if (this_component->impl->pointer_inside_component)
			{
				this_component->impl->pointer_inside_component = false;
				std::shared_ptr<GUIMessage_Pointer> msg(new GUIMessage_Pointer());
				msg->pointer_type = GUIMessage_Pointer::pointer_leave;
				msg->target = this_component;
				dispatch_message(msg);
			}
		}

		std::vector<GUIComponent *> children = this_component->get_child_components();
		std::size_t pos, size = children.size();

		for( pos=size; pos>0; pos-- )
		{
			GUIComponent *child = children[pos-1];
			if(child->is_visible())
			{
				Point P = point;
				P.x -= child->get_geometry().left;
				P.y -= child->get_geometry().top;
				process_pointer_moved_at(child, P, false);
			}
		}

	}
}

void GUIManager_Impl::dispatch_message_to_component(GUIComponent *target, const InputEvent &input_event)
{
	if (target)
	{
		// Localize mouse position:
		InputEvent local_input_event = input_event;
		local_input_event.mouse_pos = target->window_to_component_coords(input_event.mouse_pos);

		std::shared_ptr<GUIMessage> message(new GUIMessage_Input(local_input_event));
		message->target = target;
		dispatch_message(message);

		if (!message->consumed)
			accel_table.process_message(message);

		if (!message->consumed)
			process_standard_gui_keys(message);
	}
}

Image GUIManager_Impl::on_resource_cache_get_image(Canvas &canvas, const std::string &url)
{
	return Image::resource(canvas, url, resources);
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

void GUIManager_Impl::process_standard_gui_keys(std::shared_ptr<GUIMessage> &message)
{
	std::shared_ptr<GUIMessage_Input> input = std::dynamic_pointer_cast<GUIMessage_Input>(message);
	if (input)
	{
		InputEvent e = input->input_event;

		if (e.type == InputEvent::pressed && 
			(e.id == keycode_tab || e.id == keycode_left || e.id == keycode_right || e.id == keycode_up || e.id == keycode_down))
		{
			GUIComponent *focus_comp = input->target;

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
			GUIComponent *comp = input->target;
			if (comp)
			{
				GUITopLevelWindow *win = get_toplevel_window(comp);
				GUIComponent *defaulted_component = get_default_component(win->component);

				if (defaulted_component)
				{
					input->target = defaulted_component;
					dispatch_message(input);
				}
			}
		}
		else if (e.id == keycode_escape)
		{
			GUIComponent *comp = input->target;
			if (comp)
			{
				GUITopLevelWindow *win = get_toplevel_window(comp);
				GUIComponent *cancel_comp = get_cancel_component(win->component);
				if (cancel_comp)
				{
					input->target = cancel_comp;
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
