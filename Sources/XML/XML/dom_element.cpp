/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/XML/dom_element.h"
#include "API/XML/dom_node_list.h"
#include "API/XML/dom_attr.h"
#include "API/XML/dom_document.h"
#include "API/XML/dom_named_node_map.h"
#include "API/XML/dom_text.h"
#include "API/Core/Text/string_help.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

namespace clan
{
	DomElement::DomElement()
	{
	}

	DomElement::DomElement(
		DomDocument &doc,
		const DomString &tag_name,
		const DomString &namespace_uri)
		: DomNode(doc, ELEMENT_NODE)
	{
		DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
		impl->get_tree_node()->set_node_name(doc_impl, tag_name);
		impl->get_tree_node()->set_namespace_uri(doc_impl, namespace_uri);
	}

	DomElement::DomElement(const std::shared_ptr<DomNode_Impl> &impl) : DomNode(impl)
	{
	}

	DomElement::~DomElement()
	{
	}

	DomString DomElement::get_tag_name() const
	{
		return get_node_name();
	}

	bool DomElement::has_attribute(const DomString &name) const
	{
		if (impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			const DomTreeNode *tree_node = impl->get_tree_node();
			unsigned int cur_index = tree_node->first_attribute;
			const DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
			while (cur_attribute)
			{
				if (cur_attribute->get_node_name() == name)
					return true;

				cur_index = cur_attribute->next_sibling;
				cur_attribute = cur_attribute->get_next_sibling(doc_impl);
			}
			return false;
		}
		return false;
	}

	bool DomElement::has_attribute_ns(
		const DomString &namespace_uri,
		const DomString &local_name) const
	{
		if (impl)
		{
			DomNode attribute = get_attributes().get_named_item_ns(namespace_uri, local_name);
			return attribute.is_attr();
		}
		return false;
	}

	DomString DomElement::get_attribute(const DomString &name) const
	{
		if (impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			const DomTreeNode *tree_node = impl->get_tree_node();
			unsigned int cur_index = tree_node->first_attribute;
			const DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
			while (cur_attribute)
			{
				if (cur_attribute->get_node_name() == name)
					return cur_attribute->get_node_value();

				cur_index = cur_attribute->next_sibling;
				cur_attribute = cur_attribute->get_next_sibling(doc_impl);
			}
			return DomString();
		}
		return DomString();
	}

	DomString DomElement::get_attribute(const DomString &name, const DomString &default_value) const
	{
		if (impl)
		{
			DomDocument_Impl *doc_impl = (DomDocument_Impl *)impl->owner_document.lock().get();
			const DomTreeNode *tree_node = impl->get_tree_node();
			unsigned int cur_index = tree_node->first_attribute;
			const DomTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl);
			while (cur_attribute)
			{
				if (cur_attribute->get_node_name() == name)
					return cur_attribute->get_node_value();

				cur_index = cur_attribute->next_sibling;
				cur_attribute = cur_attribute->get_next_sibling(doc_impl);
			}
			return default_value;
		}
		return default_value;
	}

	DomString DomElement::get_attribute_ns(
		const DomString &namespace_uri,
		const DomString &local_name) const
	{
		if (impl)
		{
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
					return cur_attribute->get_node_value();

				cur_index = cur_attribute->next_sibling;
				cur_attribute = cur_attribute->get_next_sibling(doc_impl);
			}
			return DomString();
		}
		return DomString();
	}

	DomString DomElement::get_attribute_ns(
		const DomString &namespace_uri,
		const DomString &local_name,
		const DomString &default_value) const
	{
		if (impl)
		{
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
					return cur_attribute->get_node_value();

				cur_index = cur_attribute->next_sibling;
				cur_attribute = cur_attribute->get_next_sibling(doc_impl);
			}
			return default_value;
		}
		return default_value;
	}

	void DomElement::set_attribute(const DomString &name, const DomString &value)
	{
		if (impl)
		{
			DomNamedNodeMap attributes = get_attributes();
			DomAttr attribute = attributes.get_named_item(name).to_attr();
			if (attribute.is_attr())
			{
				attribute.set_node_value(value);
			}
			else
			{
				attribute = get_owner_document().create_attribute(name);
				attribute.set_node_value(value);
				attributes.set_named_item(attribute);
			}
		}
	}

	void DomElement::set_attribute_ns(
		const DomString &namespace_uri,
		const DomString &qualified_name,
		const DomString &value)
	{
		if (impl)
		{
			DomNamedNodeMap attributes = get_attributes();
			DomAttr attribute = attributes.get_named_item_ns(namespace_uri, qualified_name).to_attr();
			if (attribute.is_attr())
			{
				attribute.set_node_value(value);
			}
			else
			{
				attribute = get_owner_document().create_attribute_ns(namespace_uri, qualified_name);
				attribute.set_node_value(value);
				attributes.set_named_item_ns(attribute);
			}
		}
	}

	void DomElement::remove_attribute(const DomString &name)
	{
		if (impl)
		{
			DomNamedNodeMap attributes = get_attributes();
			attributes.remove_named_item(name);
		}
	}

	void DomElement::remove_attribute_ns(
		const DomString &namespace_uri,
		const DomString &local_name)
	{
		if (impl)
		{
			DomNamedNodeMap attributes = get_attributes();
			attributes.remove_named_item_ns(namespace_uri, local_name);
		}
	}

	DomAttr DomElement::get_attribute_node(const DomString &name) const
	{
		if (impl)
		{
			DomNamedNodeMap attributes = get_attributes();
			DomAttr attribute = attributes.get_named_item(name).to_attr();
			return attribute;
		}
		return DomAttr();
	}

	DomAttr DomElement::get_attribute_node_ns(
		const DomString &namespace_uri,
		const DomString &local_name) const
	{
		if (impl)
		{
			DomNamedNodeMap attributes = get_attributes();
			DomAttr attribute = attributes.get_named_item_ns(namespace_uri, local_name).to_attr();
			return attribute;
		}
		return DomAttr();
	}

	DomAttr DomElement::set_attribute_node(const DomAttr &attr)
	{
		if (impl)
			return get_attributes().set_named_item(attr).to_attr();
		return DomAttr();
	}

	DomAttr DomElement::set_attribute_node_ns(const DomAttr &attr)
	{
		if (impl)
			return get_attributes().set_named_item_ns(attr).to_attr();
		return DomAttr();
	}

	DomNodeList DomElement::get_elements_by_tag_name(const DomString &name)
	{
		return DomNodeList(*this, name);
	}

	DomNodeList DomElement::get_elements_by_tag_name_ns(
		const DomString &namespace_uri,
		const DomString &local_name)
	{
		return DomNodeList(*this, namespace_uri, local_name, true);
	}

	std::string DomElement::get_text() const
	{
		std::string str;
		if (has_child_nodes() == false)
			return str;

		DomNode cur = get_first_child();
		while (!cur.is_null())
		{
			if (cur.is_text() || cur.is_cdata_section())
				str.append(cur.get_node_value());
			if (cur.is_element())
				str.append(cur.to_element().get_text());
			cur = cur.get_next_sibling();
		}
		return str;
	}

	DomString DomElement::get_child_string(const DomString &name, const DomString &default_value) const
	{
		DomElement element = named_item(name).to_element();
		if (!element.is_null() && element.get_first_child().is_text())
			return element.get_first_child().to_text().get_node_value();
		else
			return default_value;
	}

	DomString DomElement::get_child_string_ns(const DomString &namespace_uri, const DomString &local_name, const DomString &default_value) const
	{
		DomElement element = named_item_ns(namespace_uri, local_name).to_element();
		if (!element.is_null() && element.get_first_child().is_text())
			return element.get_first_child().to_text().get_node_value();
		else
			return default_value;
	}

	int DomElement::get_child_int(const DomString &name, int default_value) const
	{
		DomString value = get_child_string(name);
		if (!value.empty())
			return StringHelp::text_to_int(value);
		else
			return default_value;
	}

	int DomElement::get_child_int_ns(const DomString &namespace_uri, const DomString &local_name, int default_value) const
	{
		DomString value = get_child_string_ns(namespace_uri, local_name);
		if (!value.empty())
			return StringHelp::text_to_int(value);
		else
			return default_value;
	}

	bool DomElement::get_child_bool(const DomString &name, bool default_value) const
	{
		DomString value = get_child_string(name);
		if (!value.empty())
			return value == "true";
		else
			return default_value;
	}

	bool DomElement::get_child_bool_ns(const DomString &namespace_uri, const DomString &local_name, bool default_value) const
	{
		DomString value = get_child_string_ns(namespace_uri, local_name);
		if (!value.empty())
			return value == "true";
		else
			return default_value;
	}

	void DomElement::set_child_string(const DomString &name, const DomString &value)
	{
		DomElement element = named_item(name).to_element();
		if (element.is_null())
		{
			element = get_owner_document().create_element(name);
			append_child(element);
		}

		while (!element.get_first_child().is_null())
		{
			DomNode my_child = element.get_first_child();
			element.remove_child(my_child);
		}

		DomText dom_text = get_owner_document().create_text_node(value);
		element.append_child(dom_text);
	}

	void DomElement::set_child_string_ns(const DomString &namespace_uri, const DomString &qualified_name, const DomString &value)
	{
		DomString local_name;
		DomString::size_type pos = qualified_name.find(L':');
		if (pos != DomString::npos)
			local_name = qualified_name.substr(pos + 1);
		else
			local_name = qualified_name;

		DomElement element = named_item_ns(namespace_uri, local_name).to_element();
		if (element.is_null())
		{
			element = get_owner_document().create_element_ns(namespace_uri, qualified_name);
			append_child(element);
		}

		DomText dom_text = get_owner_document().create_text_node(value);
		if (element.get_first_child().is_text())
		{
			DomNode temp_domnode = element.get_first_child();
			replace_child(dom_text, temp_domnode);
		}
		else
		{
			element.append_child(dom_text);
		}
	}

	void DomElement::set_child_int(const DomString &name, int value)
	{
		set_child_string(name, StringHelp::int_to_text(value));
	}

	void DomElement::set_child_int_ns(const DomString &namespace_uri, const DomString &qualified_name, int value)
	{
		set_child_string_ns(namespace_uri, qualified_name, StringHelp::int_to_text(value));
	}

	void DomElement::set_child_bool(const DomString &name, bool value)
	{
		set_child_string(name, value ? "true" : "false");
	}

	void DomElement::set_child_bool_ns(const DomString &namespace_uri, const DomString &qualified_name, bool value)
	{
		set_child_string_ns(namespace_uri, qualified_name, value ? "true" : "false");
	}

	DomElement DomElement::get_first_child_element() const
	{
		DomNode node = get_first_child();
		while (!node.is_null() && !node.is_element())
			node = node.get_next_sibling();
		return node.to_element();
	}

	DomElement DomElement::get_next_sibling_element() const
	{
		DomNode node = get_next_sibling();
		while (!node.is_null() && !node.is_element())
			node = node.get_next_sibling();
		return node.to_element();
	}

	int DomElement::get_attribute_int(const DomString &name, int default_value) const
	{
		DomString value = get_attribute(name);
		if (!value.empty())
			return StringHelp::text_to_int(value);
		else
			return default_value;
	}

	int DomElement::get_attribute_int_ns(const DomString &namespace_uri, const DomString &local_name, int default_value) const
	{
		DomString value = get_attribute_ns(namespace_uri, local_name);
		if (!value.empty())
			return StringHelp::text_to_int(value);
		else
			return default_value;
	}

	bool DomElement::get_attribute_bool(const DomString &name, bool default_value) const
	{
		DomString value = get_attribute(name);
		if (!value.empty())
			return value == "true";
		else
			return default_value;
	}

	bool DomElement::get_attribute_bool_ns(const DomString &namespace_uri, const DomString &local_name, bool default_value) const
	{
		DomString value = get_attribute_ns(namespace_uri, local_name);
		if (!value.empty())
			return value == "true";
		else
			return default_value;
	}

	float DomElement::get_attribute_float(const DomString &name, float default_value) const
	{
		DomString value = get_attribute(name);
		if (!value.empty())
			return StringHelp::text_to_float(value);
		else
			return default_value;
	}

	float DomElement::get_attribute_float_ns(const DomString &namespace_uri, const DomString &local_name, float default_value) const
	{
		DomString value = get_attribute_ns(namespace_uri, local_name);
		if (!value.empty())
			return StringHelp::text_to_float(value);
		else
			return default_value;
	}

	void DomElement::set_attribute_int(const DomString &name, int value)
	{
		set_attribute(name, StringHelp::int_to_text(value));
	}

	void DomElement::set_attribute_int_ns(const DomString &namespace_uri, const DomString &qualified_name, int value)
	{
		set_attribute_ns(namespace_uri, qualified_name, StringHelp::int_to_text(value));
	}

	void DomElement::set_attribute_bool(const DomString &name, bool value)
	{
		set_attribute(name, value ? "true" : "false");
	}

	void DomElement::set_attribute_bool_ns(const DomString &namespace_uri, const DomString &qualified_name, bool value)
	{
		set_attribute_ns(namespace_uri, qualified_name, value ? "true" : "false");
	}

	void DomElement::set_attribute_float(const DomString &name, float value, int num_decimal_places)
	{
		set_attribute(name, StringHelp::float_to_text(value, num_decimal_places));
	}

	void DomElement::set_attribute_float_ns(const DomString &namespace_uri, const DomString &qualified_name, float value, int num_decimal_places)
	{
		set_attribute_ns(namespace_uri, qualified_name, StringHelp::float_to_text(value, num_decimal_places));
	}
}
