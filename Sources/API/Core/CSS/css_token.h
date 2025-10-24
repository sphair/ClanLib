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

/// \addtogroup clanCore_CSS clanCore CSS
/// \{

#pragma once

/// \brief CSS token
///
/// !group=Core/CSS! !header=core.h!
class CL_CSSToken
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null token
	CL_CSSToken() : type(type_null) { }
/// \}

/// \name Attributes
/// \{
public:
	enum Type
	{
		type_null,
		type_ident,
		type_atkeyword,
		type_string,
		type_invalid,
		type_hash,
		type_number,
		type_percentage,
		type_dimension,
		type_uri,
		type_unicode_range,
		type_cdo,
		type_cdc,
		type_colon,
		type_semi_colon,
		type_curly_brace_begin,
		type_curly_brace_end,
		type_bracket_begin,
		type_bracket_end,
		type_square_bracket_begin,
		type_square_bracket_end,
		type_whitespace,
		type_comment,
		type_function,
		type_includes,
		type_dashmatch,
		type_delim
	};

	Type type;
	CL_String value;
	CL_String dimension;
/// \}
};

/// \}
