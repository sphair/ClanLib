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
*/



#pragma once

#include <map>
#include "API/Core/Signals/callback.h"
#include "API/Display/Window/display_window.h"
#include "API/GUI/Providers/gui_window_manager_provider.h"
#include "API/Display/2D/canvas.h"

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class GUITopLevelWindow;
class GUIWindowManagerSite;
class InputEvent;
class GUIComponent;

class GUITopLevelWindowSystem
{
public:
	Canvas canvas;
	DisplayWindow window;
};

class GUIWindowManagerProvider_System : public GUIWindowManagerProvider
{
/// \name Construction
/// \{

public:
	GUIWindowManagerProvider_System();

	~GUIWindowManagerProvider_System();


/// \}
/// \name Attributes
/// \{

public:
	GUIWindowManagerSite *site;

	GUITopLevelWindow *capture_mouse_window;

	std::map<GUITopLevelWindow *, GUITopLevelWindowSystem *> window_map;

#ifdef _WIN32
	std::map<GUITopLevelWindow *, HWND> cache_window_handles;
#endif

	std::vector<GUITopLevelWindowSystem*> cached_windows;

	int used_cached_windows;

	/// \brief Retrieves the provider.
	GUIWindowManagerProvider *get_provider();

	GUIWindowManager::WindowManagerType get_window_manager_type() const;

/// \}
/// \name Operations
/// \{

public:

	void on_displaywindow_lost_focus(GUITopLevelWindow *top_level_window);

	void on_displaywindow_got_focus(GUITopLevelWindow *top_level_window);

	void on_displaywindow_resize(int width, int height, GUITopLevelWindow *top_level_window);

	void on_displaywindow_paint(const Rect &rect, GUITopLevelWindow *top_level_window);

	void on_displaywindow_window_close(GUITopLevelWindow *top_level_window);

	void on_displaywindow_window_destroy(GUITopLevelWindow *top_level_window);

	void on_input(const InputEvent &event, GUITopLevelWindow *top_level_window);

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

	/// \}

/// \name Events
/// \{
public:
	Signal_v1<DisplayWindow> sig_toplevel_window_created;

	Signal_v1<DisplayWindow> sig_toplevel_window_destroyed;

/// \}

/// \name Implementation
/// \{

private:
	void maintain_window_cache(GUITopLevelWindow *top_level_window);

/// \}
};

}

/// \}
