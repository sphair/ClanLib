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
**    Mark Page
*/


#pragma once

#include "api_gui.h"
#include "gui_component.h"
#include "../Core/Signals/signal.h"
#include "../Core/Signals/signal.h"
#include "../Core/Signals/signal.h"
#include <vector>

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class Canvas;
class InputEvent;
class Cursor;
class DisplayWindow;
class GUITopLevelWindow;
class GUIWindowManagerProvider;
class GUIWindowManager_Impl;

/// \brief GUI window manager site.
class CL_API_DISPLAY GUIWindowManagerSite
{
/// \name Attributes
/// \{

public:
	/// \brief Pointer to focus lost notification callback function.
	Callback_v1<GUITopLevelWindow *> *func_focus_lost;

	/// \brief Pointer to focus gained notification callback function.
	Callback_v1<GUITopLevelWindow *> *func_focus_gained;

	/// \brief Pointer to focus gained notification callback function.
	Callback_v2<GUITopLevelWindow *, const Size &> *func_resize;

	/// \brief Pointer to paint notification callback function.
	Callback_v2<GUITopLevelWindow *, const Rect &> *func_paint;

	/// \brief Pointer to close notification callback function.
	Callback_v1<GUITopLevelWindow *> *func_close;

	/// \brief Pointer to destroy notification callback function.
	Callback_v1<GUITopLevelWindow *> *func_destroy;

	/// \brief Pointer to close notification callback function.
	Callback_v2<GUITopLevelWindow *, const InputEvent &> *func_input_received;
/// \}
};

/// \brief GUI window manager interface.
class CL_API_GUI GUIWindowManager
{
/// \name Enumerations
/// \{

public:
	enum WindowManagerType
	{
		cl_wm_type_system,
		cl_wm_type_texture
	};
/// \}

/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	GUIWindowManager();

	/// \brief Constructs a window manager
	///
	/// \param provider = The theme provider
	GUIWindowManager(GUIWindowManagerProvider *provider);

	/// \brief Duplicates the reference to a window manager
	GUIWindowManager(const GUIWindowManager &src);

	~GUIWindowManager();

/// \}

/// \name Attributes
/// \{
public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Retrieves the provider.
	GUIWindowManagerProvider *get_provider() const;

	/// \brief Get Window manager type
	///
	/// \return window_manager_type
	WindowManagerType get_window_manager_type() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set site
	///
	/// \param site = GUIWindow Manager Site
	void set_site(GUIWindowManagerSite *site);
	void create_window(
		GUITopLevelWindow *handle,
		GUITopLevelWindow *owner,
		GUIComponent *component,
		GUITopLevelDescription description);

	/// \brief Destroy window
	///
	/// \param handle = GUITop Level Window
	void destroy_window(GUITopLevelWindow *handle);

	/// \brief Enable window
	///
	/// \param handle = GUITop Level Window
	/// \param enable = bool
	void enable_window(GUITopLevelWindow *handle, bool enable);

	/// \brief Has focus
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	bool has_focus(GUITopLevelWindow *handle) const;

	/// \brief Set visible
	///
	/// \param handle = GUITop Level Window
	/// \param visible = bool
	/// \param activate_root_win = bool
	void set_visible(GUITopLevelWindow *handle, bool visible, bool activate_root_win);

	/// \brief Set geometry
	///
	/// \param handle = GUITop Level Window
	/// \param geometry = Rect
	/// \param client_area = bool
	void set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area);

	/// \brief Get geometry
	///
	/// \param handle = GUITop Level Window
	/// \param client_area = bool
	///
	/// \return Rect
	Rect get_geometry(GUITopLevelWindow *handle, bool client_area) const;

	/// \brief Screen to window
	///
	/// \param handle = GUITop Level Window
	/// \param screen_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	Point screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const;

	/// \brief Window to screen
	///
	/// \param handle = GUITop Level Window
	/// \param window_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	Point window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const;

	/// \brief Get canvas
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Canvas
	Canvas& get_canvas(GUITopLevelWindow *handle) const;

	/// \brief Get ic
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Input Context
	InputContext get_ic(GUITopLevelWindow *handle) const;

	/// \brief Begin paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	///
	/// \return Canvas
	Canvas begin_paint(GUITopLevelWindow *handle, const Rect &update_region);

	/// \brief Set cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	/// \param rect = Rect
	void set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect);

	/// \brief Reset cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	void reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas);

	/// \brief Push cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	/// \param rect = Clip rect
	void push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect);

	/// \brief Pop cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	void pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas);

	/// \brief End paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	void end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region);

	/// \brief Request repaint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	void request_repaint(GUITopLevelWindow *handle, const Rect &update_region);

	/// \brief Bring to front
	///
	/// \param handle = GUITop Level Window
	void bring_to_front(GUITopLevelWindow *handle);

	/// \brief Is minimized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	bool is_minimized(GUITopLevelWindow *handle) const;

	/// \brief Is maximized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	bool is_maximized(GUITopLevelWindow *handle) const;

	/// \brief Process message
	void process_message();

	/// \brief Capture mouse
	///
	/// \param handle = GUITop Level Window
	/// \param state = bool
	void capture_mouse(GUITopLevelWindow *handle, bool state);

	/// \brief Get display window
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Display Window
	DisplayWindow get_display_window(GUITopLevelWindow *handle) const;

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param cursor = Cursor
	void set_cursor(GUITopLevelWindow *handle, const Cursor &cursor);

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param type = StandardCursor
	void set_cursor(GUITopLevelWindow *handle, enum StandardCursor type);

	/// \brief Perform any updates.
	///
	/// This is called by GUIManager::exec(), after all messages has been processed
	void update();

	/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<GUIWindowManager_Impl> impl;
/// \}
};

}

/// \}
