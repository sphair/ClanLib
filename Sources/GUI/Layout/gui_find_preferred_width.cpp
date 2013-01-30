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
*/

#include "GUI/precomp.h"
#include "gui_find_preferred_width.h"
#include "gui_set_initial_used_values.h"
#include "../gui_element.h"

namespace clan
{

void GUIFindPreferredWidth::flex_horizontal_node(GUIComponent_Impl *node)
{
	node->css_used_values.width = node->component->get_preferred_content_width();

	CSSUsedValue preferred_width = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (is_normal_flow(child->impl.get()))
		{
			GUISetInitialUsedValues initial_visitor;
			initial_visitor.node(child->impl.get());

			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the width of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.width_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_width += get_used_noncontent_width(child_used_values) + child_used_values.width;
		}
	}

	node->css_used_values.width = std::max(node->css_used_values.width, preferred_width);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->element.get_css_values().get_box(), node->parent->impl->css_used_values);
}

void GUIFindPreferredWidth::flex_vertical_node(GUIComponent_Impl *node)
{
	node->css_used_values.width = node->component->get_preferred_content_width();

	CSSUsedValue preferred_width = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (is_normal_flow(child->impl.get()))
		{
			GUISetInitialUsedValues initial_visitor;
			initial_visitor.node(child->impl.get());

			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the width of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.width_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_width = std::max(preferred_width, get_used_noncontent_width(child_used_values) + child_used_values.width);
		}
	}

	node->css_used_values.width = std::max(node->css_used_values.width, preferred_width);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->element.get_css_values().get_box(), node->parent->impl->css_used_values);
}

}
