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

#include "GUI/precomp.h"

#include "API/GUI/gui_window_manager_direct.h"
#include "gui_window_manager_provider_direct.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirect Construction:

CL_GUIWindowManagerDirect::CL_GUIWindowManagerDirect()
{
}

CL_GUIWindowManagerDirect::CL_GUIWindowManagerDirect(CL_DisplayWindow display_window) 
: CL_GUIWindowManager(new CL_GUIWindowManagerProvider_Direct(display_window))
{
}

CL_GUIWindowManagerDirect::CL_GUIWindowManagerDirect(const CL_GUIWindowManager &wm)
: CL_GUIWindowManager(wm)
{
	if (dynamic_cast<CL_GUIWindowManagerProvider_Direct *>(get_provider()) == 0)
		throw CL_Exception("CL_GUIWindowManager object is not a CL_GUIWindowManagerDirect!");
}

CL_GUIWindowManagerDirect::~CL_GUIWindowManagerDirect()
{
}
/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirect Attributes:
CL_GUIWindowManagerProvider_Direct *CL_GUIWindowManagerDirect::get_provider() const
{
	return static_cast <CL_GUIWindowManagerProvider_Direct *> (CL_GUIWindowManager::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirect Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerDirect Implementation:
