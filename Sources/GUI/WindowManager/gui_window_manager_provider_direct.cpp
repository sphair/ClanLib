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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "gui_window_manager_provider_direct.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/image.h"
#include <algorithm>
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Construction:

GUIWindowManagerProvider_Direct::GUIWindowManagerProvider_Direct(DisplayWindow &display_window, Canvas &canvas)
: site(0), activated_window(0), capture_mouse_window(NULL), display_window(display_window), window_canvas(canvas)
{
	slots.connect(display_window.sig_window_close(), this, &GUIWindowManagerProvider_Direct::on_displaywindow_window_close);

	InputContext ic = display_window.get_ic();
	slots.connect(ic.get_mouse().sig_key_up(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_up);
	slots.connect(ic.get_mouse().sig_key_down(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_down);
	slots.connect(ic.get_mouse().sig_key_dblclk(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_down);
	slots.connect(ic.get_mouse().sig_pointer_move(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_move);

	slots.connect(ic.get_keyboard().sig_key_up(), this, &GUIWindowManagerProvider_Direct::on_input);
	slots.connect(ic.get_keyboard().sig_key_down(), this, &GUIWindowManagerProvider_Direct::on_input);

	for (int tc = 0; tc < ic.get_tablet_count(); ++tc)
	{
		slots.connect(ic.get_tablet(tc).sig_axis_move(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_move);
		slots.connect(ic.get_tablet(tc).sig_key_dblclk(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_down);
		slots.connect(ic.get_tablet(tc).sig_key_down(), this, &GUIWindowManagerProvider_Direct::on_input_mouse_down);
		slots.connect(ic.get_tablet(tc).sig_key_up(), this, &GUIWindowManagerProvider_Direct::on_input);
	}

}

GUIWindowManagerProvider_Direct::~GUIWindowManagerProvider_Direct()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Attributes:

GUIWindowManager::WindowManagerType GUIWindowManagerProvider_Direct::get_window_manager_type() const
{
	return GUIWindowManager::cl_wm_type_texture;		//FIXME!
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Operations:

void GUIWindowManagerProvider_Direct::on_displaywindow_window_close()
{
	if (activated_window == 0)
		return;

	site->func_close->invoke(activated_window);
}

void GUIWindowManagerProvider_Direct::on_input(const InputEvent &input_event)
{
	if (activated_window == 0)
		return;
	
	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event);

	invoke_input_received(activated_window, new_input_event);
}

void GUIWindowManagerProvider_Direct::on_input_mouse_move(const InputEvent &input_event)
{
	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event);

	bool capture_mouse_flag = false;
	if (capture_mouse_window)
	{
			capture_mouse_flag = true;
	}

	GUITopLevelWindow *toplevel_window;
	if (capture_mouse_flag)
	{
		toplevel_window = capture_mouse_window;
	}
	else
	{
		toplevel_window = get_window_at_point(new_input_event.mouse_pos);
	}

	if (toplevel_window)
		invoke_input_received(toplevel_window, new_input_event);
}

void GUIWindowManagerProvider_Direct::on_input_mouse_up(const InputEvent &input_event)
{
	// It seems multiple windows in the same app act differently for window SetCapture()
	if (!capture_mouse_window)
	{
		// Process MouseUp as normal input event when capture mouse is not used
		on_input(input_event);
		return;
	}

	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event);

	invoke_input_received(capture_mouse_window, new_input_event);
}
void GUIWindowManagerProvider_Direct::on_input_mouse_down(const InputEvent &input_event)
{
	InputEvent new_input_event = input_event;

	if (!func_input_intercept.is_null())
		func_input_intercept.invoke(new_input_event);

	// It seems multiple windows in the same app act differently for window SetCapture()
	if (capture_mouse_window)
	{
		invoke_input_received(capture_mouse_window, new_input_event);
		return;
	}

	GUITopLevelWindow *toplevel_window;
	toplevel_window = get_window_at_point(new_input_event.mouse_pos);

	if (toplevel_window)
		bring_to_front(toplevel_window);

	// Send focus lost events
	if (toplevel_window)
	{
		if (toplevel_window != activated_window)
		{
			// Change of focus required
			// From: http://msdn.microsoft.com/en-us/library/ms646274(VS.85).aspx - deactivation occurs before activation

			if (activated_window)
			{
				site->func_focus_lost->invoke(activated_window);
				toplevel_window = get_window_at_point(new_input_event.mouse_pos);
			}
		}
	}

	// Send focus gained events
	if (toplevel_window)
	{
		if (toplevel_window != activated_window)
		{
			activated_window = toplevel_window;
			site->func_focus_gained->invoke(activated_window);
			toplevel_window = get_window_at_point(new_input_event.mouse_pos);
		}
	}

	// Send mouse click event to toplevel window
	if (toplevel_window)
	{
		invoke_input_received(toplevel_window, new_input_event);
	}

}

void GUIWindowManagerProvider_Direct::bring_to_front(GUITopLevelWindow *handle)
{
	GUITopLevelWindowDirect *direct_window = get_direct_window(handle);
	if(direct_window)
	{
		// Get root direct_window
		GUITopLevelWindowDirect *root_direct_window = direct_window;
		while (root_direct_window->owner_window)
		{
			root_direct_window = root_direct_window->owner_window;
		}

		// Check root window order
		if (root_window_z_order[0] != root_direct_window)
		{
			std::vector<GUITopLevelWindowDirect *>::iterator it;
			it = std::find(root_window_z_order.begin(), root_window_z_order.end(), root_direct_window);
			root_window_z_order.erase(it);
			root_window_z_order.insert(root_window_z_order.begin(), root_direct_window);
		}

		// Check owner window order
		if (direct_window->owner_window)
		{
			std::vector<GUITopLevelWindowDirect *> &z_order = direct_window->owner_window->child_windows_zorder;
			if (z_order[0] != direct_window)
			{
				std::vector<GUITopLevelWindowDirect *>::iterator it;
				it = std::find(z_order.begin(), z_order.end(), direct_window);
				z_order.erase(it);
				z_order.insert(z_order.begin(), direct_window);
			}
		}
	}
}

void GUIWindowManagerProvider_Direct::set_site(GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void GUIWindowManagerProvider_Direct::create_window(
	GUITopLevelWindow *handle,
	GUITopLevelWindow *owner,
	GUIComponent *component,
	GUITopLevelDescription description)
{
	//Note: destroy_window() deletes this window
	GUITopLevelWindowDirect *toplevel_window = new GUITopLevelWindowDirect(handle);
	Rect geometry = description.get_position();
	toplevel_window->geometry = geometry;
	toplevel_window->enabled = true;
	toplevel_window->visible = true;

	window_map[handle] = toplevel_window;

	if (owner)
	{
		// Attach to owner
		toplevel_window->owner_window = get_direct_window(owner);
		toplevel_window->owner_window->child_windows_zorder.insert(toplevel_window->owner_window->child_windows_zorder.begin(), toplevel_window);
	}
	else
	{
		// Attach to root
		root_window_z_order.insert(root_window_z_order.begin(), toplevel_window);
	}

	activated_window = handle;

}

void GUIWindowManagerProvider_Direct::destroy_window(GUITopLevelWindow *handle)
{
	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);

	if (toplevel_window->owner_window)
	{
		// Detach from owner
		toplevel_window->owner_window->child_windows_zorder.erase(std::find(toplevel_window->owner_window->child_windows_zorder.begin(), toplevel_window->owner_window->child_windows_zorder.end(), toplevel_window));
	}
	else
	{
		// Detach from root
		root_window_z_order.erase(std::find(root_window_z_order.begin(), root_window_z_order.end(), toplevel_window));
	}

	if (handle == activated_window)	// Ensure that the destroyed window is not in focus
	{
		activated_window = NULL;
	}

	delete toplevel_window;
	window_map.erase(window_map.find(handle));
}

void GUIWindowManagerProvider_Direct::enable_window(GUITopLevelWindow *handle, bool enable)
{
	GUITopLevelWindowDirect *window_direct_window = get_direct_window(handle);
	window_direct_window->enabled = enable;
}

bool GUIWindowManagerProvider_Direct::has_focus(GUITopLevelWindow *handle) const
{
	return activated_window == handle;
}

void GUIWindowManagerProvider_Direct::set_visible(GUITopLevelWindow *handle, bool visible, bool activate)
{
	GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	toplevel_window->visible = visible;
	if (activate)
		activated_window = handle;
}

void GUIWindowManagerProvider_Direct::set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area)
{
	GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	toplevel_window->geometry = geometry;

}

Rect GUIWindowManagerProvider_Direct::get_geometry(GUITopLevelWindow *handle, bool client_area) const
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowDirect *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		return it->second->geometry;
	}
	else
	{
		return Rect();
	}
}

Point GUIWindowManagerProvider_Direct::screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const
{
	Rect geometry = get_geometry(handle, false);
	return Point(screen_point.x - geometry.left, screen_point.y - geometry.top);
}

Point GUIWindowManagerProvider_Direct::window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const
{
	Rect geometry = get_geometry(handle, false);
	return Point(window_point.x + geometry.left, window_point.y + geometry.top);
}

Canvas& GUIWindowManagerProvider_Direct::get_canvas(GUITopLevelWindow *handle)
{
	return window_canvas;
}

InputContext GUIWindowManagerProvider_Direct::get_ic(GUITopLevelWindow *handle) const
{
	return display_window.get_ic();
}

Canvas GUIWindowManagerProvider_Direct::begin_paint(GUITopLevelWindow *handle, const Rect &update_region)
{
	GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);

	Canvas canvas = window_canvas.create();

	Rect clip_rect = update_region;
	clip_rect.translate(toplevel_window->geometry.left, toplevel_window->geometry.top);
	clip_rect.overlap(toplevel_window->geometry);

	canvas.set_cliprect(clip_rect);

	// Translate model view matrix to the window position
	canvas.mult_translate(toplevel_window->geometry.left, toplevel_window->geometry.top, 0);
	return canvas;
}

void GUIWindowManagerProvider_Direct::end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region)
{
}

void GUIWindowManagerProvider_Direct::capture_mouse(GUITopLevelWindow *handle, bool state)
{
	if (state)
	{
		display_window.capture_mouse(true);
		capture_mouse_window = handle;
	}
	else
	{
		// Don't remove capture to the window, if handle does not control the capture
		if (capture_mouse_window == handle)
		{
			display_window.capture_mouse(false);
			capture_mouse_window = NULL;
		}
	}
}

DisplayWindow GUIWindowManagerProvider_Direct::get_display_window(GUITopLevelWindow *handle) const
{
	return display_window;
}

void GUIWindowManagerProvider_Direct::set_cursor(GUITopLevelWindow *handle, const Cursor &cursor)
{
}

void GUIWindowManagerProvider_Direct::set_cursor(GUITopLevelWindow *handle, StandardCursor type)
{
}

bool GUIWindowManagerProvider_Direct::is_minimized(GUITopLevelWindow *handle) const
{
	return false;
}

bool GUIWindowManagerProvider_Direct::is_maximized(GUITopLevelWindow *handle) const
{
	return false;
}

void GUIWindowManagerProvider_Direct::request_repaint(GUITopLevelWindow *handle, const Rect &update_region)
{
	// Since this window manager always redraws, request_repaint() is not required
}

void GUIWindowManagerProvider_Direct::update()
{
}

void GUIWindowManagerProvider_Direct::finalise_painting()
{

}


bool GUIWindowManagerProvider_Direct::is_constant_repaint_enabled(GUIComponent *component) const
{
	// Constant repaint is always required
	return true;
}

void GUIWindowManagerProvider_Direct::set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	Rect geometry = toplevel_window->geometry;
	Rect cliprect = rect;
	cliprect.translate(geometry.left, geometry.top);
	cliprect.overlap(geometry);
	canvas.set_cliprect(cliprect);

}

void GUIWindowManagerProvider_Direct::reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.pop_cliprect();
}

void GUIWindowManagerProvider_Direct::push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	Rect geometry = toplevel_window->geometry;
	Rect cliprect = rect;
	cliprect.translate(geometry.left, geometry.top);
	cliprect.overlap(geometry);
	canvas.push_cliprect(cliprect);
}

void GUIWindowManagerProvider_Direct::pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.pop_cliprect();
}


/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerProvider_Direct Implementation:

GUITopLevelWindow *GUIWindowManagerProvider_Direct::get_window_at_point(const Point &point)
{
	return get_window_at_point(point, root_window_z_order);
}

GUITopLevelWindow *GUIWindowManagerProvider_Direct::get_window_at_point(const Point &point, const std::vector<GUITopLevelWindowDirect *> &z_order)
{
	std::vector<GUITopLevelWindowDirect *>::size_type index, size;
	size = z_order.size();
	for (index = 0; index < size; index++)
	{
		GUITopLevelWindowDirect *toplevel_window = z_order[index];
		if (toplevel_window->visible)
		{
			// Check children first
			GUITopLevelWindow *child_window = get_window_at_point(point, toplevel_window->child_windows_zorder);
			if (child_window)
				return child_window;

			if (toplevel_window->geometry.contains(point))
			{
				return toplevel_window->window;
			}
		}
	}
	return 0;
}

void GUIWindowManagerProvider_Direct::invoke_input_received(GUITopLevelWindow *window, const InputEvent &input_event)
{
	GUITopLevelWindowDirect *gui_direct_window = get_direct_window(window);
	InputEvent inp_event = input_event;
	inp_event.mouse_pos.x -= gui_direct_window->geometry.left;
	inp_event.mouse_pos.y -= gui_direct_window->geometry.top;
	site->func_input_received->invoke(window, inp_event);
}

GUITopLevelWindowDirect *GUIWindowManagerProvider_Direct::get_direct_window(GUITopLevelWindow *handle)
{
	std::map<GUITopLevelWindow *, GUITopLevelWindowDirect *>::iterator it;
	it = window_map.find(handle);
	if (it == window_map.end())
		throw Exception("Invalid GUI Top Level Window requested");
	return it->second;
}

}
