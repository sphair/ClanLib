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

#ifndef header_dom_comment
#define header_dom_comment

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

#include "dom_character_data.h"

//: DOM Comment class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>This represents the content of a comment, i.e., all the characters between the starting
//- '&lt;!--' and ending '--&gt;'. Note that this is the definition of a comment in XML, and,
//- in practice, HTML, although some HTML tools may implement the full SGML comment structure.</p>
class CL_API_CORE CL_DomComment : public CL_DomCharacterData
{
//! Construction:
public:
	//: Constructs a DOM Comment handle.
	CL_DomComment();

	CL_DomComment(CL_DomDocument &doc, const std::string &data);

	CL_DomComment(const CL_SharedPtr<CL_DomNode_Generic> &impl);
	
	~CL_DomComment();

//! Attributes:
public:

//! Operations:
public:
};

#endif
