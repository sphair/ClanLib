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
#include "API/GUI/gui_window_manager_texture_window.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/image.h"
#include "API/Core/Math/rect.h"
#include "../gui_manager_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTextureWindow_Impl class:

class GUIWindowManagerTextureWindow_Impl
{
public:
	Subtexture texture;
	Rect geometry;
	GUITopLevelWindow *window;
};

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTextureWindow construction:

GUIWindowManagerTextureWindow::GUIWindowManagerTextureWindow()
{
}

GUIWindowManagerTextureWindow::GUIWindowManagerTextureWindow(GUITopLevelWindow *window, const Subtexture &subtexture, const Rect &geometry)
: impl(std::make_shared<GUIWindowManagerTextureWindow_Impl>())
{
	impl->texture = subtexture;
	impl->geometry = geometry;
	impl->window = window;
}

GUIWindowManagerTextureWindow::~GUIWindowManagerTextureWindow()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTextureWindow attributes:

Subtexture GUIWindowManagerTextureWindow::get_texture() const
{
	return impl->texture;
}

Rect GUIWindowManagerTextureWindow::get_geometry() const
{
	return impl->geometry;
}

GUITopLevelWindow *GUIWindowManagerTextureWindow::get_window() const
{
	return impl->window;
}

GUIComponent *GUIWindowManagerTextureWindow::get_toplevel_component() const
{
	if (!impl->window)
		return NULL;
	return impl->window->component;
}


/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTextureWindow implementation:

}
