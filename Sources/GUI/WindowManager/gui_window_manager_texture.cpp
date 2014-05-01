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
**    Kenneth Gangstoe
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_window_manager_texture.h"
#include "gui_window_manager_provider_texture.h"
#include "API/GUI/gui_window_manager_texture_window.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTexture Construction:

GUIWindowManagerTexture::GUIWindowManagerTexture()
{
}

GUIWindowManagerTexture::GUIWindowManagerTexture(DisplayWindow display_window) 
: GUIWindowManager(new GUIWindowManagerProvider_Texture(display_window))
{
}

GUIWindowManagerTexture::GUIWindowManagerTexture(const GUIWindowManager &wm)
: GUIWindowManager(wm)
{
	if (dynamic_cast<GUIWindowManagerProvider_Texture *>(get_provider()) == 0)
		throw Exception("GUIWindowManager object is not a GUIWindowManagerTexture!");
}

GUIWindowManagerTexture::~GUIWindowManagerTexture()
{
}
/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTexture Attributes:
GUIWindowManagerProvider_Texture *GUIWindowManagerTexture::get_provider() const
{
	return static_cast <GUIWindowManagerProvider_Texture *> (GUIWindowManager::get_provider());
}

std::vector<GUIWindowManagerTextureWindow> GUIWindowManagerTexture::get_windows(bool only_visible) const
{
	return get_provider()->get_windows(only_visible);
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTexture Events:

Callback<void()> &GUIWindowManagerTexture::func_repaint()
{
	return get_provider()->func_repaint;
}

Callback<void(FrameBuffer &)> &GUIWindowManagerTexture::func_setup_framebuffer()
{
	return get_provider()->func_setup_framebuffer;
}

Callback<void(InputEvent &)> &GUIWindowManagerTexture::func_input_intercept()
{
	return get_provider()->func_input_intercept;
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTexture Operations:

void GUIWindowManagerTexture::set_texture_group(TextureGroup &new_texture_group)
{
	get_provider()->set_texture_group(new_texture_group);
}

void GUIWindowManagerTexture::draw_windows(Canvas &canvas)
{
	get_provider()->draw_windows(canvas);
}

void GUIWindowManagerTexture::process()
{
	get_provider()->process();
}

/////////////////////////////////////////////////////////////////////////////
// GUIWindowManagerTexture Implementation:

}
