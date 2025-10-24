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

class CL_XMLWriter_Generic
{
/// \name Construction
/// \{

public:
	CL_XMLWriter_Generic() : insert_whitespace(true), indent(0), first_token(true), single_line_tag(true) { }


/// \}
/// \name Attributes
/// \{

public:
	CL_IODevice output;

	bool insert_whitespace;

	int indent;

	CL_StringAllocator string_allocator;

	CL_String str;

	CL_String escaped_string;

	bool first_token;
	bool single_line_tag;


/// \}
/// \name Operations
/// \{

public:
	CL_StringRef insert_escapes_fast(const CL_StringRef &str);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


