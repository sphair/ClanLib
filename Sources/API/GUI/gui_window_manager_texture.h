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
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "gui_window_manager.h"

class CL_DisplayWindow;
class CL_GUIWindowManagerTexture_Impl;
class CL_FrameBuffer;
class CL_TextureGroup;
class CL_GUIWindowManagerProvider_Texture;
class CL_GUIWindowManagerTextureWindow;

/// \brief Texture window manager.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIWindowManagerTexture : public CL_GUIWindowManager
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	CL_GUIWindowManagerTexture();

	/// \brief Constructs a GUIWindowManagerTexture
	///
	/// \param display_window = Display Window
	CL_GUIWindowManagerTexture(CL_DisplayWindow display_window);
	CL_GUIWindowManagerTexture(const CL_GUIWindowManager &wm);
	~CL_GUIWindowManagerTexture();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Retrieves the provider.
	CL_GUIWindowManagerProvider_Texture *get_provider() const;

	/// \brief Get the window textures
	///
	/// \param only_visible = True = Only return windows when they are visible
	/// \return The window textures
	std::vector<CL_GUIWindowManagerTextureWindow> get_windows(bool only_visible = true) const;
/// \}

/// \name Events
/// \{
public:
	/// \brief void func_repaint()
	CL_Callback_v0 &func_repaint();

	/// \brief void func_setup_framebuffer(CL_FrameBuffer &fb)
	CL_Callback_v1<CL_FrameBuffer &> &func_setup_framebuffer();

	/// \brief void func_input_intercept(CL_InputEvent &input_event, CL_InputState &input_state)
	CL_Callback_v2<CL_InputEvent &, CL_InputState &> &func_input_intercept();
/// \}

/// \name Operations
/// \{
public:

	/// \brief Set texture group
	///
	/// \param new_texture_group = Texture Group
	void set_texture_group(CL_TextureGroup &new_texture_group);

	/// \brief Draw the windows
	/// 
	/// This is a convenience function than can be used when func_repaint() is overridden
	///
	/// \param gc = Graphic Context
	void draw_windows(CL_GraphicContext &gc);

	/// \brief Process the windows
	/// 
	/// Can you use this function if you do not want the CL_GUIManager to control the application main loop\n
	/// Use draw_windows() after this to draw the windows to the screen
	void process();

/// \}

/// \name Implementation
/// \{
private:

/// \}
};

/// \}
