/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "XML/precomp.h"
#include "API/XML/xml_token.h"
#include "API/XML/dom_node.h"
#include "dom_document_generic.h"
#include "dom_tree_node.h"
#include "dom_named_node_map_generic.h"

namespace clan
{
	DomDocument_Impl::DomDocument_Impl()
	{
		node_index = DomDocument_Impl::allocate_tree_node();
		nodes[node_index]->node_type = DomNode::DOCUMENT_NODE;
	}

	DomDocument_Impl::~DomDocument_Impl()
	{
		std::vector<DomNode_Impl *>::size_type pos, size;

		size = nodes.size();
		for (pos = 0; pos < size; pos++)
			delete nodes[pos];

		while (!free_dom_nodes.empty())
		{
			delete free_dom_nodes.back();
			free_dom_nodes.pop_back();
		}

		while (!free_named_node_maps.empty())
		{
			delete free_named_node_maps.back();
			free_named_node_maps.pop_back();
		}
	}

	DomString DomDocument_Impl::find_namespace_uri(
		const DomString &qualified_name,
		const XMLToken &search_token,
		const DomNode &search_node)
	{
		static DomString xmlns_prefix("xmlns:");
		DomString name = qualified_name;
		DomString prefix;
		DomString::size_type pos = name.find(':');
		if (pos != DomString::npos)
			prefix = name.substr(0, pos);

		int size = (int)search_token.attributes.size();
		for (int i = 0; i < size; i++)
		{
			if (prefix.empty())
			{
				if (search_token.attributes[i].first == "xmlns")
					return search_token.attributes[i].second;
			}
			else
			{
				if (search_token.attributes[i].first.substr(0, 6) == xmlns_prefix &&
					search_token.attributes[i].first.substr(6) == prefix)
					return search_token.attributes[i].second;
			}
		}

		return search_node.find_namespace_uri(qualified_name);
	}

	unsigned int DomDocument_Impl::allocate_tree_node()
	{
		if (free_nodes.empty())
		{
			auto node = new (&node_allocator) DomTreeNode();
			nodes.push_back(node);
			return nodes.size() - 1;
		}
		else
		{
			unsigned index = free_nodes.back();
			nodes[index]->reset();
			free_nodes.pop_back();
			return index;
		}
	}

	void DomDocument_Impl::free_tree_node(unsigned int node_index)
	{
		free_nodes.push_back(node_index);
	}

	DomNode_Impl *DomDocument_Impl::allocate_dom_node()
	{
		if (free_dom_nodes.empty())
		{
			auto node = new DomNode_Impl;
			node->owner_document = owner_document;
			return node;
		}
		else
		{
			DomNode_Impl *node = free_dom_nodes.back();
			node->node_index = cl_null_node_index;
			free_dom_nodes.pop_back();
			return node;
		}
	}

	void DomDocument_Impl::free_dom_node(DomNode_Impl *node)
	{
		if (!node->owner_document.expired())
			free_dom_nodes.push_back(node);
		else
			delete node;
	}

	DomNamedNodeMap_Impl *DomDocument_Impl::allocate_named_node_map()
	{
		if (free_named_node_maps.empty())
		{
			auto map = new (&node_allocator) DomNamedNodeMap_Impl();
			map->owner_document = owner_document;
			return map;
		}
		else
		{
			DomNamedNodeMap_Impl *map = free_named_node_maps.back();
			map->map_type = DomNamedNodeMap_Impl::type_null;
			map->node_index = cl_null_node_index;
			free_named_node_maps.pop_back();
			return map;
		}
	}

	void DomDocument_Impl::free_named_node_map(DomNamedNodeMap_Impl *map)
	{
		if (!map->owner_document.expired())
			free_named_node_maps.push_back(map);
		else
			delete map;
	}
}
