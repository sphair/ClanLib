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
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "../api_gui.h"

class CL_Size;
class CL_Rect;
class CL_GUILayout_Impl;

/// \brief Layout management interface.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUILayoutProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_GUILayoutProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the size of the dialog, as set in the editor.
	virtual CL_Size get_preferred_size() const = 0;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Destroys the provider.
	virtual void destroy() = 0;

	/// \brief Set geometry
	///
	/// \param pos = Rect
	virtual void set_geometry(const CL_Rect &pos) = 0;

	/// \brief Set preferred size
	///
	/// \param size = Size
	virtual void set_preferred_size(const CL_Size &size) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}

