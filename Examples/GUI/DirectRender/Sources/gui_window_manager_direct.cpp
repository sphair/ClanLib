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

#include "precomp.h"

#include "gui_window_manager_direct.h"
#include "gui_window_manager_provider_direct.h"
#include "gui_window_manager_direct_window.h"

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerDirect Construction:

GUIWindowManagerDirect::GUIWindowManagerDirect()
{
}

GUIWindowManagerDirect::GUIWindowManagerDirect(CL_DisplayWindow display_window) 
: CL_GUIWindowManager(new GUIWindowManagerProvider_Direct(display_window))
{
}

GUIWindowManagerDirect::GUIWindowManagerDirect(const CL_GUIWindowManager &wm)
: CL_GUIWindowManager(wm)
{
	if (dynamic_cast<GUIWindowManagerProvider_Direct *>(get_provider()) == 0)
		throw CL_Exception("CL_GUIWindowManager object is not a GUIWindowManagerDirect!");
}

GUIWindowManagerDirect::~GUIWindowManagerDirect()
{
}
/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerDirect Attributes:
GUIWindowManagerProvider_Direct *GUIWindowManagerDirect::get_provider() const
{
	return static_cast <GUIWindowManagerProvider_Direct *> (CL_GUIWindowManager::get_provider());
}

std::vector<GUIWindowManagerDirectWindow> GUIWindowManagerDirect::get_windows(bool only_visible) const
{
	return get_provider()->get_windows(only_visible);
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerDirect Events:

CL_Callback_v2<CL_InputEvent &, CL_InputState &> &GUIWindowManagerDirect::func_input_intercept()
{
	return get_provider()->func_input_intercept;
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerDirect Operations:

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerDirect Implementation:
