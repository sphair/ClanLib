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
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/display_message_queue.h"
#include "API/Display/Window/display_window_message.h"
#include "API/GUI/gui_window_manager_system.h"
#include "API/Display/Render/blend_mode.h"
#include "gui_window_manager_system_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem Construction:

CL_GUIWindowManagerSystem::CL_GUIWindowManagerSystem()
: impl(new CL_GUIWindowManagerSystem_Impl)
{
}

CL_GUIWindowManagerSystem::~CL_GUIWindowManagerSystem()
{
	for (std::vector<CL_GUITopLevelWindowSystem>::size_type i = 0; i < impl->cached_windows.size(); i++)
	{
		delete impl->cached_windows[i];
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem Attributes:

CL_GUIWindowManager::CL_WindowManagerType CL_GUIWindowManagerSystem::get_window_manager_type() const
{
	return cl_wm_type_system;
}

CL_Callback_v0 &CL_GUIWindowManagerSystem::func_wait_for_message()
{
	return impl->func_wait_for_message;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem Operations:

void CL_GUIWindowManagerSystem::set_site(CL_GUIWindowManagerSite *site)
{
	impl->site = site;
}

void CL_GUIWindowManagerSystem::create_window(
	CL_GUITopLevelWindow *handle,
	CL_GUITopLevelWindow *owner,
	CL_GUITopLevelDescription description,
	bool temporary)
{
	CL_GUITopLevelWindowSystem *owner_window = 0;
	if (owner)
	{
		owner_window = impl->window_map[owner];
		description.set_owner_window(owner_window->window);
	}

	CL_GUITopLevelWindowSystem *top_level_window;

	if (temporary)
	{
		impl->used_cached_windows++;

		if (impl->used_cached_windows <= impl->cached_windows.size())
		{
			top_level_window = impl->cached_windows[impl->used_cached_windows-1];
			top_level_window->slots = CL_SlotContainer();
		}
		else
		{
			top_level_window = new CL_GUITopLevelWindowSystem;
			top_level_window->window = CL_DisplayWindow(description);
			impl->cached_windows.push_back(top_level_window);
		}
	}
	else
	{
		top_level_window = new CL_GUITopLevelWindowSystem;
		top_level_window->window = CL_DisplayWindow(description);
	}

	top_level_window->window.get_gc().set_map_mode(cl_map_2d_upper_left);

	top_level_window->slots.connect(top_level_window->window.sig_lost_focus(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_displaywindow_lost_focus, handle);
	top_level_window->slots.connect(top_level_window->window.sig_got_focus(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_displaywindow_got_focus, handle);
	top_level_window->slots.connect(top_level_window->window.sig_resize(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_displaywindow_resize, handle);
	top_level_window->slots.connect(top_level_window->window.sig_paint(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_displaywindow_paint, handle);
	top_level_window->slots.connect(top_level_window->window.sig_window_close(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_displaywindow_window_close, handle);

	CL_InputContext ic = top_level_window->window.get_ic();
	top_level_window->slots.connect(ic.get_mouse().sig_key_up(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_key_down(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_pointer_move(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
	top_level_window->slots.connect(ic.get_keyboard().sig_key_up(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
	top_level_window->slots.connect(ic.get_keyboard().sig_key_down(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);

	for (int i = 0; i < ic.get_tablet_count(); ++i)
	{
		top_level_window->slots.connect(ic.get_tablet(i).sig_axis_move(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_down(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_up(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_proximity_change(), impl.get(), &CL_GUIWindowManagerSystem_Impl::on_input, handle);
	}

	impl->window_map[handle] = top_level_window;
}

void CL_GUIWindowManagerSystem::destroy_window(CL_GUITopLevelWindow *handle)
{
	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::iterator it = impl->window_map.find(handle);

	bool cached = false;
	for (std::vector<CL_GUITopLevelWindowSystem *>::size_type i = 0; i < impl->cached_windows.size(); i++)
	{
		if ((*it).second == impl->cached_windows[i])
		{
			cached = true;
			(*it).second->window.hide();
			impl->used_cached_windows--;
			break;
		}
	}

	if (cached == false)
		delete impl->window_map[handle];

	impl->window_map.erase(it);
}

void CL_GUIWindowManagerSystem::enable_window(CL_GUITopLevelWindow *handle, bool enable)
{
	impl->window_map[handle]->window.set_enabled(enable);
}

bool CL_GUIWindowManagerSystem::has_focus(CL_GUITopLevelWindow *handle) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = impl->window_map.find(handle);
	if (it != impl->window_map.end())
		return it->second->window.has_focus();
	else
		return false;
}

void CL_GUIWindowManagerSystem::set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate)
{
	impl->window_map[handle]->window.set_visible(visible, activate);
}

void CL_GUIWindowManagerSystem::set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area)
{
	impl->window_map[handle]->window.set_position(geometry, client_area);
}

CL_Rect CL_GUIWindowManagerSystem::get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = impl->window_map.find(handle);
	if (it != impl->window_map.end())
	{
		if (client_area)
			return it->second->window.get_viewport();
		else
			return it->second->window.get_geometry();
	}
	else
	{
		return CL_Rect();
	}
}

CL_Point CL_GUIWindowManagerSystem::screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = impl->window_map.find(handle);
	if (it != impl->window_map.end())
	{
		if (client_area)
		{
			return it->second->window.screen_to_client(screen_point);
		}
		else
		{
			CL_Rect geometry = it->second->window.get_geometry();
			return CL_Point(screen_point.x - geometry.left, screen_point.y - geometry.top);
		}
	}
	else
	{
		return CL_Point();
	}
}

CL_Point CL_GUIWindowManagerSystem::window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = impl->window_map.find(handle);
	if (it != impl->window_map.end())
	{
		if (client_area)
		{
			return it->second->window.client_to_screen(window_point);
		}
		else
		{
			CL_Rect geometry = it->second->window.get_geometry();
			return CL_Point(geometry.left + window_point.x, geometry.top + window_point.y);
		}
	}
	else
	{
		return CL_Point();
	}
}

CL_GraphicContext CL_GUIWindowManagerSystem::get_gc(CL_GUITopLevelWindow *handle)
{
	return impl->window_map[handle]->window.get_gc();
}

CL_InputContext CL_GUIWindowManagerSystem::get_ic(CL_GUITopLevelWindow *handle)
{
	return impl->window_map[handle]->window.get_ic();
}

CL_GraphicContext CL_GUIWindowManagerSystem::begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext gc = impl->window_map[handle]->window.get_gc();

	gc.set_map_mode(cl_map_2d_upper_left);
	// TODO: Is gc.set_cliprect() required - Should the components clip instead?
	gc.set_cliprect(update_region);

	CL_BlendMode blendmode;
	blendmode.enable_blending(true);
	gc.set_blend_mode(blendmode);

	return gc;
}

void CL_GUIWindowManagerSystem::end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext gc = impl->window_map[handle]->window.get_gc();
	gc.reset_blend_mode();
	gc.reset_cliprect();

	impl->window_map[handle]->window.update(update_region);
}

CL_Timer CL_GUIWindowManagerSystem::create_timer(CL_GUITopLevelWindow *handle)
{
	CL_Timer timer(impl->window_map[handle]->window);
	return timer;
}

void CL_GUIWindowManagerSystem::capture_mouse(CL_GUITopLevelWindow *handle, bool state)
{
	if (state)
	{
		impl->window_map[handle]->window.capture_mouse(true);
		impl->capture_mouse_window = handle;
	}
	else
	{
		// Don't remove capture to the window, if handle does not control the capture
		if (impl->capture_mouse_window == handle)
		{
			impl->window_map[impl->capture_mouse_window]->window.capture_mouse(false);
			impl->capture_mouse_window = NULL;
		}
	}
}

CL_DisplayWindow CL_GUIWindowManagerSystem::get_display_window(CL_GUITopLevelWindow *handle) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = impl->window_map.find(handle);
	if (it != impl->window_map.end())
		return it->second->window;
	else
		return CL_DisplayWindow();
}

void CL_GUIWindowManagerSystem::set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor)
{
	impl->window_map[handle]->window.set_cursor(cursor);
}

void CL_GUIWindowManagerSystem::set_cursor(CL_GUITopLevelWindow *handle, CL_StandardCursor type)
{
	impl->window_map[handle]->window.set_cursor(type);
}

void CL_GUIWindowManagerSystem::bring_to_front(CL_GUITopLevelWindow *handle)
{
	if (handle != 0)
	{
		impl->window_map[handle]->window.bring_to_front();
	}
}

bool CL_GUIWindowManagerSystem::is_minimized(CL_GUITopLevelWindow *handle)
{
	if (handle != 0)
	{
		return impl->window_map[handle]->window.is_minimized();
	}

	return false;
}

bool CL_GUIWindowManagerSystem::is_maximized(CL_GUITopLevelWindow *handle)
{
	if (handle != 0)
	{
		return impl->window_map[handle]->window.is_maximized();
	}

	return false;
}

bool CL_GUIWindowManagerSystem::has_message()
{
	return CL_DisplayMessageQueue::wait(0);
}

void CL_GUIWindowManagerSystem::process_message()
{
	CL_DisplayWindowMessage message = CL_DisplayMessageQueue::get_message();
	CL_DisplayMessageQueue::dispatch_message(message);
}

void CL_GUIWindowManagerSystem::wait_for_message()
{
	if (impl->func_wait_for_message.is_null())
		CL_DisplayMessageQueue::wait();
	else
		impl->func_wait_for_message.invoke();
}

void CL_GUIWindowManagerSystem::invalidate_rect(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	impl->window_map[handle]->window.invalidate_rect(update_region);
}

void CL_GUIWindowManagerSystem::update()
{
	// No action is required for the system window manager
}

void CL_GUIWindowManagerSystem::setup_painting()
{
	// No action is required for the system window manager
}

void CL_GUIWindowManagerSystem::complete_painting()
{
	// No action is required for the system window manager
}


/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem Implementation:
