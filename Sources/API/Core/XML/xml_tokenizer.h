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

#ifndef header_xml_tokenizer
#define header_xml_tokenizer

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

class CL_InputSource;
class CL_XMLTokenLoad;
class CL_XMLTokenizer_Generic;

//: The XML Tokenizer breaks a XML file into XML tokens.
//- !group=Core/XML!
//- !header=core.h!
class CL_API_CORE CL_XMLTokenizer
{
//! Construction:
public:
	CL_XMLTokenizer();
	
	CL_XMLTokenizer(const CL_XMLTokenizer &copy);

	CL_XMLTokenizer(CL_InputSource *input, bool delete_input = false);
	
	virtual ~CL_XMLTokenizer();
	
//! Attributes:
public:
	//: Returns true if eat whitespace flag is set.
	bool get_eat_whitespace() const;

	//: If enabled, will eat any whitespace between tags.
	void set_eat_whitespace(bool enable);

//! Operations:
public:
	//: Returns the next token available in input stream.
	CL_XMLTokenLoad next();

//! Implementation:
private:
	CL_SharedPtr<CL_XMLTokenizer_Generic> impl;
};

#endif
