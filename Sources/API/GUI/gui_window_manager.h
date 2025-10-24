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
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "gui_component.h"
#include "../Core/Signals/signal_v1.h"
#include "../Core/Signals/signal_v2.h"
#include "../Core/Signals/signal_v3.h"
#include <vector>

class CL_GraphicContext;
class CL_InputEvent;
class CL_InputState;
class CL_Cursor;
class CL_DisplayWindow;
class CL_GUITopLevelWindow;
class CL_GUIWindowManagerProvider;
class CL_GUIWindowManager_Impl;

/// \brief GUI window manager site.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_DISPLAY CL_GUIWindowManagerSite
{
/// \name Attributes
/// \{

public:
	/// \brief Pointer to focus lost notification callback function.
	CL_Callback_v1<CL_GUITopLevelWindow *> *func_focus_lost;

	/// \brief Pointer to focus gained notification callback function.
	CL_Callback_v1<CL_GUITopLevelWindow *> *func_focus_gained;

	/// \brief Pointer to focus gained notification callback function.
	CL_Callback_v2<CL_GUITopLevelWindow *, const CL_Size &> *func_resize;

	/// \brief Pointer to paint notification callback function.
	CL_Callback_v2<CL_GUITopLevelWindow *, const CL_Rect &> *func_paint;

	/// \brief Pointer to close notification callback function.
	CL_Callback_v1<CL_GUITopLevelWindow *> *func_close;

	/// \brief Pointer to destroy notification callback function.
	CL_Callback_v1<CL_GUITopLevelWindow *> *func_destroy;

	/// \brief Pointer to close notification callback function.
	CL_Callback_v3<CL_GUITopLevelWindow *, const CL_InputEvent &, const CL_InputState &> *func_input_received;
/// \}
};

/// \brief GUI window manager interface.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIWindowManager
{
/// \name Enumerations
/// \{

public:
	enum CL_WindowManagerType
	{
		cl_wm_type_system,
		cl_wm_type_texture
	};
/// \}

/// \name Construction
/// \{
public:
	/// \brief Constructs a window manager
	///
	/// \param provider = The theme provider
	CL_GUIWindowManager(CL_GUIWindowManagerProvider *provider);

	~CL_GUIWindowManager();

/// \}

/// \name Attributes
/// \{
public:

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null();

	/// \brief Retrieves the provider.
	CL_GUIWindowManagerProvider *get_provider() const;

	/// \brief Get Window manager type
	///
	/// \return window_manager_type
	CL_WindowManagerType get_window_manager_type() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Set site
	///
	/// \param site = GUIWindow Manager Site
	void set_site(CL_GUIWindowManagerSite *site);
	void create_window(
		CL_GUITopLevelWindow *handle,
		CL_GUITopLevelWindow *owner,
		CL_GUITopLevelDescription description);

	/// \brief Destroy window
	///
	/// \param handle = GUITop Level Window
	void destroy_window(CL_GUITopLevelWindow *handle);

	/// \brief Enable window
	///
	/// \param handle = GUITop Level Window
	/// \param enable = bool
	void enable_window(CL_GUITopLevelWindow *handle, bool enable);

	/// \brief Has focus
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	bool has_focus(CL_GUITopLevelWindow *handle) const;

	/// \brief Set visible
	///
	/// \param handle = GUITop Level Window
	/// \param visible = bool
	/// \param activate_root_win = bool
	void set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate_root_win);

	/// \brief Set geometry
	///
	/// \param handle = GUITop Level Window
	/// \param geometry = Rect
	/// \param client_area = bool
	void set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area);

	/// \brief Get geometry
	///
	/// \param handle = GUITop Level Window
	/// \param client_area = bool
	///
	/// \return Rect
	CL_Rect get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const;

	/// \brief Screen to window
	///
	/// \param handle = GUITop Level Window
	/// \param screen_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	CL_Point screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const;

	/// \brief Window to screen
	///
	/// \param handle = GUITop Level Window
	/// \param window_point = Point
	/// \param client_area = bool
	///
	/// \return Point
	CL_Point window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const;

	/// \brief Get gc
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Graphic Context
	CL_GraphicContext& get_gc(CL_GUITopLevelWindow *handle) const;

	/// \brief Get ic
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Input Context
	CL_InputContext& get_ic(CL_GUITopLevelWindow *handle) const;

	/// \brief Begin paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	///
	/// \return Graphic Context
	CL_GraphicContext begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);

	/// \brief Set cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	/// \param rect = Rect
	void set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);

	/// \brief Reset cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	void reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);

	/// \brief Push cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	/// \param rect = Clip rect
	void push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);

	/// \brief Pop cliprect
	///
	/// \param handle = GUITop Level Window
	/// \param gc = Graphic Context
	void pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);

	/// \brief End paint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	void end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);

	/// \brief Request repaint
	///
	/// \param handle = GUITop Level Window
	/// \param update_region = Rect
	void request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);

	/// \brief Bring to front
	///
	/// \param handle = GUITop Level Window
	void bring_to_front(CL_GUITopLevelWindow *handle);

	/// \brief Is minimized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	bool is_minimized(CL_GUITopLevelWindow *handle) const;

	/// \brief Is maximized
	///
	/// \param handle = GUITop Level Window
	///
	/// \return bool
	bool is_maximized(CL_GUITopLevelWindow *handle) const;

	/// \brief Process message
	void process_message();

	/// \brief Capture mouse
	///
	/// \param handle = GUITop Level Window
	/// \param state = bool
	void capture_mouse(CL_GUITopLevelWindow *handle, bool state);

	/// \brief Get display window
	///
	/// \param handle = GUITop Level Window
	///
	/// \return Display Window
	CL_DisplayWindow get_display_window(CL_GUITopLevelWindow *handle) const;

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param cursor = Cursor
	void set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor);

	/// \brief Set cursor
	///
	/// \param handle = GUITop Level Window
	/// \param type = CL_StandardCursor
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
	CL_SharedPtr<CL_GUIWindowManager_Impl> impl;
/// \}
};

/// \}
