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
*/

/// \addtogroup clanCore_XML clanCore XML
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"

class CL_IODevice;
class CL_XMLToken;
class CL_XMLWriter_Generic;

/// \brief The XML Writer writes a XML file based on XML tokens.
///
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_API_CORE CL_XMLWriter
{
/// \name Construction
/// \{

public:
	CL_XMLWriter();

	/// \brief Constructs a XMLWriter
	///
	/// \param copy = XMLWriter
	CL_XMLWriter(const CL_XMLWriter &copy);

	/// \brief Constructs a XMLWriter
	///
	/// \param output = IODevice
	CL_XMLWriter(CL_IODevice &output);

	virtual ~CL_XMLWriter();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the insert whitespace flag.
	bool get_insert_whitespace() const;

	/// \brief Inserts whitespace between tags if enabled.
	void set_insert_whitespace(bool enable);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Write token to file.
	void write(const CL_XMLToken &token);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_XMLWriter_Generic> impl;
/// \}
};

/// \}
