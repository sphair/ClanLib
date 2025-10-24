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
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_document_type.h"
#include "API/Core/XML/dom_implementation.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_document_fragment.h"
#include "API/Core/XML/dom_text.h"
#include "API/Core/XML/dom_comment.h"
#include "API/Core/XML/dom_cdata_section.h"
#include "API/Core/XML/dom_processing_instruction.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_entity_reference.h"
#include "API/Core/XML/dom_node_list.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/xml_tokenizer.h"
#include "API/Core/XML/xml_writer.h"
#include "API/Core/XML/xml_token.h"
#include "dom_document_generic.h"
#include <stack>

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocument construction:

CL_DomDocument::CL_DomDocument()
: CL_DomNode(CL_SharedPtr<CL_DomNode_Generic>(new CL_DomDocument_Generic))
{
	impl->owner_document = impl;
}

CL_DomDocument::CL_DomDocument(CL_IODevice &input, bool eat_whitespace)
: CL_DomNode(CL_SharedPtr<CL_DomNode_Generic>(new CL_DomDocument_Generic))
{
	impl->owner_document = impl;
	load(input, eat_whitespace);
}

CL_DomDocument::CL_DomDocument(
	const CL_DomString &namespace_uri,
	const CL_DomString &qualified_name,
	const CL_DomDocumentType &document_type)
: CL_DomNode(CL_SharedPtr<CL_DomNode_Generic>(new CL_DomDocument_Generic))
{
	impl->owner_document = impl;
	CL_DomElement element = create_element(qualified_name);
	element.set_attribute("xmlns:" + element.get_prefix(), qualified_name);
	append_child(element);

	CL_DomDocument_Generic *doc = dynamic_cast<CL_DomDocument_Generic *>(impl.get());
	const CL_DomDocument_Generic *doctype = dynamic_cast<const CL_DomDocument_Generic *>(document_type.impl.get());
	doc->public_id = doctype->public_id;
	doc->system_id = doctype->system_id;
}

CL_DomDocument::CL_DomDocument(const CL_SharedPtr<CL_DomNode_Generic> &new_impl)
: CL_DomNode(new_impl)
{
	impl->owner_document = impl;
}

CL_DomDocument::~CL_DomDocument()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocument attributes:

CL_DomDocumentType CL_DomDocument::get_doctype()
{
	return CL_DomDocumentType(*this);
}

CL_DomImplementation CL_DomDocument::get_implementation()
{
	return CL_DomImplementation(*this);
}

CL_DomElement CL_DomDocument::get_document_element()
{
	CL_DomNode cur = get_first_child();
	while (!cur.is_null())
	{
		if (cur.is_element())
			return cur.to_element();
		cur = cur.get_next_sibling();
	}
	return CL_DomElement();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocument operations:

CL_DomElement CL_DomDocument::create_element(const CL_DomString &tag_name)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomElement(*this, doc->string_allocator.alloc(tag_name));
}

CL_DomElement CL_DomDocument::create_element_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &qualified_name)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomElement(*this, doc->string_allocator.alloc(qualified_name),
		doc->string_allocator.alloc(namespace_uri));
}

CL_DomDocumentFragment CL_DomDocument::create_document_fragment()
{
	return CL_DomDocumentFragment(*this);
}

CL_DomText CL_DomDocument::create_text_node(const CL_DomString &data)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomText(*this, doc->string_allocator.alloc(data));
}

CL_DomComment CL_DomDocument::create_comment(const CL_DomString &data)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomComment(*this, doc->string_allocator.alloc(data));
}

CL_DomCDATASection CL_DomDocument::create_cdata_section(const CL_DomString &data)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomCDATASection(*this, doc->string_allocator.alloc(data));
}

CL_DomProcessingInstruction CL_DomDocument::create_processing_instruction(
	const CL_DomString &target,
	const CL_DomString &data)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomProcessingInstruction(*this, doc->string_allocator.alloc(target),
		doc->string_allocator.alloc(data));
}

CL_DomAttr CL_DomDocument::create_attribute(const CL_DomString &name)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomAttr(*this, doc->string_allocator.alloc(name));
}

CL_DomAttr CL_DomDocument::create_attribute_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &qualified_name)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomAttr(*this, doc->string_allocator.alloc(qualified_name), doc->string_allocator.alloc(namespace_uri));
}

CL_DomEntityReference CL_DomDocument::create_entity_reference(const CL_DomString &name)
{
	CL_DomDocument_Generic *doc = static_cast<CL_DomDocument_Generic *>(impl.get());
	return CL_DomEntityReference(*this, doc->string_allocator.alloc(name));
}

CL_DomNodeList CL_DomDocument::get_elements_by_tag_name(const CL_DomString &tag_name)
{
	return CL_DomNodeList(*this, tag_name);
}

CL_DomNodeList CL_DomDocument::get_elements_by_tag_name_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &qualified_name)
{
	return CL_DomNodeList(*this, namespace_uri, qualified_name);
}

CL_DomElement CL_DomDocument::get_element_by_id(const CL_DomString &element_id)
{
	throw CL_Exception("Implement me");
}

CL_DomNode CL_DomDocument::import_node(const CL_DomNode &node, bool deep)
{
	CL_DomNode imported_node;
	switch (node.get_node_type())
	{
	case NULL_NODE:
		return imported_node;
	case ELEMENT_NODE:
		imported_node = create_element_ns(node.get_namespace_uri(), node.get_node_name());
		break;
	case ATTRIBUTE_NODE:
		imported_node = create_attribute_ns(node.get_namespace_uri(), node.get_node_name());
		imported_node.set_node_value(node.get_node_value());
		break;
	case TEXT_NODE:
		imported_node = create_text_node(node.get_node_value());
		break;
	case CDATA_SECTION_NODE:
		imported_node = create_cdata_section(node.get_node_value());
		break;
	case ENTITY_REFERENCE_NODE:
		imported_node = create_entity_reference(node.get_node_name());
		break;
	case ENTITY_NODE:
		return imported_node;
	case PROCESSING_INSTRUCTION_NODE:
		imported_node = create_processing_instruction(node.to_processing_instruction().get_target(), node.to_processing_instruction().get_data());
		break;
	case COMMENT_NODE:
		imported_node = create_comment(node.get_node_value());
		break;
	case DOCUMENT_NODE:
		imported_node = create_document_fragment();
		break;
	case DOCUMENT_TYPE_NODE:
		return imported_node;
	case DOCUMENT_FRAGMENT_NODE:
		imported_node = create_document_fragment();
		break;
	case NOTATION_NODE:
		return imported_node;
	}

	if (node.is_element())
	{
		CL_DomElement import_element = imported_node.to_element();
		CL_DomNamedNodeMap node_attributes = node.get_attributes();
		int size = node_attributes.get_length();
		for (int index = 0; index < size; index++)
		{
			CL_DomNode attr = node_attributes.item(index);
			import_element.set_attribute_node_ns(import_node(attr, deep).to_attr());
		}
	}

	if (deep)
	{
		CL_DomNode cur = node.get_first_child();
		while (!cur.is_null())
		{
			imported_node.append_child(import_node(cur, true));
			cur = cur.get_next_sibling();
		}
	}

	return imported_node;
}

std::vector<CL_DomNode> CL_DomDocument::load(
	CL_IODevice &input,
	bool eat_whitespace,
	CL_DomNode insert_point)
{
	clear_all();

	CL_XMLTokenizer tokenizer(input);
	tokenizer.set_eat_whitespace(eat_whitespace);

	if (insert_point.is_element() == false)
		insert_point = *this;
		
	std::vector<CL_DomNode> node_stack;
	node_stack.push_back(insert_point);

	std::vector<CL_DomNode> result;
	
	try
	{
		CL_XMLToken cur_token;
		tokenizer.next(&cur_token);
		while (cur_token.type != CL_XMLToken::NULL_TOKEN)
		{
			switch (cur_token.type)
			{
			case CL_XMLToken::TEXT_TOKEN:
				node_stack.back().append_child(create_text_node(cur_token.value));
				if (node_stack.back() == insert_point)
					result.push_back(node_stack.back().get_last_child());
				break;

			case CL_XMLToken::CDATA_SECTION_TOKEN:
				node_stack.back().append_child(create_cdata_section(cur_token.value));
				if (node_stack.back() == insert_point)
					result.push_back(node_stack.back().get_last_child());
				break;

			case CL_XMLToken::ELEMENT_TOKEN:
				if (cur_token.variant != CL_XMLToken::END)
				{
					CL_DomString namespace_uri = CL_DomDocument_Generic::find_namespace_uri(cur_token.name, cur_token, node_stack.back());
					CL_DomElement element = create_element_ns(namespace_uri, cur_token.name);
					node_stack.back().append_child(element);
					if (node_stack.back() == insert_point)
						result.push_back(node_stack.back().get_last_child());

					int size = (int) cur_token.attributes.size();
					for (int i=0; i<size; i++)
					{
						CL_XMLToken::Attribute &attribute = cur_token.attributes[i];
						CL_DomString attribute_namespace_uri = CL_DomDocument_Generic::find_namespace_uri(attribute.first, cur_token, node_stack.back());
						element.set_attribute_ns(
							attribute_namespace_uri,
							attribute.first,
							attribute.second);
					}
				
					if (cur_token.variant == CL_XMLToken::BEGIN)
						node_stack.push_back(element);
				}
				else
				{
					node_stack.pop_back();
					if (node_stack.empty()) throw CL_Exception("Malformed XML tree!");
				}
				break;

			case CL_XMLToken::NULL_TOKEN: 
				break;

			case CL_XMLToken::ENTITY_REFERENCE_TOKEN: 
				break;

			case CL_XMLToken::ENTITY_TOKEN: 
				break;
			
			case CL_XMLToken::COMMENT_TOKEN:
				break;

			case CL_XMLToken::DOCUMENT_TYPE_TOKEN:
				break;

			case CL_XMLToken::NOTATION_TOKEN:
				break;

			case CL_XMLToken::PROCESSING_INSTRUCTION_TOKEN:
				break;
			}		

			tokenizer.next(&cur_token);
		}
	}
	catch (CL_Exception e)
	{
		for (std::vector<CL_DomNode>::size_type i = 0; i < result.size(); i++)
		{
			insert_point.remove_child(result[i]);
		}
		throw e;
	}
	return result;
}

void CL_DomDocument::save(CL_IODevice &output, bool insert_whitespace)
{
	CL_XMLWriter writer(output);
	writer.set_insert_whitespace(insert_whitespace);

	std::vector<CL_DomNode> node_stack;
	CL_DomNode cur_node = get_first_child();
	while (!cur_node.is_null())
	{
		// Create opening node:
		CL_XMLToken opening_node;
		opening_node.type = (CL_XMLToken::TokenType) cur_node.get_node_type();
		opening_node.variant = cur_node.has_child_nodes() ? CL_XMLToken::BEGIN : CL_XMLToken::SINGLE;
		opening_node.name = cur_node.get_node_name();
		opening_node.value = cur_node.get_node_value();
		if (cur_node.is_element())
		{
			CL_DomNamedNodeMap attributes = cur_node.get_attributes();
			int length = attributes.get_length();
			for (int i = 0; i < length; ++i)
			{
				CL_DomAttr attribute = attributes.item(i).to_attr();
				opening_node.attributes.push_back(
					CL_XMLToken::Attribute(
						attribute.get_name(),
						attribute.get_value()));
			}
		}
		writer.write(opening_node);

		// Create any possible child nodes:
		if (cur_node.has_child_nodes())
		{
			node_stack.push_back(cur_node);
			cur_node = cur_node.get_first_child();
			continue;
		}

		// Create closing nodes until we reach next opening node in tree:
		while (true)
		{
			if (cur_node.has_child_nodes())
			{
				CL_XMLToken closing_node;
				closing_node.type = (CL_XMLToken::TokenType) cur_node.get_node_type();
				closing_node.name = cur_node.get_node_name();
				closing_node.variant = CL_XMLToken::END;
				writer.write(closing_node);
			}

			cur_node = cur_node.get_next_sibling();
			if (!cur_node.is_null())
				break;
			if (node_stack.empty())
				break;

			cur_node = node_stack.back();
			node_stack.pop_back();
		}
	}
}

void CL_DomDocument::clear_all()
{
	while (!get_first_child().is_null())
	{
		CL_DomNode node = get_first_child();
		remove_child(node);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocument implementation:
