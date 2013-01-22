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
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/CSSLayout/CSSDocument/css_property_value.h"
#include "API/CSSLayout/CSSDocument/css_document.h"
#include "API/CSSLayout/CSSTokenizer/css_token.h"
#include "gui_component_select_node.h"

namespace clan
{

GUIElement::GUIElement(CSSResourceCache *resource_cache) : component(0), parent(0), prev_sibling(0), next_sibling(0), first_child(0), last_child(0), style_needs_update(true), computed_values(resource_cache)
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

const CSSComputedValues &GUIElement::get_css_values() const
{
	if (style_needs_update)
	{
		const_cast<GUIElement*>(this)->update_style();
	}

	return computed_values;
}

void GUIElement::set_component(GUIComponent *new_component)
{
	component = new_component;
}

void GUIElement::set_tag_name(const std::string &name)
{
	if (tag_name != name)
	{
		tag_name = name;
		set_style_needs_update();
	}
}

void GUIElement::set_class(const std::string &name)
{
	if (class_string != name)
	{
		class_string = name;
		set_style_needs_update();
	}
}

void GUIElement::set_id(const std::string &name)
{
	if (id != name)
	{
		id = name;
		set_style_needs_update();
	}
}

void GUIElement::set_parent(GUIElement *new_parent)
{
	if (new_parent == parent)
	{
		return;
	}

	if (!parent)	// Initial set_parent
	{
		parent = new_parent;

		if (parent->last_child)
		{
			parent->last_child->next_sibling = this;
			prev_sibling = parent->last_child;
			parent->last_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
		}
	}
	else
	{
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

	if (new_parent)
	{
		computed_values.set_parent(new_parent->computed_values);
	}
	else
	{
		computed_values.set_parent(CSSComputedValues());
	}

	set_style_needs_update();
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

bool GUIElement::set_pseudo_class(const std::string &name, bool enable)
{
	for (size_t i = 0; i < pseudo_classes.size(); i++)
	{
		if (pseudo_classes[i] == name)
		{
			if (!enable)
			{
				pseudo_classes.erase(pseudo_classes.begin() + i);
				set_style_needs_update();
				component->request_repaint();
				return true;
			}
			return false;
		}
	}
	if (enable)
	{
		pseudo_classes.push_back(name);
		set_style_needs_update();
		component->request_repaint();
		return true;
	}
	return false;
}

void GUIElement::set_style_needs_update()
{
	style_needs_update = true;

	GUIElement *child = get_first_child();
	while (child)
	{
		child->set_style_needs_update();
		child = child->get_next_sibling();
	}

	sig_style_changed.invoke();
}

void GUIElement::update_style()
{
	GUIComponentSelectNode select_node(this);
	computed_values.set_specified_values(component->get_gui_manager().get_css_document().select(&select_node));

//	if (!func_apply_properties.is_null())
//		func_apply_properties.invoke(css_properties);

	style_needs_update = false;
}

std::string GUIElement::get_property(const std::string &property, const std::string &default_value) const
{
	// TODO: Decode all token types
	// TODO: If property not found, search standard types?
	const CSSComputedValues &computed_values = get_css_values();
	const std::vector<CSSValueGeneric> &generic_values = computed_values.get_box().generic_values;

	for (size_t cnt = 0; cnt < generic_values.size(); cnt++)
	{
		if (generic_values[cnt].property_name == property)
		{
			const std::vector<CSSToken> &tokens = generic_values[cnt].tokens;
			if (tokens.empty())
				return std::string("");
			return tokens[0].value;
		}
	}
	return default_value;
}

}
