/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "gui_element.h"
#include "API/CSSLayout/css_property_list.h"
#include "gui_component_select_node.h"
#include "API/CSSLayout/css_document.h"

namespace clan
{

GUIElement::GUIElement(GUIElement *parent) : parent(parent), prev_sibling(0), next_sibling(0), first_child(0), last_child(0)
{
}

GUIElement::~GUIElement()
{
	while (last_child)
		delete last_child;

	if (prev_sibling)
		prev_sibling->next_sibling = next_sibling;
	if (next_sibling)
		next_sibling->prev_sibling = prev_sibling;
	
	if (prev_sibling == 0 && parent)
		parent->first_child = next_sibling;
	if (next_sibling == 0 && parent)
		parent->last_child = prev_sibling;
}

const GUIElement *GUIElement::get_first_child() const
{
	return first_child;
}

GUIElement *GUIElement::get_first_child()
{
	return first_child;
}

const GUIElement *GUIElement::get_last_child() const
{
	return last_child;
}

GUIElement *GUIElement::get_last_child()
{
	return last_child;
}

const GUIElement *GUIElement::get_previous_sibling() const
{
	return prev_sibling;
}

GUIElement *GUIElement::get_previous_sibling()
{
	return prev_sibling;
}

const GUIElement *GUIElement::get_next_sibling() const
{
	return next_sibling;
}

GUIElement *GUIElement::get_next_sibling()
{
	return next_sibling;
}

const CSSBoxProperties &GUIElement::get_css_properties() const
{
	return css_properties;
}

CSSBoxProperties &GUIElement::get_css_properties()
{
	return css_properties;
}

const GUIElement *GUIElement::get_parent_component() const
{
	return parent;
}

std::string GUIElement::get_tag_name() const
{
	return tag_name;
}

std::string GUIElement::get_id() const
{
	return id;
}

std::string GUIElement::get_class() const
{
	return class_string;
}

bool GUIElement::get_pseudo_class(const std::string &name) const
{
	for (size_t i = 0; i < pseudo_classes.size(); i++)
	{
		if (pseudo_classes[i] == name)
			return true;
	}
	return false;
}

std::vector<std::string> GUIElement::get_pseudo_classes() const
{
	return pseudo_classes;
}

GUIElement *GUIElement::get_parent_component()
{
	return parent;
}

void GUIElement::update_style(CSSResourceCache *resource_cache, CSSDocument &document)
{
	css_properties = CSSBoxProperties();

	GUIComponentSelectNode select_node(this);
	CSSPropertyList sheet_properties = document.select(&select_node);
	css_properties.apply_properties(sheet_properties);

//FIXME:	if (!func_apply_properties.is_null())
//FIXME:		func_apply_properties.invoke(css_properties);

	if (parent)
		css_properties.compute(&parent->css_properties, resource_cache);
	else
		css_properties.compute(0, resource_cache);

//FIXME:	sig_style_changed.invoke();

	GUIElement *cur_child = first_child;
	while (cur_child)
	{
		cur_child->update_style(resource_cache, document);
		cur_child = cur_child->get_next_sibling();
	}

}

void GUIElement::set_parent(GUIElement *new_parent)
{
	if (new_parent == parent)
	{
		return;
	}

	// 1. Remove this component from parents child list.
	if (parent->first_child == parent->last_child)
	{
		parent->first_child = 0;
		parent->last_child = 0;
	}
	else if (parent->first_child == this)
	{
		parent->first_child = next_sibling;
		next_sibling->prev_sibling = 0;
	}
	else if (parent->last_child == this)
	{
		prev_sibling->next_sibling = 0;
		parent->last_child = prev_sibling;
	}
	else
	{
		prev_sibling->next_sibling = next_sibling;
		next_sibling->prev_sibling = prev_sibling;
	}

	// 2. Set this component as last child of new parent.
	if (new_parent->first_child == 0)
	{
		new_parent->first_child = this;
		prev_sibling = 0;
	}
	else if (new_parent->last_child != 0)
	{
		new_parent->last_child->next_sibling = this;
		prev_sibling = new_parent->last_child;
	}
	next_sibling = 0;
	new_parent->last_child = this;
	parent = new_parent;
}


void GUIElement::set_tag_name(const std::string &name)
{
	tag_name = name;
}
	
void GUIElement::set_class(const std::string &name)
{
	class_string = name;
}

void GUIElement::set_id(const std::string &name)
{
	id = name;
}

bool GUIElement::set_pseudo_class(const std::string &name, bool enable)
{
	for (size_t i = 0; i < pseudo_classes.size(); i++)
	{
		if (pseudo_classes[i] == name)
		{
			if (!enable)
			{
				pseudo_classes.erase(pseudo_classes.begin() + i);
				return true;
			}
			return false;
		}
	}
	if (enable)
	{
		pseudo_classes.push_back(name);
		return true;
	}
	return false;
}

GUIElement::GUIElement(GUIElement &other)
{
	throw Exception("GUIElement copy construction disallowed");
}

GUIElement &GUIElement::operator =(const GUIElement &other)
{
	throw Exception("GUIElement operator = disallowed");
}


}
