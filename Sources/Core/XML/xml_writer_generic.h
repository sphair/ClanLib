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

#ifndef header_xml_writer_generic
#define header_xml_writer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/outputsource.h"

class CL_XMLWriter_Generic
{
//! Construction:
public:
	CL_XMLWriter_Generic() : output(0), delete_output(false), insert_whitespace(true), indent(0) { return; }

	~CL_XMLWriter_Generic() { if (delete_output) delete output; }

//! Attributes:
public:
	CL_OutputSource *output;
	
	bool delete_output;

	bool insert_whitespace;

	int indent;

//! Operations:
public:

//! Implementation:
private:
};

#endif
