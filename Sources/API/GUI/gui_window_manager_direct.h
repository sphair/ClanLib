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
**    Mark Page
*/


#pragma once

#include "gui_window_manager.h"

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class GUIWindowManagerProvider_Direct;
class GUIWindowManagerDirectWindow;

/// \brief Direct window manager.
class GUIWindowManagerDirect : public GUIWindowManager
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	GUIWindowManagerDirect();

	/// \brief Constructs a GUIWindowManagerDirect
	///
	/// \param display_window = display window
	/// \param canvas = canvas
	GUIWindowManagerDirect(DisplayWindow &display_window, Canvas &canvas);
	GUIWindowManagerDirect(const GUIWindowManager &wm);
	~GUIWindowManagerDirect();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Retrieves the provider.
	GUIWindowManagerProvider_Direct *get_provider() const;
/// \}

/// \name Events
/// \{
public:
	/// \brief void func_input_intercept(InputEvent &input_event)
	std::function<void(InputEvent &)> &func_input_intercept();
/// \}

/// \name Implementation
/// \{
private:
/// \}

};

}

/// \}
