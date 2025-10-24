/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "gui_window_manager_provider_system.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/GUI/gui_window_manager_system.h"
#include "API/Display/Render/blend_mode.h"
#include "../gui_manager_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_System Construction:

CL_GUIWindowManagerProvider_System::CL_GUIWindowManagerProvider_System()
: site(0), capture_mouse_window(0), used_cached_windows(0)
{
}

CL_GUIWindowManagerProvider_System::~CL_GUIWindowManagerProvider_System()
{
	for (std::vector<CL_GUITopLevelWindowSystem>::size_type i = 0; i < cached_windows.size(); i++)
	{
		delete cached_windows[i];
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_System Attributes:

CL_GUIWindowManager::CL_WindowManagerType CL_GUIWindowManagerProvider_System::get_window_manager_type() const
{
	return CL_GUIWindowManager::cl_wm_type_system;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_System Operations:

void CL_GUIWindowManagerProvider_System::destroy()
{
	delete this;
}

void CL_GUIWindowManagerProvider_System::on_displaywindow_lost_focus(CL_GUITopLevelWindow *top_level_window)
{
	site->func_focus_lost->invoke(top_level_window);
}

void CL_GUIWindowManagerProvider_System::on_displaywindow_got_focus(CL_GUITopLevelWindow *top_level_window)
{
	site->func_focus_gained->invoke(top_level_window);
}

void CL_GUIWindowManagerProvider_System::on_displaywindow_resize(int width, int height, CL_GUITopLevelWindow *top_level_window)
{
	site->func_resize->invoke(top_level_window, CL_Size(width, height));
}

void CL_GUIWindowManagerProvider_System::on_displaywindow_paint(const CL_Rect &rect, CL_GUITopLevelWindow *top_level_window)
{
	site->func_paint->invoke(top_level_window, rect);
}

void CL_GUIWindowManagerProvider_System::on_displaywindow_window_close(CL_GUITopLevelWindow *top_level_window)
{
	site->func_close->invoke(top_level_window);
}

void CL_GUIWindowManagerProvider_System::on_displaywindow_window_destroy(CL_GUITopLevelWindow *top_level_window)
{
	maintain_window_cache(top_level_window);
	site->func_destroy->invoke(top_level_window);
}

void CL_GUIWindowManagerProvider_System::on_input(const CL_InputEvent &incoming_input_event, const CL_InputState &input_state, CL_GUITopLevelWindow *top_level_window)
{
	CL_InputEvent input_event = incoming_input_event;
	CL_GUITopLevelWindow *target = top_level_window;

	site->func_input_received->invoke(target, input_event, input_state);
}

void CL_GUIWindowManagerProvider_System::set_site(CL_GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void CL_GUIWindowManagerProvider_System::create_window(
	CL_GUITopLevelWindow *handle,
	CL_GUITopLevelWindow *owner,
	CL_GUIComponent *component,
	CL_GUITopLevelDescription description)
{
	CL_GUITopLevelWindowSystem *owner_window = 0;
	if (owner)
	{
		owner_window = window_map[owner];
		description.set_owner_window(owner_window->window);
	}

	CL_GUITopLevelWindowSystem *top_level_window;

#ifdef WIN32	// Cached windows do not work on Linux
	if (description.get_using_gui_window_cache())
	{
		used_cached_windows++;

		if (used_cached_windows <= cached_windows.size())
		{
			top_level_window = cached_windows[used_cached_windows-1];
			top_level_window->slots = CL_SlotContainer();
			cache_window_handles[handle] = top_level_window->window.get_hwnd();
		}
		else
		{
			top_level_window = new CL_GUITopLevelWindowSystem;
			top_level_window->window = CL_DisplayWindow(description);
			cached_windows.push_back(top_level_window);
			cache_window_handles[handle] = top_level_window->window.get_hwnd();
		}
	}
	else
#endif
	{
		top_level_window = new CL_GUITopLevelWindowSystem;
		top_level_window->window = CL_DisplayWindow(description);
	}

	top_level_window->window.get_gc().set_map_mode(cl_map_2d_upper_left);

	top_level_window->slots.connect(top_level_window->window.sig_lost_focus(), this, &CL_GUIWindowManagerProvider_System::on_displaywindow_lost_focus, handle);
	top_level_window->slots.connect(top_level_window->window.sig_got_focus(), this, &CL_GUIWindowManagerProvider_System::on_displaywindow_got_focus, handle);
	top_level_window->slots.connect(top_level_window->window.sig_resize(), this, &CL_GUIWindowManagerProvider_System::on_displaywindow_resize, handle);
	top_level_window->slots.connect(top_level_window->window.sig_paint(), this, &CL_GUIWindowManagerProvider_System::on_displaywindow_paint, handle);
	top_level_window->slots.connect(top_level_window->window.sig_window_close(), this, &CL_GUIWindowManagerProvider_System::on_displaywindow_window_close, handle);
	top_level_window->slots.connect(top_level_window->window.sig_window_destroy(), this, &CL_GUIWindowManagerProvider_System::on_displaywindow_window_destroy, handle);

	CL_InputContext& ic = top_level_window->window.get_ic();
	top_level_window->slots.connect(ic.get_mouse().sig_key_up(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_key_down(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_key_dblclk(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_pointer_move(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_keyboard().sig_key_up(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_keyboard().sig_key_down(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);

	for (int i = 0; i < ic.get_tablet_count(); ++i)
	{
		top_level_window->slots.connect(ic.get_tablet(i).sig_axis_move(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_down(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_dblclk(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_up(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_proximity_change(), this, &CL_GUIWindowManagerProvider_System::on_input, handle);
	}

	window_map[handle] = top_level_window;
}

void CL_GUIWindowManagerProvider_System::destroy_window(CL_GUITopLevelWindow *handle)
{
	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::iterator it = window_map.find(handle);
	if (it == window_map.end())
		throw CL_Exception("destroy window called on non-existing window.");

	bool cached = false;
#ifdef WIN32	// Cached windows do not work on Linux
	for (std::vector<CL_GUITopLevelWindowSystem *>::size_type i = 0; i < cached_windows.size(); i++)
	{
		if ((*it).second == cached_windows[i])
		{
			cached = true;
			(*it).second->window.hide();
			used_cached_windows--;
			break;
		}
	}
#endif
	if (cached == false)
		delete window_map[handle];

	window_map.erase(it);
}

void CL_GUIWindowManagerProvider_System::enable_window(CL_GUITopLevelWindow *handle, bool enable)
{
	window_map[handle]->window.set_enabled(enable);
}

bool CL_GUIWindowManagerProvider_System::has_focus(CL_GUITopLevelWindow *handle) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
		return it->second->window.has_focus();
	else
		return false;
}

void CL_GUIWindowManagerProvider_System::set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate)
{
	window_map[handle]->window.set_visible(visible, activate);
}

void CL_GUIWindowManagerProvider_System::set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area)
{
	window_map[handle]->window.set_position(geometry, client_area);
}

CL_Rect CL_GUIWindowManagerProvider_System::get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		if (client_area)
		{
			CL_Rect r = it->second->window.get_viewport();
			CL_Point tl = it->second->window.client_to_screen(r.get_top_left());
			r.left += tl.x;
			r.right += tl.x;
			r.top += tl.y;
			r.bottom += tl.y;
			return r;
		}
		else
			return it->second->window.get_geometry();
	}
	else
	{
		return CL_Rect();
	}
}

CL_Point CL_GUIWindowManagerProvider_System::screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
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

CL_Point CL_GUIWindowManagerProvider_System::window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
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

CL_GraphicContext& CL_GUIWindowManagerProvider_System::get_gc(CL_GUITopLevelWindow *handle) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it == window_map.end())
		throw CL_Exception("Cannot find window handle");

	return it->second->window.get_gc();
}

CL_InputContext& CL_GUIWindowManagerProvider_System::get_ic(CL_GUITopLevelWindow *handle) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it == window_map.end())
		throw CL_Exception("Cannot find window handle");

	return it->second->window.get_ic();
}

CL_GraphicContext CL_GUIWindowManagerProvider_System::begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext &gc = window_map[handle]->window.get_gc();

	gc.set_map_mode(cl_map_2d_upper_left);
	// TODO: Is gc.set_cliprect() required - Should the components clip instead?
	gc.set_cliprect(update_region);

	CL_BlendMode blendmode;
	blendmode.enable_blending(true);
	gc.set_blend_mode(blendmode);

	return gc;
}

void CL_GUIWindowManagerProvider_System::end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext &gc = window_map[handle]->window.get_gc();
	gc.reset_blend_mode();
	gc.reset_cliprect();

	window_map[handle]->window.update(update_region);
}

void CL_GUIWindowManagerProvider_System::capture_mouse(CL_GUITopLevelWindow *handle, bool state)
{
	if (state)
	{
		window_map[handle]->window.capture_mouse(true);
		capture_mouse_window = handle;
	}
	else
	{
		// Don't remove capture to the window, if handle does not control the capture
		if (capture_mouse_window == handle)
		{
			window_map[capture_mouse_window]->window.capture_mouse(false);
			capture_mouse_window = NULL;
		}
	}
}

CL_DisplayWindow CL_GUIWindowManagerProvider_System::get_display_window(CL_GUITopLevelWindow *handle) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
		return it->second->window;
	else
		return CL_DisplayWindow();
}

void CL_GUIWindowManagerProvider_System::set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor)
{
	window_map[handle]->window.set_cursor(cursor);
}

void CL_GUIWindowManagerProvider_System::set_cursor(CL_GUITopLevelWindow *handle, CL_StandardCursor type)
{
	window_map[handle]->window.set_cursor(type);
}

void CL_GUIWindowManagerProvider_System::bring_to_front(CL_GUITopLevelWindow *handle)
{
	if (handle != 0)
	{
		window_map[handle]->window.bring_to_front();
	}
}

bool CL_GUIWindowManagerProvider_System::is_minimized(CL_GUITopLevelWindow *handle) const
{
	if (handle != 0)
	{
		std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
		if (it == window_map.end())
			return false;

		return it->second->window.is_minimized();
	}

	return false;
}

bool CL_GUIWindowManagerProvider_System::is_maximized(CL_GUITopLevelWindow *handle) const
{
	if (handle != 0)
	{
		std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
		if (it == window_map.end())
			return false;

		return it->second->window.is_maximized();
	}

	return false;
}

void CL_GUIWindowManagerProvider_System::request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	window_map[handle]->window.request_repaint(update_region);
}

void CL_GUIWindowManagerProvider_System::update()
{
	// No action is required for the system window manager
}

void CL_GUIWindowManagerProvider_System::setup_painting()
{
	// No action is required for the system window manager
}

void CL_GUIWindowManagerProvider_System::complete_painting()
{
	// No action is required for the system window manager
}

void CL_GUIWindowManagerProvider_System::set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	gc.set_cliprect(rect);
}

void CL_GUIWindowManagerProvider_System::reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.reset_cliprect();
}

void CL_GUIWindowManagerProvider_System::push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	gc.push_cliprect(rect);
}

void CL_GUIWindowManagerProvider_System::pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.pop_cliprect();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_System Implementation:

void CL_GUIWindowManagerProvider_System::maintain_window_cache(CL_GUITopLevelWindow *destroy_top_level_window)
{
#ifdef _WIN32
	std::map<CL_GUITopLevelWindow *, HWND>::iterator it;
	it = cache_window_handles.find(destroy_top_level_window);
	if (it != cache_window_handles.end())
	{
		HWND hwnd_del = (*it).second;

		// Windows about to be destroyed need to be removed from the cache.
		std::vector<CL_GUITopLevelWindowSystem *>::size_type i;
		for (i = 0; i < cached_windows.size(); i++)
		{
			CL_GUITopLevelWindowSystem *tlws = cached_windows[i];
			HWND cache_hwnd = tlws->window.get_hwnd();
			if (cache_hwnd == hwnd_del)
			{
				cached_windows.erase(cached_windows.begin()+i);
				cache_window_handles.erase(it);
				break;
			}
		}
	}
#endif
}
