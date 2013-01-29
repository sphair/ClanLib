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
#include "../gui_manager_impl.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_System Construction:

GUIWindowManagerProvider_System::GUIWindowManagerProvider_System()
: site(0), capture_mouse_window(0), used_cached_windows(0)
{
}

GUIWindowManagerProvider_System::~GUIWindowManagerProvider_System()
{
	for (std::vector<GUITopLevelWindowSystem>::size_type i = 0; i < cached_windows.size(); i++)
	{
		delete cached_windows[i];
	}
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_System Attributes:

GUIWindowManager::WindowManagerType GUIWindowManagerProvider_System::get_window_manager_type() const
{
	return GUIWindowManager::cl_wm_type_system;
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_System Operations:

void GUIWindowManagerProvider_System::on_displaywindow_lost_focus(GUITopLevelWindow *top_level_window)
{
	site->func_focus_lost->invoke(top_level_window);
}

void GUIWindowManagerProvider_System::on_displaywindow_got_focus(GUITopLevelWindow *top_level_window)
{
	site->func_focus_gained->invoke(top_level_window);
}

void GUIWindowManagerProvider_System::on_displaywindow_resize(int width, int height, GUITopLevelWindow *top_level_window)
{
	site->func_resize->invoke(top_level_window, Size(width, height));
}

void GUIWindowManagerProvider_System::on_displaywindow_paint(const Rect &rect, GUITopLevelWindow *top_level_window)
{
	site->func_paint->invoke(top_level_window, rect);
}

void GUIWindowManagerProvider_System::on_displaywindow_window_close(GUITopLevelWindow *top_level_window)
{
	site->func_close->invoke(top_level_window);
}

void GUIWindowManagerProvider_System::on_displaywindow_window_destroy(GUITopLevelWindow *top_level_window)
{
	maintain_window_cache(top_level_window);
	site->func_destroy->invoke(top_level_window);
}

void GUIWindowManagerProvider_System::on_input(const InputEvent &incoming_input_event, GUITopLevelWindow *top_level_window)
{
	InputEvent input_event = incoming_input_event;
	GUITopLevelWindow *target = top_level_window;

	site->func_input_received->invoke(target, input_event);
}

void GUIWindowManagerProvider_System::set_site(GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void GUIWindowManagerProvider_System::create_window(
	GUITopLevelWindow *handle,
	GUITopLevelWindow *owner,
	GUIComponent *component,
	GUITopLevelDescription description)
{
	description.set_update_supported(true);

	GUITopLevelWindowSystem *owner_window = 0;
	if (owner)
	{
		owner_window = window_map[owner];
		description.set_owner_window(owner_window->window);
	}

	GUITopLevelWindowSystem *top_level_window;

#ifdef WIN32	// Cached windows do not work on Linux
	if (description.get_using_gui_window_cache())
	{
		used_cached_windows++;

		if (used_cached_windows <= cached_windows.size())
		{
			top_level_window = cached_windows[used_cached_windows-1];
			top_level_window->slots = SlotContainer();
			cache_window_handles[handle] = top_level_window->window.get_hwnd();
		}
		else
		{
			top_level_window = new GUITopLevelWindowSystem;
			top_level_window->window = DisplayWindow(description);
			top_level_window->canvas = Canvas(top_level_window->window);
			cached_windows.push_back(top_level_window);
			cache_window_handles[handle] = top_level_window->window.get_hwnd();
		}
	}
	else
#endif
	{
		top_level_window = new GUITopLevelWindowSystem;
		top_level_window->window = DisplayWindow(description);
		top_level_window->canvas = Canvas(top_level_window->window);
	}

	top_level_window->canvas.set_map_mode(map_2d_upper_left);

	top_level_window->slots.connect(top_level_window->window.sig_lost_focus(), this, &GUIWindowManagerProvider_System::on_displaywindow_lost_focus, handle);
	top_level_window->slots.connect(top_level_window->window.sig_got_focus(), this, &GUIWindowManagerProvider_System::on_displaywindow_got_focus, handle);
	top_level_window->slots.connect(top_level_window->window.sig_resize(), this, &GUIWindowManagerProvider_System::on_displaywindow_resize, handle);
	top_level_window->slots.connect(top_level_window->window.sig_paint(), this, &GUIWindowManagerProvider_System::on_displaywindow_paint, handle);
	top_level_window->slots.connect(top_level_window->window.sig_window_close(), this, &GUIWindowManagerProvider_System::on_displaywindow_window_close, handle);
	top_level_window->slots.connect(top_level_window->window.sig_window_destroy(), this, &GUIWindowManagerProvider_System::on_displaywindow_window_destroy, handle);

	InputContext ic = top_level_window->window.get_ic();
	top_level_window->slots.connect(ic.get_mouse().sig_key_up(), this, &GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_key_down(), this, &GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_key_dblclk(), this, &GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_mouse().sig_pointer_move(), this, &GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_keyboard().sig_key_up(), this, &GUIWindowManagerProvider_System::on_input, handle);
	top_level_window->slots.connect(ic.get_keyboard().sig_key_down(), this, &GUIWindowManagerProvider_System::on_input, handle);

	for (int i = 0; i < ic.get_tablet_count(); ++i)
	{
		top_level_window->slots.connect(ic.get_tablet(i).sig_axis_move(), this, &GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_down(), this, &GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_dblclk(), this, &GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_key_up(), this, &GUIWindowManagerProvider_System::on_input, handle);
		top_level_window->slots.connect(ic.get_tablet(i).sig_proximity_change(), this, &GUIWindowManagerProvider_System::on_input, handle);
	}

	window_map[handle] = top_level_window;

	sig_toplevel_window_created.invoke(top_level_window->window);
}

void GUIWindowManagerProvider_System::destroy_window(GUITopLevelWindow *handle)
{
	sig_toplevel_window_destroyed.invoke(window_map[handle]->window);

	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::iterator it = window_map.find(handle);
	if (it == window_map.end())
		throw Exception("destroy window called on non-existing window.");

	bool cached = false;
#ifdef WIN32	// Cached windows do not work on Linux
	for (std::vector<GUITopLevelWindowSystem *>::size_type i = 0; i < cached_windows.size(); i++)
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

void GUIWindowManagerProvider_System::enable_window(GUITopLevelWindow *handle, bool enable)
{
	window_map[handle]->window.set_enabled(enable);
}

bool GUIWindowManagerProvider_System::has_focus(GUITopLevelWindow *handle) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
		return it->second->window.has_focus();
	else
		return false;
}

void GUIWindowManagerProvider_System::set_visible(GUITopLevelWindow *handle, bool visible, bool activate)
{
	window_map[handle]->window.set_visible(visible, activate);
}

void GUIWindowManagerProvider_System::set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area)
{
	window_map[handle]->window.set_position(geometry, client_area);
}

Rect GUIWindowManagerProvider_System::get_geometry(GUITopLevelWindow *handle, bool client_area) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		if (client_area)
		{
			Rect r = it->second->window.get_viewport();
			Point tl = it->second->window.client_to_screen(r.get_top_left());
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
		return Rect();
	}
}

Point GUIWindowManagerProvider_System::screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		if (client_area)
		{
			return it->second->window.screen_to_client(screen_point);
		}
		else
		{
			Rect geometry = it->second->window.get_geometry();
			return Point(screen_point.x - geometry.left, screen_point.y - geometry.top);
		}
	}
	else
	{
		return Point();
	}
}

Point GUIWindowManagerProvider_System::window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		if (client_area)
		{
			return it->second->window.client_to_screen(window_point);
		}
		else
		{
			Rect geometry = it->second->window.get_geometry();
			return Point(geometry.left + window_point.x, geometry.top + window_point.y);
		}
	}
	else
	{
		return Point();
	}
}

Canvas& GUIWindowManagerProvider_System::get_canvas(GUITopLevelWindow *handle)
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it == window_map.end())
		throw Exception("Cannot find window handle");

	return it->second->canvas;
}

InputContext GUIWindowManagerProvider_System::get_ic(GUITopLevelWindow *handle) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it == window_map.end())
		throw Exception("Cannot find window handle");

	return it->second->window.get_ic();
}

Canvas GUIWindowManagerProvider_System::begin_paint(GUITopLevelWindow *handle, const Rect &update_region)
{
	Canvas canvas = window_map[handle]->canvas;
	canvas.clear(Colorf::transparent); // This line is needed by the ribbon glass effect.
	canvas.set_map_mode(map_2d_upper_left);
	canvas.set_cliprect(update_region);
	return canvas;
}

void GUIWindowManagerProvider_System::end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region)
{
	canvas.reset_cliprect();

	window_map[handle]->window.update(update_region);
}

void GUIWindowManagerProvider_System::capture_mouse(GUITopLevelWindow *handle, bool state)
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

DisplayWindow GUIWindowManagerProvider_System::get_display_window(GUITopLevelWindow *handle) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
		return it->second->window;
	else
		return DisplayWindow();
}

void GUIWindowManagerProvider_System::set_cursor(GUITopLevelWindow *handle, const Cursor &cursor)
{
	window_map[handle]->window.set_cursor(cursor);
}

void GUIWindowManagerProvider_System::set_cursor(GUITopLevelWindow *handle, StandardCursor type)
{
	window_map[handle]->window.set_cursor(type);
}

void GUIWindowManagerProvider_System::bring_to_front(GUITopLevelWindow *handle)
{
	if (handle != 0)
	{
		window_map[handle]->window.bring_to_front();
	}
}

bool GUIWindowManagerProvider_System::is_minimized(GUITopLevelWindow *handle) const
{
	if (handle != 0)
	{
		std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
		if (it == window_map.end())
			return false;

		return it->second->window.is_minimized();
	}

	return false;
}

bool GUIWindowManagerProvider_System::is_maximized(GUITopLevelWindow *handle) const
{
	if (handle != 0)
	{
		std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *>::const_iterator it = window_map.find(handle);
		if (it == window_map.end())
			return false;

		return it->second->window.is_maximized();
	}

	return false;
}

void GUIWindowManagerProvider_System::request_repaint(GUITopLevelWindow *handle, const Rect &update_region)
{
	window_map[handle]->window.request_repaint(update_region);
}

void GUIWindowManagerProvider_System::update()
{
	// No action is required for the system window manager
}

void GUIWindowManagerProvider_System::set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	canvas.set_cliprect(rect);
}

void GUIWindowManagerProvider_System::reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.reset_cliprect();
}

void GUIWindowManagerProvider_System::push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	canvas.push_cliprect(rect);
}

void GUIWindowManagerProvider_System::pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.pop_cliprect();
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_System Implementation:

void GUIWindowManagerProvider_System::maintain_window_cache(GUITopLevelWindow *destroy_top_level_window)
{
#ifdef _WIN32
	std::map<GUITopLevelWindow *, HWND>::iterator it;
	it = cache_window_handles.find(destroy_top_level_window);
	if (it != cache_window_handles.end())
	{
		HWND hwnd_del = (*it).second;

		// Windows about to be destroyed need to be removed from the cache.
		std::vector<GUITopLevelWindowSystem *>::size_type i;
		for (i = 0; i < cached_windows.size(); i++)
		{
			GUITopLevelWindowSystem *tlws = cached_windows[i];
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

}
