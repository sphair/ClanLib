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

#include "Core/precomp.h"
#include "API/Core/XML/dom_node.h"
#include "API/Core/XML/dom_node_list.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_text.h"
#include "API/Core/XML/dom_cdata_section.h"
#include "API/Core/XML/dom_entity_reference.h"
#include "API/Core/XML/dom_entity.h"
#include "API/Core/XML/dom_processing_instruction.h"
#include "API/Core/XML/dom_comment.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_document_type.h"
#include "API/Core/XML/dom_document_fragment.h"
#include "API/Core/XML/dom_notation.h"
#include "dom_node_generic.h"
#include "dom_document_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode construction:

CL_DomNode::CL_DomNode()
{
}

CL_DomNode::CL_DomNode(const CL_SharedPtr<CL_DomNode_Generic> &impl) : impl(impl)
{
}

CL_DomNode::CL_DomNode(const CL_DomNode &copy) : impl(copy.impl)
{
}

CL_DomNode::CL_DomNode(CL_DomDocument &doc, unsigned short node_type)
: impl(new CL_DomNode_Generic)
{
	impl->owner_document = doc.impl;
	impl->node_type = node_type;
}

CL_DomNode::~CL_DomNode()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode attributes:

std::string CL_DomNode::get_node_name() const
{
	if (!impl.is_null())
	{
		switch (impl->node_type)
		{
		case CDATA_SECTION_NODE:
			return "#cdata-section";
		case COMMENT_NODE:
			return "#comment";
		case DOCUMENT_NODE:
			return "#document";
		case DOCUMENT_FRAGMENT_NODE:
			return "#document-fragment";
		case TEXT_NODE:
			return "#text";
		case ATTRIBUTE_NODE:
		case DOCUMENT_TYPE_NODE:
		case ELEMENT_NODE:
		case ENTITY_NODE:
		case ENTITY_REFERENCE_NODE:
		case NOTATION_NODE:
		case PROCESSING_INSTRUCTION_NODE:
		default:
			return impl->node_name;
		}
	}
	return std::string();
}

std::string CL_DomNode::get_node_value() const
{
	if (!impl.is_null())
	{
		switch (impl->node_type)
		{
		case DOCUMENT_NODE:
		case DOCUMENT_FRAGMENT_NODE:
		case DOCUMENT_TYPE_NODE:
		case ELEMENT_NODE:
		case ENTITY_NODE:
		case ENTITY_REFERENCE_NODE:
		case NOTATION_NODE:
			return std::string();

		case TEXT_NODE:
		case ATTRIBUTE_NODE:
		case PROCESSING_INSTRUCTION_NODE:
		default:
			return impl->node_value;
		}
	}
	return std::string();
}

void CL_DomNode::set_node_value(const std::string &value)
{
	if (!impl.is_null()) impl->node_value = value;
}

unsigned short CL_DomNode::get_node_type() const
{
	if (!impl.is_null()) return impl->node_type;
	return NULL_NODE;
}

CL_DomNode CL_DomNode::get_parent_node() const
{
	if (!impl.is_null()) return CL_DomNode(impl->parent);
	return CL_DomNode();
}

CL_DomNodeList CL_DomNode::get_child_nodes() const
{
	CL_DomNodeList lst;
	CL_DomNode node = get_first_child();
	while(!node.is_null()) 
	{
		lst.add_item(node);
		node = node.get_next_sibling();
	}
	return lst;
}

CL_DomNode CL_DomNode::get_first_child() const
{
	if (!impl.is_null()) return CL_DomNode(impl->first_child);
	return CL_DomNode();
}

CL_DomNode CL_DomNode::get_last_child() const
{
	if (!impl.is_null()) return CL_DomNode(impl->last_child);
	return CL_DomNode();
}

CL_DomNode CL_DomNode::get_previous_sibling() const
{
	if (!impl.is_null()) return CL_DomNode(impl->previous_sibling);
	return CL_DomNode();
}

CL_DomNode CL_DomNode::get_next_sibling() const
{
	if (!impl.is_null()) return CL_DomNode(impl->next_sibling);
	return CL_DomNode();
}

CL_DomNamedNodeMap CL_DomNode::get_attributes()
{
	if (!impl.is_null() && impl->node_type == ELEMENT_NODE)
		return impl->attributes;
	return CL_DomNamedNodeMap();
}

CL_DomDocument CL_DomNode::get_owner_document()
{
	if (!impl.is_null()) return CL_DomDocument(impl->owner_document);
	return CL_DomDocument();
}

bool CL_DomNode::is_null() const
{
	return get_node_type() == NULL_NODE;
}

bool CL_DomNode::is_element() const
{
	return get_node_type() == ELEMENT_NODE;
}

bool CL_DomNode::is_attr() const
{
	return get_node_type() == ATTRIBUTE_NODE;
}

bool CL_DomNode::is_text() const
{
	return get_node_type() == TEXT_NODE;
}

bool CL_DomNode::is_cdata_section() const
{
	return get_node_type() == CDATA_SECTION_NODE;
}

bool CL_DomNode::is_entity_reference() const
{
	return get_node_type() == ENTITY_REFERENCE_NODE;
}

bool CL_DomNode::is_entity() const
{
	return get_node_type() == ENTITY_NODE;
}

bool CL_DomNode::is_processing_instruction() const
{
	return get_node_type() == PROCESSING_INSTRUCTION_NODE;
}

bool CL_DomNode::is_comment() const
{
	return get_node_type() == COMMENT_NODE;
}

bool CL_DomNode::is_document() const
{
	return get_node_type() == DOCUMENT_NODE;
}

bool CL_DomNode::is_document_type() const
{
	return get_node_type() == DOCUMENT_TYPE_NODE;
}

bool CL_DomNode::is_document_fragment() const
{
	return get_node_type() == DOCUMENT_FRAGMENT_NODE;
}

bool CL_DomNode::is_notation() const
{
	return get_node_type() == NOTATION_NODE;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode operations:

CL_DomNode &CL_DomNode::operator =(const CL_DomNode &copy)
{
	impl = copy.impl;
	return *this;
}

bool CL_DomNode::operator ==(const CL_DomNode &other) const
{
	return (impl == other.impl);
}

CL_DomNode CL_DomNode::insert_before(CL_DomNode &new_child, CL_DomNode &ref_child)
{
	if (!impl.is_null())
	{
		new_child.impl->previous_sibling = ref_child.impl->previous_sibling;
		new_child.impl->next_sibling = ref_child.impl;
		ref_child.impl->previous_sibling = new_child.impl;
		if (!new_child.impl->previous_sibling.is_null())
			new_child.impl->previous_sibling->next_sibling = new_child.impl;
		if (impl->first_child == ref_child.impl) impl->first_child = new_child.impl;
		new_child.impl->parent = impl;

		return new_child;
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::replace_child(CL_DomNode &new_child, CL_DomNode &old_child)
{
	if (!impl.is_null())
	{
		CL_SharedPtr<CL_DomNode_Generic> &prev = old_child.impl->previous_sibling;
		CL_SharedPtr<CL_DomNode_Generic> &next = old_child.impl->next_sibling;

		// update child siblings
		if (!next.is_null()) next->previous_sibling = new_child.impl;
		if (!prev.is_null()) prev->next_sibling = new_child.impl;

		// update new child
		new_child.impl->previous_sibling = prev;
		new_child.impl->next_sibling = next;
		new_child.impl->parent = impl;

		// update self (parent)
		if (impl->first_child == old_child.impl) impl->first_child = new_child.impl;
		if (impl->last_child == old_child.impl) impl->last_child = new_child.impl;

		// update old child
		old_child.impl->previous_sibling = CL_SharedPtr<CL_DomNode_Generic>();
		old_child.impl->next_sibling = CL_SharedPtr<CL_DomNode_Generic>();
		old_child.impl->parent = CL_WeakPtr<CL_DomNode_Generic>();
		return new_child;
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::remove_child(CL_DomNode &old_child)
{
	if (!impl.is_null())
	{
		CL_SharedPtr<CL_DomNode_Generic> &prev = old_child.impl->previous_sibling;
		CL_SharedPtr<CL_DomNode_Generic> &next = old_child.impl->next_sibling;
		if (!next.is_null()) next->previous_sibling = prev;
		if (!prev.is_null()) prev->next_sibling = next;
		if (impl->first_child == old_child.impl) impl->first_child = next;
		if (impl->last_child == old_child.impl) impl->last_child = prev;
		old_child.impl->previous_sibling = CL_SharedPtr<CL_DomNode_Generic>();
		old_child.impl->next_sibling = CL_SharedPtr<CL_DomNode_Generic>();
		old_child.impl->parent = CL_WeakPtr<CL_DomNode_Generic>();
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::append_child(CL_DomNode new_child)
{
	if (!impl.is_null())
	{
		if (!impl->last_child.is_null())
		{
			impl->last_child->next_sibling = new_child.impl;
			new_child.impl->previous_sibling = impl->last_child;
			impl->last_child = new_child.impl;
		}
		else
		{
			impl->first_child = new_child.impl;
			impl->last_child = new_child.impl;
		}
		new_child.impl->parent = impl;
		return new_child;
	}
	return CL_DomNode();
}

bool CL_DomNode::has_child_nodes() const
{
	if (!impl.is_null()) return !impl->first_child.is_null();
	return false;
}

CL_DomNode CL_DomNode::clone_node(bool deep) const
{
	return CL_DomNode();
}

bool CL_DomNode::is_same_node(const CL_DomNode &node) const
{
	return impl.get() == node.impl.get();
}

CL_DomElement CL_DomNode::to_element() const
{
	if (is_element()) return CL_DomElement(impl);
	return CL_DomElement();
}

CL_DomAttr CL_DomNode::to_attr() const
{
	if (is_attr()) return CL_DomAttr(impl);
	return CL_DomAttr();
}

CL_DomText CL_DomNode::to_text() const
{
	if (is_text()) return CL_DomText(impl);
	return CL_DomText();
}

CL_DomCDATASection CL_DomNode::to_cdata_section() const
{
	if (is_cdata_section()) return CL_DomCDATASection(impl);
	return CL_DomCDATASection();
}

CL_DomEntityReference CL_DomNode::to_entity_reference() const
{
	if (is_entity_reference()) return CL_DomEntityReference(impl);
	return CL_DomEntityReference();
}

CL_DomEntity CL_DomNode::to_entity() const
{
	if (is_entity()) return CL_DomEntity(impl);
	return CL_DomEntity();
}

CL_DomProcessingInstruction CL_DomNode::to_processing_instruction() const
{
	if (is_processing_instruction()) return CL_DomProcessingInstruction(impl);
	return CL_DomProcessingInstruction();
}

CL_DomComment CL_DomNode::to_comment() const
{
	if (is_comment()) return CL_DomComment(impl);
	return CL_DomComment();
}

CL_DomDocument CL_DomNode::to_document() const
{
	if (is_document()) return CL_DomDocument(impl);
	return CL_DomDocument();
}

CL_DomDocumentType CL_DomNode::to_document_type() const
{
	if (is_document_type()) return CL_DomDocumentType(impl);
	return CL_DomDocumentType();
}

CL_DomDocumentFragment CL_DomNode::to_document_fragment() const
{
	if (is_document_fragment()) return CL_DomDocumentFragment(impl);
	return CL_DomDocumentFragment();
}

CL_DomNotation CL_DomNode::to_notation() const
{
	if (is_notation()) return CL_DomNotation(impl);
	return CL_DomNotation();
}

CL_DomNode CL_DomNode::named_item(const std::string &name) const
{
	CL_DomNode node = get_first_child();
	while (node.is_null() == false)
	{
		if (node.get_node_name() == name) return node;
		node = node.get_next_sibling();
	}
	return CL_DomNode();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode implementation:
