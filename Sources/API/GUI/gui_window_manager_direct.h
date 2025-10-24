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

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "gui_window_manager.h"

class CL_GUIWindowManagerProvider_Direct;
class CL_GUIWindowManagerDirectWindow;

/// \brief Direct window manager.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIWindowManagerDirect : public CL_GUIWindowManager
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	CL_GUIWindowManagerDirect();

	/// \brief Constructs a GUIWindowManagerTexture
	///
	/// \param display_window = Display Window
	CL_GUIWindowManagerDirect(CL_DisplayWindow display_window);
	CL_GUIWindowManagerDirect(const CL_GUIWindowManager &wm);
	~CL_GUIWindowManagerDirect();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Retrieves the provider.
	CL_GUIWindowManagerProvider_Direct *get_provider() const;

/// \}

/// \name Implementation
/// \{
private:
/// \}

};

/// \}
