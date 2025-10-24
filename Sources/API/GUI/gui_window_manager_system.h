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
#include "gui_window_manager.h"
#include "../Core/System/sharedptr.h"

class CL_GUIWindowManagerSystem_Impl;

/// \brief System (OS) window manager.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIWindowManagerSystem : public CL_GUIWindowManager
{
/// \name Construction
/// \{

public:
	CL_GUIWindowManagerSystem();

	~CL_GUIWindowManagerSystem();


/// \}
/// \name Attributes
/// \{

public:
	CL_WindowManagerType get_window_manager_type() const;

	/// \brief void func_wait_for_message()
	CL_Callback_v0 &func_wait_for_message();


/// \}
/// \name Operations
/// \{

public:
	void set_site(CL_GUIWindowManagerSite *site);

	void create_window(
		CL_GUITopLevelWindow *handle,
		CL_GUITopLevelWindow *owner,
		CL_GUITopLevelDescription description,
		bool temporary);

	void destroy_window(CL_GUITopLevelWindow *handle);

	void enable_window(CL_GUITopLevelWindow *handle, bool enable);

	bool has_focus(CL_GUITopLevelWindow *handle) const;

	void set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate);

	void set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area);

	CL_Rect get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const;

	CL_Point screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const;

	CL_Point window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const;

	CL_GraphicContext get_gc(CL_GUITopLevelWindow *handle);

	CL_InputContext get_ic(CL_GUITopLevelWindow *handle);

	CL_GraphicContext begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);

	void end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);

	void invalidate_rect(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);

	CL_Timer create_timer(CL_GUITopLevelWindow *handle);

	void capture_mouse(CL_GUITopLevelWindow *handle, bool state);

	CL_DisplayWindow get_display_window(CL_GUITopLevelWindow *handle) const;

	void set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor);

	void set_cursor(CL_GUITopLevelWindow *handle, enum CL_StandardCursor type);

	void bring_to_front(CL_GUITopLevelWindow *handle);

	bool is_minimized(CL_GUITopLevelWindow *handle);

	bool is_maximized(CL_GUITopLevelWindow *handle);

	bool has_message();

	void process_message();

	void wait_for_message();

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
	CL_SharedPtr<CL_GUIWindowManagerSystem_Impl> impl;
/// \}
};


/// \}
