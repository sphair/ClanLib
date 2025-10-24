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
#include "API/Core/XML/xml_tokenizer.h"
#include "API/Core/XML/xml_writer.h"
#include "API/Core/XML/xml_token.h"
#include "dom_document_generic.h"
#include <stack>

#include "API/Core/XML/xml_token_load.h"
#include "API/Core/XML/xml_token_save.h"
#include "API/Core/XML/xml_token_string.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocument construction:

CL_DomDocument::CL_DomDocument() : CL_DomNode(CL_SharedPtr<CL_DomNode_Generic>(new CL_DomDocument_Generic))
{
}

CL_DomDocument::CL_DomDocument(CL_InputSource *input, bool delete_input, bool eat_whitespace)
: CL_DomNode(CL_SharedPtr<CL_DomNode_Generic>(new CL_DomDocument_Generic))
{
	load(input, delete_input, eat_whitespace);
}

CL_DomDocument::CL_DomDocument(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
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
	CL_DomNode cur(impl->first_child);
	while (!cur.is_null())
	{
		if (cur.is_element()) return cur.to_element();
		cur = cur.get_next_sibling();
	}
	return CL_DomElement();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocument operations:

CL_DomElement CL_DomDocument::create_element(const std::string &tag_name)
{
	return CL_DomElement(*this, tag_name);
}

CL_DomDocumentFragment CL_DomDocument::create_document_fragment()
{
	return CL_DomDocumentFragment(*this);
}

CL_DomText CL_DomDocument::create_text_node(const std::string &data)
{
	return CL_DomText(*this, data);
}

CL_DomComment CL_DomDocument::create_comment(const std::string &data)
{
	return CL_DomComment(*this, data);
}

CL_DomCDATASection CL_DomDocument::create_cdata_section(const std::string &data)
{
	return CL_DomCDATASection(*this, data);
}

CL_DomProcessingInstruction CL_DomDocument::create_processing_instruction(
	const std::string &target,
	const std::string &data)
{
	return CL_DomProcessingInstruction(*this, target, data);
}

CL_DomAttr CL_DomDocument::create_attribute(const std::string &name)
{
	return CL_DomAttr(*this, name);
}

CL_DomEntityReference CL_DomDocument::create_entity_reference(const std::string &name)
{
	return CL_DomEntityReference(*this, name);
}

CL_DomNodeList CL_DomDocument::get_elements_by_tag_name(const std::string &tag_name)
{
	return CL_DomNodeList(*this, tag_name);
}

std::vector<CL_DomNode> CL_DomDocument::load(
	CL_InputSource *input,
	bool delete_input,
	bool eat_whitespace,
	CL_DomNode insert_point)
{
	clear_all();

	CL_XMLTokenizer tokenizer(input, delete_input);
	tokenizer.set_eat_whitespace(eat_whitespace);

	if (insert_point.is_element() == false)
		insert_point = *this;
		
	std::stack<CL_DomNode> node_stack;
	node_stack.push(insert_point);

	std::vector<CL_DomNode> result;
	
	try
	{
		CL_XMLTokenLoad cur_token = tokenizer.next();
		while (cur_token.get_type() != CL_XMLToken::NULL_TOKEN)
		{
			switch (cur_token.get_type())
			{
			case CL_XMLToken::TEXT_TOKEN:
				node_stack.top().append_child(create_text_node(cur_token.get_value()));
				if (node_stack.top() == insert_point)
					result.push_back(node_stack.top().get_last_child());
				break;

			case CL_XMLToken::CDATA_SECTION_TOKEN:
				node_stack.top().append_child(create_cdata_section(cur_token.get_value()));
				if (node_stack.top() == insert_point)
					result.push_back(node_stack.top().get_last_child());
				break;

			case CL_XMLToken::ELEMENT_TOKEN:
				if (cur_token.get_variant() != CL_XMLToken::END)
				{
					CL_DomElement element = create_element(cur_token.get_name());
					node_stack.top().append_child(element);
					if (node_stack.top() == insert_point)
						result.push_back(node_stack.top().get_last_child());

					int size = cur_token.get_attributes_number();
					for (int i=0; i<size; i++)
					{
						std::pair<CL_XMLTokenString, CL_XMLTokenString> const & attribute = cur_token.get_attribute_fast(i);
						element.set_attribute(attribute.first.to_string(), attribute.second.to_string());
					}
				
					if (cur_token.get_variant() == CL_XMLToken::BEGIN)
						node_stack.push(element);
				}
				else
				{
					node_stack.pop();
					if (node_stack.empty()) throw CL_Error("Malformed XML tree!");
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

			cur_token = tokenizer.next();
		}
	}
	catch (CL_Error e)
	{
		for (std::vector<CL_DomNode>::size_type i = 0; i < result.size(); i++)
		{
			insert_point.remove_child(result[i]);
		}
		throw e;
	}
	return result;
}

void CL_DomDocument::save(CL_OutputSource *output, bool delete_output, bool insert_whitespace)
{
	CL_XMLWriter writer(output, delete_output);
	writer.set_insert_whitespace(insert_whitespace);

	std::stack<CL_DomNode> node_stack;
	CL_DomNode cur_node = get_first_child();
	while (!cur_node.is_null())
	{
		// Create opening node:
		CL_XMLTokenSave opening_node;
		opening_node.set_type((CL_XMLToken::TokenType) cur_node.get_node_type());
		opening_node.set_variant(cur_node.has_child_nodes() ? CL_XMLToken::BEGIN : CL_XMLToken::SINGLE);
		opening_node.set_name(cur_node.get_node_name());
		opening_node.set_value(cur_node.get_node_value());
		if (cur_node.is_element())
		{
			for (int i = 0; i < cur_node.impl->attributes.get_length(); ++i)
			{
				opening_node.set_attribute(cur_node.impl->attributes.item(i).to_attr().get_name(),
													cur_node.impl->attributes.item(i).to_attr().get_value());
			}
		}
		writer.write(opening_node);

		// Create any possible child nodes:
		if (cur_node.has_child_nodes())
		{
			node_stack.push(cur_node);
			cur_node = cur_node.get_first_child();
			continue;
		}

		// Create closing nodes until we reach next opening node in tree:
		while (true)
		{
			if (cur_node.has_child_nodes())
			{
				CL_XMLTokenSave closing_node;
				closing_node.set_type((CL_XMLToken::TokenType) cur_node.get_node_type());
				closing_node.set_name(cur_node.get_node_name());
				closing_node.set_variant(CL_XMLToken::END);
				writer.write(closing_node);
			}

			cur_node = cur_node.get_next_sibling();
			if (!cur_node.is_null()) break;
			if (node_stack.empty()) break;

			cur_node = node_stack.top();
			node_stack.pop();
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
