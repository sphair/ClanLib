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
#include "dom_named_node_map_generic.h"
#include "API/XML/dom_named_node_map.h"
#include "API/XML/dom_attr.h"
#include "API/XML/dom_node.h"
#include "dom_document_generic.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

namespace clan
{
	DomNamedNodeMap::DomNamedNodeMap()
	{
	}

	DomNamedNodeMap::DomNamedNodeMap(const std::shared_ptr<DomNamedNodeMap_Impl> &impl)
		: impl(impl)
	{
	}

	DomNamedNodeMap::DomNamedNodeMap(const DomNamedNodeMap &copy)
		: impl(copy.impl)
	{
	}

	DomNamedNodeMap::~DomNamedNodeMap()
	{
	}

	unsigned long DomNamedNodeMap::get_length() const
	{
		if (!impl)
			return 0;
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		const DomTreeNode *tree_node = impl->get_tree_node();
		const DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		unsigned long length = 0;
		while (cur_attribute)
		{
			length++;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}
		return length;
	}

	DomNode DomNamedNodeMap::get_named_item(const DomString &name) const
	{
		if (!impl)
			return DomNode();
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		const DomTreeNode *tree_node = impl->get_tree_node();
		unsigned int cur_index = tree_node->first_attribute;
		const DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		while (cur_attribute)
		{
			if (cur_attribute->get_node_name() == name)
			{
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = cur_index;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}
		return DomNode();
	}

	DomNode DomNamedNodeMap::get_named_item_ns(
		const DomString &namespace_uri,
		const DomString &local_name) const
	{
		if (!impl)
			return DomNode();
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		const DomTreeNode *tree_node = impl->get_tree_node();
		unsigned int cur_index = tree_node->first_attribute;
		const DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		while (cur_attribute)
		{
			std::string lname = cur_attribute->get_node_name();
			std::string::size_type lpos = lname.find_first_of(':');
			if (lpos != std::string::npos)
				lname = lname.substr(lpos + 1);

			if (cur_attribute->get_namespace_uri() == namespace_uri && lname == local_name)
			{
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = cur_index;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}
		return DomNode();
	}

	DomNode DomNamedNodeMap::set_named_item(const DomNode &node)
	{
		if (!impl)
			return DomNode();
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		DomString name = node.get_node_name();
		DomTreeNode *new_tree_node = (DomTreeNode *)node.impl->get_tree_node();
		DomTreeNode *tree_node = impl->get_tree_node();
		if (new_tree_node == tree_node)
			return node;
		unsigned int cur_index = tree_node->first_attribute;
		unsigned int last_index = cl_null_node_index;
		DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		while (cur_attribute)
		{
			if (cur_attribute->get_node_name() == name)
			{
				new_tree_node->parent = cur_attribute->parent;
				new_tree_node->previous_sibling = cur_attribute->previous_sibling;
				new_tree_node->next_sibling = cur_attribute->next_sibling;
				if (cur_attribute->previous_sibling == cl_null_node_index)
					tree_node->first_attribute = node.impl->node_index;
				else
					cur_attribute->get_previous_sibling(doc_impl)->next_sibling = node.impl->node_index;
				if (cur_attribute->next_sibling != cl_null_node_index)
					cur_attribute->get_next_sibling(doc_impl)->previous_sibling = node.impl->node_index;
				cur_attribute->parent = cl_null_node_index;
				cur_attribute->previous_sibling = cl_null_node_index;
				cur_attribute->next_sibling = cl_null_node_index;
				return node;
			}
			last_index = cur_index;
			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}
		if (last_index == cl_null_node_index)
		{
			tree_node->first_attribute = node.impl->node_index;
			new_tree_node->parent = impl->node_index;
			new_tree_node->previous_sibling = cl_null_node_index;
			new_tree_node->next_sibling = cl_null_node_index;
		}
		else
		{
			new_tree_node->parent = impl->node_index;
			new_tree_node->previous_sibling = last_index;
			new_tree_node->next_sibling = cl_null_node_index;
			doc_impl->nodes[last_index]->next_sibling = node.impl->node_index;
		}
		return node;
	}

	DomNode DomNamedNodeMap::set_named_item_ns(const DomNode &node)
	{
		if (!impl)
			return DomNode();
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		DomString namespace_uri = node.get_namespace_uri();
		DomString local_name = node.get_local_name();
		DomTreeNode *new_tree_node = (DomTreeNode *)node.impl->get_tree_node();
		DomTreeNode *tree_node = impl->get_tree_node();
		if (new_tree_node == tree_node)
			return node;
		unsigned int cur_index = tree_node->first_attribute;
		unsigned int last_index = cl_null_node_index;
		DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		while (cur_attribute)
		{
			std::string lname = cur_attribute->get_node_name();
			std::string::size_type lpos = lname.find_first_of(':');
			if (lpos != std::string::npos)
				lname = lname.substr(lpos + 1);

			if (cur_attribute->get_namespace_uri() == namespace_uri && lname == local_name)
			{
				new_tree_node->parent = cur_attribute->parent;
				new_tree_node->previous_sibling = cur_attribute->previous_sibling;
				new_tree_node->next_sibling = cur_attribute->next_sibling;
				if (cur_attribute->previous_sibling == cl_null_node_index)
					tree_node->first_attribute = node.impl->node_index;
				else
					cur_attribute->get_previous_sibling(doc_impl)->next_sibling = node.impl->node_index;
				if (cur_attribute->next_sibling != cl_null_node_index)
					cur_attribute->get_next_sibling(doc_impl)->previous_sibling = node.impl->node_index;
				cur_attribute->parent = cl_null_node_index;
				cur_attribute->previous_sibling = cl_null_node_index;
				cur_attribute->next_sibling = cl_null_node_index;
				return node;
			}
			last_index = cur_index;
			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}
		if (last_index == cl_null_node_index)
		{
			tree_node->first_attribute = node.impl->node_index;
			new_tree_node->parent = impl->node_index;
			new_tree_node->previous_sibling = cl_null_node_index;
			new_tree_node->next_sibling = cl_null_node_index;
		}
		else
		{
			new_tree_node->parent = impl->node_index;
			new_tree_node->previous_sibling = last_index;
			new_tree_node->next_sibling = cl_null_node_index;
			doc_impl->nodes[last_index]->next_sibling = node.impl->node_index;
		}
		return node;
	}

	DomNode DomNamedNodeMap::remove_named_item(const DomString &name)
	{
		if (!impl)
			return DomNode();
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		DomTreeNode *tree_node = impl->get_tree_node();
		unsigned int cur_index = tree_node->first_attribute;
		unsigned int last_index = cl_null_node_index;
		DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		while (cur_attribute)
		{
			if (cur_attribute->get_node_name() == name)
			{
				if (cur_attribute->previous_sibling == cl_null_node_index)
					tree_node->first_attribute = cur_attribute->next_sibling;
				else
					cur_attribute->get_previous_sibling(doc_impl)->next_sibling = cur_attribute->next_sibling;
				if (cur_attribute->next_sibling != cl_null_node_index)
					cur_attribute->get_next_sibling(doc_impl)->previous_sibling = cur_attribute->previous_sibling;
				cur_attribute->parent = cl_null_node_index;
				cur_attribute->previous_sibling = cl_null_node_index;
				cur_attribute->next_sibling = cl_null_node_index;

				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = cur_index;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
			last_index = cur_index;
			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}

		return DomNode();
	}

	DomNode DomNamedNodeMap::remove_named_item_ns(
		const DomString &namespace_uri,
		const DomString &local_name)
	{
		if (!impl)
			return DomNode();
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		DomTreeNode *tree_node = impl->get_tree_node();
		unsigned int cur_index = tree_node->first_attribute;
		unsigned int last_index = cl_null_node_index;
		DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		while (cur_attribute)
		{
			std::string lname = cur_attribute->get_node_name();
			std::string::size_type lpos = lname.find_first_of(':');
			if (lpos != std::string::npos)
				lname = lname.substr(lpos + 1);

			if (cur_attribute->get_namespace_uri() == namespace_uri && lname == local_name)
			{
				if (cur_attribute->previous_sibling == cl_null_node_index)
					tree_node->first_attribute = cur_attribute->next_sibling;
				else
					cur_attribute->get_previous_sibling(doc_impl)->next_sibling = cur_attribute->next_sibling;
				if (cur_attribute->next_sibling != cl_null_node_index)
					cur_attribute->get_next_sibling(doc_impl)->previous_sibling = cur_attribute->previous_sibling;
				cur_attribute->parent = cl_null_node_index;
				cur_attribute->previous_sibling = cl_null_node_index;
				cur_attribute->next_sibling = cl_null_node_index;

				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = cur_index;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
			last_index = cur_index;
			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}

		return DomNode();
	}

	DomNode DomNamedNodeMap::item(unsigned long index) const
	{
		if (!impl)
			return DomNode();
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		const DomTreeNode *tree_node = impl->get_tree_node();
		unsigned int cur_index = tree_node->first_attribute;
		const DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
		unsigned int pos = 0;
		while (cur_attribute)
		{
			if (index == pos)
			{
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = cur_index;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
			pos++;
			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl);
		}
		return DomNode();
	}

	/////////////////////////////////////////////////////////////////////////////

	DomNamedNodeMap_Impl::DomNamedNodeMap_Impl()
		: map_type(type_null), node_index(cl_null_node_index)
	{
	}

	DomNamedNodeMap_Impl::~DomNamedNodeMap_Impl()
	{
	}

	inline DomTreeNode *DomNamedNodeMap_Impl::get_tree_node()
	{
		if (node_index == cl_null_node_index)
			return nullptr;
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)owner_document.lock().get();
		return doc_impl->nodes[node_index];
	}

	inline const DomTreeNode *DomNamedNodeMap_Impl::get_tree_node() const
	{
		if (node_index == cl_null_node_index)
			return nullptr;
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)owner_document.lock().get();
		return doc_impl->nodes[node_index];
	}
}
