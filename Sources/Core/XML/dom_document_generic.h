/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Core/System/block_allocator.h"
#include <vector>
#include <stack>

namespace clan
{
	class DomTreeNode;
	class XMLToken;
	class DomNamedNodeMap_Impl;

	class DomDocument_Impl : public DomNode_Impl
	{
	public:
		DomDocument_Impl();
		~DomDocument_Impl();

		std::string qualified_name;
		std::string public_id;
		std::string system_id;
		std::string internal_subset;
		BlockAllocator node_allocator;
		std::vector<DomTreeNode *> nodes;
		std::vector<int> free_nodes;
		std::vector<DomNode_Impl *> free_dom_nodes;
		std::vector<DomNamedNodeMap_Impl *> free_named_node_maps;

		static DomString find_namespace_uri(
			const DomString &qualified_name,
			const XMLToken &search_token,
			const DomNode &search_node);

		unsigned int allocate_tree_node();
		void free_tree_node(unsigned int node_index);
		DomNode_Impl *allocate_dom_node();
		void free_dom_node(DomNode_Impl *node);
		DomNamedNodeMap_Impl *allocate_named_node_map();
		void free_named_node_map(DomNamedNodeMap_Impl *map);

		struct NodeDeleter
		{
			DomDocument_Impl *doc;

			NodeDeleter(DomDocument_Impl *doc) : doc(doc) { }
			void operator()(DomNode_Impl *node) { doc->free_dom_node(node); }
		};

		struct NamedNodeMapDeleter
		{
			DomDocument_Impl *doc;

			NamedNodeMapDeleter(DomDocument_Impl *doc) : doc(doc) { }
			void operator()(DomNamedNodeMap_Impl *map) { doc->free_named_node_map(map); }
		};
	};
}
