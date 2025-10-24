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

#ifndef header_xml_token_load_generic
#define header_xml_token_load_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/XML/xml_token.h"
#include "API/Core/XML/xml_token_string.h"

class CL_XMLTokenLoad_Generic
{
//! Construction:
public:
	CL_XMLTokenLoad_Generic() : type(CL_XMLToken::NULL_TOKEN), variant(CL_XMLToken::SINGLE) { return; }

	~CL_XMLTokenLoad_Generic() { return; }
	
//! Attributes:
public:
	CL_XMLToken::TokenType type;

	CL_XMLToken::TokenVariant variant;

	CL_XMLTokenString name;

	CL_XMLTokenString value;
	
	std::vector< std::pair<CL_XMLTokenString, CL_XMLTokenString> > attributes;

//! Implementation:
private:
};

#endif
