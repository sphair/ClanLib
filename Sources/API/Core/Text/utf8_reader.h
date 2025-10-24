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
**   Mark Page
*/

#pragma once

/// \addtogroup clanCore_Text clanCore Text
/// \{

#include "../api_core.h"
#include "../System/sharedptr.h"
#include "string_types.h"

class CL_UTF8_Reader_Impl;

/// \brief UTF8 reader helper functions.
///
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
class CL_API_CORE CL_UTF8_Reader
{
public:
/// \name Construction
/// \{
	CL_UTF8_Reader(const CL_StringRef8 &text);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Returns true if the current position is at the end of the string
	bool is_end();

	/// \brief Get the character at the current position
	unsigned int get_char();

	/// \brief Returns the length of the current character
	CL_String8::size_type get_char_length();

	/// \brief Moves position to the previous character
	void prev();

	/// \brief Moves position to the next character
	void next();

	/// \brief Moves position to the lead byte of the character
	void move_to_leadbyte();

	/// \brief Get the current position of the reader
	CL_String8::size_type get_position();

	/// \brief Set the current position of the reader
	void set_position(CL_String8::size_type position);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_UTF8_Reader_Impl> impl;
/// \}
};
/// \}
