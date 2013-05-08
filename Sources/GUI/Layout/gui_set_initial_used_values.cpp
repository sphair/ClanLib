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
#include "gui_set_initial_used_values.h"
#include "../gui_element.h"

namespace clan
{

void GUISetInitialUsedValues::node(GUIComponent_Impl *node)
{
	const CSSComputedValues &properties = node->element.get_css_values();
	if (node->parent)
	{
		if (!node->use_auto_geometry)
		{
			// Components in manual geometry mode behave like absolute positioned elements as if all the left + top + right + bottom + width + height properties
			// had been specified to match the manual geometry (border box) dimensions.

			GUICSSUsedValues containing_box = node->parent->impl->css_used_values;
			containing_box.width_undetermined = false;
			containing_box.height_undetermined = false;
			GUICSSInitialUsedValues::visit(node->css_used_values, properties, containing_box);

			node->css_used_values.width =
				node->geometry.get_width() -
				node->css_used_values.border.left -
				node->css_used_values.border.right -
				node->css_used_values.padding.left -
				node->css_used_values.padding.right;

			node->css_used_values.width_undetermined = false;

			node->css_used_values.height =
				node->geometry.get_height() -
				node->css_used_values.border.top -
				node->css_used_values.border.bottom -
				node->css_used_values.padding.top -
				node->css_used_values.padding.bottom;

			node->css_used_values.height_undetermined = false;

			// TBD: what to do about the margin values? Since geometry is the border box it doesn't really matter what values they get.
			node->css_used_values.margin.top = 0.0f;
			node->css_used_values.margin.bottom = 0.0f;
			node->css_used_values.margin.left = 0.0f;
			node->css_used_values.margin.right = 0.0f;

			GUICSSApplyMinMaxConstraints::visit(node->css_used_values, properties.get_box(), containing_box); // TBD: does min/max constraints win over the manual set_geometry() size?
		}
		else if (properties.get_box().position.type == CSSValuePosition::type_absolute || properties.get_box().position.type == CSSValuePosition::type_fixed)
		{
			// To do: find nearest ancestor with 'position:relative'
			GUICSSUsedValues containing_box = node->parent->impl->css_used_values;
			containing_box.width_undetermined = false;
			containing_box.height_undetermined = false;
			GUICSSInitialUsedValues::visit(node->css_used_values, properties, containing_box);
		}
		else
		{
			GUICSSInitialUsedValues::visit(node->css_used_values, properties, node->parent->impl->css_used_values);
		}
	}
	else
	{
		Rect viewport = node->component->get_viewport();

		GUICSSUsedValues initial_containing_box;
		initial_containing_box.width = viewport.get_width();
		initial_containing_box.height = viewport.get_height();

		GUICSSInitialUsedValues::visit(node->css_used_values, properties, initial_containing_box);
		GUICSSApplyMinMaxConstraints::visit(node->css_used_values, properties.get_box(), initial_containing_box);

		if (node->css_used_values.width_undetermined)
		{
			node->css_used_values.width = initial_containing_box.width - node->css_used_values.margin.left - node->css_used_values.margin.right - node->css_used_values.border.left - node->css_used_values.border.right - node->css_used_values.padding.left - node->css_used_values.padding.right;
			node->css_used_values.width_undetermined = false;
		}

		// TBD: this isn't the default in normal CSS
		if (node->css_used_values.height_undetermined)
		{
			node->css_used_values.height = initial_containing_box.height - node->css_used_values.margin.top - node->css_used_values.margin.top - node->css_used_values.border.top - node->css_used_values.border.bottom - node->css_used_values.padding.top - node->css_used_values.padding.bottom;
			node->css_used_values.height_undetermined = false;
		}
	}
}

}
