/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_xml_tokenizer_generic
#define header_xml_tokenizer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/inputsource.h"

class CL_XMLTokenizer_Generic
{
//! Construction:
public:
	CL_XMLTokenizer_Generic() : input(0), delete_input(false), pos(0), size(0), eat_whitespace(true) { return; }

	~CL_XMLTokenizer_Generic() { if (delete_input) delete input; }

//! Attributes:
public:
	CL_InputSource *input;
	
	bool delete_input;

	std::string::size_type pos, size;

	std::string data;

	bool eat_whitespace;

//! Operations:
public:
	// used to get the line number when there is an error in the xml file
	int get_line_number()
	{
		int line = 1;
		std::string::size_type tmp_pos = 0;

		std::string::const_iterator it;
		for( it = data.begin(); it != data.end() && tmp_pos <= pos; ++it, tmp_pos++ )
		{
			if( (*it) == '\n' )
				line++;
		}
	
		return line;
	}

//! Implementation:
private:
};

#endif
