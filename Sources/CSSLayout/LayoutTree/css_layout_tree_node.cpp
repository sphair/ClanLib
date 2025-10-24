/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "css_layout_tree_node.h"
#include "css_block_formatting_context.h"
#include "css_stacking_context.h"
#include "css_layout_cursor.h"
#include "css_layout_graphics.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "css_background_renderer.h"
#include "css_border_renderer.h"

CL_CSSLayoutTreeNode::CL_CSSLayoutTreeNode(CL_CSSBoxElement *element_node)
: preferred_width(0.0f), min_width(0.0f), preferred_width_calculated(false), min_width_calculated(false),
  relative_x(0.0f), relative_y(0.0f), static_position_parent(0), element_node(element_node), formatting_context(0),
  formatting_context_root(false), stacking_context(0), stacking_context_root(false)
{
}

CL_CSSLayoutTreeNode::~CL_CSSLayoutTreeNode()
{
	if (formatting_context_root)
		delete formatting_context;
//	if (stacking_context_root)
//		delete stacking_context;
}

void CL_CSSLayoutTreeNode::prepare(CL_CSSBlockFormattingContext *current_formatting_context, CL_CSSStackingContext *current_stacking_context)
{
	if (current_formatting_context == 0 || element_node->is_inline_block_level() || element_node->is_float() || element_node->is_table() || element_node->is_table_cell() || is_replaced() || element_node->is_absolute() || !element_node->is_overflow_visible())
		set_formatting_context(new CL_CSSBlockFormattingContext(current_formatting_context), true);
	else if (element_node->is_fixed())
		set_formatting_context(new CL_CSSBlockFormattingContext(0), true);
	else
		set_formatting_context(current_formatting_context, false);
	establish_stacking_context_if_needed(current_stacking_context);
	prepare_children();
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_margin_width(const CL_CSSBoxMarginWidth &margin_width, CL_CSSUsedWidth containing_width)
{
	switch (margin_width.type)
	{
	case CL_CSSBoxMarginWidth::type_auto: return 0.0f;
	case CL_CSSBoxMarginWidth::type_length: return margin_width.length.value;
	case CL_CSSBoxMarginWidth::type_percentage: return containing_width.expanding ? 0.0f : margin_width.percentage * containing_width.value / 100.0f;
	default: return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_padding_width(const CL_CSSBoxPaddingWidth &padding_width, CL_CSSUsedWidth containing_width)
{
	switch (padding_width.type)
	{
	case CL_CSSBoxPaddingWidth::type_length: return padding_width.length.value;
	case CL_CSSBoxPaddingWidth::type_percentage: return containing_width.expanding ? 0.0f : padding_width.percentage * containing_width.value / 100.0f;
	default: return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_margin_height(const CL_CSSBoxMarginWidth &margin_width, CL_CSSUsedHeight containing_height)
{
	switch (margin_width.type)
	{
	case CL_CSSBoxMarginWidth::type_auto: return 0.0f;
	case CL_CSSBoxMarginWidth::type_length: return margin_width.length.value;
	case CL_CSSBoxMarginWidth::type_percentage: return containing_height.use_content ? 0.0f : margin_width.percentage * containing_height.value / 100.0f;
	default: return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_padding_height(const CL_CSSBoxPaddingWidth &padding_width, CL_CSSUsedHeight containing_height)
{
	switch (padding_width.type)
	{
	case CL_CSSBoxPaddingWidth::type_length: return padding_width.length.value;
	case CL_CSSBoxPaddingWidth::type_percentage: return containing_height.use_content ? 0.0f : padding_width.percentage * containing_height.value / 100.0f;
	default: return 0.0f;
	}
}

void CL_CSSLayoutTreeNode::calculate_absolute_widths(LayoutStrategy strategy)
{
	margin.left = get_css_margin_width(element_node->computed_properties.margin_width_left, containing_width);
	margin.right = get_css_margin_width(element_node->computed_properties.margin_width_right, containing_width);
	border.left = element_node->computed_properties.border_width_left.length.value;
	border.right = element_node->computed_properties.border_width_right.length.value;
	padding.left = get_css_padding_width(element_node->computed_properties.padding_width_left, containing_width);
	padding.right = get_css_padding_width(element_node->computed_properties.padding_width_right, containing_width);

	CL_CSSUsedValue left = 0.0f;
	if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_length)
		left = element_node->computed_properties.left.length.value;
	else if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_percentage)
		left = element_node->computed_properties.left.percentage * containing_width.value / 100.0f;

	CL_CSSUsedValue right = 0.0f;
	if (element_node->computed_properties.right.type == CL_CSSBoxRight::type_length)
		right = element_node->computed_properties.right.length.value;
	else if (element_node->computed_properties.right.type == CL_CSSBoxRight::type_percentage)
		right = element_node->computed_properties.right.percentage * containing_width.value / 100.0f;

	width.value = 0.0f;
	width.expanding = false;
	if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_length)
		width.value = element_node->computed_properties.width.length.value;
	else
		width.value = element_node->computed_properties.width.percentage * containing_width.value / 100.0f;

	if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_auto &&
		element_node->computed_properties.right.type == CL_CSSBoxRight::type_auto &&
		element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
	{
		width.value = 0.0f;
		width.expanding = true;
	}
	else if (element_node->computed_properties.left.type != CL_CSSBoxLeft::type_auto &&
		element_node->computed_properties.right.type != CL_CSSBoxRight::type_auto &&
		element_node->computed_properties.width.type != CL_CSSBoxWidth::type_auto)
	{
		if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
		{
			CL_CSSUsedValue space_left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - left - right;
			if (space_left < 0.0f)
			{
				if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
				{
					margin.left = 0.0f;
					margin.right = space_left;
				}
				else
				{
					margin.left = space_left;
					margin.right = 0.0f;
				}
			}
			else
			{
				margin.left = space_left / 2.0f;
				margin.right = margin.left;
			}
		}
		else if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.right - left - right;
		}
		else if (element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.right = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.left - left - right;
		}
	}
	else if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_auto &&
		element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto &&
		element_node->computed_properties.right.type != CL_CSSBoxRight::type_auto) // rule #1
	{
		width.value = 0.0f;
		width.expanding = true;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto &&
		element_node->computed_properties.right.type == CL_CSSBoxRight::type_auto &&
		element_node->computed_properties.left.type != CL_CSSBoxLeft::type_auto) // rule #3
	{
		width.value = 0.0f;
		width.expanding = true;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto &&
		element_node->computed_properties.left.type != CL_CSSBoxLeft::type_auto &&
		element_node->computed_properties.right.type != CL_CSSBoxRight::type_auto) // rule #5
	{
		width.value = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - left - right;
	}

	if (width.expanding && strategy == normal_strategy)
	{
		width.value = cl_actual_to_used(
			cl_used_to_actual(containing_width.value) -
			cl_used_to_actual(margin.left) -
			cl_used_to_actual(margin.right) -
			cl_used_to_actual(border.left) -
			cl_used_to_actual(border.right) -
			cl_used_to_actual(padding.left) -
			cl_used_to_actual(padding.right));
	}

	css_max_width.expanding = true;
	css_max_width.value = 0.0f;
	if (element_node->computed_properties.max_width.type != CL_CSSBoxMaxWidth::type_none)
	{
		if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_length)
		{
			css_max_width.value = element_node->computed_properties.max_width.length.value;
			css_max_width.expanding = false;
		}
		else if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_percentage && !containing_width.expanding)
		{
			css_max_width.value = element_node->computed_properties.max_width.percentage * containing_width.value / 100.0f;
			css_max_width.expanding = false;
		}
	}

	if (!css_max_width.expanding && width.value > css_max_width.value)
		apply_absolute_widths_constraint(css_max_width.value, left, right);

	if (width.value < css_min_width)
		apply_absolute_widths_constraint(min_width, left, right);
}

void CL_CSSLayoutTreeNode::apply_absolute_widths_constraint(CL_CSSUsedValue constraint_width, CL_CSSUsedValue left, CL_CSSUsedValue right)
{
	width.value = constraint_width;
	if (element_node->computed_properties.left.type != CL_CSSBoxLeft::type_auto &&
		element_node->computed_properties.right.type != CL_CSSBoxRight::type_auto)
	{
		if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
		{
			CL_CSSUsedValue space_left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - left - right;
			if (space_left < 0.0f)
			{
				if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
				{
					margin.left = 0.0f;
					margin.right = space_left;
				}
				else
				{
					margin.left = space_left;
					margin.right = 0.0f;
				}
			}
			else
			{
				margin.left = space_left / 2.0f;
				margin.right = margin.left;
			}
		}
		else if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.right - left - right;
		}
		else if (element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.right = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.left - left - right;
		}
	}
}

void CL_CSSLayoutTreeNode::calculate_absolute_heights()
{
	margin.top = get_css_margin_height(element_node->computed_properties.margin_width_top, containing_height);
	margin.bottom = get_css_margin_height(element_node->computed_properties.margin_width_bottom, containing_height);
	border.top = element_node->computed_properties.border_width_top.length.value;
	border.bottom = element_node->computed_properties.border_width_bottom.length.value;
	padding.top = get_css_padding_height(element_node->computed_properties.padding_width_top, containing_height);
	padding.bottom = get_css_padding_height(element_node->computed_properties.padding_width_bottom, containing_height);

	CL_CSSUsedValue top = 0.0f;
	if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_length)
		top = element_node->computed_properties.top.length.value;
	else if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_percentage)
		top = element_node->computed_properties.top.percentage * containing_height.value / 100.0f;

	CL_CSSUsedValue bottom = 0.0f;
	if (element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_length)
		bottom = element_node->computed_properties.bottom.length.value;
	else if (element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_percentage)
		bottom = element_node->computed_properties.bottom.percentage * containing_height.value / 100.0f;

	height.value = 0.0f;
	height.use_content = false;
	if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_length)
		height.value = element_node->computed_properties.height.length.value;
	else
		height.value = element_node->computed_properties.height.percentage * containing_height.value / 100.0f;

	if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_auto &&
		element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_auto &&
		element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else if (element_node->computed_properties.top.type != CL_CSSBoxTop::type_auto &&
		element_node->computed_properties.bottom.type != CL_CSSBoxBottom::type_auto &&
		element_node->computed_properties.height.type != CL_CSSBoxHeight::type_auto)
	{
		if (element_node->computed_properties.margin_width_top.type == CL_CSSBoxMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_bottom.type == CL_CSSBoxMarginWidth::type_auto)
		{
			CL_CSSUsedValue space_left = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - top - bottom;
			if (space_left < 0.0f)
			{
				margin.top = 0.0f;
				margin.bottom = space_left;
			}
			else
			{
				margin.top = space_left / 2.0f;
				margin.bottom = margin.top;
			}
		}
		else if (element_node->computed_properties.margin_width_top.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.bottom - top - bottom;
		}
		else if (element_node->computed_properties.margin_width_bottom.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.top - top - bottom;
		}
	}
	else if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_auto &&
		element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto &&
		element_node->computed_properties.bottom.type != CL_CSSBoxBottom::type_auto) // rule #1
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto &&
		element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_auto &&
		element_node->computed_properties.top.type != CL_CSSBoxTop::type_auto) // rule #3
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto &&
		element_node->computed_properties.top.type != CL_CSSBoxTop::type_auto &&
		element_node->computed_properties.bottom.type != CL_CSSBoxBottom::type_auto) // rule #5
	{
		height.value = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - top - bottom;
	}

	if (!css_max_height.use_content && height.value > css_max_height.value)
		apply_absolute_heights_constraint(css_max_height.value, top, bottom);

	if (height.value < css_min_height)
		apply_absolute_heights_constraint(css_min_height, top, bottom);
}

void CL_CSSLayoutTreeNode::apply_absolute_heights_constraint(CL_CSSUsedValue constraint_height, CL_CSSUsedValue top, CL_CSSUsedValue bottom)
{
	height.value = constraint_height;
	if (element_node->computed_properties.top.type != CL_CSSBoxTop::type_auto &&
		element_node->computed_properties.bottom.type != CL_CSSBoxBottom::type_auto)
	{
		if (element_node->computed_properties.margin_width_top.type == CL_CSSBoxMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_bottom.type == CL_CSSBoxMarginWidth::type_auto)
		{
			CL_CSSUsedValue space_left = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - top - bottom;
			if (space_left < 0.0f)
			{
				margin.top = 0.0f;
				margin.bottom = space_left;
			}
			else
			{
				margin.top = space_left / 2.0f;
				margin.bottom = margin.top;
			}
		}
		else if (element_node->computed_properties.margin_width_top.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.bottom - top - bottom;
		}
		else if (element_node->computed_properties.margin_width_bottom.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.top - top - bottom;
		}
	}
}

void CL_CSSLayoutTreeNode::calculate_static_widths(LayoutStrategy strategy)
{
	bool is_table_cell = element_node->computed_properties.display.type == CL_CSSBoxDisplay::type_table_cell;
	bool is_float = element_node->computed_properties.float_box.type != CL_CSSBoxFloat::type_none;
	//bool is_overflow_visible = (element_node->get_parent() == 0) || element_node->is_overflow_visible();

	margin.left = get_css_margin_width(element_node->computed_properties.margin_width_left, containing_width);
	margin.right = get_css_margin_width(element_node->computed_properties.margin_width_right, containing_width);
	border.left = element_node->computed_properties.border_width_left.length.value;
	border.right = element_node->computed_properties.border_width_right.length.value;
	padding.left = get_css_padding_width(element_node->computed_properties.padding_width_left, containing_width);
	padding.right = get_css_padding_width(element_node->computed_properties.padding_width_right, containing_width);

	if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_length)
	{
		width.value = element_node->computed_properties.width.length.value;
		width.expanding = false;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_percentage)
	{
		if (containing_width.expanding)
		{
			width.value = 0.0f;
			width.expanding = true;
		}
		else
		{
			width.value = element_node->computed_properties.width.percentage * containing_width.value / 100.0f;
			width.expanding = false;
		}
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
	{
		if (containing_width.expanding || is_table_cell || is_float /*|| !is_overflow_visible*/)
		{
			width.value = 0.0f;
			width.expanding = true;
		}
		else
		{
			width.value = cl_max(0.0f, containing_width.value - margin.left - margin.right - border.left - border.right - padding.left - padding.right);
			width.expanding = false;
		}
	}
	else
	{
		// Unreachable code (unless something is seriously wrong elsewhere).
		width.value = 0.0f;
		width.expanding = false;
	}

	if (width.expanding)
	{
		if (strategy == normal_strategy)
			width.value = cl_actual_to_used(
				cl_used_to_actual(containing_width.value) -
				cl_used_to_actual(margin.left) -
				cl_used_to_actual(margin.right) -
				cl_used_to_actual(border.left) -
				cl_used_to_actual(border.right) -
				cl_used_to_actual(padding.left) -
				cl_used_to_actual(padding.right));

		if (!css_max_width.expanding)
			width.value = cl_min(width.value, css_max_width.value);
		width.value = cl_max(width.value, css_min_width);
	}
	else
	{
		if (!css_max_width.expanding)
			width.value = cl_min(width.value, css_max_width.value);
		width.value = cl_max(width.value, css_min_width);

		if (!containing_width.expanding && !is_float)
		{
			if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto && element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.left = cl_max(0.0f, (containing_width.value-border.left-border.right-padding.left-padding.right-width.value)/2.0f);
				margin.right = cl_max(0.0f, containing_width.value-border.left-border.right-padding.left-padding.right-width.value-margin.left);
			}
			else if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.left = cl_max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
			else if (element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.right = cl_max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
			}

			if (margin.left + border.left + width.value + border.right + padding.right + margin.right > containing_width.value)
			{
				if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
					margin.right = cl_max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
				else
					margin.left = cl_max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
		}
	}
}

void CL_CSSLayoutTreeNode::calculate_static_heights()
{
	margin.top = get_css_margin_height(element_node->computed_properties.margin_width_top, containing_height);
	margin.bottom = get_css_margin_height(element_node->computed_properties.margin_width_bottom, containing_height);
	border.top = element_node->computed_properties.border_width_top.length.value;
	border.bottom = element_node->computed_properties.border_width_bottom.length.value;
	padding.top = get_css_padding_height(element_node->computed_properties.padding_width_top, containing_height);
	padding.bottom = get_css_padding_height(element_node->computed_properties.padding_width_bottom, containing_height);

	if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_length)
	{
		height.value = element_node->computed_properties.height.length.value;
		height.use_content = false;
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_percentage)
	{
		if (containing_height.use_content)
		{
			height.value = 0.0f;
			height.use_content = true;
		}
		else
		{
			height.value = containing_height.value * element_node->computed_properties.height.percentage / 100.0f;
			height.use_content = false;
		}
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else
	{
		// Unreachable code (unless something is seriously wrong elsewhere).
		height.value = 0.0f;
		height.use_content = false;
	}

	if (!css_max_height.use_content)
		height.value = cl_min(height.value, css_max_width.value);
	height.value = cl_max(height.value, css_min_width);
}

void CL_CSSLayoutTreeNode::calculate_top_down_widths(LayoutStrategy strategy)
{
	css_max_width.expanding = true;
	css_max_width.value = 0.0f;
	if (element_node->computed_properties.max_width.type != CL_CSSBoxMaxWidth::type_none)
	{
		if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_length)
		{
			css_max_width.value = element_node->computed_properties.max_width.length.value;
			css_max_width.expanding = false;
		}
		else if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_percentage && !containing_width.expanding)
		{
			css_max_width.value = element_node->computed_properties.max_width.percentage * containing_width.value / 100.0f;
			css_max_width.expanding = false;
		}
	}

	css_min_width = 0.0f;
	if (element_node->computed_properties.min_width.type == CL_CSSBoxMinWidth::type_length)
		css_min_width = element_node->computed_properties.min_width.length.value;
	else if (element_node->computed_properties.min_width.type == CL_CSSBoxMinWidth::type_percentage && !containing_width.expanding)
		css_min_width = element_node->computed_properties.min_width.percentage * containing_width.value / 100.0f;


	if (element_node->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
		element_node->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
	{
		calculate_absolute_widths(strategy);
	}
	else
	{
		calculate_static_widths(strategy);
	}
}

void CL_CSSLayoutTreeNode::calculate_top_down_heights()
{
	css_max_height.value = 0.0f;
	css_max_height.use_content = true;
	if (element_node->computed_properties.max_height.type != CL_CSSBoxMaxHeight::type_none)
	{
		if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_length)
		{
			css_max_height.value = element_node->computed_properties.max_height.length.value;
			css_max_height.use_content = false;
		}
		else if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_percentage && !containing_height.use_content)
		{
			css_max_height.value = element_node->computed_properties.max_height.percentage * containing_height.value / 100.0f;
			css_max_height.use_content = false;
		}
	}

	css_min_height = 0.0f;
	if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_length)
		css_min_height = element_node->computed_properties.min_height.length.value;
	else if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_percentage && !containing_height.use_content)
		css_min_height = element_node->computed_properties.min_height.percentage * containing_height.value / 100.0f;


	if (element_node->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
		element_node->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
	{
		calculate_absolute_heights();
	}
	else
	{
		calculate_static_heights();
	}

	calculate_content_top_down_heights();
}

void CL_CSSLayoutTreeNode::set_root_block_position(int x, int y)
{
	if (!formatting_context_root)
		throw CL_Exception("CL_CSSLayoutTreeNode::set_root_block_position misuse");

	if (element_node->is_table() || element_node->is_inline_table())
	{
		content_box = CL_Rect(CL_Point(x+cl_used_to_actual(margin.left), y+cl_used_to_actual(margin.top)), content_box.get_size());
	}
	else
	{
		content_box = CL_Rect(
			CL_Point(
				x+cl_used_to_actual(margin.left)+cl_used_to_actual(border.left)+cl_used_to_actual(padding.left),
				y+cl_used_to_actual(margin.top)+cl_used_to_actual(border.top)+cl_used_to_actual(padding.top)),
				content_box.get_size());
	}
	formatting_context->set_position(content_box.left, content_box.top);
}

void CL_CSSLayoutTreeNode::set_root_content_position(int x, int y)
{
	if (!formatting_context_root)
		throw CL_Exception("CL_CSSLayoutTreeNode::set_root_content_position misuse");
	content_box = CL_Rect(CL_Point(x, y), content_box.get_size());
	formatting_context->set_position(content_box.left, content_box.top);
}

void CL_CSSLayoutTreeNode::calc_preferred(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources)
{
	if (!preferred_width_calculated)
	{
		calculate_top_down_widths(preferred_strategy);
		calculate_top_down_heights();
		layout_formatting_root_helper(graphics, resources, preferred_strategy);
	}
}

void CL_CSSLayoutTreeNode::calc_minimum(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources)
{
	if (!min_width_calculated)
	{
		calculate_top_down_widths(minimum_strategy);
		calculate_top_down_heights();
		layout_formatting_root_helper(graphics, resources, minimum_strategy);
	}
}

void CL_CSSLayoutTreeNode::layout_absolute_or_fixed(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, const CL_Rect &containing_block, const CL_Size &viewport_size)
{
	containing_width.value = containing_block.get_width();
	containing_width.expanding = false;
	containing_height.value = containing_block.get_height();
	containing_height.use_content = false;
	calculate_top_down_widths(normal_strategy);
	calculate_top_down_heights();

	CL_CSSUsedValue left = 0.0f;
	if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_length)
		left = element_node->computed_properties.left.length.value;
	else if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_percentage)
		left = element_node->computed_properties.left.percentage * containing_width.value / 100.0f;

	CL_CSSUsedValue right = 0.0f;
	if (element_node->computed_properties.right.type == CL_CSSBoxRight::type_length)
		right = element_node->computed_properties.right.length.value;
	else if (element_node->computed_properties.right.type == CL_CSSBoxRight::type_percentage)
		right = element_node->computed_properties.right.percentage * containing_width.value / 100.0f;

	if (width.expanding)
	{
		CL_CSSUsedValue available_width = containing_width.value;
		if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_auto)
		{
			available_width = containing_width.value - margin.left - border.left - padding.left - padding.right - border.right - margin.right - right;
		}
		else if (element_node->computed_properties.right.type == CL_CSSBoxRight::type_auto)
		{
			available_width = containing_width.value - margin.left - border.left - padding.left - padding.right - border.right - margin.right - left;
		}
		layout_shrink_to_fit(graphics, resources, available_width);
	}
	else
	{
		layout_formatting_root_helper(graphics, resources, normal_strategy);
	}

	if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_auto &&
		element_node->computed_properties.right.type == CL_CSSBoxRight::type_auto &&
		element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
	{
		if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
		{
			CL_CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
			left = offset_x + static_position.left - containing_block.left;
			right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
		}
		else
		{
			CL_CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
			right = offset_x + static_position.right - containing_block.left;
			left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
		}
	}
	else
	{
		if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_auto &&
			element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto &&
			element_node->computed_properties.right.type != CL_CSSBoxRight::type_auto) // rule #1
		{
			left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
		}
		else if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_auto &&
			element_node->computed_properties.right.type == CL_CSSBoxRight::type_auto &&
			element_node->computed_properties.width.type != CL_CSSBoxWidth::type_auto) // rule #2
		{
			if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
			{
				CL_CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
				left = offset_x + static_position.left - containing_block.left;
				right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
			}
			else
			{
				CL_CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
				right = offset_x + static_position.right - containing_block.left;
				left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
			}
		}
		else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto &&
			element_node->computed_properties.right.type == CL_CSSBoxRight::type_auto &&
			element_node->computed_properties.left.type != CL_CSSBoxLeft::type_auto) // rule #3
		{
			right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
		}
		else if (element_node->computed_properties.left.type == CL_CSSBoxLeft::type_auto &&
			element_node->computed_properties.width.type != CL_CSSBoxWidth::type_auto &&
			element_node->computed_properties.right.type != CL_CSSBoxRight::type_auto) // rule #4
		{
			left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
		}
		else if (element_node->computed_properties.right.type == CL_CSSBoxRight::type_auto &&
			element_node->computed_properties.left.type != CL_CSSBoxLeft::type_auto &&
			element_node->computed_properties.width.type != CL_CSSBoxWidth::type_auto) // rule #6
		{
			right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
		}
	}


	CL_CSSUsedValue top = 0.0f;
	if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_length)
		top = element_node->computed_properties.top.length.value;
	else if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_percentage)
		top = element_node->computed_properties.top.percentage * containing_height.value / 100.0f;

	CL_CSSUsedValue bottom = 0.0f;
	if (element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_length)
		bottom = element_node->computed_properties.bottom.length.value;
	else if (element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_percentage)
		bottom = element_node->computed_properties.bottom.percentage * containing_height.value / 100.0f;

	if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_auto &&
		element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_auto &&
		element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		CL_CSSActualValue offset_y = static_position_parent ? static_position_parent->formatting_context->get_y() : 0;
		top = offset_y + static_position.top - containing_block.top;
		bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
	}
	else
	{
		if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_auto &&
			element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto &&
			element_node->computed_properties.bottom.type != CL_CSSBoxBottom::type_auto) // rule #1
		{
			top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - bottom;
		}
		else if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_auto &&
			element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_auto &&
			element_node->computed_properties.height.type != CL_CSSBoxHeight::type_auto) // rule #2
		{
			CL_CSSActualValue offset_y = static_position_parent ? static_position_parent->formatting_context->get_y() : 0;
			top = offset_y + static_position.top - containing_block.top;
			bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
		}
		else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto &&
			element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_auto &&
			element_node->computed_properties.top.type != CL_CSSBoxTop::type_auto) // rule #3
		{
			bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
		}
		else if (element_node->computed_properties.top.type == CL_CSSBoxTop::type_auto &&
			element_node->computed_properties.height.type != CL_CSSBoxHeight::type_auto &&
			element_node->computed_properties.bottom.type != CL_CSSBoxBottom::type_auto) // rule #4
		{
			top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - bottom;
		}
		else if (element_node->computed_properties.bottom.type == CL_CSSBoxBottom::type_auto &&
			element_node->computed_properties.top.type != CL_CSSBoxTop::type_auto &&
			element_node->computed_properties.height.type != CL_CSSBoxHeight::type_auto) // rule #6
		{
			bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
		}
	}

	int x = containing_block.left + cl_used_to_actual(left);
	int y = containing_block.top + cl_used_to_actual(top);
	if (element_node->is_absolute() && formatting_context->get_parent())
	{
		x -= formatting_context->get_parent()->get_x();
		y -= formatting_context->get_parent()->get_y();
	}
	set_root_block_position(x, y);
}

void CL_CSSLayoutTreeNode::layout_shrink_to_fit(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, CL_CSSUsedValue available_width)
{
	calc_preferred(graphics, resources);
	CL_CSSUsedValue shrink_to_fit_width;
	if (preferred_width > available_width + 0.1f)
	{
		calc_minimum(graphics, resources);
		shrink_to_fit_width = cl_max(min_width, available_width);
	}
	else
	{
		shrink_to_fit_width = preferred_width;
	}
	calculate_top_down_widths(normal_strategy);
	calculate_top_down_heights();
	if (width.expanding)
		width.value = shrink_to_fit_width;
	layout_formatting_root_helper(graphics, resources, normal_strategy);
}

void CL_CSSLayoutTreeNode::layout_float(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, LayoutStrategy strategy)
{
	if (strategy == normal_strategy && element_node->is_shrink_to_fit())
	{
		layout_shrink_to_fit(graphics, resources, containing_width.value);
	}
	else
	{
		calculate_top_down_widths(strategy);
		calculate_top_down_heights();
		layout_formatting_root_helper(graphics, resources, strategy);
	}

/*	if (element_node->computed_properties.background_color.type == CL_CSSBoxBackgroundColor::type_color &&
		element_node->computed_properties.background_color.color == CL_Colorf::green)
	{
		cl_debug_breakpoint = 0xdeadbabe;
	}*/
}

void CL_CSSLayoutTreeNode::layout_formatting_root_helper(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, LayoutStrategy strategy)
{
/*	if (element_node->name.find("#navigation") != CL_String::npos)
	{
		Sleep(1);
	}*/

	formatting_context->clear();

	CL_CSSLayoutCursor cursor;
	cursor.x = 0;
	cursor.y = 0;
	cursor.relative_x = relative_x;
	cursor.relative_y = relative_y;
	cursor.resources = resources;
	add_content_margin_top(cursor);
	layout_content(graphics, cursor, strategy);
	cursor.apply_margin();

	if (strategy == preferred_strategy)
	{
		preferred_width = width.value;
		preferred_width_calculated = true;
	}
	else if (strategy == minimum_strategy)
	{
		min_width = width.value;
		min_width_calculated = true;
	}

	if (height.use_content)
	{
		height.value = cursor.y;
		CL_CSSActualValue left_float_height = formatting_context->find_left_clearance();
		CL_CSSActualValue right_float_height = formatting_context->find_right_clearance();
		height.value = cl_max(height.value, cl_actual_to_used(cl_max(left_float_height, right_float_height)));

		if (!css_max_height.use_content)
			height.value = cl_min(height.value, css_max_height.value);
		height.value = cl_max(height.value, css_min_height);
	}

	content_box = CL_Size(cl_used_to_actual(width.value), cl_used_to_actual(height.value));
}

void CL_CSSLayoutTreeNode::layout_normal(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
/*	if (element_node->name.find("head") != CL_String::npos)
	{
		Sleep(1);
	}*/

	calculate_top_down_widths(strategy);

	CL_CSSActualValue old_x = cursor.x;
	CL_CSSUsedValue old_relative_x = cursor.relative_x;
	CL_CSSUsedValue old_relative_y = cursor.relative_y;
	cursor.x += cl_used_to_actual(margin.left) + cl_used_to_actual(border.left) + cl_used_to_actual(padding.left);
	cursor.relative_x += get_local_relative_x();
	cursor.relative_y += get_local_relative_y();

	relative_x = cursor.relative_x;
	relative_y = cursor.relative_y;

	add_margin_top(cursor);

	if (element_node->computed_properties.clear.type == CL_CSSBoxClear::type_left || element_node->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		CL_CSSActualValue clear_left = formatting_context->find_left_clearance();
		if (cursor.y+cursor.get_total_margin() < clear_left)
			cursor.y = clear_left-cursor.get_total_margin();
	}
	if (element_node->computed_properties.clear.type == CL_CSSBoxClear::type_right || element_node->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		CL_CSSActualValue clear_right = formatting_context->find_right_clearance();
		if (cursor.y+cursor.get_total_margin() < clear_right)
			cursor.y = clear_right-cursor.get_total_margin();
	}

	if (border.top > 0 || padding.top > 0)
	{
		cursor.apply_margin();
		cursor.y += cl_used_to_actual(border.top) + cl_used_to_actual(padding.top);
	}

	CL_CSSActualValue before_content_y = cursor.y;

	content_box.left = cursor.x;
	content_box.top = cursor.y+cursor.get_total_margin();
	content_box.right = content_box.left+cl_used_to_actual(width.value);
	content_box.bottom = content_box.top+cl_used_to_actual(height.value);

	layout_content(graphics, cursor, strategy);

	if (height.use_content)
	{
		if (cursor.y == before_content_y)
		{
			height.value = cl_actual_to_used(0);
		}
		else
		{
			height.value = cl_actual_to_used(cl_max(0, cursor.y + cursor.get_total_margin() - content_box.top));
		}

		if (!css_max_height.use_content)
			height.value = cl_min(height.value, css_max_height.value);
		height.value = cl_max(height.value, css_min_height);

		content_box.bottom = content_box.top + cl_used_to_actual(height.value);
	}
	else
	{
		if (height.value > 0.0f)
			cursor.apply_margin();
		cursor.y = content_box.top + cl_used_to_actual(height.value);
	}


	if (strategy != normal_strategy)
	{
		content_box.right = content_box.left+cl_used_to_actual(width.value);
	}

	if (border.bottom > 0 || padding.bottom > 0)
	{
		cursor.apply_margin();
		cursor.y += cl_used_to_actual(border.bottom) + cl_used_to_actual(padding.bottom);
	}

	cursor.add_margin(margin.bottom);
	cursor.x = old_x;
	cursor.relative_x = old_relative_x;
	cursor.relative_y = old_relative_y;
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_local_relative_x() const
{
	return get_local_relative_x(element_node, containing_width.value);
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_local_relative_y() const
{
	return get_local_relative_y(element_node, containing_height.value);
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_local_relative_x(const CL_CSSBoxElement *element, CL_CSSUsedValue containing_width)
{
	if (element->computed_properties.position.type == CL_CSSBoxPosition::type_relative)
	{
		if (element->computed_properties.left.type == CL_CSSBoxLeft::type_length)
			return element->computed_properties.left.length.value;
		else if (element->computed_properties.left.type == CL_CSSBoxLeft::type_percentage)
			return element->computed_properties.left.percentage / 100.0f * containing_width;
		else
			return 0.0f;
	}
	else
	{
		return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_local_relative_y(const CL_CSSBoxElement *element, CL_CSSUsedValue containing_height)
{
	if (element->computed_properties.position.type == CL_CSSBoxPosition::type_relative)
	{
		if (element->computed_properties.top.type == CL_CSSBoxTop::type_length)
			return element->computed_properties.top.length.value;
		else if (element->computed_properties.top.type == CL_CSSBoxTop::type_percentage)
			return element->computed_properties.top.percentage / 100.0f * containing_height;
		else
			return 0.0f;
	}
	else
	{
		return 0.0f;
	}
}

bool CL_CSSLayoutTreeNode::add_margin_top(CL_CSSLayoutCursor &cursor)
{
/*	if (element_node->name.find("body") != CL_String::npos)
	{
		Sleep(1);
	}
*/
	if (element_node->is_inline_block_level() ||
		element_node->is_float() ||
		element_node->is_table() ||
		element_node->is_table_cell() ||
		element_node->is_absolute() ||
		element_node->is_fixed())
	{
		return false;
	}
	else
	{
		cursor.add_margin(margin.top);

		if (border.top > 0 || padding.top > 0)
			return true;

		if (add_content_margin_top(cursor))
			return true;

		float min_height = 0.0f;
		if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_length)
			min_height = element_node->computed_properties.min_height.length.value;
		else if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_percentage)
			min_height = element_node->computed_properties.min_height.percentage * containing_height.value / 100.0f;

		if (min_height > 0.0f || border.bottom > 0.0f || padding.bottom > 0.0f)
			return true;

		cursor.add_margin(margin.bottom); // Margins are adjoining
		return false;
	}
}

void CL_CSSLayoutTreeNode::set_formatting_context(CL_CSSBlockFormattingContext *new_formatting_context, bool is_root)
{
	if (formatting_context_root)
		delete formatting_context;
	formatting_context = new_formatting_context;
	formatting_context_root = is_root;
}

void CL_CSSLayoutTreeNode::establish_stacking_context_if_needed(CL_CSSStackingContext *current_stacking_context)
{
	if (element_node->computed_properties.position.type != CL_CSSBoxPosition::type_static && element_node->computed_properties.z_index.type != CL_CSSBoxZIndex::type_auto)
	{
		stacking_context = new CL_CSSStackingContext(this);
		stacking_context_root = true;
		current_stacking_context->push_back(stacking_context);
	}
	else
	{
		stacking_context = current_stacking_context;
		stacking_context_root = false;
	}
}

CL_CSSActualValue CL_CSSLayoutTreeNode::get_block_width() const
{
	return cl_used_to_actual(margin.left) +
		cl_used_to_actual(border.left) +
		cl_used_to_actual(padding.left) +
		cl_used_to_actual(width.value) +
		cl_used_to_actual(padding.right) +
		cl_used_to_actual(border.right) +
		cl_used_to_actual(margin.right);
}

CL_CSSActualValue CL_CSSLayoutTreeNode::get_block_height() const
{
	return cl_used_to_actual(margin.top) +
		cl_used_to_actual(border.top) +
		cl_used_to_actual(padding.top) +
		cl_used_to_actual(height.value) +
		cl_used_to_actual(padding.bottom) +
		cl_used_to_actual(border.bottom) +
		cl_used_to_actual(margin.bottom);
}

void CL_CSSLayoutTreeNode::render_non_content(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, bool root)
{
	if (element_node->computed_properties.visibility.type == CL_CSSBoxVisibility::type_visible &&
		element_node->computed_properties.display.type != CL_CSSBoxDisplay::type_table_cell)
	{
		render_background(graphics, resource_cache, root);
		render_border(graphics, resource_cache);

		if (element_node->computed_properties.display.type == CL_CSSBoxDisplay::type_list_item &&
			element_node->computed_properties.list_style_type.type != CL_CSSBoxListStyleType::type_none)
		{
			CL_String bullet = CL_StringHelp::wchar_to_utf8(8226);
			CL_Font font = graphics->get_font(element_node->computed_properties);
			CL_Size offset = graphics->get_text_size(font, bullet);
			offset.width += 8;
			// to do: find baseline of first item
			graphics->draw_text(font, cl_used_to_actual(relative_x) + formatting_context->get_x() + content_box.left-offset.width, cl_used_to_actual(relative_y) + formatting_context->get_y() + content_box.top + (int)(graphics->get_font_metrics(font).get_ascent()), bullet, element_node->computed_properties.color.color);
		}
	}
}

CL_Rect CL_CSSLayoutTreeNode::get_border_box() const
{
	CL_Rect border_rect = get_padding_box();
	border_rect.expand(cl_used_to_actual(border.left), cl_used_to_actual(border.top), cl_used_to_actual(border.right), cl_used_to_actual(border.bottom));
	return border_rect;
}

CL_Rect CL_CSSLayoutTreeNode::get_padding_box() const
{
	CL_Rect padding_rect = content_box;
	if (!formatting_context_root)
		padding_rect.translate(formatting_context->get_x(), formatting_context->get_y());
	else if (formatting_context->get_parent())
		padding_rect.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
	padding_rect.expand(cl_used_to_actual(padding.left), cl_used_to_actual(padding.top), cl_used_to_actual(padding.right), cl_used_to_actual(padding.bottom));
	return padding_rect;
}

void CL_CSSLayoutTreeNode::render_background(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, bool root)
{
	CL_Rect box = content_box;
	if (!formatting_context_root)
		box.translate(formatting_context->get_x(), formatting_context->get_y());
	else if (formatting_context->get_parent())
		box.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
	box.translate(cl_used_to_actual(relative_x), cl_used_to_actual(relative_y));

	CL_CSSBackgroundRenderer renderer(graphics, resource_cache, element_node);
	renderer.set_is_root(root);
	renderer.set_initial_containing_box(CL_Rect(0, 0, cl_used_to_actual(containing_width.value), cl_used_to_actual(containing_height.value))); // Bug: this is wrong except for the root
	renderer.set_content_box(box);
	box.expand(cl_used_to_actual(padding.left), cl_used_to_actual(padding.top), cl_used_to_actual(padding.right), cl_used_to_actual(padding.bottom));
	renderer.set_padding_box(box);
	box.expand(cl_used_to_actual(border.left), cl_used_to_actual(border.top), cl_used_to_actual(border.right), cl_used_to_actual(border.bottom));
	renderer.set_border_box(box);
	renderer.render();
}

void CL_CSSLayoutTreeNode::render_border(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache)
{
	CL_Rect border_box = get_border_box();
	border_box.translate(relative_x, relative_y);

	CL_CSSBorderRenderer renderer(graphics, resource_cache, element_node);
	renderer.set_border_values(border.left, border.top, border.right, border.bottom);
	renderer.set_border_box(border_box);
	renderer.render();
}
