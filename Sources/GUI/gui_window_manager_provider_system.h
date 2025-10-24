/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/// \addtogroup clanGUI_System clanGUI System
/// \{


#pragma once


#include <map>
#include "API/Core/Signals/slot_container.h"
#include "API/Core/Signals/callback_v0.h"
#include "API/Display/Window/display_window.h"
#include "API/GUI/Providers/gui_window_manager_provider.h"

class CL_GUITopLevelWindow;
class CL_GUIWindowManagerSite;
class CL_InputEvent;
class CL_InputState;
class CL_GUIComponent;

class CL_GUITopLevelWindowSystem
{
public:
	CL_DisplayWindow window;
	CL_SlotContainer slots;
};

class CL_GUIWindowManagerProvider_System : public CL_GUIWindowManagerProvider
{
/// \name Construction
/// \{

public:
	CL_GUIWindowManagerProvider_System();

	~CL_GUIWindowManagerProvider_System();


/// \}
/// \name Attributes
/// \{

public:
	CL_GUIWindowManagerSite *site;

	CL_GUITopLevelWindow *capture_mouse_window;

	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *> window_map;

#ifdef _WIN32
	std::map<CL_GUITopLevelWindow *, HWND> cache_window_handles;
#endif

	std::vector<CL_GUITopLevelWindowSystem*> cached_windows;

	int used_cached_windows;

	/// \brief Retrieves the provider.
	CL_GUIWindowManagerProvider *get_provider();

	CL_GUIWindowManager::CL_WindowManagerType get_window_manager_type() const;

/// \}
/// \name Operations
/// \{

public:

	void destroy();

	void on_displaywindow_lost_focus(CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_got_focus(CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_resize(int width, int height, CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_paint(const CL_Rect &rect, CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_window_close(CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_window_destroy(CL_GUITopLevelWindow *top_level_window);

	void on_input(const CL_InputEvent &event, const CL_InputState &input_state, CL_GUITopLevelWindow *top_level_window);

	void set_site(CL_GUIWindowManagerSite *site);
	void create_window(
		CL_GUITopLevelWindow *handle,
		CL_GUITopLevelWindow *owner,
		CL_GUITopLevelDescription description);

	void destroy_window(CL_GUITopLevelWindow *handle);
	void enable_window(CL_GUITopLevelWindow *handle, bool enable);
	bool has_focus(CL_GUITopLevelWindow *handle) const;
	void set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate_root_win);
	void set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area);
	CL_Rect get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const;
	CL_Point screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const;
	CL_Point window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const;
	CL_GraphicContext& get_gc(CL_GUITopLevelWindow *handle) const;
	CL_InputContext& get_ic(CL_GUITopLevelWindow *handle) const;
	CL_GraphicContext begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);
	void reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);
	void push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);
	void pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);
	void end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void bring_to_front(CL_GUITopLevelWindow *handle);
	bool is_minimized(CL_GUITopLevelWindow *handle) const;
	bool is_maximized(CL_GUITopLevelWindow *handle) const;
	void capture_mouse(CL_GUITopLevelWindow *handle, bool state);
	CL_DisplayWindow get_display_window(CL_GUITopLevelWindow *handle) const;
	void set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor);
	void set_cursor(CL_GUITopLevelWindow *handle, enum CL_StandardCursor type);

	/// \brief Perform any updates.
	///
	/// This is called by CL_GUIManager::exec(), after all messages has been processed
	void update();

	/// \brief Initial setup for painting all top level windows
	void setup_painting();

	/// \brief Final completion for painting all top level windows
	void complete_painting();

	/// \}
/// \name Implementation
/// \{

private:
	void maintain_window_cache(CL_GUITopLevelWindow *top_level_window);

/// \}
};


/// \}
