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
#include "gui_element_impl.h"

namespace clan
{

GUIElement::GUIElement(GUIElement *parent)
{
	impl = std::shared_ptr<GUIElement_Impl>(new GUIElement_Impl(this, parent));
}

const GUIElement *GUIElement::get_first_child() const
{
	return impl->first_child;
}

GUIElement *GUIElement::get_first_child()
{
	return impl->first_child;
}

const GUIElement *GUIElement::get_last_child() const
{
	return impl->last_child;
}

GUIElement *GUIElement::get_last_child()
{
	return impl->last_child;
}

const GUIElement *GUIElement::get_previous_sibling() const
{
	return impl->prev_sibling;
}

GUIElement *GUIElement::get_previous_sibling()
{
	return impl->prev_sibling;
}

const GUIElement *GUIElement::get_next_sibling() const
{
	return impl->next_sibling;
}

GUIElement *GUIElement::get_next_sibling()
{
	return impl->next_sibling;
}

const CSSBoxProperties &GUIElement::get_css_properties() const
{
	return impl->css_properties;
}

CSSBoxProperties &GUIElement::get_css_properties()
{
	return impl->css_properties;
}

const GUIElement *GUIElement::get_parent_component() const
{
	return impl->parent;
}

std::string GUIElement::get_tag_name() const
{
	return impl->tag_name;
}

std::string GUIElement::get_id() const
{
	return impl->id;
}

std::string GUIElement::get_class() const
{
	return impl->class_string;
}

bool GUIElement::get_pseudo_class(const std::string &name) const
{
	for (size_t i = 0; i < impl->pseudo_classes.size(); i++)
	{
		if (impl->pseudo_classes[i] == name)
			return true;
	}
	return false;
}

std::vector<std::string> GUIElement::get_pseudo_classes() const
{
	return impl->pseudo_classes;
}

GUIElement *GUIElement::get_parent_component()
{
	return impl->parent;
}

void GUIElement::update_style(CSSResourceCache *resource_cache, CSSDocument &document)
{
	impl->update_style(resource_cache, document);
}

void GUIElement::set_parent(GUIElement *new_parent)
{
	if (new_parent == impl->parent)
	{
		return;
	}

	// 1. Remove this component from parents child list.
	if (impl->parent->impl->first_child == impl->parent->impl->last_child)
	{
		impl->parent->impl->first_child = 0;
		impl->parent->impl->last_child = 0;
	}
	else if (impl->parent->impl->first_child == this)
	{
		impl->parent->impl->first_child = impl->next_sibling;
		impl->next_sibling->impl->prev_sibling = 0;
	}
	else if (impl->parent->impl->last_child == this)
	{
		impl->prev_sibling->impl->next_sibling = 0;
		impl->parent->impl->last_child = impl->prev_sibling;
	}
	else
	{
		impl->prev_sibling->impl->next_sibling = impl->next_sibling;
		impl->next_sibling->impl->prev_sibling = impl->prev_sibling;
	}

	// 2. Set this component as last child of new parent.
	if (new_parent->impl->first_child == 0)
	{
		new_parent->impl->first_child = this;
		impl->prev_sibling = 0;
	}
	else if (new_parent->impl->last_child != 0)
	{
		new_parent->impl->last_child->impl->next_sibling = this;
		impl->prev_sibling = new_parent->impl->last_child;
	}
	impl->next_sibling = 0;
	new_parent->impl->last_child = this;
	impl->parent = new_parent;
}


void GUIElement::set_tag_name(const std::string &name)
{
	impl->tag_name = name;
}
	
void GUIElement::set_class(const std::string &name)
{
	impl->class_string = name;
}

void GUIElement::set_id(const std::string &name)
{
	impl->id = name;
}

bool GUIElement::set_pseudo_class(const std::string &name, bool enable)
{
	for (size_t i = 0; i < impl->pseudo_classes.size(); i++)
	{
		if (impl->pseudo_classes[i] == name)
		{
			if (!enable)
			{
				impl->pseudo_classes.erase(impl->pseudo_classes.begin() + i);
				return true;
			}
			return false;
		}
	}
	if (enable)
	{
		impl->pseudo_classes.push_back(name);
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
