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

#pragma once

#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_allocator.h"

class CL_XMLTokenizer_Generic
{
/// \name Construction
/// \{

public:
	CL_XMLTokenizer_Generic() : pos(0), size(0), eat_whitespace(true) { return; }

	~CL_XMLTokenizer_Generic() { return; }


/// \}
/// \name Attributes
/// \{

public:
	CL_IODevice input;

	CL_String::size_type pos, size;

	CL_String data;

	bool eat_whitespace;

	CL_StringAllocator string_allocator;


/// \}
/// \name Operations
/// \{

public:
	static void throw_exception(const CL_StringRef &str);

	bool next_text_node(CL_XMLToken *out_token);

	bool next_tag_node(CL_XMLToken *out_token);

	bool next_exclamation_mark_node(CL_XMLToken *out_token);

	// used to get the line number when there is an error in the xml file
	int get_line_number();

	void unescape(CL_StringRef &text_out, const CL_StringRef &text_in);

	void unescape(CL_StringRef &text, const CL_StringRef &search, CL_String::char_type replace);

	CL_StringRef trim_whitespace(const CL_StringRef &text);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


