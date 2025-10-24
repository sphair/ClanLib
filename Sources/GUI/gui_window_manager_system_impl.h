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


#include <map>
#include "API/Core/Signals/slot_container.h"
#include "API/Core/Signals/callback_v0.h"
#include "API/Display/Window/display_window.h"

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

class CL_GUIWindowManagerSystem_Impl
{
/// \name Construction
/// \{

public:
	CL_GUIWindowManagerSystem_Impl();

	~CL_GUIWindowManagerSystem_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_GUIWindowManagerSite *site;

	CL_GUITopLevelWindow *capture_mouse_window;

	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowSystem *> window_map;

	std::vector<CL_GUITopLevelWindowSystem*> cached_windows;

	int used_cached_windows;

	CL_Callback_v0 func_wait_for_message;


/// \}
/// \name Operations
/// \{

public:
	void on_displaywindow_lost_focus(CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_got_focus(CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_resize(int width, int height, CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_paint(const CL_Rect &rect, CL_GUITopLevelWindow *top_level_window);

	void on_displaywindow_window_close(CL_GUITopLevelWindow *top_level_window);

	void on_input(const CL_InputEvent &event, const CL_InputState &input_state, CL_GUITopLevelWindow *top_level_window);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
