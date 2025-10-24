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

#ifndef header_dom_entity_reference
#define header_dom_entity_reference

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

//: DOM Entity Reference class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>EntityReference objects may be inserted into the structure model when an entity reference
//- is in the source document, or when the user wishes to insert an entity reference. Note that
//- character references and references to predefined entities are considered to be expanded by
//- the HTML or XML processor so that characters are represented by their Unicode equivalent rather
//- than by an entity reference. Moreover, the XML processor may completely expand references to
//- entities while building the structure model, instead of providing EntityReference objects. If
//- it does provide such objects, then for a given EntityReference node, it may be that there is no
//- Entity node representing the referenced entity; but if such an Entity exists, then the child list
//- of the EntityReference node is the same as that of the Entity node. As with the Entity node, all
//- descendants of the EntityReference are readonly.</p>
class CL_API_CORE CL_DomEntityReference : public CL_DomNode
{
//! Construction:
public:
	//: Constructs a DOM Entity Reference handle.
	CL_DomEntityReference();
	
	CL_DomEntityReference(CL_DomDocument &doc, const std::string &name);

	CL_DomEntityReference(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomEntityReference();

//! Attributes:
public:

//! Operations:
public:
};

#endif
