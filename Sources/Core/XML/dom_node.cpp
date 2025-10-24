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
#include "API/Core/XML/xpath_evaluator.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "dom_node_generic.h"
#include "dom_document_generic.h"
#include "dom_tree_node.h"
#include "dom_named_node_map_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode construction:

CL_DomNode::CL_DomNode()
{
}

CL_DomNode::CL_DomNode(const CL_SharedPtr<CL_DomNode_Generic> &impl)
: impl(impl)
{
}

CL_DomNode::CL_DomNode(const CL_DomNode &copy)
: impl(copy.impl)
{
}

CL_DomNode::CL_DomNode(CL_DomDocument doc, unsigned short node_type)
{
	CL_DomDocument_Generic *doc_impl = static_cast<CL_DomDocument_Generic *>(doc.impl.get());
	impl = CL_SharedPtr<CL_DomNode_Generic>(
		doc_impl->allocate_dom_node(),
		doc_impl, &CL_DomDocument_Generic::free_dom_node);

	impl->node_index = doc_impl->allocate_tree_node();
	CL_DomTreeNode *tree_node = impl->get_tree_node();
	tree_node->node_type = node_type;
}

CL_DomNode::~CL_DomNode()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode attributes:

CL_DomString CL_DomNode::get_node_name() const
{
	if (impl)
	{
		const CL_DomTreeNode *tree_node = impl->get_tree_node();
		switch (tree_node->node_type)
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
			return tree_node->get_node_name();
		}
	}
	return CL_DomString();
}

CL_DomString CL_DomNode::get_node_value() const
{
	if (impl)
	{
		const CL_DomTreeNode *tree_node = impl->get_tree_node();
		switch (tree_node->node_type)
		{
		case DOCUMENT_NODE:
		case DOCUMENT_FRAGMENT_NODE:
		case DOCUMENT_TYPE_NODE:
		case ELEMENT_NODE:
		case ENTITY_NODE:
		case ENTITY_REFERENCE_NODE:
		case NOTATION_NODE:
			return CL_DomString();

		case TEXT_NODE:
		case ATTRIBUTE_NODE:
		case PROCESSING_INSTRUCTION_NODE:
		default:
			return tree_node->get_node_value();
		}
	}
	return CL_DomString();
}

CL_DomString CL_DomNode::get_namespace_uri() const
{
	if (impl)
		return impl->get_tree_node()->get_namespace_uri();
	return CL_DomString();
}

CL_DomString CL_DomNode::get_prefix() const
{
	if (impl)
	{
		CL_DomString node_name = impl->get_tree_node()->get_node_name();
		CL_DomString::size_type pos = node_name.find(':');
		if (pos != CL_DomString::npos)
			return node_name.substr(0, pos);
	}
	return CL_DomString();
}

void CL_DomNode::set_prefix(const CL_DomString &prefix)
{
	if (impl)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
		CL_DomString node_name = impl->get_tree_node()->get_node_name();
		CL_DomString::size_type pos = node_name.find(':');
		if (pos == CL_DomString::npos)
			impl->get_tree_node()->set_node_name(doc_impl, prefix + ':' + node_name);
		else
			impl->get_tree_node()->set_node_name(doc_impl, prefix + node_name.substr(pos));
	}
}

CL_DomString CL_DomNode::get_local_name() const
{
	if (impl)
	{
		CL_DomString node_name = impl->get_tree_node()->get_node_name();
		CL_DomString::size_type pos = node_name.find(':');
		if (pos != CL_DomString::npos)
			return node_name.substr(pos + 1);
		else
			return node_name;
	}
	return CL_DomString();
}

void CL_DomNode::set_node_value(const CL_DomString &value)
{
	if (impl)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
		impl->get_tree_node()->set_node_value(doc_impl, value);
	}
}

unsigned short CL_DomNode::get_node_type() const
{
	if (impl)
		return impl->get_tree_node()->node_type;
	return NULL_NODE;
}

CL_DomNode CL_DomNode::get_parent_node() const
{
	if (impl)
	{
		const CL_DomTreeNode *tree_node = impl->get_tree_node();
		if (tree_node->parent != cl_null_node_index)
		{
			CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = tree_node->parent;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
	}
	return CL_DomNode();
}

CL_DomNodeList CL_DomNode::get_child_nodes() const
{
	CL_DomNodeList list;
	CL_DomNode node = get_first_child();
	while(!node.is_null())
	{
		list.add_item(node);
		node = node.get_next_sibling();
	}
	return list;
}

CL_DomNode CL_DomNode::get_first_child() const
{
	if (impl)
	{
		const CL_DomTreeNode *tree_node = impl->get_tree_node();
		if (tree_node->first_child != cl_null_node_index)
		{
			CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = tree_node->first_child;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::get_last_child() const
{
	if (impl)
	{
		const CL_DomTreeNode *tree_node = impl->get_tree_node();
		if (tree_node->last_child != cl_null_node_index)
		{
			CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = tree_node->last_child;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::get_previous_sibling() const
{
	if (impl)
	{
		const CL_DomTreeNode *tree_node = impl->get_tree_node();
		if (tree_node->previous_sibling != cl_null_node_index)
		{
			CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = tree_node->previous_sibling;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::get_next_sibling() const
{
	if (impl)
	{
		const CL_DomTreeNode *tree_node = impl->get_tree_node();
		if (tree_node->next_sibling != cl_null_node_index)
		{
			CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
			CL_DomNode_Generic *dom_node = doc_impl->allocate_dom_node();
			dom_node->node_index = tree_node->next_sibling;
			return CL_DomNode(
				CL_SharedPtr<CL_DomNode_Generic>(
					dom_node,
					doc_impl, &CL_DomDocument_Generic::free_dom_node));
		}
	}
	return CL_DomNode();
}

CL_DomNamedNodeMap CL_DomNode::get_attributes() const
{
	if (impl && impl->get_tree_node()->node_type == ELEMENT_NODE)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
		CL_DomNamedNodeMap_Generic *map = doc_impl->allocate_named_node_map();
		map->node_index = impl->node_index;
		return CL_DomNamedNodeMap(
			CL_SharedPtr<CL_DomNamedNodeMap_Generic>(
				map,
				doc_impl, &CL_DomDocument_Generic::free_named_node_map));
	}
	return CL_DomNamedNodeMap();
}

CL_DomDocument CL_DomNode::get_owner_document() const
{
	if (impl)
		return CL_DomDocument(impl->owner_document);
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

bool CL_DomNode::is_supported(
	const CL_DomString &feature,
	const CL_DomString &version) const
{
	if (CL_StringHelp::compare(feature, "xml") == 0)
	{
		if (version.empty() || version == "1.0" || version == "2.0")
			return true;
	}
	return false;
}

bool CL_DomNode::has_attributes() const
{
	if (!impl)
		return false;
	return (impl->get_tree_node()->first_attribute != cl_null_node_index);
}

bool CL_DomNode::has_child_nodes() const
{
	if (impl)
		return (impl->get_tree_node()->first_child != cl_null_node_index);
	return false;
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

void CL_DomNode::normalize()
{
}

CL_DomNode CL_DomNode::insert_before(CL_DomNode &new_child, CL_DomNode &ref_child)
{
	if (ref_child.impl.is_null())
	{
		append_child(new_child);
		return new_child;
	}

	if (impl && new_child.impl && ref_child.impl)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
		CL_DomTreeNode *tree_node = impl->get_tree_node();
		CL_DomTreeNode *new_tree_node = new_child.impl->get_tree_node();
		CL_DomTreeNode *ref_tree_node = ref_child.impl->get_tree_node();

		new_tree_node->previous_sibling = ref_tree_node->previous_sibling;
		new_tree_node->next_sibling = ref_child.impl->node_index;
		ref_tree_node->previous_sibling = new_child.impl->node_index;
		if (new_tree_node->previous_sibling != cl_null_node_index)
			new_tree_node->get_previous_sibling(doc_impl)->next_sibling = new_child.impl->node_index;
		if (tree_node->first_child == ref_child.impl->node_index)
			tree_node->first_child = new_child.impl->node_index;
		new_tree_node->parent = impl->node_index;

		return new_child;
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::replace_child(CL_DomNode &new_child, CL_DomNode &old_child)
{
	if (impl && new_child.impl && old_child.impl)
	{
		CL_DomTreeNode *tree_node = impl->get_tree_node();
		CL_DomTreeNode *new_tree_node = new_child.impl->get_tree_node();
		CL_DomTreeNode *old_tree_node = old_child.impl->get_tree_node();

		new_tree_node->previous_sibling = old_tree_node->previous_sibling;
		new_tree_node->next_sibling = old_tree_node->next_sibling;
		new_tree_node->parent = impl->node_index;
		if (tree_node->first_child == old_child.impl->node_index)
			tree_node->first_child = new_child.impl->node_index;
		if (tree_node->last_child == old_child.impl->node_index)
			tree_node->last_child = new_child.impl->node_index;
		old_tree_node->previous_sibling = cl_null_node_index;
		old_tree_node->next_sibling = cl_null_node_index;
		old_tree_node->parent = cl_null_node_index;

		return new_child;
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::remove_child(CL_DomNode &old_child)
{
	if (impl && old_child.impl)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
		CL_DomTreeNode *tree_node = impl->get_tree_node();
		CL_DomTreeNode *old_tree_node = old_child.impl->get_tree_node();
		unsigned int prev_index = old_tree_node->previous_sibling;
		unsigned int next_index = old_tree_node->next_sibling;
		CL_DomTreeNode *prev = old_tree_node->get_previous_sibling(doc_impl);
		CL_DomTreeNode *next = old_tree_node->get_next_sibling(doc_impl);
		if (next)
			next->previous_sibling = prev_index;
		if (prev)
			prev->next_sibling = next_index;
		if (tree_node->first_child == old_child.impl->node_index)
			tree_node->first_child = next_index;
		if (tree_node->last_child == old_child.impl->node_index)
			tree_node->last_child = prev_index;
		old_tree_node->previous_sibling = cl_null_node_index;
		old_tree_node->next_sibling = cl_null_node_index;
		old_tree_node->parent = cl_null_node_index;
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::append_child(CL_DomNode new_child)
{
	if (impl && new_child.impl)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
		CL_DomTreeNode *tree_node = impl->get_tree_node();
		CL_DomTreeNode *new_tree_node = new_child.impl->get_tree_node();
		if (tree_node->last_child != cl_null_node_index)
		{
			CL_DomTreeNode *last_tree_node = tree_node->get_last_child(doc_impl);
			last_tree_node->next_sibling = new_child.impl->node_index;
			new_tree_node->previous_sibling = tree_node->last_child;
			tree_node->last_child = new_child.impl->node_index;
		}
		else
		{
			tree_node->first_child = new_child.impl->node_index;
			tree_node->last_child = new_child.impl->node_index;
		}
		new_tree_node->parent = impl->node_index;
		return new_child;
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::clone_node(bool deep) const
{
	throw CL_Exception("Implement me");
}

CL_DomElement CL_DomNode::to_element() const
{
	if (is_element())
		return CL_DomElement(impl);
	return CL_DomElement();
}

CL_DomAttr CL_DomNode::to_attr() const
{
	if (is_attr())
		return CL_DomAttr(impl);
	return CL_DomAttr();
}

CL_DomText CL_DomNode::to_text() const
{
	if (is_text())
		return CL_DomText(impl);
	return CL_DomText();
}

CL_DomCDATASection CL_DomNode::to_cdata_section() const
{
	if (is_cdata_section())
		return CL_DomCDATASection(impl);
	return CL_DomCDATASection();
}

CL_DomEntityReference CL_DomNode::to_entity_reference() const
{
	if (is_entity_reference())
		return CL_DomEntityReference(impl);
	return CL_DomEntityReference();
}

CL_DomEntity CL_DomNode::to_entity() const
{
	if (is_entity())
		return CL_DomEntity(impl);
	return CL_DomEntity();
}

CL_DomProcessingInstruction CL_DomNode::to_processing_instruction() const
{
	if (is_processing_instruction())
		return CL_DomProcessingInstruction(impl);
	return CL_DomProcessingInstruction();
}

CL_DomComment CL_DomNode::to_comment() const
{
	if (is_comment())
		return CL_DomComment(impl);
	return CL_DomComment();
}

CL_DomDocument CL_DomNode::to_document() const
{
	if (is_document())
		return CL_DomDocument(impl);
	return CL_DomDocument();
}

CL_DomDocumentType CL_DomNode::to_document_type() const
{
	if (is_document_type())
		return CL_DomDocumentType(impl);
	return CL_DomDocumentType();
}

CL_DomDocumentFragment CL_DomNode::to_document_fragment() const
{
	if (is_document_fragment())
		return CL_DomDocumentFragment(impl);
	return CL_DomDocumentFragment();
}

CL_DomNotation CL_DomNode::to_notation() const
{
	if (is_notation())
		return CL_DomNotation(impl);
	return CL_DomNotation();
}

CL_DomNode CL_DomNode::named_item(const CL_DomString &name) const
{
	CL_DomNode node = get_first_child();
	while (node.is_null() == false)
	{
		if (node.get_node_name() == name) return node;
		node = node.get_next_sibling();
	}
	return CL_DomNode();
}

CL_DomNode CL_DomNode::named_item_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name) const
{
	CL_DomNode node = get_first_child();
	while (node.is_null() == false)
	{
		if (node.get_namespace_uri() == namespace_uri && node.get_local_name() == local_name)
			return node;
		node = node.get_next_sibling();
	}
	return CL_DomNode();
}

CL_DomString CL_DomNode::find_namespace_uri(const CL_DomString &qualified_name) const
{
	static CL_DomString xmlns_prefix("xmlns:");
	static CL_DomString xmlns_xml("xml");
	static CL_DomString xmlns_xml_uri("http://www.w3.org/XML/1998/namespace");
	static CL_DomString xmlns_xmlns("xmlns");
	static CL_DomString xmlns_xmlns_uri("http://www.w3.org/2000/xmlns/");

	CL_DomString prefix;
	CL_DomString::size_type pos = qualified_name.find(':');
	if (pos != CL_DomString::npos)
		prefix = qualified_name.substr(0, pos);

	if (prefix == xmlns_xml)
		return xmlns_xml;
	else if (prefix == xmlns_xmlns || qualified_name == xmlns_xmlns)
		return xmlns_xmlns;

	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	const CL_DomTreeNode *cur = impl->get_tree_node();
	while (cur)
	{
		const CL_DomTreeNode *cur_attr = cur->get_first_attribute(doc_impl);
		while (cur_attr)
		{
			CL_StringRef node_name = cur_attr->get_node_name();
			if (prefix.empty())
			{
				if (node_name == xmlns_xmlns)
					return cur_attr->get_node_value();
			}
			else
			{
				if (node_name.substr(0, 6) == xmlns_prefix && node_name.substr(6) == prefix)
					return cur_attr->get_node_value();
			}
			cur_attr = cur_attr->get_next_sibling(doc_impl);
		}
		cur = cur->get_parent(doc_impl);
	}
	return CL_DomString();
}

CL_DomString CL_DomNode::find_prefix(const CL_DomString &namespace_uri) const
{
	CL_DomElement cur = to_element();
	while (!cur.is_null())
	{
		CL_DomNamedNodeMap attributes = cur.get_attributes();
		int size = attributes.get_length();
		for (int index = 0; index < size; index++)
		{
			CL_DomNode attribute = attributes.item(index);
			if (attribute.get_prefix() == "xmlns" &&
				attribute.get_node_value() == namespace_uri)
			{
				return attribute.get_local_name();
			}
		}
		cur = cur.get_parent_node().to_element();
	}
	return CL_DomString();
}

std::vector<CL_DomNode> CL_DomNode::select_nodes(const CL_DomString &xpath_expression) const
{
	CL_XPathEvaluator evaluator;
	return evaluator.evaluate(xpath_expression, *this).get_node_set();
}

CL_DomNode CL_DomNode::select_node(const CL_DomString &xpath_expression) const
{
	std::vector<CL_DomNode> nodes = select_nodes(xpath_expression);
	if (nodes.empty())
		throw CL_Exception(cl_format("Xpath did not match any node: %1", xpath_expression));
	return nodes[0];
}

CL_String CL_DomNode::select_string(const CL_DomString &xpath_expression) const
{
	CL_DomNode node = select_node(xpath_expression);
	if (node.is_element())
		return node.to_element().get_text();
	else
		return node.get_node_value();
}

int CL_DomNode::select_int(const CL_DomString &xpath_expression) const
{
	CL_String v = select_string(xpath_expression);
	return CL_StringHelp::text_to_int(v);
}

float CL_DomNode::select_float(const CL_DomString &xpath_expression) const
{
	CL_String v = select_string(xpath_expression);
	return CL_StringHelp::text_to_float(v);
}

bool CL_DomNode::select_bool(const CL_DomString &xpath_expression) const
{
	CL_String v = select_string(xpath_expression);
	return CL_StringHelp::text_to_bool(v);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode implementation:

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode_Generic construction:

CL_DomNode_Generic::CL_DomNode_Generic()
: node_index(cl_null_node_index)
{
}

CL_DomNode_Generic::~CL_DomNode_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomNode_Generic operations:

CL_DomTreeNode *CL_DomNode_Generic::get_tree_node()
{
	if (node_index == cl_null_node_index)
		return 0;
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) owner_document.get();
	return doc_impl->nodes[node_index];
}

const CL_DomTreeNode *CL_DomNode_Generic::get_tree_node() const
{
	if (node_index == cl_null_node_index)
		return 0;
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) owner_document.get();
	return doc_impl->nodes[node_index];
}
