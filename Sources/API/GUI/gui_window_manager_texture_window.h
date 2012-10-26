/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "gui_window_manager.h"
#include <memory>

namespace clan
{

class Texture;
class Subtexture;
class GUIWindowManagerTextureWindow_Impl;

/// \brief Texture window manager window.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class GUIWindowManagerTextureWindow
{
/// \name Construction
/// \{

public:
	GUIWindowManagerTextureWindow();

	/// \brief Constructs a GUIWindowManagerTextureWindow
	///
	/// \param window = GUITop Level Window
	/// \param sub_texture = Subtexture
	/// \param geometry = Rect
	GUIWindowManagerTextureWindow(GUITopLevelWindow *window, const Subtexture &sub_texture, const Rect &geometry);

	~GUIWindowManagerTextureWindow();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Texture
	///
	/// \return texture
	Subtexture get_texture() const;

	/// \brief Get Geometry
	///
	/// \return geometry
	Rect get_geometry() const;

	/// \brief Get Window
	///
	/// \return window
	GUITopLevelWindow *get_window() const;

	/// \brief Get Toplevel component
	///
	/// \return toplevel_component
	GUIComponent *get_toplevel_component() const;

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<GUIWindowManagerTextureWindow_Impl> impl;
/// \}
};

}

/// \}
