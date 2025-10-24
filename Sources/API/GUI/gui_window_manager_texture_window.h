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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "gui_window_manager.h"
#include "../Core/System/sharedptr.h"

class CL_Texture;
class CL_Subtexture;
class CL_GUIWindowManagerTextureWindow_Impl;

/// \brief Texture window manager window.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_GUIWindowManagerTextureWindow
{
/// \name Construction
/// \{

public:
	CL_GUIWindowManagerTextureWindow();

	/// \brief Constructs a GUIWindowManagerTextureWindow
	///
	/// \param window = GUITop Level Window
	/// \param sub_texture = Subtexture
	/// \param geometry = Rect
	CL_GUIWindowManagerTextureWindow(CL_GUITopLevelWindow *window, const CL_Subtexture &sub_texture, const CL_Rect &geometry);

	~CL_GUIWindowManagerTextureWindow();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Texture
	///
	/// \return texture
	CL_Subtexture get_texture() const;

	/// \brief Get Geometry
	///
	/// \return geometry
	CL_Rect get_geometry() const;

	/// \brief Get Window
	///
	/// \return window
	CL_GUITopLevelWindow *get_window() const;

	/// \brief Get Toplevel component
	///
	/// \return toplevel_component
	CL_GUIComponent *get_toplevel_component() const;

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_GUIWindowManagerTextureWindow_Impl> impl;
/// \}
};

/// \}
