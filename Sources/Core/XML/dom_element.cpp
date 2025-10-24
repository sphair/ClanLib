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
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_node_list.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_document.h"
#include "dom_node_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement construction:

CL_DomElement::CL_DomElement()
{
}

CL_DomElement::CL_DomElement(CL_DomDocument &doc, const std::string &tag_name)
: CL_DomNode(doc, ELEMENT_NODE)
{
	impl->node_name = tag_name;
}

CL_DomElement::CL_DomElement(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomElement::~CL_DomElement()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement attributes:

std::string CL_DomElement::get_tag_name() const
{
	if (impl) return impl->node_name;
	return std::string();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement operations:

bool CL_DomElement::has_attribute(const std::string &name) const
{
	if (impl)
	{
		for(int i = 0; i < (int)impl->attributes.get_length(); ++i)
			if (impl->attributes.item(i).to_attr().get_name() == name)
				return true;
	}
	return false;
}

std::string CL_DomElement::get_attribute(const std::string &name) const
{
	if (impl)
	{
		for(int i = 0; i < (int)impl->attributes.get_length(); ++i)
			if (impl->attributes.item(i).to_attr().get_name() == name)
				return impl->attributes.item(i).to_attr().get_value();
	}
	return std::string();
}

std::string CL_DomElement::get_attribute(const std::string &name, const std::string &default_value) const
{
	if (impl)
	{
		for(int i = 0; i < (int)impl->attributes.get_length(); ++i)
			if (impl->attributes.item(i).to_attr().get_name() == name)
				return impl->attributes.item(i).to_attr().get_value();
	}
	return default_value;
}

void CL_DomElement::set_attribute(const std::string &name, const std::string &value)
{
	if (impl)
	{
		for(int i = 0; i < (int)impl->attributes.get_length(); ++i)
			if (impl->attributes.item(i).to_attr().get_name() == name)
			{
				impl->attributes.item(i).to_attr().set_value(value);
				return;
			}

		CL_DomAttr attr(get_owner_document(), name);
		impl->attributes.set_named_item(attr);
		attr.impl->node_value = value;
	}
}

void CL_DomElement::remove_attribute(const std::string &name)
{
	if (impl)
	{
		impl->attributes.remove_named_item(name);
	}
}

CL_DomNodeList CL_DomElement::get_elements_by_tag_name(const std::string &name)
{
	return CL_DomNodeList(*this, name);
}

std::string CL_DomElement::get_text() const
{
	std::string str;
	if (has_child_nodes() == false)
		return str;

	CL_DomNode cur = get_first_child();
	while (!cur.is_null())
	{
		if (cur.is_text())
			str.append(cur.get_node_value());
		cur = cur.get_next_sibling();
	}
	return str;
}

void CL_DomElement::normalize()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomElement implementation:
