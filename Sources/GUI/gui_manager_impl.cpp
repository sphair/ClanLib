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
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_state.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/Font/font.h"
#include "API/Core/Text/string_format.h"
#include "gui_component_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Impl Construction:

CL_GUIManager_Impl::CL_GUIManager_Impl()
: mouse_capture_component(0), mouse_over_component(0), theme(0), exit_flag(false), exit_code(0), destroy_signal_connected(false),window_manager(NULL)
{
	func_focus_lost.set(this, &CL_GUIManager_Impl::on_focus_lost);
	func_focus_gained.set(this, &CL_GUIManager_Impl::on_focus_gained);
	func_resize.set(this, &CL_GUIManager_Impl::on_resize);
	func_paint.set(this, &CL_GUIManager_Impl::on_paint);
	func_close.set(this, &CL_GUIManager_Impl::on_close);
	func_input_received.set(this, &CL_GUIManager_Impl::on_input_received);

	wm_site.func_focus_lost = &func_focus_lost;
	wm_site.func_focus_gained = &func_focus_gained;
	wm_site.func_resize = &func_resize;
	wm_site.func_paint = &func_paint;
	wm_site.func_close = &func_close;
	wm_site.func_input_received = &func_input_received;
}

CL_GUIManager_Impl::~CL_GUIManager_Impl()
{
	while (!root_components.empty())
	{
		CL_GUITopLevelWindow *window = root_components.front();

		// The following line deletes the CL_GUIComponent, which should cause it to clean up every aspect of the window.
		// Do not add additional window clean up code here, since that would cause it not to be called if the
		// application deletes a component by itself. -- mbn 22. Oct 2008
		delete window->component;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Impl Attributes:

CL_Font CL_GUIManager_Impl::get_registered_font(const CL_FontDescription &desc)
{
	std::vector<NamedFontCacheEntry>::iterator it;
	for (it = named_font_cache.begin(); it != named_font_cache.end(); ++it)
	{
		if ((*it).desc == desc)
			return (*it).font;
	}

	return CL_Font();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Impl Operations:

void CL_GUIManager_Impl::add_component(CL_GUIComponent *component, CL_GUIComponent *owner, CL_GUITopLevelDescription desc, bool temporary)
{
	CL_GUITopLevelWindow *top_level_window = new CL_GUITopLevelWindow();
	top_level_window->component = component;
	top_level_window->focused_component = component;
	top_level_window->owner = 0;
	top_level_window->proximity_component = 0;

	CL_GUITopLevelWindow *owner_window = 0;

	if (owner)
	{
		top_level_window->owner = owner;

		CL_GUIComponent *root_owner = top_level_window->owner->get_top_level_component();
		std::vector<CL_GUITopLevelWindow *>::size_type pos, size;
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
		window_manager.create_window(top_level_window, owner_window, desc, temporary);
	}
	catch (CL_Exception e)
	{
		delete top_level_window;
		throw e;
	}

	if (!destroy_signal_connected)
	{
		destroy_signal_connected = true;
		slot_gc_destruction_imminent = component->get_gc().sig_gc_destruction_imminent().connect(this, &CL_GUIManager_Impl::on_gc_destruction_imminent);
	}

	root_components.push_back(top_level_window);
}

void CL_GUIManager_Impl::remove_component(CL_GUIComponent_Impl *component_impl)
{
	if (mouse_capture_component && mouse_capture_component->impl == component_impl)
	{
		window_manager.capture_mouse(get_toplevel_window(mouse_capture_component), false);
		mouse_capture_component = 0;
	}

	if (mouse_over_component && mouse_over_component->impl == component_impl)
		mouse_over_component = 0;

	std::vector<CL_GUITopLevelWindow>::size_type index, size;
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		CL_GUITopLevelWindow *cur = root_components[index];

		// Clear focus pointer if needed:
		if (cur->focused_component && cur->focused_component->impl == component_impl)
			cur->focused_component = 0;

		// Remove display window if needed:
		if (cur->component->impl == component_impl)
		{
			window_manager.destroy_window(cur);
			root_components.erase(root_components.begin() + index);
			delete cur;
			break;
		}
	}

	// Remove all messages queued for this window:
	CL_MutexSection mutex_lock(&message_queue_mutex);
	std::vector<CL_GUIMessage>::iterator it_message_queue = message_queue.begin();
	while (it_message_queue != message_queue.end())
	{
		CL_GUIMessage &message = *it_message_queue;
		CL_GUIComponent *target = message.get_target();

		if (target && target->impl.get() == component_impl)
			it_message_queue = message_queue.erase(it_message_queue);
		else
			++it_message_queue;
	}
}

void CL_GUIManager_Impl::gain_focus(CL_GUIComponent *component)
{
	CL_GUITopLevelWindow *toplevel_window = get_toplevel_window(component);
	if (toplevel_window == NULL)
	{
		throw CL_Exception(cl_text("No top level window is set"));
	}

	if (toplevel_window->focused_component == component)
		return;

	if (window_manager.has_focus(toplevel_window))
	{
		if (toplevel_window->focused_component)
		{
			CL_GUIMessage_FocusChange message;
			message.set_target(toplevel_window->focused_component);
			message.set_focus_type(CL_GUIMessage_FocusChange::losing_focus);
			send_message(message);
		}

		toplevel_window->focused_component = component;

		// The defaulted component (usually a CL_PushButton in dialogs) needs to update its "default" CL_GUIThemePart state depending on 
		// whether the focused component consumes certain input events.
		// This should be done before sending the gained_focus message
		CL_GUIComponent *default_comp = get_default_component(toplevel_window->component);
		if (default_comp && !default_comp->func_style_changed().is_null())
			default_comp->func_style_changed().invoke();

		if (toplevel_window->focused_component)
		{
			CL_GUIMessage_FocusChange message;
			message.set_target(toplevel_window->focused_component);
			message.set_focus_type(CL_GUIMessage_FocusChange::gained_focus);
			send_message(message);
		}
	}
	else
	{
		// Don't need to send messages if this window is not in focus - it this the correct behaviour?
		toplevel_window->focused_component = component;
	}
}

void CL_GUIManager_Impl::loose_focus(CL_GUIComponent *component)
{
	CL_GUITopLevelWindow *toplevel_window = get_toplevel_window(component);
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
			CL_GUIMessage_FocusChange message;
			message.set_target(toplevel_window->focused_component);
			message.set_focus_type(CL_GUIMessage_FocusChange::losing_focus);
			send_message(message);
		}

		toplevel_window->focused_component = NULL;

		// The defaulted component (usually a CL_PushButton in dialogs) needs to update its "default" CL_GUIThemePart state depending on 
		// whether the focused component consumes certain input events.
		// This should be done before sending the gained_focus message
		CL_GUIComponent *default_comp = get_default_component(toplevel_window->component);
		if (default_comp && !default_comp->func_style_changed().is_null())
			default_comp->func_style_changed().invoke();
	}
	else
	{
		// Don't need to send messages if this window is not in focus - it this the correct behaviour?
		toplevel_window->focused_component = NULL;
	}
}

void CL_GUIManager_Impl::set_enabled(CL_GUIComponent *component, bool enable)
{
	window_manager.enable_window(get_toplevel_window(component), enable);
}

bool CL_GUIManager_Impl::has_focus(const CL_GUIComponent *component) const
{
	CL_GUITopLevelWindow *toplevel_window = get_toplevel_window(component);
	if (toplevel_window->focused_component == component)
		return window_manager.has_focus(toplevel_window);
	else
		return false;
}

CL_GUITopLevelWindow *CL_GUIManager_Impl::get_toplevel_window(const CL_GUIComponent * const component) const
{
	CL_GUIComponent *root = (CL_GUIComponent *) component;
	while (root->get_parent_component())
		root = root->get_parent_component();

	std::vector<CL_GUITopLevelWindow>::size_type index, size;
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		CL_GUITopLevelWindow *cur = root_components[index];
		if (cur->component == root)
			return cur;
	}
	return 0;
}

CL_GUIComponent *CL_GUIManager_Impl::get_owner_component(const CL_GUIComponent *component)
{
	std::vector<CL_GUITopLevelWindow>::size_type index, size;
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		CL_GUITopLevelWindow *cur = root_components[index];
		if (cur->component == component)
			return cur->owner;
	}
	return 0;
}

const CL_GUIComponent *CL_GUIManager_Impl::get_owner_component(const CL_GUIComponent *component) const
{
	std::vector<CL_GUITopLevelWindow>::size_type index, size;
	size = root_components.size();
	for (index = 0; index < size; index++)
	{
		CL_GUITopLevelWindow *cur = root_components[index];
		if (cur->component == component)
			return cur->owner;
	}
	return 0;
}

void CL_GUIManager_Impl::post_message(const CL_GUIMessage &message)
{
	CL_MutexSection mutex_lock(&message_queue_mutex);
	message_queue.push_back(message);
}

void CL_GUIManager_Impl::deliver_message(CL_GUIMessage &m)
{
	if (func_filter_message.is_null() || func_filter_message.invoke(m))
	{
		CL_GUIComponent *target = m.get_target();
		if (target)
		{
			if (!target->func_process_message().is_null())
				target->func_process_message().invoke(m);

			if (!m.is_consumed())
			{
				if (m.get_type() == CL_GUIMessage_ActivationChange::get_type_name())
				{
					switch (CL_GUIMessage_ActivationChange(m).get_activation_type())
					{
					case CL_GUIMessage_ActivationChange::activation_lost:
						if (!target->func_deactivated().is_null() && target->func_deactivated().invoke())
							m.set_consumed();
						break;
					case CL_GUIMessage_ActivationChange::activation_gained:
						if (!target->func_activated().is_null() && target->func_activated().invoke())
							m.set_consumed();
						break;
					}
				}
				else if (m.get_type() == CL_GUIMessage_Close::get_type_name())
				{
					if (!target->func_close().is_null() && target->func_close().invoke())
						m.set_consumed();
				}
				else if (m.get_type() == CL_GUIMessage_FocusChange::get_type_name())
				{
					switch (CL_GUIMessage_FocusChange(m).get_focus_type())
					{
					case CL_GUIMessage_FocusChange::losing_focus:
						if (!target->func_focus_lost().is_null() && target->func_focus_lost().invoke())
							m.set_consumed();
						break;
					case CL_GUIMessage_FocusChange::gained_focus:
						if (!target->func_focus_gained().is_null() && target->func_focus_gained().invoke())
							m.set_consumed();
						break;
					}
				}
				else if (m.get_type() == CL_GUIMessage_Input::get_type_name())
				{
					CL_GUIMessage_Input m_input(m);
					CL_InputEvent e = m_input.get_event();

					if (!target->func_input().is_null() && target->func_input().invoke(e))
						m.set_consumed();

					switch (e.type)
					{
					case CL_InputEvent::pressed:
						if (!target->func_input_pressed().is_null() && target->func_input_pressed().invoke(e))
							m.set_consumed();
						break;
					case CL_InputEvent::released:
						if (!target->func_input_released().is_null() && target->func_input_released().invoke(e))
							m.set_consumed();
						break;
					case CL_InputEvent::doubleclick:
						if (!target->func_input_doubleclick().is_null() && target->func_input_doubleclick().invoke(e))
							m.set_consumed();
						break;
					case CL_InputEvent::pointer_moved:
						if (!target->func_input_pointer_moved().is_null() && target->func_input_pointer_moved().invoke(e))
							m.set_consumed();
						break;
					default:
						break;
					}
				}
				else if (m.get_type() == CL_GUIMessage_Pointer::get_type_name())
				{
					switch (CL_GUIMessage_Pointer(m).get_pointer_type())
					{
					case CL_GUIMessage_Pointer::pointer_enter:
						if (!target->func_pointer_enter().is_null() && target->func_pointer_enter().invoke())
							m.set_consumed();
						break;
					case CL_GUIMessage_Pointer::pointer_leave:
						if (!target->func_pointer_exit().is_null() && target->func_pointer_exit().invoke())
							m.set_consumed();
						break;
					}
				}
				else if (m.get_type() == CL_GUIMessage_Resize::get_type_name())
				{
					// No need to do anything here. Already handled elsewhere.
					m.set_consumed();
				}
			}
		}
	}
}

void CL_GUIManager_Impl::send_message(CL_GUIMessage &message)
{
	CL_GUIMessage m = message;
	deliver_message(m);

	CL_GUIComponent *message_original_target = m.get_target();

	// Unconsumed input message are sent up the component tree until a null parent is encountered or the message gets consumed.
	if (!m.is_consumed() && m.get_target() && m.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		while (m.get_target() && !m.is_consumed())
		{
			// Translate mouse pos to parent coordinates.
			CL_GUIMessage_Input input(m);
			CL_InputEvent event = input.get_event();
			event.mouse_pos.x += m.get_target()->get_geometry().left;
			event.mouse_pos.y += m.get_target()->get_geometry().top;
			input.set_event(event);

			m.set_target(m.get_target()->get_parent_component());

			deliver_message(m);
		}
	}

	if (m.get_target() == 0 && !m.is_consumed())
		m.set_target(message_original_target);
}

CL_GUIComponent *CL_GUIManager_Impl::get_focus_component()
{
	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
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

void CL_GUIManager_Impl::check_for_new_messages()
{
	// Handle posted messages before looking for input messages.
	CL_MutexSection mutex_lock(&message_queue_mutex);
	if (!message_queue.empty())
		return;
	mutex_lock.unlock();

	// Handle window manager messages:
	while (window_manager.has_message())
	{
		window_manager.process_message();
		mutex_lock.lock();
		if (!message_queue.empty())
			return;
		mutex_lock.unlock();
	}
}

std::vector<CL_CSSRuleSet> &CL_GUIManager_Impl::get_rulesets(const CL_StringRef &element_name) const
{
	std::map< CL_String, std::vector<CL_CSSRuleSet> >::iterator it = rulesets_cache.find(element_name);
	if (it != rulesets_cache.end())
		return it->second;
	CL_CSSDocument css_document_const_hack = css_document;
	rulesets_cache[element_name] = css_document_const_hack.select(element_name);
	return rulesets_cache[element_name];
}

void CL_GUIManager_Impl::reset_rulesets()
{
	rulesets_cache.clear();
}

void CL_GUIManager_Impl::register_font(const CL_Font &font, const CL_FontDescription &desc)
{
	if (get_registered_font(desc).is_null())
	{
		NamedFontCacheEntry entry;
		entry.desc = desc;
		entry.font = font;
		named_font_cache.push_back(entry);
	}
}

bool CL_GUIManager_Impl::is_constant_repaint_enabled() const
{
	for (std::vector<CL_GUITopLevelWindow *>::size_type i = 0; i < root_components.size(); i++)
	{
		if (is_constant_repaint_enabled(root_components[i]->component))
			return true;
	}
	return false;
}

bool CL_GUIManager_Impl::is_constant_repaint_enabled(CL_GUIComponent *component) const
{
	if (component->get_constant_repaint())
	{
		return true;
	}
	else
	{
		std::vector<CL_GUIComponent *> components = component->get_child_components();
		for (std::vector<CL_GUIComponent *>::size_type i = 0; i < components.size(); i++)
		{
			if (is_constant_repaint_enabled(components[i]))
				return true;
		}
		return false;
	}
}

void CL_GUIManager_Impl::invalidate_constant_repaint_components()
{
	for (std::vector<CL_GUITopLevelWindow *>::size_type i = 0; i < root_components.size(); i++)
	{
		if (is_constant_repaint_enabled(root_components[i]->component))
		{
			CL_GUITopLevelWindow *cur = root_components[i];
			cur->component->request_repaint();
//			CL_Rect geometry = cur->component->get_geometry();
//			cur->component->paint(CL_Rect(CL_Point(0,0), geometry.get_size()));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Impl Implementation:

void CL_GUIManager_Impl::on_focus_lost(CL_GUITopLevelWindow *toplevel_window)
{
	if (toplevel_window->focused_component)
	{
		CL_GUIMessage_FocusChange message;
		message.set_target(toplevel_window->focused_component);
		message.set_focus_type(CL_GUIMessage_FocusChange::losing_focus);
		post_message(message);
	}

	// toplevel_window->focused_component = 0;

	CL_GUIMessage_ActivationChange message;
	message.set_target(toplevel_window->component);
	message.set_activation_type(CL_GUIMessage_ActivationChange::activation_lost);
	post_message(message);
}

void CL_GUIManager_Impl::on_focus_gained(CL_GUITopLevelWindow *toplevel_window)
{
	if (toplevel_window->focused_component == 0)
		toplevel_window->focused_component = toplevel_window->component;

	if (toplevel_window->focused_component)
	{
		CL_GUIMessage_FocusChange message;
		message.set_target(toplevel_window->focused_component);
		message.set_focus_type(CL_GUIMessage_FocusChange::gained_focus);
		post_message(message);
	}

	CL_GUIMessage_ActivationChange message;
	message.set_target(toplevel_window->component);
	message.set_activation_type(CL_GUIMessage_ActivationChange::activation_gained);
	post_message(message);
}

void CL_GUIManager_Impl::on_resize(CL_GUITopLevelWindow *toplevel_window, const CL_Size &new_size)
{
	CL_GUIComponent *component = toplevel_window->component;
	component->impl->geometry = component->get_geometry();
	component->impl->geometry_updated();

	CL_GUIMessage_Resize message;
	message.set_target(component);
	message.set_geometry(component->get_geometry());
	post_message(message);
}

void CL_GUIManager_Impl::on_paint(CL_GUITopLevelWindow *toplevel_window, const CL_Rect &update_rect)
{
	toplevel_window->component->paint(update_rect);
}

void CL_GUIManager_Impl::on_close(CL_GUITopLevelWindow *toplevel_window)
{
	CL_GUIMessage_Close message;
	message.set_target(toplevel_window->component);
	post_message(message);
}

void CL_GUIManager_Impl::on_input_received(
	CL_GUITopLevelWindow *toplevel_window,
	const CL_InputEvent &input_event,
	const CL_InputState &input_state)
{
	// Find target for input message:
	CL_GUIComponent *target = 0;
	if (input_event.device.get_type() == CL_InputDevice::pointer)
	{
		target = mouse_capture_component;

		if (input_event.type == CL_InputEvent::proximity_change && toplevel_window->proximity_component)
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
		CL_InputEvent local_input_event = input_event;
		local_input_event.mouse_pos = target->window_to_component_coords(input_event.mouse_pos);

		// For pointer movements we may need to generate enter and leave messages:
		if (local_input_event.type == CL_InputEvent::pointer_moved &&
			target != mouse_over_component)
		{
			if (mouse_over_component)
			{
				CL_GUIMessage_Pointer msg;
				msg.set_pointer_type(CL_GUIMessage_Pointer::pointer_leave);
				msg.set_target(mouse_over_component);
				post_message(msg);
			}
			mouse_over_component = target;
			if (mouse_over_component)
			{
				CL_GUIMessage_Pointer msg;
				msg.set_pointer_type(CL_GUIMessage_Pointer::pointer_enter);
				msg.set_target(mouse_over_component);
				post_message(msg);
			}
		}

		CL_GUIMessage_Input message;
		message.set_target(target);
		message.set_event(local_input_event);
		post_message(message);
	}
}

CL_GUIComponent *CL_GUIManager_Impl::get_cancel_component(CL_GUIComponent *comp)
{
	if (comp && comp->is_cancel())
		return comp;

	CL_GUIComponent *child = comp->get_first_child();

	while (child)
	{
		if (child->is_cancel())
			return child;

		if (child->has_child_components())
		{
			CL_GUIComponent *def = get_cancel_component(child);
			if (def != 0)
			{
				return def;
			}
		}

		child = child->get_next_sibling();
	}

	return 0;
}

CL_GUIComponent *CL_GUIManager_Impl::get_default_component(CL_GUIComponent *comp)
{
	if (comp && comp->is_default())
		return comp;

	CL_GUIComponent *child = comp->get_first_child();

	while (child)
	{
		if (child->is_default())
			return child;

		if (child->has_child_components())
		{
			CL_GUIComponent *def = get_default_component(child);
			if (def != 0)
			{
				return def;
			}
		}

		child = child->get_next_sibling();
	}

	return 0;
}

void CL_GUIManager_Impl::on_gc_destruction_imminent()
{
	// The last gc is about to be destroyed - release all resources that depend on gc.
	font_cache = CL_GUIFontCache();
}