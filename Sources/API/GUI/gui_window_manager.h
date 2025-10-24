/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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


#include "api_gui.h"
#include "gui_component.h"
#include "../Core/Signals/signal_v1.h"
#include "../Core/Signals/signal_v2.h"
#include "../Core/Signals/signal_v3.h"
#include <vector>

class CL_GraphicContext;
class CL_Timer;
class CL_InputEvent;
class CL_InputState;
class CL_Cursor;
class CL_DisplayWindow;
class CL_GUITopLevelWindow;

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
	virtual ~CL_GUIWindowManager() { }


/// \}
/// \name Attributes
/// \{

public:
	virtual CL_WindowManagerType get_window_manager_type() const = 0;


/// \}
/// \name Operations
/// \{

public:
	virtual void set_site(CL_GUIWindowManagerSite *site) = 0;

	virtual void create_window(
		CL_GUITopLevelWindow *handle,
		CL_GUITopLevelWindow *owner,
		CL_GUITopLevelDescription description,
		bool temporary) = 0;

	virtual void destroy_window(CL_GUITopLevelWindow *handle) = 0;

	virtual void enable_window(CL_GUITopLevelWindow *handle, bool enable) = 0;

	virtual bool has_focus(CL_GUITopLevelWindow *handle) const = 0;

	virtual void set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate_root_win) = 0;

	virtual void set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area) = 0;

	virtual CL_Rect get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const = 0;

	virtual CL_Point screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const = 0;

	virtual CL_Point window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const = 0;

	virtual CL_GraphicContext get_gc(CL_GUITopLevelWindow *handle) = 0;

	virtual CL_InputContext get_ic(CL_GUITopLevelWindow *handle) = 0;

	virtual CL_GraphicContext begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region) = 0;

	virtual void end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region) = 0;

	virtual void invalidate_rect(CL_GUITopLevelWindow *handle, const CL_Rect &update_region) = 0;

	virtual void bring_to_front(CL_GUITopLevelWindow *handle) = 0;

	virtual bool is_minimized(CL_GUITopLevelWindow *handle) = 0;

	virtual bool is_maximized(CL_GUITopLevelWindow *handle) = 0;

	virtual bool has_message() = 0;

	virtual void process_message() = 0;

	virtual void wait_for_message() = 0;

	virtual CL_Timer create_timer(CL_GUITopLevelWindow *handle) = 0;

	virtual void capture_mouse(CL_GUITopLevelWindow *handle, bool state) = 0;

	virtual CL_DisplayWindow get_display_window(CL_GUITopLevelWindow *handle) const = 0;

	virtual void set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor) = 0;

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
