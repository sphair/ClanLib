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

#pragma once

#include "API/Core/System/block_allocator.h"
#include "dom_document_generic.h"

namespace clan
{
	#define cl_null_node_index 0xffffffff

	class DomDocument_Impl;

	class DomTreeNode : public BlockAllocated
	{
	public:
		DomTreeNode()
			: node_type(0), parent(cl_null_node_index), first_child(cl_null_node_index),
			last_child(cl_null_node_index), previous_sibling(cl_null_node_index),
			next_sibling(cl_null_node_index), first_attribute(cl_null_node_index)
		{
		}

		virtual ~DomTreeNode()
		{
		}

		std::string node_name;
		std::string node_value;
		std::string namespace_uri;
		unsigned short node_type;
		unsigned int parent;
		unsigned int first_child;
		unsigned int last_child;
		unsigned int previous_sibling;
		unsigned int next_sibling;
		unsigned int first_attribute;

		void reset()
		{
			node_name.clear();
			node_value.clear();
			namespace_uri.clear();
			node_type = 0;
			parent = cl_null_node_index;
			first_child = cl_null_node_index;
			last_child = cl_null_node_index;
			previous_sibling = cl_null_node_index;
			next_sibling = cl_null_node_index;
			first_attribute = cl_null_node_index;
		}

		std::string get_node_name() const
		{
			return node_name;
		}

		std::string get_node_value() const
		{
			return node_value;
		}

		std::string get_namespace_uri() const
		{
			return namespace_uri;
		}

		void set_node_name(DomDocument_Impl *owner_document, const DomString &str)
		{
			node_name = str;
		}

		void set_node_value(DomDocument_Impl *owner_document, const DomString &str)
		{
			node_value = str;
		}

		void set_namespace_uri(DomDocument_Impl *owner_document, const DomString &str)
		{
			namespace_uri = str;
		}

		DomTreeNode *get_parent(DomDocument_Impl *owner_document)
		{
			return parent != cl_null_node_index ? owner_document->nodes[parent] : nullptr;
		}

		const DomTreeNode *get_parent(DomDocument_Impl *owner_document) const
		{
			return parent != cl_null_node_index ? owner_document->nodes[parent] : nullptr;
		}

		DomTreeNode *get_first_child(DomDocument_Impl *owner_document)
		{
			return first_child != cl_null_node_index ? owner_document->nodes[first_child] : nullptr;
		}

		const DomTreeNode *get_first_child(DomDocument_Impl *owner_document) const
		{
			return first_child != cl_null_node_index ? owner_document->nodes[first_child] : nullptr;
		}

		DomTreeNode *get_last_child(DomDocument_Impl *owner_document)
		{
			return last_child != cl_null_node_index ? owner_document->nodes[last_child] : nullptr;
		}

		const DomTreeNode *get_last_child(DomDocument_Impl *owner_document) const
		{
			return last_child != cl_null_node_index ? owner_document->nodes[last_child] : nullptr;
		}

		DomTreeNode *get_previous_sibling(DomDocument_Impl *owner_document)
		{
			return previous_sibling != cl_null_node_index ? owner_document->nodes[previous_sibling] : nullptr;
		}

		const DomTreeNode *get_previous_sibling(DomDocument_Impl *owner_document) const
		{
			return previous_sibling != cl_null_node_index ? owner_document->nodes[previous_sibling] : nullptr;
		}

		DomTreeNode *get_next_sibling(DomDocument_Impl *owner_document)
		{
			return next_sibling != cl_null_node_index ? owner_document->nodes[next_sibling] : nullptr;
		}

		const DomTreeNode *get_next_sibling(DomDocument_Impl *owner_document) const
		{
			return next_sibling != cl_null_node_index ? owner_document->nodes[next_sibling] : nullptr;
		}

		DomTreeNode *get_first_attribute(DomDocument_Impl *owner_document)
		{
			return first_attribute != cl_null_node_index ? owner_document->nodes[first_attribute] : nullptr;
		}

		const DomTreeNode *get_first_attribute(DomDocument_Impl *owner_document) const
		{
			return first_attribute != cl_null_node_index ? owner_document->nodes[first_attribute] : nullptr;
		}
	};
}
