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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_window_manager_texture_window.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/subtexture.h"
#include "API/Core/Math/rect.h"
#include "gui_manager_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTextureWindow_Impl class:

class CL_GUIWindowManagerTextureWindow_Impl
{
public:
	CL_Subtexture texture;
	CL_Rect geometry;
	CL_GUITopLevelWindow *window;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTextureWindow construction:

CL_GUIWindowManagerTextureWindow::CL_GUIWindowManagerTextureWindow()
{
}

CL_GUIWindowManagerTextureWindow::CL_GUIWindowManagerTextureWindow(CL_GUITopLevelWindow *window, const CL_Subtexture &subtexture, const CL_Rect &geometry)
: impl(new CL_GUIWindowManagerTextureWindow_Impl)
{
	impl->texture = subtexture;
	impl->geometry = geometry;
	impl->window = window;
}

CL_GUIWindowManagerTextureWindow::~CL_GUIWindowManagerTextureWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTextureWindow attributes:

CL_Subtexture CL_GUIWindowManagerTextureWindow::get_texture() const
{
	return impl->texture;
}

CL_Rect CL_GUIWindowManagerTextureWindow::get_geometry() const
{
	return impl->geometry;
}

CL_GUITopLevelWindow *CL_GUIWindowManagerTextureWindow::get_window() const
{
	return impl->window;
}

CL_GUIComponent *CL_GUIWindowManagerTextureWindow::get_toplevel_component() const
{
	if (!impl->window)
		return NULL;
	return impl->window->component;
}


/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTextureWindow implementation:
