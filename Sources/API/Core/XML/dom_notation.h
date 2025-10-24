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

#ifndef header_dom_notation
#define header_dom_notation

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

#include "dom_node.h"

//: DOM Notation class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>This interface represents a notation declared in the DTD. A notation either declares,
//- by name, the format of an unparsed entity (see section 4.7 of the XML 1.0 specification),
//- or is used for formal declaration of Processing Instruction targets (see section 2.6 of
//- the XML 1.0 specification). The node name attribute inherited from Node is set to the
//- declared name of the notation.</p>
//- <p>The DOM Level 1 does not support editing Notation nodes; they are therefore readonly.</p>
//- <p>A Notation node does not have any parent.</p>
class CL_API_CORE CL_DomNotation : public CL_DomNode
{
//! Construction:
public:
	//: Constructs a DOM Notation handle.
	CL_DomNotation();
	
	CL_DomNotation(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomNotation();
	
//! Attributes:
public:
	//: The public identifier of this notation.
	//- <p>If the public identifier was not specified, this is null.</p>
	std::string get_public_id() const;

	//: The system identifier of this notation.
	//- <p>If the system identifier was not specified, this is null.</p>
	std::string get_system_id() const;

//! Operations:
public:
};

#endif
