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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"

class CL_GUIThemePart;
class CL_GUIThemePart_Impl;
class CL_GUIThemePartProperty_Impl;

/// \brief GUI theme part property.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIThemePartProperty
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a GUI theme part property.
	CL_GUIThemePartProperty();

	/// \brief Constructs a GUIThemePartProperty
	///
	/// \param property_name = String Ref
	/// \param default_value = String Ref
	CL_GUIThemePartProperty(const CL_StringRef &property_name, const CL_StringRef &default_value = CL_StringRef());
	~CL_GUIThemePartProperty();

/// \}
/// \name Attributes
/// \{
public:

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_GUIThemePartProperty_Impl> impl;

	friend class CL_GUIThemePart;
/// \}
};
/// \}
