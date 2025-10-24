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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include "../Core/System/sharedptr.h"

class CL_GUIComponentDescription_Impl;

/// \brief GUI component description.
///
/// \xmlonly !group=GUI/System! !header=gui.h! \endxmlonly
class CL_API_GUI CL_GUIComponentDescription
{
/// \name Construction
/// \{

public:
	CL_GUIComponentDescription();

	/// \brief Constructs a GUIComponentDescription
	///
	/// \param copy = GUIComponent Description
	/// \param default_type_name = String Ref
	CL_GUIComponentDescription(const CL_GUIComponentDescription &copy, const CL_StringRef &default_type_name);

	virtual ~CL_GUIComponentDescription();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the component type name.
	CL_StringRef get_type_name() const;

	/// \brief Returns the component class name.
	CL_StringRef get_class_name() const;

	/// \brief Returns the component ID name.
	CL_StringRef get_id_name() const;

	/// \brief Controls whether a component can be resized.
	bool get_allow_resize() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the component type name.
	void set_type_name(const CL_StringRef &name);

	/// \brief Sets the component class name.
	void set_class_name(const CL_StringRef &name);

	/// \brief Sets the component ID name.
	void set_id_name(const CL_StringRef &name);

	/// \brief Controls whether a component can be resized.
	void set_allow_resize(bool value);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_GUIComponentDescription_Impl> impl;

/// \}
};

/// \}
