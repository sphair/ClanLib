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
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "api_gui.h"
#include <memory>

namespace clan
{

class GUIComponentDescription_Impl;

/// \brief GUI component description.
class CL_API_GUI GUIComponentDescription
{
/// \name Construction
/// \{

public:
	GUIComponentDescription();

	/// \brief Constructs a GUIComponentDescription
	///
	/// \param copy = GUIComponent Description
	/// \param default_type_name = String Ref
	GUIComponentDescription(const GUIComponentDescription &copy, const std::string &default_type_name);

	virtual ~GUIComponentDescription();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the component type name.
	std::string get_tag_name() const;

	/// \brief Returns the component class name.
	std::string get_class() const;

	/// \brief Returns the component ID name.
	std::string get_id() const;

	/// \brief Controls whether a component can be resized.
	bool get_allow_resize() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Sets the component type name.
	void set_tag_name(const std::string &name);

	/// \brief Sets the component class name.
	void set_class(const std::string &name);

	/// \brief Sets the component ID name.
	void set_id(const std::string &name);

	/// \brief Controls whether a component can be resized.
	void set_allow_resize(bool value);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<GUIComponentDescription_Impl> impl;

/// \}
};

}

/// \}
