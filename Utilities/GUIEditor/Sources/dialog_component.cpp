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

#include "precomp.h"
#include "dialog_component.h"
#include "dialog_container.h"
#include "dialog_document.h"

DialogComponent::DialogComponent(DialogContainer *container, CL_DomElement element, int id)
: owner(container), element(element), id(id)
{
	CL_DomNode cur = element.get_first_child();
	while (!cur.is_null())
	{
		if (cur.is_element())
		{
			CL_DomElement element_container = cur.to_element();
			CL_DomNamedNodeMap attributes = element.get_attributes();
			if (element_container.get_namespace_uri() == DialogDocument::namespace_uri && element_container.get_local_name() == "container")
			{
				containers.push_back(new DialogContainer(get_document(), element_container));
			}
		}
		cur = cur.get_next_sibling();
	}
}

CL_StringRef DialogComponent::get_type() const
{
	return element.get_local_name();
}

CL_StringRef DialogComponent::get_name() const
{
	return element.get_attribute_ns(DialogDocument::namespace_uri, "name");
}

std::vector<DialogProperty> DialogComponent::get_properties() const
{
	CL_DomNamedNodeMap attributes = element.get_attributes();
	int num_properties = attributes.get_length();

	std::vector<DialogProperty> properties;
	properties.reserve(num_properties);

	for (int i = 0; i < num_properties; i++)
	{
		CL_DomAttr attr = attributes.item(i).to_attr();
		if (attr.get_namespace_uri() == DialogDocument::namespace_uri)
		{
			DialogProperty property;
			property.name = attr.get_local_name();
			property.value = attr.get_value();
			properties.push_back(property);
		}
	}

	return properties;
}

CL_StringRef DialogComponent::get_property(const CL_StringRef &name, const CL_StringRef &default_value) const
{
	return element.get_attribute_ns(DialogDocument::namespace_uri, name, default_value);
}

void DialogComponent::set_property(const CL_StringRef &name, const CL_StringRef &value)
{
	CL_String prefix = element.get_prefix();
	if (!prefix.empty())
		prefix.append(":");
	element.set_attribute_ns(DialogDocument::namespace_uri, prefix + name, value);
}

DialogContainer *DialogComponent::create_container()
{
	CL_String prefix = element.get_prefix();
	if (!prefix.empty())
		prefix.append(":");
	CL_DomElement element_container = element.get_owner_document().create_element_ns(DialogDocument::namespace_uri, prefix + "container");
	element.append_child(element_container);
	DialogContainer *container = new DialogContainer(get_document(), element_container);
	containers.push_back(container);
	return container;
}

DialogDocument *DialogComponent::get_document()
{
	return owner->get_document();
}

void DialogComponent::destroy_component()
{
	owner->destroy_component(id);
}
