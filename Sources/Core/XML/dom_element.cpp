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
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_node_list.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/dom_text.h"
#include "API/Core/Text/string_help.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement construction:

CL_DomElement::CL_DomElement()
{
}

CL_DomElement::CL_DomElement(
	CL_DomDocument &doc,
	const CL_DomString &tag_name,
	const CL_DomString &namespace_uri)
: CL_DomNode(doc, ELEMENT_NODE)
{
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	impl->get_tree_node()->set_node_name(doc_impl, tag_name);
	impl->get_tree_node()->set_namespace_uri(doc_impl, namespace_uri);
}

CL_DomElement::CL_DomElement(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomElement::~CL_DomElement()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement attributes:

CL_DomString CL_DomElement::get_tag_name() const
{
	return get_node_name();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement operations:

bool CL_DomElement::has_attribute(const CL_DomString &name) const
{
	if (impl)
	{
		CL_DomNode attribute = get_attributes().get_named_item(name);
		return attribute.is_attr();
	}
	return false;
}

bool CL_DomElement::has_attribute_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name) const
{
	if (impl)
	{
		CL_DomNode attribute = get_attributes().get_named_item_ns(namespace_uri, local_name);
		return attribute.is_attr();
	}
	return false;
}

CL_DomString CL_DomElement::get_attribute(const CL_DomString &name) const
{
	if (impl)
	{
		CL_DomAttr attribute = get_attributes().get_named_item(name).to_attr();
		return attribute.get_value();
	}
	return CL_DomString();
}

CL_DomString CL_DomElement::get_attribute(const CL_DomString &name, const CL_DomString &default_value) const
{
	if (impl)
	{
		CL_DomAttr attribute = get_attributes().get_named_item(name).to_attr();
		return attribute.is_attr() ? attribute.get_value() : default_value;
	}
	return default_value;
}

CL_DomString CL_DomElement::get_attribute_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name) const
{
	if (impl)
	{
		CL_DomAttr attribute = get_attributes().get_named_item_ns(namespace_uri, local_name).to_attr();
		return attribute.get_value();
	}
	return CL_DomString();
}

CL_DomString CL_DomElement::get_attribute_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name,
	const CL_DomString &default_value) const
{
	if (impl)
	{
		CL_DomAttr attribute = get_attributes().get_named_item_ns(namespace_uri, local_name).to_attr();
		return attribute.is_attr() ? attribute.get_value() : default_value;
	}
	return CL_DomString();
}

void CL_DomElement::set_attribute(const CL_DomString &name, const CL_DomString &value)
{
	if (impl)
	{
		CL_DomNamedNodeMap attributes = get_attributes();
		CL_DomAttr attribute = attributes.get_named_item(name).to_attr();
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

void CL_DomElement::set_attribute_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &qualified_name,
	const CL_DomString &value)
{
	if (impl)
	{
		CL_DomNamedNodeMap attributes = get_attributes();
		CL_DomAttr attribute = attributes.get_named_item_ns(namespace_uri, qualified_name).to_attr();
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

void CL_DomElement::remove_attribute(const CL_DomString &name)
{
	if (impl)
	{
		CL_DomNamedNodeMap attributes = get_attributes();
		attributes.remove_named_item(name);
	}
}

void CL_DomElement::remove_attribute_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name)
{
	if (impl)
	{
		CL_DomNamedNodeMap attributes = get_attributes();
		attributes.remove_named_item_ns(namespace_uri, local_name);
	}
}

CL_DomAttr CL_DomElement::get_attribute_node(const CL_DomString &name) const
{
	if (impl)
	{
		CL_DomNamedNodeMap attributes = get_attributes();
		CL_DomAttr attribute = attributes.get_named_item(name).to_attr();
		return attribute;
	}
	return CL_DomAttr();
}

CL_DomAttr CL_DomElement::get_attribute_node_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name) const
{
	if (impl)
	{
		CL_DomNamedNodeMap attributes = get_attributes();
		CL_DomAttr attribute = attributes.get_named_item_ns(namespace_uri, local_name).to_attr();
		return attribute;
	}
	return CL_DomAttr();
}

CL_DomAttr CL_DomElement::set_attribute_node(const CL_DomAttr &attr)
{
	if (impl)
		return get_attributes().set_named_item(attr).to_attr();
	return CL_DomAttr();
}

CL_DomAttr CL_DomElement::set_attribute_node_ns(const CL_DomAttr &attr)
{
	if (impl)
		return get_attributes().set_named_item_ns(attr).to_attr();
	return CL_DomAttr();
}

CL_DomNodeList CL_DomElement::get_elements_by_tag_name(const CL_DomString &name)
{
	return CL_DomNodeList(*this, name);
}

CL_DomNodeList CL_DomElement::get_elements_by_tag_name_ns(
	const CL_DomString &namespace_uri,
	const CL_DomString &local_name)
{
	return CL_DomNodeList(*this, namespace_uri, local_name, true);
}

CL_String CL_DomElement::get_text() const
{
	CL_String str;
	if (has_child_nodes() == false)
		return str;

	CL_DomNode cur = get_first_child();
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

CL_DomString CL_DomElement::get_child_string(const CL_DomString &name, const CL_DomString &default_value) const
{
	CL_DomElement element = named_item(name).to_element();
	if (!element.is_null() && element.get_first_child().is_text())
		return element.get_first_child().to_text().get_node_value();
	else
		return default_value;
}

CL_DomString CL_DomElement::get_child_string_ns(const CL_DomString &namespace_uri, const CL_DomString &local_name, const CL_DomString &default_value) const
{
	CL_DomElement element = named_item_ns(namespace_uri, local_name).to_element();
	if (!element.is_null() && element.get_first_child().is_text())
		return element.get_first_child().to_text().get_node_value();
	else
		return default_value;
}

int CL_DomElement::get_child_int(const CL_DomString &name, int default_value) const
{
	CL_DomString value = get_child_string(name);
	if (!value.empty())
		return CL_StringHelp::text_to_int(value);
	else
		return default_value;
}

int CL_DomElement::get_child_int_ns(const CL_DomString &namespace_uri, const CL_DomString &local_name, int default_value) const
{
	CL_DomString value = get_child_string_ns(namespace_uri, local_name);
	if (!value.empty())
		return CL_StringHelp::text_to_int(value);
	else
		return default_value;
}

bool CL_DomElement::get_child_bool(const CL_DomString &name, bool default_value) const
{
	CL_DomString value = get_child_string(name);
	if (!value.empty())
		return value == "true";
	else
		return default_value;
}

bool CL_DomElement::get_child_bool_ns(const CL_DomString &namespace_uri, const CL_DomString &local_name, bool default_value) const
{
	CL_DomString value = get_child_string_ns(namespace_uri, local_name);
	if (!value.empty())
		return value == "true";
	else
		return default_value;
}

void CL_DomElement::set_child_string(const CL_DomString &name, const CL_DomString &value)
{
	CL_DomElement element = named_item(name).to_element();
	if (element.is_null())
	{
		element = get_owner_document().create_element(name);
		append_child(element);
	}

	CL_DomText dom_text = get_owner_document().create_text_node(value);
	if (element.get_first_child().is_text())
	{
		CL_DomNode temp_domnode = element.get_first_child();
		replace_child(dom_text, temp_domnode);
	}
	else
	{
		element.append_child(dom_text);
	}
}

void CL_DomElement::set_child_string_ns(const CL_DomString &namespace_uri, const CL_DomString &qualified_name, const CL_DomString &value)
{
	CL_DomString local_name;
	CL_DomString::size_type pos = qualified_name.find(L':');
	if (pos != CL_DomString::npos)
		local_name = qualified_name.substr(pos+1);
	else
		local_name = qualified_name;

	CL_DomElement element = named_item_ns(namespace_uri, local_name).to_element();
	if (element.is_null())
	{
		element = get_owner_document().create_element_ns(namespace_uri, qualified_name);
		append_child(element);
	}

	CL_DomText dom_text = get_owner_document().create_text_node(value);
	if (element.get_first_child().is_text())
	{
		CL_DomNode temp_domnode = element.get_first_child();
		replace_child(dom_text, temp_domnode);
	}
	else
	{
		element.append_child(dom_text);
	}
}

void CL_DomElement::set_child_int(const CL_DomString &name, int value)
{
	set_child_string(name, CL_StringHelp::int_to_text(value));
}

void CL_DomElement::set_child_int_ns(const CL_DomString &namespace_uri, const CL_DomString &qualified_name, int value)
{
	set_child_string_ns(namespace_uri, qualified_name, CL_StringHelp::int_to_text(value));
}

void CL_DomElement::set_child_bool(const CL_DomString &name, bool value)
{
	set_child_string(name, value ? "true" : "false");
}

void CL_DomElement::set_child_bool_ns(const CL_DomString &namespace_uri, const CL_DomString &qualified_name, bool value)
{
	set_child_string_ns(namespace_uri, qualified_name, value ? "true" : "false");
}

CL_DomElement CL_DomElement::get_first_child_element() const
{
	CL_DomNode node = get_first_child();
	while (!node.is_null() && !node.is_element())
		node = node.get_next_sibling();
	return node.to_element();
}

CL_DomElement CL_DomElement::get_next_sibling_element() const
{
	CL_DomNode node = get_next_sibling();
	while (!node.is_null() && !node.is_element())
		node = node.get_next_sibling();
	return node.to_element();
}

int CL_DomElement::get_attribute_int(const CL_DomString &name, int default_value) const
{
	CL_DomString value = get_attribute(name);
	if (!value.empty())
		return CL_StringHelp::text_to_int(value);
	else
		return default_value;
}

int CL_DomElement::get_attribute_int_ns(const CL_DomString &namespace_uri, const CL_DomString &local_name, int default_value) const
{
	CL_DomString value = get_attribute_ns(namespace_uri, local_name);
	if (!value.empty())
		return CL_StringHelp::text_to_int(value);
	else
		return default_value;
}

bool CL_DomElement::get_attribute_bool(const CL_DomString &name, bool default_value) const
{
	CL_DomString value = get_attribute(name);
	if (!value.empty())
		return value == "true";
	else
		return default_value;
}

bool CL_DomElement::get_attribute_bool_ns(const CL_DomString &namespace_uri, const CL_DomString &local_name, bool default_value) const
{
	CL_DomString value = get_attribute_ns(namespace_uri, local_name);
	if (!value.empty())
		return value == "true";
	else
		return default_value;
}

void CL_DomElement::set_attribute_int(const CL_DomString &name, int value)
{
	set_attribute(name, CL_StringHelp::int_to_text(value));
}

void CL_DomElement::set_attribute_int_ns(const CL_DomString &namespace_uri, const CL_DomString &qualified_name, int value)
{
	set_attribute_ns(namespace_uri, qualified_name, CL_StringHelp::int_to_text(value));
}

void CL_DomElement::set_attribute_bool(const CL_DomString &name, bool value)
{
	set_attribute(name, value ? "true" : "false");
}

void CL_DomElement::set_attribute_bool_ns(const CL_DomString &namespace_uri, const CL_DomString &qualified_name, bool value)
{
	set_attribute_ns(namespace_uri, qualified_name, value ? "true" : "false");
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement implementation:
