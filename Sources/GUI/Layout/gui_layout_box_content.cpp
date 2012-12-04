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
#include "gui_layout_box_content.h"
#include "gui_find_preferred_width.h"
#include "gui_find_preferred_height.h"
#include "gui_css_flex_math.h"
#include "../gui_element.h"

namespace clan
{

void GUILayoutBoxContent::find_preferred_width(GUIComponent_Impl *node)
{
	GUIFindPreferredWidth preferred_visitor;
	preferred_visitor.node(node);
}

void GUILayoutBoxContent::find_preferred_height(GUIComponent_Impl *node)
{
	GUIFindPreferredHeight preferred_visitor;
	preferred_visitor.node(node);
}

void GUILayoutBoxContent::flex_horizontal_node(GUIComponent_Impl *node)
{
	// Calculate min/preferred/max widths of all child boxes
	GUICSSFlexMath box_math;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the width of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.width_undetermined)
			{
				find_preferred_width(child->impl.get());
			}

			CSSUsedValue used_noncontent_width = get_used_noncontent_width(child_used_values);

			box_math.used_min_lengths.push_back(used_noncontent_width + child_used_values.min_width);
			box_math.used_lengths.push_back(used_noncontent_width + child_used_values.width);
			box_math.used_max_lengths.push_back(used_noncontent_width + child_used_values.max_width);

			const CSSBoxProperties &properties = child->get_css_properties();
			box_math.used_shrink_weights.push_back(properties.flex_shrink.number);
			box_math.used_expand_weights.push_back(properties.flex_grow.number);
		}
	}

	// Adjust the widths of the boxes
	box_math.adjust(node->css_used_values.width);

	// Save adjusted width values and calculate the resulting box heights
	int i = 0;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// Save the result of the horizontal adjustment
			child_used_values.width = box_math.used_lengths[i] - get_used_noncontent_width(child_used_values);
			i++;

			// If the height of the box could not be determined from CSS, then ask the component:
			if (child_used_values.height_undetermined)
			{
				find_preferred_height(child->impl.get());
			}

			CSSUsedValue used_noncontent_height = get_used_noncontent_height(child_used_values);

			// Adjust height of box based on the shrink/expand factors
			GUICSSFlexMath perpendicular_math;

			perpendicular_math.used_min_lengths.push_back(used_noncontent_height + child_used_values.min_height);
			perpendicular_math.used_lengths.push_back(used_noncontent_height + child_used_values.height);
			perpendicular_math.used_max_lengths.push_back(used_noncontent_height + child_used_values.max_height);

			if (child->impl->element->get_css_properties().align_self.type == CSSBoxAlignSelf::type_stretch)
			{
				perpendicular_math.used_shrink_weights.push_back(1.0f);
				perpendicular_math.used_expand_weights.push_back(1.0f);
			}
			else
			{
				perpendicular_math.used_shrink_weights.push_back(0.0f);
				perpendicular_math.used_expand_weights.push_back(0.0f);
			}

			perpendicular_math.adjust(node->css_used_values.height);

			// Save the result of the vertical adjustment
			child_used_values.height = perpendicular_math.used_lengths[0] - used_noncontent_height;

		}
	}

	set_horizontal_geometry(node);
}

void GUILayoutBoxContent::flex_vertical_node(GUIComponent_Impl *node)
{
	GUICSSFlexMath box_math;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the width of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.width_undetermined)
			{
				find_preferred_width(child->impl.get());
			}

			CSSUsedValue used_noncontent_width = get_used_noncontent_width(child_used_values);

			// Adjust width of box based on the shrink/expand factors
			GUICSSFlexMath perpendicular_math;

			perpendicular_math.used_min_lengths.push_back(used_noncontent_width + child_used_values.min_width);
			perpendicular_math.used_lengths.push_back(used_noncontent_width + child_used_values.width);
			perpendicular_math.used_max_lengths.push_back(used_noncontent_width + child_used_values.max_width);

			if (child->impl->element->get_css_properties().align_self.type == CSSBoxAlignSelf::type_stretch)
			{
				perpendicular_math.used_shrink_weights.push_back(1.0f);
				perpendicular_math.used_expand_weights.push_back(1.0f);
			}
			else
			{
				perpendicular_math.used_shrink_weights.push_back(0.0f);
				perpendicular_math.used_expand_weights.push_back(0.0f);
			}

			perpendicular_math.adjust(node->css_used_values.width);

			// Save the result of the horizontal adjustment
			child_used_values.width = perpendicular_math.used_lengths[0] - used_noncontent_width;

			// If the height of the box could not be determined from CSS, then ask the component:
			if (child_used_values.height_undetermined)
			{
				find_preferred_height(child->impl.get());
			}

			// Set up vertical box adjustment math:

			CSSUsedValue used_noncontent_height = get_used_noncontent_height(child_used_values);

			box_math.used_min_lengths.push_back(used_noncontent_height + child_used_values.min_height);
			box_math.used_lengths.push_back(used_noncontent_height + child_used_values.height);
			box_math.used_max_lengths.push_back(used_noncontent_height + child_used_values.max_height);

			const CSSBoxProperties &properties = child->impl->element->get_css_properties();

			box_math.used_shrink_weights.push_back(properties.flex_shrink.number);
			box_math.used_expand_weights.push_back(properties.flex_grow.number);
		}
	}

	// Adjust the heights of the boxes
	box_math.adjust(node->css_used_values.height);

	// Save adjusted height values
	int i = 0;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling(), i++)
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;
			child_used_values.height = box_math.used_lengths[i] - get_used_noncontent_height(child_used_values);
		}
	}

	set_vertical_geometry(node);
}

CSSUsedValue GUILayoutBoxContent::align_vertical(GUIComponent_Impl *node, GUIComponent *child)
{
	const CSSBoxProperties &properties = child->impl->element->get_css_properties();

	if (properties.align_self.type == CSSBoxAlignSelf::type_flex_start)
	{
		return 0.0f;
	}
	else if (properties.align_self.type == CSSBoxAlignSelf::type_flex_end)
	{
		return node->css_used_values.height - child->impl->css_used_values.height - get_used_noncontent_height(child->impl->css_used_values);
	}
	else if (properties.align_self.type == CSSBoxAlignSelf::type_center)
	{
		return (node->css_used_values.height - child->impl->css_used_values.height - get_used_noncontent_height(child->impl->css_used_values)) * 0.5f;
	}
	else if (properties.align_self.type == CSSBoxAlignSelf::type_baseline)
	{
		return 0.0f; // To do: implement this
	}
	else // type_stretch
	{
		return 0.0f;
	}
}

CSSUsedValue GUILayoutBoxContent::align_horizontal(GUIComponent_Impl *node, GUIComponent *child)
{
	const CSSBoxProperties &properties = child->impl->element->get_css_properties();
	if (properties.align_self.type == CSSBoxAlignSelf::type_flex_start)
	{
		return 0.0f;
	}
	else if (properties.align_self.type == CSSBoxAlignSelf::type_flex_end)
	{
		return node->css_used_values.width - child->impl->css_used_values.width - get_used_noncontent_width(child->impl->css_used_values);
	}
	else if (properties.align_self.type == CSSBoxAlignSelf::type_center)
	{
		return (node->css_used_values.width - child->impl->css_used_values.width - get_used_noncontent_width(child->impl->css_used_values)) * 0.5f;
	}
	else if (properties.align_self.type == CSSBoxAlignSelf::type_baseline)
	{
		return 0.0f; // To do: implement this
	}
	else // type_stretch
	{
		return 0.0f;
	}
}

void GUILayoutBoxContent::set_horizontal_geometry(GUIComponent_Impl *node)
{
	CSSUsedValue x = node->css_used_values.border.left + node->css_used_values.padding.left;
	CSSUsedValue y = node->css_used_values.border.top + node->css_used_values.padding.top;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			set_child_geometry(node, child, x, y + align_vertical(node, child));

			GUICSSUsedValues &child_used_values = child->impl->css_used_values;
			x += get_used_noncontent_width(child_used_values) + child_used_values.width;
		}
	}
}

void GUILayoutBoxContent::set_vertical_geometry(GUIComponent_Impl *node)
{
	CSSUsedValue x = node->css_used_values.border.left + node->css_used_values.padding.left;
	CSSUsedValue y = node->css_used_values.border.top + node->css_used_values.padding.top;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			set_child_geometry(node, child, x + align_horizontal(node, child), y);

			GUICSSUsedValues &child_used_values = child->impl->css_used_values;
			y += get_used_noncontent_height(child_used_values) + child_used_values.height;
		}
	}
}

void GUILayoutBoxContent::set_child_geometry(GUIComponent_Impl *node, GUIComponent *child, CSSUsedValue x, CSSUsedValue y)
{
	GUICSSUsedValues &child_used_values = child->impl->css_used_values;

	x += get_css_relative_x(child->impl.get(), node->css_used_values.width);
	y += get_css_relative_y(child->impl.get(), node->css_used_values.height);

	x += child_used_values.margin.left;
	y += child_used_values.margin.top;

	CSSUsedValue used_border_box_width = child_used_values.width + child_used_values.padding.left + child_used_values.padding.right + child_used_values.border.left + child_used_values.border.right;
	CSSUsedValue used_border_box_height = child_used_values.height + child_used_values.padding.top + child_used_values.padding.bottom + child_used_values.border.top + child_used_values.border.bottom;

	CSSActualValue x1 = (CSSActualValue)(x);
	CSSActualValue y1 = (CSSActualValue)(y);
	CSSActualValue x2 = (CSSActualValue)(x + used_border_box_width + 0.5f);
	CSSActualValue y2 = (CSSActualValue)(y + used_border_box_height + 0.5f);
	child->set_geometry(Rect(x1, y1, x2, y2));
}

}
