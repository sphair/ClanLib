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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_window_manager_system.h"
#include "gui_window_manager_provider_system.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerSystem Construction:

GUIWindowManagerSystem::GUIWindowManagerSystem() : GUIWindowManager( new GUIWindowManagerProvider_System())
{
}

GUIWindowManagerSystem::~GUIWindowManagerSystem()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerSystem Attributes:

GUIWindowManagerProvider_System *GUIWindowManagerSystem::get_provider() const
{
	return static_cast <GUIWindowManagerProvider_System *> (GUIWindowManager::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerSystem Operations:


/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerSystem Events:

Signal<DisplayWindow> & GUIWindowManagerSystem::sig_toplevel_window_created()
{
	return get_provider()->sig_toplevel_window_created;
}

Signal<DisplayWindow> & GUIWindowManagerSystem::sig_toplevel_window_destroyed()
{
	return get_provider()->sig_toplevel_window_destroyed;
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerSystem Implementation:

}
