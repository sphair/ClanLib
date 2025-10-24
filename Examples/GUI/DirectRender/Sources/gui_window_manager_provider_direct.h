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

#pragma once

#include <map>

class GUIWindowManagerDirectWindow;

class GUITopLevelWindowDirect
{
public:
	GUITopLevelWindowDirect(CL_GUITopLevelWindow *window) : window(window), enabled(true), visible(true), dirty(true), owner_window(NULL) { }

	CL_GUITopLevelWindow *window;	// The window that this texture belongs to
	CL_Rect geometry;
	bool enabled;
	bool visible;
	bool dirty;

	GUITopLevelWindowDirect *owner_window;
	std::vector<GUITopLevelWindowDirect *> child_windows_zorder;	// Beginning is at the top

	std::vector<CL_Rect> update_region_list;		// Only valid when "dirty" is set to true
};

class GUIWindowManagerProvider_Direct : public CL_GUIWindowManagerProvider
{

public:
	GUIWindowManagerProvider_Direct(CL_DisplayWindow &display_window);
	~GUIWindowManagerProvider_Direct();

public:

	std::vector<GUIWindowManagerDirectWindow> get_windows(bool only_visible = true) const;

	/// \brief Retrieves the provider.
	CL_GUIWindowManagerProvider *get_provider();

	CL_GUIWindowManager::CL_WindowManagerType get_window_manager_type() const;

	CL_GUIWindowManagerSite *site;
	std::map<CL_GUITopLevelWindow *, GUITopLevelWindowDirect *> window_map;
	std::vector<GUITopLevelWindowDirect *> root_window_z_order;	// Beginning is at the top
	CL_Callback_v2<CL_InputEvent &, CL_InputState &> func_input_intercept;
	CL_GUITopLevelWindow *activated_window;
	CL_GUITopLevelWindow *capture_mouse_window;
	CL_DisplayWindow display_window;
	CL_SlotContainer slots;
	bool setup_painting_called; // True when setup_painting() was called in GUIWindowManagerDirect
	bool painting_set;	// True when the frame buffer is active (used by setup_painting() )

public:

	void destroy();

	void on_displaywindow_window_close();

	void on_input(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_up(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_down(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_move(const CL_InputEvent &event, const CL_InputState &input_state);

	void update_paint();

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
	void finalise_painting();

	/// \brief Final completion for painting all top level windows
	void complete_painting();

private:
	CL_GUITopLevelWindow *get_window_at_point(const CL_Point &point);
	CL_GUITopLevelWindow *get_window_at_point(const CL_Point &point, const std::vector<GUITopLevelWindowDirect *> &z_order);

	GUITopLevelWindowDirect *get_window_texture(CL_GUITopLevelWindow *handle);
	void get_all_windows_zorder(bool only_visible, std::vector<GUIWindowManagerDirectWindow> &windows_dest_list, const std::vector<GUITopLevelWindowDirect *> &z_order) const;
	void invoke_input_received(CL_GUITopLevelWindow *window, const CL_InputEvent &input_event, const CL_InputState &input_state);
	bool is_constant_repaint_enabled(CL_GUIComponent *component) const;

};


