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

#pragma once

#include "API/Core/System/block_allocator.h"
#include "dom_document_generic.h"

#define cl_null_node_index 0xffffffff

class CL_DomDocument_Generic;

class CL_DomTreeNode : public CL_BlockAllocated
{
/// \name Construction
/// \{

public:
	CL_DomTreeNode()
	: node_type(0), parent(cl_null_node_index), first_child(cl_null_node_index),
	  last_child(cl_null_node_index), previous_sibling(cl_null_node_index),
	  next_sibling(cl_null_node_index), first_attribute(cl_null_node_index)
	{
		node_name.str = 0;
		node_name.length = 0;
		node_value.str = 0;
		node_value.length = 0;
		namespace_uri.str = 0;
		namespace_uri.length = 0;
	}

	virtual ~CL_DomTreeNode()
	{
	}


/// \}
/// \name Attributes
/// \{

public:
	struct StringPtr
	{
		const CL_DomString::char_type *str;

		CL_DomString::size_type length;
	};

	StringPtr node_name;

	StringPtr node_value;

	StringPtr namespace_uri;

	unsigned short node_type;

	unsigned int parent;

	unsigned int first_child;

	unsigned int last_child;

	unsigned int previous_sibling;

	unsigned int next_sibling;

	unsigned int first_attribute;


/// \}
/// \name Operations
/// \{

public:
	void reset()
	{
		node_name.str = 0;
		node_name.length = 0;
		node_value.str = 0;
		node_value.length = 0;
		namespace_uri.str = 0;
		namespace_uri.length = 0;
		node_type = 0;
		parent = cl_null_node_index;
		first_child = cl_null_node_index;
		last_child = cl_null_node_index;
		previous_sibling = cl_null_node_index;
		next_sibling = cl_null_node_index;
		first_attribute = cl_null_node_index;
	}

	CL_StringRef get_node_name() const
	{
		return CL_StringRef(node_name.str, node_name.length, true);
	}

	CL_StringRef get_node_value() const
	{
		return CL_StringRef(node_value.str, node_value.length, true);
	}

	CL_StringRef get_namespace_uri() const
	{
		return CL_StringRef(namespace_uri.str, namespace_uri.length, true);
	}

	void set_node_name(CL_DomDocument_Generic *owner_document, const CL_DomString &str)
	{
		node_name.str = owner_document->string_allocator.alloc(str).data();
		node_name.length = str.length();
	}

	void set_node_value(CL_DomDocument_Generic *owner_document, const CL_DomString &str)
	{
		node_value.str = owner_document->string_allocator.alloc(str).data();
		node_value.length = str.length();
	}

	void set_namespace_uri(CL_DomDocument_Generic *owner_document, const CL_DomString &str)
	{
		namespace_uri.str = owner_document->string_allocator.alloc(str).data();
		namespace_uri.length = str.length();
	}

	CL_DomTreeNode *get_parent(CL_DomDocument_Generic *owner_document)
	{
		return parent != cl_null_node_index ? owner_document->nodes[parent] : 0;
	}

	const CL_DomTreeNode *get_parent(CL_DomDocument_Generic *owner_document) const
	{
		return parent != cl_null_node_index ? owner_document->nodes[parent] : 0;
	}

	CL_DomTreeNode *get_first_child(CL_DomDocument_Generic *owner_document)
	{
		return first_child != cl_null_node_index ? owner_document->nodes[first_child] : 0;
	}

	const CL_DomTreeNode *get_first_child(CL_DomDocument_Generic *owner_document) const
	{
		return first_child != cl_null_node_index ? owner_document->nodes[first_child] : 0;
	}

	CL_DomTreeNode *get_last_child(CL_DomDocument_Generic *owner_document)
	{
		return last_child != cl_null_node_index ? owner_document->nodes[last_child] : 0;
	}

	const CL_DomTreeNode *get_last_child(CL_DomDocument_Generic *owner_document) const
	{
		return last_child != cl_null_node_index ? owner_document->nodes[last_child] : 0;
	}

	CL_DomTreeNode *get_previous_sibling(CL_DomDocument_Generic *owner_document)
	{
		return previous_sibling != cl_null_node_index ? owner_document->nodes[previous_sibling] : 0;
	}

	const CL_DomTreeNode *get_previous_sibling(CL_DomDocument_Generic *owner_document) const
	{
		return previous_sibling != cl_null_node_index ? owner_document->nodes[previous_sibling] : 0;
	}

	CL_DomTreeNode *get_next_sibling(CL_DomDocument_Generic *owner_document)
	{
		return next_sibling != cl_null_node_index ? owner_document->nodes[next_sibling] : 0;
	}

	const CL_DomTreeNode *get_next_sibling(CL_DomDocument_Generic *owner_document) const
	{
		return next_sibling != cl_null_node_index ? owner_document->nodes[next_sibling] : 0;
	}

	CL_DomTreeNode *get_first_attribute(CL_DomDocument_Generic *owner_document)
	{
		return first_attribute != cl_null_node_index ? owner_document->nodes[first_attribute] : 0;
	}

	const CL_DomTreeNode *get_first_attribute(CL_DomDocument_Generic *owner_document) const
	{
		return first_attribute != cl_null_node_index ? owner_document->nodes[first_attribute] : 0;
	}


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


