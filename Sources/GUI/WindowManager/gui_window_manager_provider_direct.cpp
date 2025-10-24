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
**    Mark Page
*/

#include "GUI/precomp.h"

#include "gui_window_manager_provider_direct.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_state.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/image.h"
#include "API/Display/Render/blend_mode.h"
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_Direct Construction:

CL_GUIWindowManagerProvider_Direct::CL_GUIWindowManagerProvider_Direct(CL_DisplayWindow &display_window)
: site(0), activated_window(0), capture_mouse_window(NULL), display_window(display_window), 
  setup_painting_called(false), painting_set(false)
{
	slots.connect(display_window.sig_window_close(), this, &CL_GUIWindowManagerProvider_Direct::on_displaywindow_window_close);

	CL_InputContext& ic = display_window.get_ic();
	slots.connect(ic.get_mouse().sig_key_up(), this, &CL_GUIWindowManagerProvider_Direct::on_input_mouse_up);
	slots.connect(ic.get_mouse().sig_key_down(), this, &CL_GUIWindowManagerProvider_Direct::on_input_mouse_down);
	slots.connect(ic.get_mouse().sig_pointer_move(), this, &CL_GUIWindowManagerProvider_Direct::on_input_mouse_move);

	slots.connect(ic.get_keyboard().sig_key_up(), this, &CL_GUIWindowManagerProvider_Direct::on_input);
	slots.connect(ic.get_keyboard().sig_key_down(), this, &CL_GUIWindowManagerProvider_Direct::on_input);

	for (int tc = 0; tc < ic.get_tablet_count(); ++tc)
	{
		slots.connect(ic.get_tablet(tc).sig_axis_move(), this, &CL_GUIWindowManagerProvider_Direct::on_input_mouse_move);
		slots.connect(ic.get_tablet(tc).sig_key_down(), this, &CL_GUIWindowManagerProvider_Direct::on_input_mouse_down);
		slots.connect(ic.get_tablet(tc).sig_key_up(), this, &CL_GUIWindowManagerProvider_Direct::on_input);
	}

}

CL_GUIWindowManagerProvider_Direct::~CL_GUIWindowManagerProvider_Direct()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_Direct Attributes:

CL_GUIWindowManager::CL_WindowManagerType CL_GUIWindowManagerProvider_Direct::get_window_manager_type() const
{
	return CL_GUIWindowManager::cl_wm_type_texture;		//FIXME!
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_Direct Operations:

void CL_GUIWindowManagerProvider_Direct::destroy()
{
	delete this;
}

void CL_GUIWindowManagerProvider_Direct::on_displaywindow_window_close()
{
	if (activated_window == 0)
		return;

	site->func_close->invoke(activated_window);
}

void CL_GUIWindowManagerProvider_Direct::on_input(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	if (activated_window == 0)
		return;
	
	invoke_input_received(activated_window, input_event, input_state);
}

void CL_GUIWindowManagerProvider_Direct::on_input_mouse_move(const CL_InputEvent &input_event, const CL_InputState &input_state)
{

	bool capture_mouse_flag = false;
	if (capture_mouse_window)
	{
			capture_mouse_flag = true;
	}

	CL_GUITopLevelWindow *toplevel_window;
	if (capture_mouse_flag)
	{
		toplevel_window = capture_mouse_window;
	}
	else
	{
		toplevel_window = get_window_at_point(input_event.mouse_pos);
	}

	if (toplevel_window)
		invoke_input_received(toplevel_window, input_event, input_state);
}

void CL_GUIWindowManagerProvider_Direct::on_input_mouse_up(const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	// It seems multiple windows in the same app act differently for window SetCapture()
	if (!capture_mouse_window)
	{
		// Process MouseUp as normal input event when capture mouse is not used
		on_input(input_event, input_state);
		return;
	}

	invoke_input_received(capture_mouse_window, input_event, input_state);


}
void CL_GUIWindowManagerProvider_Direct::on_input_mouse_down(const CL_InputEvent &input_event, const CL_InputState &input_state)
{

	// It seems multiple windows in the same app act differently for window SetCapture()
	if (capture_mouse_window)
	{
		invoke_input_received(capture_mouse_window, input_event, input_state);
		return;
	}

	CL_GUITopLevelWindow *toplevel_window;
	toplevel_window = get_window_at_point(input_event.mouse_pos);

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
				toplevel_window = get_window_at_point(input_event.mouse_pos);
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
			toplevel_window = get_window_at_point(input_event.mouse_pos);
		}
	}

	// Send mouse click event to toplevel window
	if (toplevel_window)
	{
		invoke_input_received(toplevel_window, input_event,input_state);
	}

}

void CL_GUIWindowManagerProvider_Direct::bring_to_front(CL_GUITopLevelWindow *handle)
{
	CL_GUITopLevelWindowDirect *direct_window = get_direct_window(handle);
	if(direct_window)
	{
		// Get root direct_window
		CL_GUITopLevelWindowDirect *root_direct_window = direct_window;
		while (root_direct_window->owner_window)
		{
			root_direct_window = root_direct_window->owner_window;
		}

		// Check root window order
		if (root_window_z_order[0] != root_direct_window)
		{
			std::vector<CL_GUITopLevelWindowDirect *>::iterator it;
			it = std::find(root_window_z_order.begin(), root_window_z_order.end(), root_direct_window);
			root_window_z_order.erase(it);
			root_window_z_order.insert(root_window_z_order.begin(), root_direct_window);
		}

		// Check owner window order
		if (direct_window->owner_window)
		{
			std::vector<CL_GUITopLevelWindowDirect *> &z_order = direct_window->owner_window->child_windows_zorder;
			if (z_order[0] != direct_window)
			{
				std::vector<CL_GUITopLevelWindowDirect *>::iterator it;
				it = std::find(z_order.begin(), z_order.end(), direct_window);
				z_order.erase(it);
				z_order.insert(z_order.begin(), direct_window);
			}
		}
	}
}

void CL_GUIWindowManagerProvider_Direct::set_site(CL_GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void CL_GUIWindowManagerProvider_Direct::create_window(
	CL_GUITopLevelWindow *handle,
	CL_GUITopLevelWindow *owner,
	CL_GUIComponent *component,
	CL_GUITopLevelDescription description)
{
	//Note: destroy_window() deletes this window
	CL_GUITopLevelWindowDirect *toplevel_window = new CL_GUITopLevelWindowDirect(handle);
	CL_Rect geometry = description.get_position();
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

void CL_GUIWindowManagerProvider_Direct::destroy_window(CL_GUITopLevelWindow *handle)
{
	capture_mouse(handle, false);	// Ensure the destroyed window has not captured the mouse

	CL_GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);

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

void CL_GUIWindowManagerProvider_Direct::enable_window(CL_GUITopLevelWindow *handle, bool enable)
{
	CL_GUITopLevelWindowDirect *window_direct_window = get_direct_window(handle);
	window_direct_window->enabled = enable;
}

bool CL_GUIWindowManagerProvider_Direct::has_focus(CL_GUITopLevelWindow *handle) const
{
	return activated_window == handle;
}

void CL_GUIWindowManagerProvider_Direct::set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate)
{
	CL_GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	toplevel_window->visible = visible;
	if (activate)
		activated_window = handle;
}

void CL_GUIWindowManagerProvider_Direct::set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area)
{
	CL_GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	toplevel_window->geometry = geometry;

}

CL_Rect CL_GUIWindowManagerProvider_Direct::get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowDirect *>::const_iterator it = window_map.find(handle);
	if (it != window_map.end())
	{
		return it->second->geometry;
	}
	else
	{
		return CL_Rect();
	}
}

CL_Point CL_GUIWindowManagerProvider_Direct::screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const
{
	CL_Rect geometry = get_geometry(handle, false);
	return CL_Point(screen_point.x - geometry.left, screen_point.y - geometry.top);
}

CL_Point CL_GUIWindowManagerProvider_Direct::window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const
{
	CL_Rect geometry = get_geometry(handle, false);
	return CL_Point(window_point.x + geometry.left, window_point.y + geometry.top);
}

CL_GraphicContext& CL_GUIWindowManagerProvider_Direct::get_gc(CL_GUITopLevelWindow *handle) const
{
	return display_window.get_gc();
}

CL_InputContext& CL_GUIWindowManagerProvider_Direct::get_ic(CL_GUITopLevelWindow *handle) const
{
	return display_window.get_ic();
}

CL_GraphicContext CL_GUIWindowManagerProvider_Direct::begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext &gc = display_window.get_gc();

	CL_GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);

	if ( (painting_set == false) || (setup_painting_called == false) )
	{
		painting_set = true;
	}

	gc.set_map_mode(cl_map_2d_upper_left);

	CL_Rect clip_rect = update_region;
	clip_rect.translate(toplevel_window->geometry.left, toplevel_window->geometry.top);
	clip_rect.overlap(toplevel_window->geometry);

	gc.set_cliprect(clip_rect);

	// Translate model view matrix to the window position
	gc.push_modelview();
	gc.mult_translate(toplevel_window->geometry.left, toplevel_window->geometry.top, 0);

	return gc;
}

void CL_GUIWindowManagerProvider_Direct::end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	CL_GraphicContext &gc = display_window.get_gc();
	gc.flush_batcher();

	// Restore the model view matrix
	gc.pop_modelview();

	// Only reset the frame buffer at this point when setup_painting() was not used
	if (!setup_painting_called)
	{
		gc.reset_cliprect();
	}
}

void CL_GUIWindowManagerProvider_Direct::capture_mouse(CL_GUITopLevelWindow *handle, bool state)
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

CL_DisplayWindow CL_GUIWindowManagerProvider_Direct::get_display_window(CL_GUITopLevelWindow *handle) const
{
	return display_window;
}

void CL_GUIWindowManagerProvider_Direct::set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor)
{
}

void CL_GUIWindowManagerProvider_Direct::set_cursor(CL_GUITopLevelWindow *handle, CL_StandardCursor type)
{
}

bool CL_GUIWindowManagerProvider_Direct::is_minimized(CL_GUITopLevelWindow *handle) const
{
	return false;
}

bool CL_GUIWindowManagerProvider_Direct::is_maximized(CL_GUITopLevelWindow *handle) const
{
	return false;
}

void CL_GUIWindowManagerProvider_Direct::request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region)
{
	// Since this window manager always redraws, request_repaint() is not required
}

void CL_GUIWindowManagerProvider_Direct::update()
{
}

void CL_GUIWindowManagerProvider_Direct::setup_painting()
{
	setup_painting_called = true;
	painting_set = false;
}

void CL_GUIWindowManagerProvider_Direct::finalise_painting()
{
	setup_painting_called = false;

	if ( painting_set )
	{
		painting_set = false;
		CL_GraphicContext &gc = display_window.get_gc();
		gc.reset_blend_mode();
		gc.reset_cliprect();
		gc.reset_frame_buffer();
	}
}

void CL_GUIWindowManagerProvider_Direct::complete_painting()
{
	finalise_painting();
}

bool CL_GUIWindowManagerProvider_Direct::is_constant_repaint_enabled(CL_GUIComponent *component) const
{
	// Constant repaint is always required
	return true;
}

void CL_GUIWindowManagerProvider_Direct::set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	CL_GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	CL_Rect geometry = toplevel_window->geometry;
	CL_Rect cliprect = rect;
	cliprect.translate(geometry.left, geometry.top);
	cliprect.overlap(geometry);
	gc.set_cliprect(cliprect);

}

void CL_GUIWindowManagerProvider_Direct::reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.pop_cliprect();
}

void CL_GUIWindowManagerProvider_Direct::push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect)
{
	CL_GUITopLevelWindowDirect *toplevel_window = get_direct_window(handle);
	CL_Rect geometry = toplevel_window->geometry;
	CL_Rect cliprect = rect;
	cliprect.translate(geometry.left, geometry.top);
	cliprect.overlap(geometry);
	gc.push_cliprect(cliprect);
}

void CL_GUIWindowManagerProvider_Direct::pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc)
{
	gc.pop_cliprect();
}


/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerProvider_Direct Implementation:

CL_GUITopLevelWindow *CL_GUIWindowManagerProvider_Direct::get_window_at_point(const CL_Point &point)
{
	return get_window_at_point(point, root_window_z_order);
}

CL_GUITopLevelWindow *CL_GUIWindowManagerProvider_Direct::get_window_at_point(const CL_Point &point, const std::vector<CL_GUITopLevelWindowDirect *> &z_order)
{
	std::vector<CL_GUITopLevelWindowDirect *>::size_type index, size;
	size = z_order.size();
	for (index = 0; index < size; index++)
	{
		CL_GUITopLevelWindowDirect *toplevel_window = z_order[index];
		if (toplevel_window->visible)
		{
			// Check children first
			CL_GUITopLevelWindow *child_window = get_window_at_point(point, toplevel_window->child_windows_zorder);
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

void CL_GUIWindowManagerProvider_Direct::invoke_input_received(CL_GUITopLevelWindow *window, const CL_InputEvent &input_event, const CL_InputState &input_state)
{
	CL_GUITopLevelWindowDirect *gui_direct_window = get_direct_window(window);
	CL_InputEvent inp_event = input_event;
	inp_event.mouse_pos.x -= gui_direct_window->geometry.left;
	inp_event.mouse_pos.y -= gui_direct_window->geometry.top;
	site->func_input_received->invoke(window, inp_event, input_state);
}

CL_GUITopLevelWindowDirect *CL_GUIWindowManagerProvider_Direct::get_direct_window(CL_GUITopLevelWindow *handle)
{
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowDirect *>::iterator it;
	it = window_map.find(handle);
	if (it == window_map.end())
		throw CL_Exception("Invalid GUI Top Level Window requested");
	return it->second;
}


