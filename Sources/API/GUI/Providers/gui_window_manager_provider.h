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
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_window_manager.h"

class CL_GUIWindowManager;
class CL_GraphicContext;
class CL_InputEvent;
class CL_InputState;
class CL_Cursor;
class CL_DisplayWindow;
class CL_GUITopLevelWindow;

/// \brief GUI window manager interface.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIWindowManagerProvider
{
/// \name Construction
/// \{
public:
	virtual ~CL_GUIWindowManagerProvider() { }
/// \}

/// \name Attributes
/// \{
public:
	virtual CL_GUIWindowManager::CL_WindowManagerType get_window_manager_type() const = 0;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Destroys the provider.
	virtual void destroy() = 0;

	/// \brief Set site
	///
	/// \param site = GUIWindow Manager Site
	virtual void set_site(CL_GUIWindowManagerSite *site) = 0;
	virtual void create_window(
		CL_GUITopLevelWindow *handle,
		CL_GUITopLevelWindow *owner,
		CL_GUIComponent *component,
		CL_GUITopLevelDescription description) = 0;

	/// \brief Destroy window
	///
	/// \param handle = GUITop Level Window
	virtual void destroy_window(CL_GUITopLevelWindow *handle) = 0;

	/// \brief Enable window
	///
	/// \param handle = GUITop Level Window
	/// \param enable = bool
	virtual void enable_window(CL_GUITopLevelWindow *handle, bool enable) = 0;

	/// \brief Has focus
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	virtual bool has_focus(CL_GUITopLevelWindow *handle) const = 0;

	/// \brief Set visible
	///
	/// \param handle = GUITop Level Window
	/// \param visible = bool
	/// \param activate_root_win = bool
	virtual void set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate_root_win) = 0;

	/// \brief Set geometry
	///
	/// \param handle = GUITop Level Window
	/// \param geometry = Rect
	/// \param client_area = bool
	virtual void set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area) = 0;

	/// \brief Get geometry
	///
	/// \param handle = GUITop Level Window
	/// \param client_area = bool
	///
	/// \return Rect
	virtual CL_Rect get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const = 0;

	/// \brief Screen to window
	///
	/// \param handle = GUITop Level Window
	/// \param screen_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	virtual CL_Point screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const = 0;

	/// \brief Window to screen
	///
	/// \param handle = GUITop Level Window
	/// \param window_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	virtual CL_Point window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const = 0;

	/// \brief Get gc
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Graphic Context
	virtual CL_GraphicContext& get_gc(CL_GUITopLevelWindow *handle) const = 0;

	/// \brief Get ic
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Input Context
	virtual CL_InputContext& get_ic(CL_GUITopLevelWindow *handle) const = 0;

	/// \brief Begin paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	///
	/// \return Graphic Context
	virtual CL_GraphicContext begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region) = 0;

	/// \brief Set cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	/// \param rect = Rect
	virtual void set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect) = 0;

	/// \brief Reset cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	virtual void reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc) = 0;

	/// \brief Push cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	/// \param rect = Clip rect
	virtual void push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect) = 0;

	/// \brief Push cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	virtual void pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc) = 0;

	/// \brief End paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	virtual void end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region) = 0;

	/// \brief Request repaint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	virtual void request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region) = 0;

	/// \brief Bring to front
	///
	/// \param handle = GUITop Level Window
	virtual void bring_to_front(CL_GUITopLevelWindow *handle) = 0;

	/// \brief Is minimized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	virtual bool is_minimized(CL_GUITopLevelWindow *handle) const = 0;

	/// \brief Is maximized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	virtual bool is_maximized(CL_GUITopLevelWindow *handle) const = 0;

	/// \brief Capture mouse
	///
	/// \param handle = GUITop Level Window
	/// \param state = bool
	virtual void capture_mouse(CL_GUITopLevelWindow *handle, bool state) = 0;

	/// \brief Get display window
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Display Window
	virtual CL_DisplayWindow get_display_window(CL_GUITopLevelWindow *handle) const = 0;

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param cursor = Cursor
	virtual void set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor) = 0;

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param type = CL_StandardCursor
	virtual void set_cursor(CL_GUITopLevelWindow *handle, enum CL_StandardCursor type) = 0;

	/// \brief Perform any updates.
	///
	/// This is called by CL_GUIManager::exec(), after all messages has been processed
	virtual void update() = 0;

	/// \brief Initial setup for painting all top level windows
	virtual void setup_painting() = 0;

	/// \brief Final completion for painting all top level windows
	virtual void complete_painting() = 0;
/// \}

/// \name Implementation
/// \{
private:
/// \}

};

/// \}
