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
#include "../Text/string_types.h"
#include <vector>
#include <utility>

/// \brief XML token in a XML file.
///
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_API_CORE CL_XMLToken
{
/// \name Construction
/// \{

public:
	CL_XMLToken() : type(NULL_TOKEN), variant(SINGLE)
	{
	}

/// \}
/// \name Attributes
/// \{

public:
	enum TokenType
	{
		NULL_TOKEN                     = 0,
		ELEMENT_TOKEN                  = 1,
		TEXT_TOKEN                     = 3,
		CDATA_SECTION_TOKEN            = 4,
		ENTITY_REFERENCE_TOKEN         = 5,
		ENTITY_TOKEN                   = 6,
		PROCESSING_INSTRUCTION_TOKEN   = 7,
		COMMENT_TOKEN                  = 8,
		DOCUMENT_TYPE_TOKEN            = 10,
		NOTATION_TOKEN                 = 12
	};

	enum TokenVariant
	{
		BEGIN  = 1,
		END    = 2,
		SINGLE = 3
	};

	// Attribute name/value pair.
	typedef std::pair<CL_StringRef, CL_StringRef> Attribute;

	/// \brief The token type.
	TokenType type;

	/// \brief The token variant.
	TokenVariant variant;

	/// \brief The name of the token.
	CL_StringRef name;

	/// \brief Returns the value of the token.
	CL_StringRef value;

	/// \brief All the attributes attached to the token.
	std::vector<Attribute> attributes;

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
