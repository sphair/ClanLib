/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "gui_find_preferred_height.h"
#include "../gui_element.h"
#include <algorithm>

namespace clan
{

void GUIFindPreferredHeight::flex_horizontal_node(GUIComponent_Impl *node)
{
	node->css_used_values.height = node->component->get_preferred_content_height(node->css_used_values.width);

	CSSUsedValue preferred_height = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (is_normal_flow(child->impl.get()))
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the height of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.height_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_height = std::max(preferred_height, get_used_noncontent_height(child_used_values) + child_used_values.height);
		}
	}

	node->css_used_values.height = std::max(node->css_used_values.height, preferred_height);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->element.get_css_values().get_box(), node->parent->impl->css_used_values);
}

void GUIFindPreferredHeight::flex_vertical_node(GUIComponent_Impl *node)
{
	node->css_used_values.height = node->component->get_preferred_content_height(node->css_used_values.width);

	CSSUsedValue preferred_height = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (is_normal_flow(child->impl.get()))
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the height of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.height_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_height += get_used_noncontent_height(child_used_values) + child_used_values.height;
		}
	}

	node->css_used_values.height = std::max(node->css_used_values.height, preferred_height);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->element.get_css_values().get_box(), node->parent->impl->css_used_values);
}

}
