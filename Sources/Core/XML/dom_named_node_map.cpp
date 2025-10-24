/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "Core/precomp.h"
#include "Core/XML/dom_named_node_map_generic.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_node.h"
#include "dom_document_generic.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap construction:

CL_DomNamedNodeMap::CL_DomNamedNodeMap()
{
}

CL_DomNamedNodeMap::CL_DomNamedNodeMap(const CL_SharedPtr<CL_DomNamedNodeMap_Generic> &impl)
: impl(impl)
{
}

CL_DomNamedNodeMap::CL_DomNamedNodeMap(const CL_DomNamedNodeMap &copy)
: impl(copy.impl)
{
}

CL_DomNamedNodeMap::~CL_DomNamedNodeMap()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap attributes:

int CL_DomNamedNodeMap::get_length() const
{
	if (impl.is_null())
		return 0;
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	const CL_DomTreeNode *tree_node = impl->get_tree_node();
	const CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
	int length = 0;
	while (cur_attribute)
	{
		length++;
		cur_attribute = cur_attribute->get_next_sibling(doc_impl);
	}
	return length;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap operations:

CL_DomNode CL_DomNamedNodeMap::get_named_item(const CL_DomString &name) const
{
	if (impl.is_null())
		return CL_DomNode();
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	const CL_DomTreeNode *tree_node = impl->get_tree_node();
	unsigned int cur_index = tree_node->first_attribute;
	const CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
	while (cur_attribute)
	{
		if (cur_attribute->get_node_name() == name)
		{
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = cur_index;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
		cur_index = cur_attribute->next_sibling;
		cur_attribute = cur_attribute->get_next_sibling(doc_impl);
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNamedNodeMap::get_named_item_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name) const
{
	if (impl.is_null())
		return CL_DomNode();
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	const CL_DomTreeNode *tree_node = impl->get_tree_node();
	unsigned int cur_index = tree_node->first_attribute;
	const CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
	while (cur_attribute)
	{
		CL_StringRef lname = cur_attribute->get_node_name();
		CL_StringRef::size_type lpos = lname.find_first_of(':');
		if (lpos != CL_StringRef::npos)
			lname = lname.substr(lpos + 1);

		if (cur_attribute->get_namespace_uri() == namespace_uri && lname == local_name)
		{
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = cur_index;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));

		}
		cur_index = cur_attribute->next_sibling;
		cur_attribute = cur_attribute->get_next_sibling(doc_impl);
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNamedNodeMap::set_named_item(const CL_DomNode &node)
{
	if (impl.is_null())
		return CL_DomNode();
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	CL_DomString name = node.get_node_name();
	CL_DomTreeNode *new_tree_node = (CL_DomTreeNode *) node.impl->get_tree_node();
	CL_DomTreeNode *tree_node = impl->get_tree_node();
	if (new_tree_node == tree_node)
		return node;
	unsigned int cur_index = tree_node->first_attribute;
	unsigned int last_index = cl_null_node_index;
	CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
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

CL_DomNode CL_DomNamedNodeMap::set_named_item_ns(const CL_DomNode &node)
{
	if (impl.is_null())
		return CL_DomNode();
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	CL_DomString namespace_uri = node.get_namespace_uri();
	CL_DomString local_name = node.get_local_name();
	CL_DomTreeNode *new_tree_node = (CL_DomTreeNode *) node.impl->get_tree_node();
	CL_DomTreeNode *tree_node = impl->get_tree_node();
	if (new_tree_node == tree_node)
		return node;
	unsigned int cur_index = tree_node->first_attribute;
	unsigned int last_index = cl_null_node_index;
	CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
	while (cur_attribute)
	{
		CL_StringRef lname = cur_attribute->get_node_name();
		CL_StringRef::size_type lpos = lname.find_first_of(':');
		if (lpos != CL_StringRef::npos)
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

CL_DomNode CL_DomNamedNodeMap::remove_named_item(const CL_DomString &name)
{
	if (impl.is_null())
		return CL_DomNode();
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	CL_DomTreeNode *tree_node = impl->get_tree_node();
	unsigned int cur_index = tree_node->first_attribute;
	unsigned int last_index = cl_null_node_index;
	CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
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

			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = cur_index;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
		last_index = cur_index;
		cur_index = cur_attribute->next_sibling;
		cur_attribute = cur_attribute->get_next_sibling(doc_impl);
	}

	return CL_DomNode();
}

CL_DomNode CL_DomNamedNodeMap::remove_named_item_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name)
{
	if (impl.is_null())
		return CL_DomNode();
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	CL_DomTreeNode *tree_node = impl->get_tree_node();
	unsigned int cur_index = tree_node->first_attribute;
	unsigned int last_index = cl_null_node_index;
	CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
	while (cur_attribute)
	{
		CL_StringRef lname = cur_attribute->get_node_name();
		CL_StringRef::size_type lpos = lname.find_first_of(':');
		if (lpos != CL_StringRef::npos)
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

			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = cur_index;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
		last_index = cur_index;
		cur_index = cur_attribute->next_sibling;
		cur_attribute = cur_attribute->get_next_sibling(doc_impl);
	}

	return CL_DomNode();
}

CL_DomNode CL_DomNamedNodeMap::item(unsigned long index) const
{
	if (impl.is_null())
		return CL_DomNode();
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	const CL_DomTreeNode *tree_node = impl->get_tree_node();
	unsigned int cur_index = tree_node->first_attribute;
	const CL_DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
	unsigned int pos = 0;
	while (cur_attribute)
	{
		if (index == pos)
		{
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = cur_index;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
 		}
		pos++;
		cur_index = cur_attribute->next_sibling;
		cur_attribute = cur_attribute->get_next_sibling(doc_impl);
	}
	return CL_DomNode();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap implementation:

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap_Generic construction:

CL_DomNamedNodeMap_Generic::CL_DomNamedNodeMap_Generic()
: map_type(type_null), node_index(cl_null_node_index)
{
}

CL_DomNamedNodeMap_Generic::~CL_DomNamedNodeMap_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNamedNodeMap_Generic operations:

inline CL_DomTreeNode *CL_DomNamedNodeMap_Generic::get_tree_node()
{
	if (node_index == cl_null_node_index)
		return 0;
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) owner_document.get();
	return doc_impl->nodes[node_index];
}

inline const CL_DomTreeNode *CL_DomNamedNodeMap_Generic::get_tree_node() const
{
	if (node_index == cl_null_node_index)
		return 0;
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) owner_document.get();
	return doc_impl->nodes[node_index];
}
