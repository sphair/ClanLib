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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Text/string_types.h"

class CL_ProgramAttribute_Impl;

/// \brief OpenGL program object vertex attribute information class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_ProgramAttribute
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	CL_ProgramAttribute();

	/// \brief Constructs a ProgramAttribute
	///
	/// \param name = String Ref
	/// \param size = value
	/// \param type = value
	/// \param location = value
	CL_ProgramAttribute(const CL_StringRef &name, int size, int type, int location);

	~CL_ProgramAttribute();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get attribute name.
	CL_StringRef get_name() const;

	/// \brief Get attribute size.
	int get_size() const;

	/// \brief Get attribute OpenGL type.
	int get_type() const;

	/// \brief Get attribute location.
	int get_location() const;

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ProgramAttribute_Impl> impl;
/// \}
};

/// \}
