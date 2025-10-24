/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
*/

#pragma once

#include "dom_node_generic.h"
#include "API/Core/Text/string_allocator.h"
#include "API/Core/System/block_allocator.h"
#include <vector>
#include <stack>

class CL_DomTreeNode;
class CL_XMLToken;
class CL_DomNamedNodeMap_Generic;

class CL_DomDocument_Generic : public CL_DomNode_Generic
{
/// \name Construction
/// \{

public:
	CL_DomDocument_Generic();

	~CL_DomDocument_Generic();


/// \}
/// \name Attributes
/// \{

public:
	CL_String qualified_name;

	CL_String public_id;

	CL_String system_id;

	CL_String internal_subset;

	CL_StringAllocator string_allocator;

	CL_BlockAllocator node_allocator;

	std::vector<CL_DomTreeNode *> nodes;

	std::vector<int> free_nodes;

	std::vector<CL_DomNode_Generic *> free_dom_nodes;

	std::vector<CL_DomNamedNodeMap_Generic *> free_named_node_maps;


/// \}
/// \name Operations
/// \{

public:
	static CL_DomString find_namespace_uri(
		const CL_DomString &qualified_name,
		const CL_XMLToken &search_token,
		const CL_DomNode &search_node);

	unsigned int allocate_tree_node();

	void free_tree_node(unsigned int node_index);

	CL_DomNode_Generic *allocate_dom_node();

	void free_dom_node(CL_DomNode_Generic *node);

	CL_DomNamedNodeMap_Generic *allocate_named_node_map();

	void free_named_node_map(CL_DomNamedNodeMap_Generic *map);
/// \}
};


