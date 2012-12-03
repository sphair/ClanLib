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
#include "API/GUI/gui_element.h"
#include "API/CSSLayout/css_property_list.h"
#include "gui_element_impl.h"
#include "Layout/gui_layout_content.h"
#include "Layout/gui_set_initial_used_values.h"
#include "Layout/gui_layout_box_content.h"
#include "Layout/gui_layout_absolute_or_fixed.h"
#include "gui_component_select_node.h"
#include "API/CSSLayout/css_document.h"

namespace clan
{

GUIElement_Impl::GUIElement_Impl(GUIElement *component, GUIElement *parent) : component(component), parent(parent), prev_sibling(0), next_sibling(0), first_child(0), last_child(0)
{
}

GUIElement_Impl::~GUIElement_Impl()
{
	while (last_child)
		delete last_child;

	if (prev_sibling)
		prev_sibling->impl->next_sibling = next_sibling;
	if (next_sibling)
		next_sibling->impl->prev_sibling = prev_sibling;
	
	if (prev_sibling == 0 && parent)
		parent->impl->first_child = next_sibling;
	if (next_sibling == 0 && parent)
		parent->impl->last_child = prev_sibling;
}

void GUIElement_Impl::update_style(CSSResourceCache *resource_cache, CSSDocument &document)
{
	css_properties = CSSBoxProperties();

	GUIComponentSelectNode select_node(component);
	CSSPropertyList sheet_properties = document.select(&select_node);
	css_properties.apply_properties(sheet_properties);

//FIXME:	if (!func_apply_properties.is_null())
//FIXME:		func_apply_properties.invoke(css_properties);

	if (parent)
		css_properties.compute(&parent->impl->css_properties, resource_cache);
	else
		css_properties.compute(0, resource_cache);

//FIXME:	sig_style_changed.invoke();

	GUIElement *cur_child = first_child;
	while (cur_child)
	{
		cur_child->impl->update_style(resource_cache, document);
		cur_child = cur_child->get_next_sibling();
	}

}

}
