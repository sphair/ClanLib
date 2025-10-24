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

#ifndef header_dom_node_list
#define header_dom_node_list

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
#include <vector>

class CL_DomNode;
class CL_DomNodeList_Generic;
class CL_DomDocument;

//: DOM Node List class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>The NodeList interface provides the abstraction of an ordered collection of nodes,
//- without defining or constraining how this collection is implemented.</p>
class CL_API_CORE CL_DomNodeList
{
//! Construction:
public:
	//: Constructs a DOM NodeList handle.
	CL_DomNodeList();
	
	CL_DomNodeList(CL_DomNode &node, const std::string &tag_name);

	~CL_DomNodeList();
	
//! Attributes:
public:
	//: The number of nodes in the list.
	int get_length() const;

//! Operations:
public:
	//: Returns the indexth item in the collection.
	//- <p>If index is greater than or equal to the number of nodes in the list, this returns an empty node.</p>
	CL_DomNode item(unsigned long index) const;
	
	//: Adds a CL_DomNode to the list.
	void add_item(CL_DomNode &to_add);
//! Implementation:
private:
	//CL_SharedPtr<CL_DomNodeList_Generic> impl;
	std::vector<CL_DomNode> node_list;
};

#endif
