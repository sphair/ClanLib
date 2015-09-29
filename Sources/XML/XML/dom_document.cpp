/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/XML/dom_document.h"
#include "API/XML/dom_document_type.h"
#include "API/XML/dom_implementation.h"
#include "API/XML/dom_element.h"
#include "API/XML/dom_document_fragment.h"
#include "API/XML/dom_text.h"
#include "API/XML/dom_comment.h"
#include "API/XML/dom_cdata_section.h"
#include "API/XML/dom_processing_instruction.h"
#include "API/XML/dom_attr.h"
#include "API/XML/dom_entity_reference.h"
#include "API/XML/dom_node_list.h"
#include "API/XML/dom_named_node_map.h"
#include "API/XML/xml_tokenizer.h"
#include "API/XML/xml_writer.h"
#include "API/XML/xml_token.h"
#include "dom_document_generic.h"
#include <stack>

namespace clan
{
	DomDocument::DomDocument()
		: DomNode(std::shared_ptr<DomNode_Impl>(new DomDocument_Impl))
	{
		impl->owner_document = impl;
	}

	DomDocument::DomDocument(IODevice &input, bool eat_whitespace)
		: DomNode(std::shared_ptr<DomNode_Impl>(new DomDocument_Impl))
	{
		impl->owner_document = impl;
		load(input, eat_whitespace);
	}

	DomDocument::DomDocument(
		const DomString &namespace_uri,
		const DomString &qualified_name,
		const DomDocumentType &document_type)
		: DomNode(std::shared_ptr<DomNode_Impl>(new DomDocument_Impl))
	{
		impl->owner_document = impl;
		DomElement element = create_element(qualified_name);
		element.set_attribute("xmlns:" + element.get_prefix(), qualified_name);
		append_child(element);

		DomDocument_Impl *doc = dynamic_cast<DomDocument_Impl *>(impl.get());
		const DomDocument_Impl *doctype = dynamic_cast<const DomDocument_Impl *>(document_type.impl.get());
		doc->public_id = doctype->public_id;
		doc->system_id = doctype->system_id;
	}

	DomDocument::DomDocument(const std::shared_ptr<DomNode_Impl> &new_impl)
		: DomNode(new_impl)
	{
		impl->owner_document = impl;
	}

	DomDocument::~DomDocument()
	{
	}

	DomDocumentType DomDocument::get_doctype()
	{
		return DomDocumentType(*this);
	}

	DomImplementation DomDocument::get_implementation()
	{
		return DomImplementation(*this);
	}

	DomElement DomDocument::get_document_element()
	{
		DomNode cur = get_first_child();
		while (!cur.is_null())
		{
			if (cur.is_element())
				return cur.to_element();
			cur = cur.get_next_sibling();
		}
		return DomElement();
	}

	DomElement DomDocument::create_element(const DomString &tag_name)
	{
		return DomElement(*this, tag_name);
	}

	DomElement DomDocument::create_element_ns(
		const DomString &namespace_uri,
		const DomString &qualified_name)
	{
		return DomElement(*this, qualified_name, namespace_uri);
	}

	DomDocumentFragment DomDocument::create_document_fragment()
	{
		return DomDocumentFragment(*this);
	}

	DomText DomDocument::create_text_node(const DomString &data)
	{
		return DomText(*this, data);
	}

	DomComment DomDocument::create_comment(const DomString &data)
	{
		return DomComment(*this, data);
	}

	DomCDATASection DomDocument::create_cdata_section(const DomString &data)
	{
		return DomCDATASection(*this, data);
	}

	DomProcessingInstruction DomDocument::create_processing_instruction(
		const DomString &target,
		const DomString &data)
	{
		return DomProcessingInstruction(*this, target, data);
	}

	DomAttr DomDocument::create_attribute(const DomString &name)
	{
		return DomAttr(*this, name);
	}

	DomAttr DomDocument::create_attribute_ns(
		const DomString &namespace_uri,
		const DomString &qualified_name)
	{
		return DomAttr(*this, qualified_name, namespace_uri);
	}

	DomEntityReference DomDocument::create_entity_reference(const DomString &name)
	{
		return DomEntityReference(*this, name);
	}

	DomNodeList DomDocument::get_elements_by_tag_name(const DomString &tag_name)
	{
		return DomNodeList(*this, tag_name);
	}

	DomNodeList DomDocument::get_elements_by_tag_name_ns(
		const DomString &namespace_uri,
		const DomString &qualified_name)
	{
		return DomNodeList(*this, namespace_uri, qualified_name);
	}

	DomElement DomDocument::get_element_by_id(const DomString &element_id)
	{
		throw Exception("Implement me");
	}

	DomNode DomDocument::import_node(const DomNode &node, bool deep)
	{
		DomNode imported_node;
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
			DomElement import_element = imported_node.to_element();
			DomNamedNodeMap node_attributes = node.get_attributes();
			int size = node_attributes.get_length();
			for (int index = 0; index < size; index++)
			{
				DomNode attr = node_attributes.item(index);
				import_element.set_attribute_node_ns(import_node(attr, deep).to_attr());
			}
		}

		if (deep)
		{
			DomNode cur = node.get_first_child();
			while (!cur.is_null())
			{
				imported_node.append_child(import_node(cur, true));
				cur = cur.get_next_sibling();
			}
		}

		return imported_node;
	}

	std::vector<DomNode> DomDocument::load(
		IODevice &input,
		bool eat_whitespace,
		DomNode insert_point)
	{
		clear_all();

		XMLTokenizer tokenizer(input);
		tokenizer.set_eat_whitespace(eat_whitespace);

		if (insert_point.is_element() == false)
			insert_point = *this;

		std::vector<DomNode> node_stack;
		node_stack.push_back(insert_point);

		std::vector<DomNode> result;
		try
		{
			XMLToken cur_token;
			tokenizer.next(&cur_token);
			while (cur_token.type != XMLToken::NULL_TOKEN)
			{
				switch (cur_token.type)
				{
				case XMLToken::TEXT_TOKEN:
					node_stack.back().append_child(create_text_node(cur_token.value));
					if (node_stack.back() == insert_point)
						result.push_back(node_stack.back().get_last_child());
					break;

				case XMLToken::CDATA_SECTION_TOKEN:
					node_stack.back().append_child(create_cdata_section(cur_token.value));
					if (node_stack.back() == insert_point)
						result.push_back(node_stack.back().get_last_child());
					break;

				case XMLToken::ELEMENT_TOKEN:
					if (cur_token.variant != XMLToken::END)
					{
						DomString namespace_uri = DomDocument_Impl::find_namespace_uri(cur_token.name, cur_token, node_stack.back());
						DomElement element = create_element_ns(namespace_uri, cur_token.name);
						node_stack.back().append_child(element);
						if (node_stack.back() == insert_point)
							result.push_back(node_stack.back().get_last_child());

						int size = (int)cur_token.attributes.size();
						for (int i = 0; i < size; i++)
						{
							XMLToken::Attribute &attribute = cur_token.attributes[i];
							DomString attribute_namespace_uri = DomDocument_Impl::find_namespace_uri(attribute.first, cur_token, node_stack.back());
							element.set_attribute_ns(
								attribute_namespace_uri,
								attribute.first,
								attribute.second);
						}

						if (cur_token.variant == XMLToken::BEGIN)
							node_stack.push_back(element);
					}
					else
					{
						node_stack.pop_back();
						if (node_stack.empty()) throw Exception("Malformed XML tree!");
					}
					break;

				case XMLToken::NULL_TOKEN:
					break;

				case XMLToken::ENTITY_REFERENCE_TOKEN:
					break;

				case XMLToken::ENTITY_TOKEN:
					break;

				case XMLToken::COMMENT_TOKEN:
					node_stack.back().append_child(create_comment(cur_token.value));
					if (node_stack.back() == insert_point)
						result.push_back(node_stack.back().get_last_child());
					break;

				case XMLToken::DOCUMENT_TYPE_TOKEN:
					break;

				case XMLToken::NOTATION_TOKEN:
					break;

				case XMLToken::PROCESSING_INSTRUCTION_TOKEN:
					node_stack.back().append_child(create_processing_instruction(cur_token.name, cur_token.value));
					if (node_stack.back() == insert_point)
						result.push_back(node_stack.back().get_last_child());
					break;
				}

				tokenizer.next(&cur_token);
			}
		}
		catch (const Exception& e)
		{
			for (auto & elem : result)
			{
				insert_point.remove_child(elem);
			}
			throw;
		}
		return result;
	}

	void DomDocument::save(IODevice &output, bool insert_whitespace)
	{
		XMLWriter writer(output);
		writer.set_insert_whitespace(insert_whitespace);

		std::vector<DomNode> node_stack;
		DomNode cur_node = get_first_child();
		while (!cur_node.is_null())
		{
			// Create opening node:
			XMLToken opening_node;
			opening_node.type = (XMLToken::TokenType) cur_node.get_node_type();
			opening_node.variant = cur_node.has_child_nodes() ? XMLToken::BEGIN : XMLToken::SINGLE;
			opening_node.name = cur_node.get_node_name();
			opening_node.value = cur_node.get_node_value();
			if (cur_node.is_element())
			{
				DomNamedNodeMap attributes = cur_node.get_attributes();
				int length = attributes.get_length();
				for (int i = 0; i < length; ++i)
				{
					DomAttr attribute = attributes.item(i).to_attr();
					opening_node.attributes.push_back(
						XMLToken::Attribute(
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
					XMLToken closing_node;
					closing_node.type = (XMLToken::TokenType) cur_node.get_node_type();
					closing_node.name = cur_node.get_node_name();
					closing_node.variant = XMLToken::END;
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

	void DomDocument::clear_all()
	{
		while (!get_first_child().is_null())
		{
			DomNode node = get_first_child();
			remove_child(node);
		}
	}
}
