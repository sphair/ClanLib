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

#ifndef header_dom_node_generic
#define header_dom_node_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/System/sharedptr.h"
#include "API/Core/System/weakptr.h"
#include "API/Core/XML/dom_named_node_map.h"

#include <map>
#include <string>

class CL_DomDocument_Generic;

class CL_DomNode_Generic
{
//! Construction:
public:
	CL_DomNode_Generic()
	{
	}

	virtual ~CL_DomNode_Generic()
	{
		CL_SharedPtr<CL_DomNode_Generic> temp = last_child;

		while (!temp.is_null ())
		{ 
			temp->free_refs ();

			temp = temp->previous_sibling;
		}
	}

//! Attributes:
public:
	std::string node_name;

	std::string node_value;

	unsigned short node_type;

	CL_WeakPtr<CL_DomNode_Generic> parent;

	CL_SharedPtr<CL_DomNode_Generic> first_child;

	CL_SharedPtr<CL_DomNode_Generic> last_child;

	CL_SharedPtr<CL_DomNode_Generic> previous_sibling;

	CL_SharedPtr<CL_DomNode_Generic> next_sibling;

	CL_WeakPtr<CL_DomNode_Generic> owner_document;

	CL_DomNamedNodeMap attributes;

//! Operations:
public:

//! Implementation:
private:
	void free_refs (void)
	{
		next_sibling.release ();
	};
};

#endif
