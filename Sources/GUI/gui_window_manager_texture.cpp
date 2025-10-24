/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Construction:

CL_GUIWindowManagerTexture::CL_GUIWindowManagerTexture(CL_DisplayWindow display_window) 
: CL_GUIWindowManager(new CL_GUIWindowManagerProvider_Texture(display_window))
{
}

CL_GUIWindowManagerTexture::CL_GUIWindowManagerTexture(const CL_GUIWindowManager &wm)
: CL_GUIWindowManager(wm)
{
	if (dynamic_cast<CL_GUIWindowManagerProvider_Texture *>(get_provider()) == 0)
		throw CL_Exception("CL_GUIWindowManager object is not a CL_GUIWindowManagerTexture!");
}

CL_GUIWindowManagerTexture::~CL_GUIWindowManagerTexture()
{
}
/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Attributes:
CL_GUIWindowManagerProvider_Texture *CL_GUIWindowManagerTexture::get_provider() const
{
	return static_cast <CL_GUIWindowManagerProvider_Texture *> (CL_GUIWindowManager::get_provider());
}

std::vector<CL_GUIWindowManagerTextureWindow> CL_GUIWindowManagerTexture::get_windows(bool only_visible) const
{
	return get_provider()->get_windows(only_visible);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Events:

CL_Callback_v0 &CL_GUIWindowManagerTexture::func_repaint()
{
	return get_provider()->func_repaint;
}

CL_Callback_v1<CL_FrameBuffer &> &CL_GUIWindowManagerTexture::func_setup_framebuffer()
{
	return get_provider()->func_setup_framebuffer;
}

CL_Callback_v2<CL_InputEvent &, CL_InputState &> &CL_GUIWindowManagerTexture::func_input_intercept()
{
	return get_provider()->func_input_intercept;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Operations:

void CL_GUIWindowManagerTexture::set_texture_group(CL_TextureGroup &new_texture_group)
{
	get_provider()->set_texture_group(new_texture_group);
}

void CL_GUIWindowManagerTexture::draw_windows(CL_GraphicContext &gc)
{
	get_provider()->draw_windows(gc);
}

void CL_GUIWindowManagerTexture::process()
{
	get_provider()->process();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIWindowManagerTexture Implementation:
