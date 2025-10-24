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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "gui_theme.h"

class CL_GUIThemePart;
class CL_GraphicContext;
class CL_GUIThemeProvider_Default;

/// \brief Default GUI theme.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIThemeDefault : public CL_GUITheme
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a default GUI theme.
	CL_GUIThemeDefault();

	~CL_GUIThemeDefault();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Retrieves the gui theme provider.
	CL_GUIThemeProvider_Default *get_provider() const;

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:

/// \}
};

/// \}
