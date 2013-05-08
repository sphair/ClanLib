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
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "gui_window_manager.h"

namespace clan
{

class DisplayWindow;
class GUIWindowManagerTexture_Impl;
class FrameBuffer;
class TextureGroup;
class GUIWindowManagerProvider_Texture;
class GUIWindowManagerTextureWindow;

/// \brief Texture window manager.
class CL_API_GUI GUIWindowManagerTexture : public GUIWindowManager
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	GUIWindowManagerTexture();

	/// \brief Constructs a GUIWindowManagerTexture
	///
	/// \param display_window = Display Window
	GUIWindowManagerTexture(DisplayWindow display_window);
	GUIWindowManagerTexture(const GUIWindowManager &wm);
	~GUIWindowManagerTexture();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Retrieves the provider.
	GUIWindowManagerProvider_Texture *get_provider() const;

	/// \brief Get the window textures
	///
	/// \param only_visible = True = Only return windows when they are visible
	/// \return The window textures
	std::vector<GUIWindowManagerTextureWindow> get_windows(bool only_visible = true) const;
/// \}

/// \name Events
/// \{
public:
	/// \brief void func_repaint()
	Callback_v0 &func_repaint();

	/// \brief void func_setup_framebuffer(FrameBuffer &fb)
	Callback_v1<FrameBuffer &> &func_setup_framebuffer();

	/// \brief void func_input_intercept(InputEvent &input_event)
	Callback_v1<InputEvent &> &func_input_intercept();
/// \}

/// \name Operations
/// \{
public:

	/// \brief Set texture group
	///
	/// \param new_texture_group = Texture Group
	void set_texture_group(TextureGroup &new_texture_group);

	/// \brief Draw the windows
	/// 
	/// This is a convenience function than can be used when func_repaint() is overridden
	///
	/// \param canvas = Canvas
	void draw_windows(Canvas &canvas);

	/// \brief Process the windows
	/// 
	/// Can you use this function if you do not want the GUIManager to control the application main loop\n
	/// Use draw_windows() after this to draw the windows to the screen
	void process();

/// \}

/// \name Implementation
/// \{
private:

/// \}
};

}

/// \}
