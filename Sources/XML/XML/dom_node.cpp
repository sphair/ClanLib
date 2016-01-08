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
#include "API/XML/dom_node.h"
#include "API/XML/dom_node_list.h"
#include "API/XML/dom_named_node_map.h"
#include "API/XML/dom_element.h"
#include "API/XML/dom_attr.h"
#include "API/XML/dom_text.h"
#include "API/XML/dom_cdata_section.h"
#include "API/XML/dom_entity_reference.h"
#include "API/XML/dom_entity.h"
#include "API/XML/dom_processing_instruction.h"
#include "API/XML/dom_comment.h"
#include "API/XML/dom_document.h"
#include "API/XML/dom_document_type.h"
#include "API/XML/dom_document_fragment.h"
#include "API/XML/dom_notation.h"
#include "API/XML/xpath_evaluator.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "dom_node_generic.h"
#include "dom_document_generic.h"
#include "dom_tree_node.h"
#include "dom_named_node_map_generic.h"

namespace clan
{
	DomNode::DomNode()
	{
	}

	DomNode::DomNode(const std::shared_ptr<DomNode_Impl> &impl)
		: impl(impl)
	{
	}

	DomNode::DomNode(const DomNode &copy)
		: impl(copy.impl)
	{
	}

	DomNode::DomNode(DomDocument doc, unsigned short node_type)
	{
		DomDocument_Impl *doc_impl = static_cast<DomDocument_Impl *>(doc.impl.get());
		impl = std::shared_ptr<DomNode_Impl>(doc_impl->allocate_dom_node(), DomDocument_Impl::NodeDeleter(doc_impl));

		impl->node_index = doc_impl->allocate_tree_node();
		DomTreeNode *tree_node = impl->get_tree_node();
		tree_node->node_type = node_type;
	}

	DomNode::~DomNode()
	{
	}

	DomString DomNode::get_node_name() const
	{
		if (impl)
		{
			const DomTreeNode *tree_node = impl->get_tree_node();
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
		return DomString();
	}

	DomString DomNode::get_node_value() const
	{
		if (impl)
		{
			const DomTreeNode *tree_node = impl->get_tree_node();
			switch (tree_node->node_type)
			{
			case DOCUMENT_NODE:
			case DOCUMENT_FRAGMENT_NODE:
			case DOCUMENT_TYPE_NODE:
			case ELEMENT_NODE:
			case ENTITY_NODE:
			case ENTITY_REFERENCE_NODE:
			case NOTATION_NODE:
				return DomString();

			case TEXT_NODE:
			case ATTRIBUTE_NODE:
			case PROCESSING_INSTRUCTION_NODE:
			default:
				return tree_node->get_node_value();
			}
		}
		return DomString();
	}

	DomString DomNode::get_namespace_uri() const
	{
		if (impl)
			return impl->get_tree_node()->get_namespace_uri();
		return DomString();
	}

	DomString DomNode::get_prefix() const
	{
		if (impl)
		{
			DomString node_name = impl->get_tree_node()->get_node_name();
			DomString::size_type pos = node_name.find(':');
			if (pos != DomString::npos)
				return node_name.substr(0, pos);
		}
		return DomString();
	}

	void DomNode::set_prefix(const DomString &prefix)
	{
		if (impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			DomString node_name = impl->get_tree_node()->get_node_name();
			DomString::size_type pos = node_name.find(':');
			if (pos == DomString::npos)
				impl->get_tree_node()->set_node_name(doc_impl, prefix + ':' + node_name);
			else
				impl->get_tree_node()->set_node_name(doc_impl, prefix + node_name.substr(pos));
		}
	}

	DomString DomNode::get_local_name() const
	{
		if (impl)
		{
			DomString node_name = impl->get_tree_node()->get_node_name();
			DomString::size_type pos = node_name.find(':');
			if (pos != DomString::npos)
				return node_name.substr(pos + 1);
			else
				return node_name;
		}
		return DomString();
	}

	void DomNode::set_node_value(const DomString &value)
	{
		if (impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			impl->get_tree_node()->set_node_value(doc_impl, value);
		}
	}

	unsigned short DomNode::get_node_type() const
	{
		if (impl)
			return impl->get_tree_node()->node_type;
		return NULL_NODE;
	}

	DomNode DomNode::get_parent_node() const
	{
		if (impl)
		{
			const DomTreeNode *tree_node = impl->get_tree_node();
			if (tree_node->parent != cl_null_node_index)
			{
				DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = tree_node->parent;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
		}
		return DomNode();
	}

	DomNodeList DomNode::get_child_nodes() const
	{
		DomNodeList list;
		DomNode node = get_first_child();
		while (!node.is_null())
		{
			list.add_item(node);
			node = node.get_next_sibling();
		}
		return list;
	}

	DomNode DomNode::get_first_child() const
	{
		if (impl)
		{
			const DomTreeNode *tree_node = impl->get_tree_node();
			if (tree_node->first_child != cl_null_node_index)
			{
				DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = tree_node->first_child;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
		}
		return DomNode();
	}

	DomNode DomNode::get_last_child() const
	{
		if (impl)
		{
			const DomTreeNode *tree_node = impl->get_tree_node();
			if (tree_node->last_child != cl_null_node_index)
			{
				DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = tree_node->last_child;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
		}
		return DomNode();
	}

	DomNode DomNode::get_previous_sibling() const
	{
		if (impl)
		{
			const DomTreeNode *tree_node = impl->get_tree_node();
			if (tree_node->previous_sibling != cl_null_node_index)
			{
				DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = tree_node->previous_sibling;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
		}
		return DomNode();
	}

	DomNode DomNode::get_next_sibling() const
	{
		if (impl)
		{
			const DomTreeNode *tree_node = impl->get_tree_node();
			if (tree_node->next_sibling != cl_null_node_index)
			{
				DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
				DomNode_Impl *dom_node = doc_impl->allocate_dom_node();
				dom_node->node_index = tree_node->next_sibling;
				return DomNode(std::shared_ptr<DomNode_Impl>(dom_node, DomDocument_Impl::NodeDeleter(doc_impl)));
			}
		}
		return DomNode();
	}

	DomNamedNodeMap DomNode::get_attributes() const
	{
		if (impl && impl->get_tree_node()->node_type == ELEMENT_NODE)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			DomNamedNodeMap_Impl *map = doc_impl->allocate_named_node_map();
			map->node_index = impl->node_index;
			return DomNamedNodeMap(std::shared_ptr<DomNamedNodeMap_Impl>(map, DomDocument_Impl::NamedNodeMapDeleter(doc_impl)));
		}
		return DomNamedNodeMap();
	}

	DomDocument DomNode::get_owner_document() const
	{
		if (impl)
			return DomDocument(impl->owner_document.lock());
		return DomDocument();
	}

	bool DomNode::is_null() const
	{
		return get_node_type() == NULL_NODE;
	}

	bool DomNode::is_element() const
	{
		return get_node_type() == ELEMENT_NODE;
	}

	bool DomNode::is_attr() const
	{
		return get_node_type() == ATTRIBUTE_NODE;
	}

	bool DomNode::is_text() const
	{
		return get_node_type() == TEXT_NODE;
	}

	bool DomNode::is_cdata_section() const
	{
		return get_node_type() == CDATA_SECTION_NODE;
	}

	bool DomNode::is_entity_reference() const
	{
		return get_node_type() == ENTITY_REFERENCE_NODE;
	}

	bool DomNode::is_entity() const
	{
		return get_node_type() == ENTITY_NODE;
	}

	bool DomNode::is_processing_instruction() const
	{
		return get_node_type() == PROCESSING_INSTRUCTION_NODE;
	}

	bool DomNode::is_comment() const
	{
		return get_node_type() == COMMENT_NODE;
	}

	bool DomNode::is_document() const
	{
		return get_node_type() == DOCUMENT_NODE;
	}

	bool DomNode::is_document_type() const
	{
		return get_node_type() == DOCUMENT_TYPE_NODE;
	}

	bool DomNode::is_document_fragment() const
	{
		return get_node_type() == DOCUMENT_FRAGMENT_NODE;
	}

	bool DomNode::is_notation() const
	{
		return get_node_type() == NOTATION_NODE;
	}

	bool DomNode::is_supported(
		const DomString &feature,
		const DomString &version) const
	{
		if (StringHelp::compare(feature, "xml") == 0)
		{
			if (version.empty() || version == "1.0" || version == "2.0")
				return true;
		}
		return false;
	}

	bool DomNode::has_attributes() const
	{
		if (!impl)
			return false;
		return (impl->get_tree_node()->first_attribute != cl_null_node_index);
	}

	bool DomNode::has_child_nodes() const
	{
		if (impl)
			return (impl->get_tree_node()->first_child != cl_null_node_index);
		return false;
	}

	DomNode &DomNode::operator =(const DomNode &copy)
	{
		impl = copy.impl;
		return *this;
	}

	bool DomNode::operator ==(const DomNode &other) const
	{
		if (is_null() || other.is_null())
			return is_null() == other.is_null();
		else
			return impl->node_index == other.impl->node_index;
	}

	bool DomNode::operator !=(const DomNode &other) const
	{
		return !(*this == other);
	}

	void DomNode::normalize()
	{
	}

	DomNode DomNode::insert_before(DomNode &new_child, DomNode &ref_child)
	{
		if (!ref_child.impl)
		{
			append_child(new_child);
			return new_child;
		}

		if (impl && new_child.impl && ref_child.impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			DomTreeNode *tree_node = impl->get_tree_node();
			DomTreeNode *new_tree_node = new_child.impl->get_tree_node();
			DomTreeNode *ref_tree_node = ref_child.impl->get_tree_node();

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
		return DomNode();
	}

	DomNode DomNode::replace_child(DomNode &new_child, DomNode &old_child)
	{
		if (impl && new_child.impl && old_child.impl)
		{
			DomTreeNode *tree_node = impl->get_tree_node();
			DomTreeNode *new_tree_node = new_child.impl->get_tree_node();
			DomTreeNode *old_tree_node = old_child.impl->get_tree_node();

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
		return DomNode();
	}

	DomNode DomNode::remove_child(DomNode &old_child)
	{
		if (impl && old_child.impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			DomTreeNode *tree_node = impl->get_tree_node();
			DomTreeNode *old_tree_node = old_child.impl->get_tree_node();
			unsigned int prev_index = old_tree_node->previous_sibling;
			unsigned int next_index = old_tree_node->next_sibling;
			DomTreeNode *prev = old_tree_node->get_previous_sibling(doc_impl);
			DomTreeNode *next = old_tree_node->get_next_sibling(doc_impl);
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
		return DomNode();
	}

	DomNode DomNode::append_child(DomNode new_child)
	{
		if (impl && new_child.impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			DomTreeNode *tree_node = impl->get_tree_node();
			DomTreeNode *new_tree_node = new_child.impl->get_tree_node();
			if (tree_node->last_child != cl_null_node_index)
			{
				DomTreeNode *last_tree_node = tree_node->get_last_child(doc_impl);
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
		return DomNode();
	}

	DomNode DomNode::clone_node(bool deep) const
	{
		throw Exception("Implement me");
	}

	DomElement DomNode::to_element() const
	{
		if (is_element())
			return DomElement(impl);
		return DomElement();
	}

	DomAttr DomNode::to_attr() const
	{
		if (is_attr())
			return DomAttr(impl);
		return DomAttr();
	}

	DomText DomNode::to_text() const
	{
		if (is_text())
			return DomText(impl);
		return DomText();
	}

	DomCDATASection DomNode::to_cdata_section() const
	{
		if (is_cdata_section())
			return DomCDATASection(impl);
		return DomCDATASection();
	}

	DomEntityReference DomNode::to_entity_reference() const
	{
		if (is_entity_reference())
			return DomEntityReference(impl);
		return DomEntityReference();
	}

	DomEntity DomNode::to_entity() const
	{
		if (is_entity())
			return DomEntity(impl);
		return DomEntity();
	}

	DomProcessingInstruction DomNode::to_processing_instruction() const
	{
		if (is_processing_instruction())
			return DomProcessingInstruction(impl);
		return DomProcessingInstruction();
	}

	DomComment DomNode::to_comment() const
	{
		if (is_comment())
			return DomComment(impl);
		return DomComment();
	}

	DomDocument DomNode::to_document() const
	{
		if (is_document())
			return DomDocument(impl);
		return DomDocument();
	}

	DomDocumentType DomNode::to_document_type() const
	{
		if (is_document_type())
			return DomDocumentType(impl);
		return DomDocumentType();
	}

	DomDocumentFragment DomNode::to_document_fragment() const
	{
		if (is_document_fragment())
			return DomDocumentFragment(impl);
		return DomDocumentFragment();
	}

	DomNotation DomNode::to_notation() const
	{
		if (is_notation())
			return DomNotation(impl);
		return DomNotation();
	}

	DomNode DomNode::named_item(const DomString &name) const
	{
		DomNode node = get_first_child();
		while (node.is_null() == false)
		{
			if (node.get_node_name() == name) return node;
			node = node.get_next_sibling();
		}
		return DomNode();
	}

	DomNode DomNode::named_item_ns(
		const DomString &namespace_uri,
		const DomString &local_name) const
	{
		DomNode node = get_first_child();
		while (node.is_null() == false)
		{
			if (node.get_namespace_uri() == namespace_uri && node.get_local_name() == local_name)
				return node;
			node = node.get_next_sibling();
		}
		return DomNode();
	}

	DomString DomNode::find_namespace_uri(const DomString &qualified_name) const
	{
		static DomString xmlns_prefix("xmlns:");
		static DomString xmlns_xml("xml");
		static DomString xmlns_xml_uri("http://www.w3.org/XML/1998/namespace");
		static DomString xmlns_xmlns("xmlns");
		static DomString xmlns_xmlns_uri("http://www.w3.org/2000/xmlns/");

		DomString prefix;
		DomString::size_type pos = qualified_name.find(':');
		if (pos != DomString::npos)
			prefix = qualified_name.substr(0, pos);

		if (prefix == xmlns_xml)
			return xmlns_xml;
		else if (prefix == xmlns_xmlns || qualified_name == xmlns_xmlns)
			return xmlns_xmlns;

		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		const DomTreeNode *cur = impl->get_tree_node();
		while (cur)
		{
			const DomTreeNode *cur_attr = cur->get_first_attribute(doc_impl);
			while (cur_attr)
			{
				std::string node_name = cur_attr->get_node_name();
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
		return DomString();
	}

	DomString DomNode::find_prefix(const DomString &namespace_uri) const
	{
		DomElement cur = to_element();
		while (!cur.is_null())
		{
			DomNamedNodeMap attributes = cur.get_attributes();
			int size = attributes.get_length();
			for (int index = 0; index < size; index++)
			{
				DomNode attribute = attributes.item(index);
				if (attribute.get_prefix() == "xmlns" &&
					attribute.get_node_value() == namespace_uri)
				{
					return attribute.get_local_name();
				}
			}
			cur = cur.get_parent_node().to_element();
		}
		return DomString();
	}

	std::vector<DomNode> DomNode::select_nodes(const DomString &xpath_expression) const
	{
		XPathEvaluator evaluator;
		return evaluator.evaluate(xpath_expression, *this).get_node_set();
	}

	DomNode DomNode::select_node(const DomString &xpath_expression) const
	{
		std::vector<DomNode> nodes = select_nodes(xpath_expression);
		if (nodes.empty())
			throw Exception(string_format("Xpath did not match any node: %1", xpath_expression));
		return nodes[0];
	}

	std::string DomNode::select_string(const DomString &xpath_expression) const
	{
		DomNode node = select_node(xpath_expression);
		if (node.is_element())
			return node.to_element().get_text();
		else
			return node.get_node_value();
	}

	int DomNode::select_int(const DomString &xpath_expression) const
	{
		std::string v = select_string(xpath_expression);
		return StringHelp::text_to_int(v);
	}

	float DomNode::select_float(const DomString &xpath_expression) const
	{
		std::string v = select_string(xpath_expression);
		return StringHelp::text_to_float(v);
	}

	bool DomNode::select_bool(const DomString &xpath_expression) const
	{
		std::string v = select_string(xpath_expression);
		return StringHelp::text_to_bool(v);
	}

	/////////////////////////////////////////////////////////////////////////////

	DomNode_Impl::DomNode_Impl()
		: node_index(cl_null_node_index)
	{
	}

	DomNode_Impl::~DomNode_Impl()
	{
	}

	DomTreeNode *DomNode_Impl::get_tree_node()
	{
		if (node_index == cl_null_node_index)
			return nullptr;
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)owner_document.lock().get();
		return doc_impl->nodes[node_index];
	}

	const DomTreeNode *DomNode_Impl::get_tree_node() const
	{
		if (node_index == cl_null_node_index)
			return nullptr;
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)owner_document.lock().get();
		return doc_impl->nodes[node_index];
	}
}
