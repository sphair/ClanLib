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

#include "../gui_window_manager.h"

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class GUIWindowManager;
class Canvas;
class InputEvent;
class Cursor;
class DisplayWindow;
class GUITopLevelWindow;

/// \brief GUI window manager interface.
class GUIWindowManagerProvider
{
/// \name Construction
/// \{
public:
	virtual ~GUIWindowManagerProvider() { }
/// \}

/// \name Attributes
/// \{
public:
	virtual GUIWindowManager::WindowManagerType get_window_manager_type() const = 0;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Set site
	///
	/// \param site = GUIWindow Manager Site
	virtual void set_site(GUIWindowManagerSite *site) = 0;
	virtual void create_window(
		GUITopLevelWindow *handle,
		GUITopLevelWindow *owner,
		GUIComponent *component,
		GUITopLevelDescription description) = 0;

	/// \brief Destroy window
	///
	/// \param handle = GUITop Level Window
	virtual void destroy_window(GUITopLevelWindow *handle) = 0;

	/// \brief Enable window
	///
	/// \param handle = GUITop Level Window
	/// \param enable = bool
	virtual void enable_window(GUITopLevelWindow *handle, bool enable) = 0;

	/// \brief Has focus
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	virtual bool has_focus(GUITopLevelWindow *handle) const = 0;

	/// \brief Set visible
	///
	/// \param handle = GUITop Level Window
	/// \param visible = bool
	/// \param activate_root_win = bool
	virtual void set_visible(GUITopLevelWindow *handle, bool visible, bool activate_root_win) = 0;

	/// \brief Set geometry
	///
	/// \param handle = GUITop Level Window
	/// \param geometry = Rect
	/// \param client_area = bool
	virtual void set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area) = 0;

	/// \brief Get geometry
	///
	/// \param handle = GUITop Level Window
	/// \param client_area = bool
	///
	/// \return Rect
	virtual Rect get_geometry(GUITopLevelWindow *handle, bool client_area) const = 0;

	/// \brief Screen to window
	///
	/// \param handle = GUITop Level Window
	/// \param screen_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	virtual Point screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const = 0;

	/// \brief Window to screen
	///
	/// \param handle = GUITop Level Window
	/// \param window_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	virtual Point window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const = 0;

	/// \brief Get canvas
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Canvas
	virtual Canvas& get_canvas(GUITopLevelWindow *handle) = 0;

	/// \brief Get ic
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Input Context
	virtual InputContext get_ic(GUITopLevelWindow *handle) const = 0;

	/// \brief Begin paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	///
	/// \return Canvas
	virtual Canvas begin_paint(GUITopLevelWindow *handle, const Rect &update_region) = 0;

	/// \brief Set cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	/// \param rect = Rect
	virtual void set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect) = 0;

	/// \brief Reset cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	virtual void reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas) = 0;

	/// \brief Push cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	/// \param rect = Clip rect
	virtual void push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect) = 0;

	/// \brief Push cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param canvas = Canvas
	virtual void pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas) = 0;

	/// \brief End paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	virtual void end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region) = 0;

	/// \brief Request repaint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	virtual void request_repaint(GUITopLevelWindow *handle, const Rect &update_region) = 0;

	/// \brief Bring to front
	///
	/// \param handle = GUITop Level Window
	virtual void bring_to_front(GUITopLevelWindow *handle) = 0;

	/// \brief Is minimized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	virtual bool is_minimized(GUITopLevelWindow *handle) const = 0;

	/// \brief Is maximized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	virtual bool is_maximized(GUITopLevelWindow *handle) const = 0;

	/// \brief Capture mouse
	///
	/// \param handle = GUITop Level Window
	/// \param state = bool
	virtual void capture_mouse(GUITopLevelWindow *handle, bool state) = 0;

	/// \brief Get display window
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Display Window
	virtual DisplayWindow get_display_window(GUITopLevelWindow *handle) const = 0;

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param cursor = Cursor
	virtual void set_cursor(GUITopLevelWindow *handle, const Cursor &cursor) = 0;

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param type = StandardCursor
	virtual void set_cursor(GUITopLevelWindow *handle, enum StandardCursor type) = 0;

	/// \brief Perform any updates.
	///
	/// This is called by GUIManager::exec(), after all messages has been processed
	virtual void update() = 0;

/// \}

/// \name Implementation
/// \{
private:
/// \}

};

}

/// \}
