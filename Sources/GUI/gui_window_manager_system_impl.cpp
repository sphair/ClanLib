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

#include "GUI/precomp.h"
#include "gui_window_manager_system_impl.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem_Impl Construction:

CL_GUIWindowManagerSystem_Impl::CL_GUIWindowManagerSystem_Impl()
: site(0), capture_mouse_window(0), used_cached_windows(0)
{
}

CL_GUIWindowManagerSystem_Impl::~CL_GUIWindowManagerSystem_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem_Impl Operations:

void CL_GUIWindowManagerSystem_Impl::on_displaywindow_lost_focus(CL_GUITopLevelWindow *top_level_window)
{
	site->func_focus_lost->invoke(top_level_window);
}

void CL_GUIWindowManagerSystem_Impl::on_displaywindow_got_focus(CL_GUITopLevelWindow *top_level_window)
{
	site->func_focus_gained->invoke(top_level_window);
}

void CL_GUIWindowManagerSystem_Impl::on_displaywindow_resize(int width, int height, CL_GUITopLevelWindow *top_level_window)
{
	site->func_resize->invoke(top_level_window, CL_Size(width, height));
}

void CL_GUIWindowManagerSystem_Impl::on_displaywindow_paint(const CL_Rect &rect, CL_GUITopLevelWindow *top_level_window)
{
	site->func_paint->invoke(top_level_window, rect);
}

void CL_GUIWindowManagerSystem_Impl::on_displaywindow_window_close(CL_GUITopLevelWindow *top_level_window)
{
	site->func_close->invoke(top_level_window);
}

void CL_GUIWindowManagerSystem_Impl::on_input(const CL_InputEvent &incoming_input_event, const CL_InputState &input_state, CL_GUITopLevelWindow *top_level_window)
{
	CL_GUITopLevelWindowSystem *top_level_system = window_map[top_level_window];
	CL_InputEvent input_event = incoming_input_event;
	CL_GUITopLevelWindow *target = top_level_window;

	site->func_input_received->invoke(target, input_event, input_state);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerSystem_Impl Implementation:
