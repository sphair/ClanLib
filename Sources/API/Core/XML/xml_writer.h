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

//! clanCore="XML"
//! header=core.h

#ifndef header_xml_writer
#define header_xml_writer

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../System/sharedptr.h"

class CL_OutputSource;
class CL_XMLTokenSave;
class CL_XMLWriter_Generic;

//: The XML Writer writes a XML file based on XML tokens.
//- !group=Core/XML!
//- !header=core.h!
class CL_API_CORE CL_XMLWriter
{
//! Construction:
public:
	CL_XMLWriter();
	
	CL_XMLWriter(const CL_XMLWriter &copy);
	
	CL_XMLWriter(CL_OutputSource *output, bool delete_output = false);
	
	virtual ~CL_XMLWriter();

//! Attributes:
public:
	//: Returns the insert whitespace flag.
	bool get_insert_whitespace() const;

	//: Inserts whitespace between tags if enabled.
	void set_insert_whitespace(bool enable);

//! Operations:
public:
	//: Write token to file.
	void write(const CL_XMLTokenSave &token);
	
//! Implementation:
private:
	CL_SharedPtr<CL_XMLWriter_Generic> impl;
};

#endif
