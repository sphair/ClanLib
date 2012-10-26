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

#pragma once

#include <map>
#include "API/Core/Signals/slot_container.h"
#include "API/Core/Signals/callback_v0.h"
#include "API/Display/Window/display_window.h"
#include "API/GUI/Providers/gui_window_manager_provider.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

class GUIWindowManagerDirectWindow;

class GUITopLevelWindowDirect
{
public:
	GUITopLevelWindowDirect(GUITopLevelWindow *window) : window(window), enabled(true), visible(true), owner_window(NULL) { }

	GUITopLevelWindow *window;	// The window that this texture belongs to
	Rect geometry;
	bool enabled;
	bool visible;

	GUITopLevelWindowDirect *owner_window;
	std::vector<GUITopLevelWindowDirect *> child_windows_zorder;	// Beginning is at the top
};

class GUIWindowManagerProvider_Direct : public GUIWindowManagerProvider
{

public:
	GUIWindowManagerProvider_Direct(DisplayWindow &display_window, Canvas &canvas);
	~GUIWindowManagerProvider_Direct();

public:

	/// \brief Retrieves the provider.
	GUIWindowManagerProvider *get_provider();

	GUIWindowManager::WindowManagerType get_window_manager_type() const;

	GUIWindowManagerSite *site;
	std::map<GUITopLevelWindow *, GUITopLevelWindowDirect *> window_map;
	std::vector<GUITopLevelWindowDirect *> root_window_z_order;	// Beginning is at the top
	GUITopLevelWindow *activated_window;
	GUITopLevelWindow *capture_mouse_window;
	DisplayWindow display_window;
	Canvas window_canvas;
	SlotContainer slots;

public:
	void on_displaywindow_window_close();

	void on_input(const InputEvent &event);
	void on_input_mouse_up(const InputEvent &event);
	void on_input_mouse_down(const InputEvent &event);
	void on_input_mouse_move(const InputEvent &event);
	
	void set_site(GUIWindowManagerSite *site);
	void create_window(
		GUITopLevelWindow *handle,
		GUITopLevelWindow *owner,
		GUIComponent *component,
		GUITopLevelDescription description);

	void destroy_window(GUITopLevelWindow *handle);
	void enable_window(GUITopLevelWindow *handle, bool enable);
	bool has_focus(GUITopLevelWindow *handle) const;
	void set_visible(GUITopLevelWindow *handle, bool visible, bool activate_root_win);
	void set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area);
	Rect get_geometry(GUITopLevelWindow *handle, bool client_area) const;
	Point screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const;
	Point window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const;
	Canvas& get_canvas(GUITopLevelWindow *handle);
	InputContext get_ic(GUITopLevelWindow *handle) const;
	Canvas begin_paint(GUITopLevelWindow *handle, const Rect &update_region);
	void set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect);
	void reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas);
	void push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect);
	void pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas);
	void end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region);
	void request_repaint(GUITopLevelWindow *handle, const Rect &update_region);
	void bring_to_front(GUITopLevelWindow *handle);
	bool is_minimized(GUITopLevelWindow *handle) const;
	bool is_maximized(GUITopLevelWindow *handle) const;
	void capture_mouse(GUITopLevelWindow *handle, bool state);
	DisplayWindow get_display_window(GUITopLevelWindow *handle) const;
	void set_cursor(GUITopLevelWindow *handle, const Cursor &cursor);
	void set_cursor(GUITopLevelWindow *handle, enum StandardCursor type);

	/// \brief Perform any updates.
	///
	/// This is called by GUIManager::exec(), after all messages has been processed
	void update();

	/// \brief Final completion for painting all top level windows
	void finalise_painting();

private:
	GUITopLevelWindow *get_window_at_point(const Point &point);
	GUITopLevelWindow *get_window_at_point(const Point &point, const std::vector<GUITopLevelWindowDirect *> &z_order);

	GUITopLevelWindowDirect *get_direct_window(GUITopLevelWindow *handle);
	void invoke_input_received(GUITopLevelWindow *window, const InputEvent &input_event);
	bool is_constant_repaint_enabled(GUIComponent *component) const;

};

}
