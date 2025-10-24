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
*/

#include "GUI/precomp.h"
#include "API/Core/CSS/css_document.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/GUI/accelerator_table.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message_input.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/display_message_queue.h"
#include "gui_manager_impl.h"
#include "API/Display/Window/keys.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/event.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Construction:

CL_GUIManager::CL_GUIManager()
: impl(new CL_GUIManager_Impl)
{
}

CL_GUIManager::CL_GUIManager(CL_SharedPtr<CL_GUIManager_Impl> impl)
: impl(impl)
{
}

CL_GUIManager::~CL_GUIManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Attributes:

CL_CSSDocument CL_GUIManager::get_css_document() const
{
	return impl->css_document;
}

CL_GUITheme *CL_GUIManager::get_theme() const
{
	return impl->theme;
}

CL_GUIWindowManager *CL_GUIManager::get_window_manager() const
{
	return impl->window_manager;
}

CL_GUIComponent *CL_GUIManager::get_capture_component() const
{
	return impl->mouse_capture_component;
}

bool CL_GUIManager::get_exit_flag() const
{
	return impl->exit_flag;
}

int CL_GUIManager::get_exit_code() const
{
	return impl->exit_code;
}

CL_UnknownSharedPtr CL_GUIManager::get_userdata()
{
	return impl->userdata;
}

CL_Font CL_GUIManager::get_named_font(const CL_FontDescription &desc)
{
	return impl->get_named_font(desc);
}

CL_GUIComponent * CL_GUIManager::get_focused_component()
{
	return impl->get_focus_component();
}

CL_String CL_GUIManager::get_clipboard_text() const
{
	if (impl->root_components.empty())
		return CL_String();

	CL_GUITopLevelWindow *cur = impl->root_components[0];
	return get_window_manager()->get_display_window(cur).get_clipboard_text();
}


/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Events:

CL_Callback_1<bool, CL_GUIMessage &> &CL_GUIManager::func_filter_message()
{
	return impl->func_filter_message;
}

CL_Callback_2<int, CL_AcceleratorTable &, bool> &CL_GUIManager::func_exec_handler()
{
	return impl->func_exec_handler;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Operations:

void CL_GUIManager::set_theme(CL_GUITheme *theme)
{
	impl->theme = theme;
}

void CL_GUIManager::set_css_document(CL_CSSDocument css)
{
	impl->css_document = css;
	impl->reset_rulesets();	// Clear the ruleset cache
}

void CL_GUIManager::set_css_document(const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_IODevice file = CL_File(path + filename, CL_File::open_existing);

	CL_CSSDocument css;
	css.load(path, file); 
	set_css_document(css);
}

void CL_GUIManager::set_window_manager(CL_GUIWindowManager *window_manager)
{
	impl->window_manager = window_manager;
	window_manager->set_site(&impl->wm_site);
}

void CL_GUIManager::set_capture_component(CL_GUIComponent *component, bool state)
{
	if (state)
	{
		impl->mouse_capture_component = component;
		impl->window_manager->capture_mouse(impl->get_toplevel_window(component), true);
	}
	else
	{
		if (impl->mouse_capture_component == component)
		{
			impl->window_manager->capture_mouse(impl->get_toplevel_window(component), false);
			impl->mouse_capture_component = NULL;
		}
	}
}

void CL_GUIManager::set_tablet_proximity_component(CL_GUIComponent *comp, bool state)
{
	CL_GUITopLevelWindow *win = impl->get_toplevel_window(comp);

	if (state)
	{
		win->proximity_component = comp;
	}
	else
	{
		if (win->proximity_component == comp)
		{
			win->proximity_component = 0;
		}
	}
}

void CL_GUIManager::process_messages(CL_AcceleratorTable &accel_table)
{
	while (wait(0))
	{
		// Note, peek message is required, as CL_DisplayMessageQueue::wait() may say there is input that get_message() does not use
		CL_GUIMessage message = peek_message(0);
		if (!message.is_null())
		{
			message = get_message();

			dispatch_message(message);

			if (!message.is_consumed())
				accel_table.process_message(message);

			if (!message.is_consumed())
				process_standard_gui_keys(message);
		}
	}

	get_window_manager()->setup_painting();
	impl->invalidate_constant_repaint_components();
	get_window_manager()->update();
	get_window_manager()->complete_painting();
}

bool CL_GUIManager::wait(int timeout)
{
	if (impl->is_constant_repaint_enabled())
		timeout = 0;

	std::vector<CL_KeepAliveObject *> objects = CL_KeepAlive::get_objects();
	std::vector<CL_Event> events;
	for (std::vector<CL_KeepAliveObject *>::size_type i = 0; i < objects.size(); i++)
	{
		events.push_back(objects[i]->get_wakeup_event());
	}

	// Process all display queue messages at once
	int wakeup_reason;
	do
	{
		wakeup_reason = CL_DisplayMessageQueue::wait(events, timeout);

		// After the first event, we want to clear the timeout, so that the next call to CL_DisplayMessageQueue::wait() does not wait forever
		timeout = 0; // To do: strictly speaking it should not be set to 0, but to the delta time between the wake up and initial wait request

		if (wakeup_reason >= 0)
		{
			objects[wakeup_reason]->process();
		}
		else if (wakeup_reason == -2)
		{
			return true;
		}
	} while(wakeup_reason != -1);

	return false;
}

void CL_GUIManager::process_messages()
{
	CL_AcceleratorTable table;
	process_messages(table);
}

int CL_GUIManager::exec(bool loop_until_complete)
{
	CL_AcceleratorTable table;
	return exec(table, loop_until_complete);
}

int CL_GUIManager::exec(CL_AcceleratorTable &accel_table, bool loop_until_complete)
{
	if (!impl->func_exec_handler.is_null())
		return impl->func_exec_handler.invoke(accel_table, loop_until_complete);

	while (!impl->exit_flag)
	{
		impl->exit_code = 0;

		if (loop_until_complete)	// Check for a modal dialog code
		{
			if (!impl->func_exec_handler.is_null())
				impl->func_exec_handler.invoke(accel_table, loop_until_complete);
		}

		wait(loop_until_complete ? -1 : 0);
		process_messages(accel_table);

		// The user wants the exits now
		if (!loop_until_complete)
		{
			if (!impl->exit_flag)	// Return now, only if the exist flag is not set
			{
				return 0;
			}
		}
	}
	impl->exit_flag = false;
	return impl->exit_code;
}

void CL_GUIManager::exit_with_code(int exit_code)
{
	impl->exit_flag = true;
	impl->exit_code = exit_code;
}

void CL_GUIManager::clear_exit_flag()
{
	impl->exit_flag = false;
}

CL_GUIMessage CL_GUIManager::peek_message(bool block)
{
	while (true)
	{
		if (impl->root_components.empty())
		{
			CL_GUIMessage message;
			message.set_null();
			return message;
		}

		impl->check_for_new_messages();

		CL_MutexSection mutex_lock(&impl->message_queue_mutex);
		if (!impl->message_queue.empty())
			return impl->message_queue[0];
		mutex_lock.unlock();

		if (!block)
		{
			CL_GUIMessage message;
			message.set_null();
			return message;
		}

		// Check if we should wait for new messages:
		std::vector<CL_GUITopLevelWindow>::size_type pos, size;
		bool regions_updated = true;
		size = impl->root_components.size();
		for (pos = 0; pos < size; pos++)
		{
			CL_GUITopLevelWindow *cur = impl->root_components[pos];
			if (!cur->update_regions.empty())
				regions_updated = false;
		}

		if (impl->message_queue.empty() && regions_updated)
		{
			impl->window_manager->wait_for_message();
		}
	}
}

CL_GUIMessage CL_GUIManager::get_message()
{
	CL_GUIMessage message = peek_message(true);
	CL_MutexSection mutex_lock(&impl->message_queue_mutex);
	if (!impl->message_queue.empty())
		impl->message_queue.erase(impl->message_queue.begin());
	return message;
}

void CL_GUIManager::dispatch_message(CL_GUIMessage message)
{
	impl->send_message(message);
}

void CL_GUIManager::post_message(const CL_GUIMessage &message)
{
	impl->post_message(message);
}

void CL_GUIManager::send_message(CL_GUIMessage &message)
{
	impl->send_message(message);
}

void CL_GUIManager::invalidate_rect(const CL_Rect &rect, CL_GUIComponent *root_component)
{
	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->root_components.size();

	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->root_components[pos];

		if (cur->component == root_component)
		{
			get_window_manager()->invalidate_rect(cur, rect);
			//cur->update_regions.push_back(rect);
			break;
		}
	}
}

void CL_GUIManager::render_windows()
{
	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->root_components.size();

	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->root_components[pos];
		CL_Rect geometry = cur->component->get_geometry();
		cur->component->paint(CL_Rect(CL_Point(0,0), geometry.get_size()));
	}
}

void CL_GUIManager::set_userdata(CL_UnknownSharedPtr ptr)
{
	impl->userdata = ptr;
}

void CL_GUIManager::set_named_font(const CL_Font &font, const CL_FontDescription &desc)
{
	impl->set_named_font(font, desc);
}

void CL_GUIManager::set_clipboard_text(const CL_StringRef &str)
{
	if (impl->root_components.empty())
		return;

	CL_GUITopLevelWindow *cur = impl->root_components[0];
	get_window_manager()->get_display_window(cur).set_clipboard_text(str);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Implementation:

void CL_GUIManager::process_standard_gui_keys(CL_GUIMessage &message)
{
	if (message.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input = message;
		CL_InputEvent e = input.get_event();

		if (e.type == CL_InputEvent::pressed && 
			(e.id == CL_KEY_TAB || e.id == CL_KEY_LEFT || e.id == CL_KEY_RIGHT || e.id == CL_KEY_UP || e.id == CL_KEY_DOWN))
		{
			CL_GUIComponent *tab_order_controller = input.get_target()->get_tab_order_controller();

			if (tab_order_controller == 0)
				return;

			if (e.id == CL_KEY_TAB)
			{
				if (!e.shift)
					tab_order_controller->focus_next();
				else
					tab_order_controller->focus_previous();
			}
			else if (e.id == CL_KEY_LEFT || e.id == CL_KEY_UP)
			{
				tab_order_controller->focus_next();
			}
			else if (e.id == CL_KEY_RIGHT || e.id == CL_KEY_DOWN)
			{
				tab_order_controller->focus_previous();
			}
		}
		else if ((e.id == CL_KEY_ENTER || e.id == CL_KEY_RETURN) && e.type == CL_InputEvent::released)
		{
			CL_GUIComponent *comp = input.get_target();
			if (comp)
			{
				CL_GUITopLevelWindow *win = impl->get_toplevel_window(comp);
				CL_GUIComponent *defaulted_component = impl->get_default_component(win->component);

				if (defaulted_component)
				{
					input.set_target(defaulted_component);
					send_message(input);
				}
			}
		}
		else if (e.id == CL_KEY_ESCAPE)
		{
			CL_GUIComponent *comp = input.get_target();
			if (comp)
			{
				CL_GUITopLevelWindow *win = impl->get_toplevel_window(comp);
				CL_GUIComponent *cancel_comp = impl->get_cancel_component(win->component);
				if (cancel_comp)
				{
					input.set_target(cancel_comp);
					send_message(input);
				}
			}
		}
	}
}

