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
#include "dialog_container.h"
#include "dialog_document.h"
#include "dialog_component.h"

DialogContainer::DialogContainer(DialogDocument *doc, CL_DomElement element_gui)
: doc(doc), element_gui(element_gui)
{
	CL_DomNode cur = element_gui.get_first_child();
	while (!cur.is_null())
	{
		if (cur.is_element())
		{
			CL_DomElement element = cur.to_element();
			CL_DomNamedNodeMap attributes = element.get_attributes();
			if (element.get_namespace_uri() == DialogDocument::namespace_uri)
			{
				children.push_back(new DialogComponent(this, element, doc->next_free_id++));
			}
		}
		cur = cur.get_next_sibling();
	}
}

DialogContainer::~DialogContainer()
{
	std::vector<DialogComponent>::size_type index;
	for (index = 0; index < children.size(); index++)
	{
		delete children[index];
	}
}

DialogComponent *DialogContainer::create_component(const CL_StringRef &type, const CL_StringRef &name)
{
	CL_String prefix = element_gui.get_prefix();
	if (!prefix.empty())
		prefix.append(":");
	CL_DomElement element = element_gui.get_owner_document().create_element_ns(DialogDocument::namespace_uri, prefix + name);
	element_gui.append_child(element);
	DialogComponent *component = new DialogComponent(this, element, doc->next_free_id++);
	children.push_back(component);
	return component;
}

void DialogContainer::destroy_component(int id)
{
	std::vector<DialogComponent>::size_type index;
	for (index = 0; index < children.size(); index++)
	{
		if (children[index]->id == id)
		{
			children[index]->element.get_parent_node().remove_child(children[index]->element);
			delete children[index];
			children.erase(children.begin() + index);
			return;
		}
	}
	throw CL_Exception(cl_format("Unknown component ID %1", id));
}
