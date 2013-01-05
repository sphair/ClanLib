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

#include "CSSLayout/precomp.h"
#include "css_layout_tree_node.h"
#include "css_block_formatting_context.h"
#include "css_stacking_context.h"
#include "css_layout_cursor.h"
#include "css_layout_graphics.h"
#include "../../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "css_background_renderer.h"
#include "css_border_renderer.h"

namespace clan
{

CSSLayoutTreeNode::CSSLayoutTreeNode(CSSBoxElement *element_node)
: preferred_width(0.0f), min_width(0.0f), preferred_width_calculated(false), min_width_calculated(false),
  relative_x(0.0f), relative_y(0.0f), static_position_parent(0), element_node(element_node), formatting_context(0),
  formatting_context_root(false), stacking_context(0), stacking_context_root(false)
{
}

CSSLayoutTreeNode::~CSSLayoutTreeNode()
{
	if (formatting_context_root)
		delete formatting_context;
//	if (stacking_context_root)
//		delete stacking_context;
}

void CSSLayoutTreeNode::prepare(CSSBlockFormattingContext *current_formatting_context, CSSStackingContext *current_stacking_context)
{
	if (current_formatting_context == 0 || element_node->is_inline_block_level() || element_node->is_float() || element_node->is_table() || element_node->is_table_cell() || is_replaced() || element_node->is_absolute() || !element_node->is_overflow_visible())
		set_formatting_context(new CSSBlockFormattingContext(current_formatting_context), true);
	else if (element_node->is_fixed())
		set_formatting_context(new CSSBlockFormattingContext(0), true);
	else
		set_formatting_context(current_formatting_context, false);
	establish_stacking_context_if_needed(current_stacking_context);
	prepare_children();
}

CSSUsedValue CSSLayoutTreeNode::get_css_margin_width(const CSSValueMarginWidth &margin_width, CSSUsedWidth containing_width)
{
	switch (margin_width.type)
	{
	case CSSValueMarginWidth::type_auto: return 0.0f;
	case CSSValueMarginWidth::type_length: return margin_width.length.value;
	case CSSValueMarginWidth::type_percentage: return containing_width.expanding ? 0.0f : margin_width.percentage * containing_width.value / 100.0f;
	default: return 0.0f;
	}
}

CSSUsedValue CSSLayoutTreeNode::get_css_padding_width(const CSSValuePaddingWidth &padding_width, CSSUsedWidth containing_width)
{
	switch (padding_width.type)
	{
	case CSSValuePaddingWidth::type_length: return padding_width.length.value;
	case CSSValuePaddingWidth::type_percentage: return containing_width.expanding ? 0.0f : padding_width.percentage * containing_width.value / 100.0f;
	default: return 0.0f;
	}
}

CSSUsedValue CSSLayoutTreeNode::get_css_margin_height(const CSSValueMarginWidth &margin_width, CSSUsedHeight containing_height)
{
	switch (margin_width.type)
	{
	case CSSValueMarginWidth::type_auto: return 0.0f;
	case CSSValueMarginWidth::type_length: return margin_width.length.value;
	case CSSValueMarginWidth::type_percentage: return containing_height.use_content ? 0.0f : margin_width.percentage * containing_height.value / 100.0f;
	default: return 0.0f;
	}
}

CSSUsedValue CSSLayoutTreeNode::get_css_padding_height(const CSSValuePaddingWidth &padding_width, CSSUsedHeight containing_height)
{
	switch (padding_width.type)
	{
	case CSSValuePaddingWidth::type_length: return padding_width.length.value;
	case CSSValuePaddingWidth::type_percentage: return containing_height.use_content ? 0.0f : padding_width.percentage * containing_height.value / 100.0f;
	default: return 0.0f;
	}
}

void CSSLayoutTreeNode::calculate_absolute_widths(LayoutStrategy strategy)
{
	margin.left = get_css_margin_width(element_node->computed_properties.margin_width_left, containing_width);
	margin.right = get_css_margin_width(element_node->computed_properties.margin_width_right, containing_width);
	border.left = element_node->computed_properties.border_width_left.length.value;
	border.right = element_node->computed_properties.border_width_right.length.value;
	padding.left = get_css_padding_width(element_node->computed_properties.padding_width_left, containing_width);
	padding.right = get_css_padding_width(element_node->computed_properties.padding_width_right, containing_width);

	CSSUsedValue left = 0.0f;
	if (element_node->computed_properties.left.type == CSSValueLeft::type_length)
		left = element_node->computed_properties.left.length.value;
	else if (element_node->computed_properties.left.type == CSSValueLeft::type_percentage)
		left = element_node->computed_properties.left.percentage * containing_width.value / 100.0f;

	CSSUsedValue right = 0.0f;
	if (element_node->computed_properties.right.type == CSSValueRight::type_length)
		right = element_node->computed_properties.right.length.value;
	else if (element_node->computed_properties.right.type == CSSValueRight::type_percentage)
		right = element_node->computed_properties.right.percentage * containing_width.value / 100.0f;

	width.value = 0.0f;
	width.expanding = false;
	if (element_node->computed_properties.width.type == CSSValueWidth::type_length)
		width.value = element_node->computed_properties.width.length.value;
	else
		width.value = element_node->computed_properties.width.percentage * containing_width.value / 100.0f;

	if (element_node->computed_properties.left.type == CSSValueLeft::type_auto &&
		element_node->computed_properties.right.type == CSSValueRight::type_auto &&
		element_node->computed_properties.width.type == CSSValueWidth::type_auto)
	{
		width.value = 0.0f;
		width.expanding = true;
	}
	else if (element_node->computed_properties.left.type != CSSValueLeft::type_auto &&
		element_node->computed_properties.right.type != CSSValueRight::type_auto &&
		element_node->computed_properties.width.type != CSSValueWidth::type_auto)
	{
		if (element_node->computed_properties.margin_width_left.type == CSSValueMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
		{
			CSSUsedValue space_left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - left - right;
			if (space_left < 0.0f)
			{
				if (element_node->computed_properties.direction.type == CSSValueDirection::type_ltr)
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
		else if (element_node->computed_properties.margin_width_left.type == CSSValueMarginWidth::type_auto)
		{
			margin.left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.right - left - right;
		}
		else if (element_node->computed_properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
		{
			margin.right = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.left - left - right;
		}
	}
	else if (element_node->computed_properties.left.type == CSSValueLeft::type_auto &&
		element_node->computed_properties.width.type == CSSValueWidth::type_auto &&
		element_node->computed_properties.right.type != CSSValueRight::type_auto) // rule #1
	{
		width.value = 0.0f;
		width.expanding = true;
	}
	else if (element_node->computed_properties.width.type == CSSValueWidth::type_auto &&
		element_node->computed_properties.right.type == CSSValueRight::type_auto &&
		element_node->computed_properties.left.type != CSSValueLeft::type_auto) // rule #3
	{
		width.value = 0.0f;
		width.expanding = true;
	}
	else if (element_node->computed_properties.width.type == CSSValueWidth::type_auto &&
		element_node->computed_properties.left.type != CSSValueLeft::type_auto &&
		element_node->computed_properties.right.type != CSSValueRight::type_auto) // rule #5
	{
		width.value = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - left - right;
	}

	if (width.expanding && strategy == normal_strategy)
	{
		width.value = actual_to_used(
			used_to_actual(containing_width.value) -
			used_to_actual(margin.left) -
			used_to_actual(margin.right) -
			used_to_actual(border.left) -
			used_to_actual(border.right) -
			used_to_actual(padding.left) -
			used_to_actual(padding.right));
	}

	css_max_width.expanding = true;
	css_max_width.value = 0.0f;
	if (element_node->computed_properties.max_width.type != CSSValueMaxWidth::type_none)
	{
		if (element_node->computed_properties.max_width.type == CSSValueMaxWidth::type_length)
		{
			css_max_width.value = element_node->computed_properties.max_width.length.value;
			css_max_width.expanding = false;
		}
		else if (element_node->computed_properties.max_width.type == CSSValueMaxWidth::type_percentage && !containing_width.expanding)
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

void CSSLayoutTreeNode::apply_absolute_widths_constraint(CSSUsedValue constraint_width, CSSUsedValue left, CSSUsedValue right)
{
	width.value = constraint_width;
	if (element_node->computed_properties.left.type != CSSValueLeft::type_auto &&
		element_node->computed_properties.right.type != CSSValueRight::type_auto)
	{
		if (element_node->computed_properties.margin_width_left.type == CSSValueMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
		{
			CSSUsedValue space_left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - left - right;
			if (space_left < 0.0f)
			{
				if (element_node->computed_properties.direction.type == CSSValueDirection::type_ltr)
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
		else if (element_node->computed_properties.margin_width_left.type == CSSValueMarginWidth::type_auto)
		{
			margin.left = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.right - left - right;
		}
		else if (element_node->computed_properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
		{
			margin.right = containing_width.value - border.left - border.right - padding.left - padding.right - width.value - margin.left - left - right;
		}
	}
}

void CSSLayoutTreeNode::calculate_absolute_heights()
{
	margin.top = get_css_margin_height(element_node->computed_properties.margin_width_top, containing_height);
	margin.bottom = get_css_margin_height(element_node->computed_properties.margin_width_bottom, containing_height);
	border.top = element_node->computed_properties.border_width_top.length.value;
	border.bottom = element_node->computed_properties.border_width_bottom.length.value;
	padding.top = get_css_padding_height(element_node->computed_properties.padding_width_top, containing_height);
	padding.bottom = get_css_padding_height(element_node->computed_properties.padding_width_bottom, containing_height);

	CSSUsedValue top = 0.0f;
	if (element_node->computed_properties.top.type == CSSValueTop::type_length)
		top = element_node->computed_properties.top.length.value;
	else if (element_node->computed_properties.top.type == CSSValueTop::type_percentage)
		top = element_node->computed_properties.top.percentage * containing_height.value / 100.0f;

	CSSUsedValue bottom = 0.0f;
	if (element_node->computed_properties.bottom.type == CSSValueBottom::type_length)
		bottom = element_node->computed_properties.bottom.length.value;
	else if (element_node->computed_properties.bottom.type == CSSValueBottom::type_percentage)
		bottom = element_node->computed_properties.bottom.percentage * containing_height.value / 100.0f;

	height.value = 0.0f;
	height.use_content = false;
	if (element_node->computed_properties.height.type == CSSValueHeight::type_length)
		height.value = element_node->computed_properties.height.length.value;
	else
		height.value = element_node->computed_properties.height.percentage * containing_height.value / 100.0f;

	if (element_node->computed_properties.top.type == CSSValueTop::type_auto &&
		element_node->computed_properties.bottom.type == CSSValueBottom::type_auto &&
		element_node->computed_properties.height.type == CSSValueHeight::type_auto)
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else if (element_node->computed_properties.top.type != CSSValueTop::type_auto &&
		element_node->computed_properties.bottom.type != CSSValueBottom::type_auto &&
		element_node->computed_properties.height.type != CSSValueHeight::type_auto)
	{
		if (element_node->computed_properties.margin_width_top.type == CSSValueMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
		{
			CSSUsedValue space_left = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - top - bottom;
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
		else if (element_node->computed_properties.margin_width_top.type == CSSValueMarginWidth::type_auto)
		{
			margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.bottom - top - bottom;
		}
		else if (element_node->computed_properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
		{
			margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.top - top - bottom;
		}
	}
	else if (element_node->computed_properties.top.type == CSSValueTop::type_auto &&
		element_node->computed_properties.height.type == CSSValueHeight::type_auto &&
		element_node->computed_properties.bottom.type != CSSValueBottom::type_auto) // rule #1
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else if (element_node->computed_properties.height.type == CSSValueHeight::type_auto &&
		element_node->computed_properties.bottom.type == CSSValueBottom::type_auto &&
		element_node->computed_properties.top.type != CSSValueTop::type_auto) // rule #3
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else if (element_node->computed_properties.height.type == CSSValueHeight::type_auto &&
		element_node->computed_properties.top.type != CSSValueTop::type_auto &&
		element_node->computed_properties.bottom.type != CSSValueBottom::type_auto) // rule #5
	{
		height.value = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - top - bottom;
	}

	if (!css_max_height.use_content && height.value > css_max_height.value)
		apply_absolute_heights_constraint(css_max_height.value, top, bottom);

	if (height.value < css_min_height)
		apply_absolute_heights_constraint(css_min_height, top, bottom);
}

void CSSLayoutTreeNode::apply_absolute_heights_constraint(CSSUsedValue constraint_height, CSSUsedValue top, CSSUsedValue bottom)
{
	height.value = constraint_height;
	if (element_node->computed_properties.top.type != CSSValueTop::type_auto &&
		element_node->computed_properties.bottom.type != CSSValueBottom::type_auto)
	{
		if (element_node->computed_properties.margin_width_top.type == CSSValueMarginWidth::type_auto &&
			element_node->computed_properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
		{
			CSSUsedValue space_left = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - top - bottom;
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
		else if (element_node->computed_properties.margin_width_top.type == CSSValueMarginWidth::type_auto)
		{
			margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.bottom - top - bottom;
		}
		else if (element_node->computed_properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
		{
			margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height.value - margin.top - top - bottom;
		}
	}
}

void CSSLayoutTreeNode::calculate_static_widths(LayoutStrategy strategy)
{
	bool is_table_cell = element_node->computed_properties.display.type == CSSValueDisplay::type_table_cell;
	bool is_float = element_node->computed_properties.float_box.type != CSSValueFloat::type_none;
	//bool is_overflow_visible = (element_node->get_parent() == 0) || element_node->is_overflow_visible();

	margin.left = get_css_margin_width(element_node->computed_properties.margin_width_left, containing_width);
	margin.right = get_css_margin_width(element_node->computed_properties.margin_width_right, containing_width);
	border.left = element_node->computed_properties.border_width_left.length.value;
	border.right = element_node->computed_properties.border_width_right.length.value;
	padding.left = get_css_padding_width(element_node->computed_properties.padding_width_left, containing_width);
	padding.right = get_css_padding_width(element_node->computed_properties.padding_width_right, containing_width);

	if (element_node->computed_properties.width.type == CSSValueWidth::type_length)
	{
		width.value = element_node->computed_properties.width.length.value;
		width.expanding = false;
	}
	else if (element_node->computed_properties.width.type == CSSValueWidth::type_percentage)
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
	else if (element_node->computed_properties.width.type == CSSValueWidth::type_auto)
	{
		if (containing_width.expanding || is_table_cell || is_float /*|| !is_overflow_visible*/)
		{
			width.value = 0.0f;
			width.expanding = true;
		}
		else
		{
			width.value = max(0.0f, containing_width.value - margin.left - margin.right - border.left - border.right - padding.left - padding.right);
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
			width.value = actual_to_used(
				used_to_actual(containing_width.value) -
				used_to_actual(margin.left) -
				used_to_actual(margin.right) -
				used_to_actual(border.left) -
				used_to_actual(border.right) -
				used_to_actual(padding.left) -
				used_to_actual(padding.right));

		if (!css_max_width.expanding)
			width.value = min(width.value, css_max_width.value);
		width.value = max(width.value, css_min_width);
	}
	else
	{
		if (!css_max_width.expanding)
			width.value = min(width.value, css_max_width.value);
		width.value = max(width.value, css_min_width);

		if (!containing_width.expanding && !is_float)
		{
			if (element_node->computed_properties.margin_width_left.type == CSSValueMarginWidth::type_auto && element_node->computed_properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
			{
				margin.left = max(0.0f, (containing_width.value-border.left-border.right-padding.left-padding.right-width.value)/2.0f);
				margin.right = max(0.0f, containing_width.value-border.left-border.right-padding.left-padding.right-width.value-margin.left);
			}
			else if (element_node->computed_properties.margin_width_left.type == CSSValueMarginWidth::type_auto)
			{
				margin.left = max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
			else if (element_node->computed_properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
			{
				margin.right = max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
			}

			if (margin.left + border.left + width.value + border.right + padding.right + margin.right > containing_width.value)
			{
				if (element_node->computed_properties.direction.type == CSSValueDirection::type_ltr)
					margin.right = max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
				else
					margin.left = max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
		}
	}
}

void CSSLayoutTreeNode::calculate_static_heights()
{
	margin.top = get_css_margin_height(element_node->computed_properties.margin_width_top, containing_height);
	margin.bottom = get_css_margin_height(element_node->computed_properties.margin_width_bottom, containing_height);
	border.top = element_node->computed_properties.border_width_top.length.value;
	border.bottom = element_node->computed_properties.border_width_bottom.length.value;
	padding.top = get_css_padding_height(element_node->computed_properties.padding_width_top, containing_height);
	padding.bottom = get_css_padding_height(element_node->computed_properties.padding_width_bottom, containing_height);

	if (element_node->computed_properties.height.type == CSSValueHeight::type_length)
	{
		height.value = element_node->computed_properties.height.length.value;
		height.use_content = false;
	}
	else if (element_node->computed_properties.height.type == CSSValueHeight::type_percentage)
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
	else if (element_node->computed_properties.height.type == CSSValueHeight::type_auto)
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
		height.value = min(height.value, css_max_width.value);
	height.value = max(height.value, css_min_width);
}

void CSSLayoutTreeNode::calculate_top_down_widths(LayoutStrategy strategy)
{
	css_max_width.expanding = true;
	css_max_width.value = 0.0f;
	if (element_node->computed_properties.max_width.type != CSSValueMaxWidth::type_none)
	{
		if (element_node->computed_properties.max_width.type == CSSValueMaxWidth::type_length)
		{
			css_max_width.value = element_node->computed_properties.max_width.length.value;
			css_max_width.expanding = false;
		}
		else if (element_node->computed_properties.max_width.type == CSSValueMaxWidth::type_percentage && !containing_width.expanding)
		{
			css_max_width.value = element_node->computed_properties.max_width.percentage * containing_width.value / 100.0f;
			css_max_width.expanding = false;
		}
	}

	css_min_width = 0.0f;
	if (element_node->computed_properties.min_width.type == CSSValueMinWidth::type_length)
		css_min_width = element_node->computed_properties.min_width.length.value;
	else if (element_node->computed_properties.min_width.type == CSSValueMinWidth::type_percentage && !containing_width.expanding)
		css_min_width = element_node->computed_properties.min_width.percentage * containing_width.value / 100.0f;


	if (element_node->computed_properties.position.type == CSSValuePosition::type_absolute ||
		element_node->computed_properties.position.type == CSSValuePosition::type_fixed)
	{
		calculate_absolute_widths(strategy);
	}
	else
	{
		calculate_static_widths(strategy);
	}
}

void CSSLayoutTreeNode::calculate_top_down_heights()
{
	css_max_height.value = 0.0f;
	css_max_height.use_content = true;
	if (element_node->computed_properties.max_height.type != CSSValueMaxHeight::type_none)
	{
		if (element_node->computed_properties.max_height.type == CSSValueMaxHeight::type_length)
		{
			css_max_height.value = element_node->computed_properties.max_height.length.value;
			css_max_height.use_content = false;
		}
		else if (element_node->computed_properties.max_height.type == CSSValueMaxHeight::type_percentage && !containing_height.use_content)
		{
			css_max_height.value = element_node->computed_properties.max_height.percentage * containing_height.value / 100.0f;
			css_max_height.use_content = false;
		}
	}

	css_min_height = 0.0f;
	if (element_node->computed_properties.min_height.type == CSSValueMinHeight::type_length)
		css_min_height = element_node->computed_properties.min_height.length.value;
	else if (element_node->computed_properties.min_height.type == CSSValueMinHeight::type_percentage && !containing_height.use_content)
		css_min_height = element_node->computed_properties.min_height.percentage * containing_height.value / 100.0f;


	if (element_node->computed_properties.position.type == CSSValuePosition::type_absolute ||
		element_node->computed_properties.position.type == CSSValuePosition::type_fixed)
	{
		calculate_absolute_heights();
	}
	else
	{
		calculate_static_heights();
	}

	calculate_content_top_down_heights();
}

void CSSLayoutTreeNode::set_root_block_position(int x, int y)
{
	if (!formatting_context_root)
		throw Exception("CSSLayoutTreeNode::set_root_block_position misuse");

	if (element_node->is_table() || element_node->is_inline_table())
	{
		content_box = Rect(Point(x+used_to_actual(margin.left), y+used_to_actual(margin.top)), content_box.get_size());
	}
	else
	{
		content_box = Rect(
			Point(
				x+used_to_actual(margin.left)+used_to_actual(border.left)+used_to_actual(padding.left),
				y+used_to_actual(margin.top)+used_to_actual(border.top)+used_to_actual(padding.top)),
				content_box.get_size());
	}
	formatting_context->set_position(content_box.left, content_box.top);
}

void CSSLayoutTreeNode::set_root_content_position(int x, int y)
{
	if (!formatting_context_root)
		throw Exception("CSSLayoutTreeNode::set_root_content_position misuse");
	content_box = Rect(Point(x, y), content_box.get_size());
	formatting_context->set_position(content_box.left, content_box.top);
}

void CSSLayoutTreeNode::calc_preferred(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	if (!preferred_width_calculated)
	{
		calculate_top_down_widths(preferred_strategy);
		calculate_top_down_heights();
		layout_formatting_root_helper(graphics, resources, preferred_strategy);
	}
}

void CSSLayoutTreeNode::calc_minimum(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	if (!min_width_calculated)
	{
		calculate_top_down_widths(minimum_strategy);
		calculate_top_down_heights();
		layout_formatting_root_helper(graphics, resources, minimum_strategy);
	}
}

void CSSLayoutTreeNode::layout_absolute_or_fixed(CSSLayoutGraphics *graphics, CSSResourceCache *resources, const Rect &containing_block, const Size &viewport_size)
{
	containing_width.value = containing_block.get_width();
	containing_width.expanding = false;
	containing_height.value = containing_block.get_height();
	containing_height.use_content = false;
	calculate_top_down_widths(normal_strategy);
	calculate_top_down_heights();

	CSSUsedValue left = 0.0f;
	if (element_node->computed_properties.left.type == CSSValueLeft::type_length)
		left = element_node->computed_properties.left.length.value;
	else if (element_node->computed_properties.left.type == CSSValueLeft::type_percentage)
		left = element_node->computed_properties.left.percentage * containing_width.value / 100.0f;

	CSSUsedValue right = 0.0f;
	if (element_node->computed_properties.right.type == CSSValueRight::type_length)
		right = element_node->computed_properties.right.length.value;
	else if (element_node->computed_properties.right.type == CSSValueRight::type_percentage)
		right = element_node->computed_properties.right.percentage * containing_width.value / 100.0f;

	if (width.expanding)
	{
		CSSUsedValue available_width = containing_width.value;
		if (element_node->computed_properties.left.type == CSSValueLeft::type_auto)
		{
			available_width = containing_width.value - margin.left - border.left - padding.left - padding.right - border.right - margin.right - right;
		}
		else if (element_node->computed_properties.right.type == CSSValueRight::type_auto)
		{
			available_width = containing_width.value - margin.left - border.left - padding.left - padding.right - border.right - margin.right - left;
		}
		layout_shrink_to_fit(graphics, resources, available_width);
	}
	else
	{
		layout_formatting_root_helper(graphics, resources, normal_strategy);
	}

	if (element_node->computed_properties.left.type == CSSValueLeft::type_auto &&
		element_node->computed_properties.right.type == CSSValueRight::type_auto &&
		element_node->computed_properties.width.type == CSSValueWidth::type_auto)
	{
		if (element_node->computed_properties.direction.type == CSSValueDirection::type_ltr)
		{
			CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
			left = offset_x + static_position.left - containing_block.left;
			right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
		}
		else
		{
			CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
			right = offset_x + static_position.right - containing_block.left;
			left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
		}
	}
	else
	{
		if (element_node->computed_properties.left.type == CSSValueLeft::type_auto &&
			element_node->computed_properties.width.type == CSSValueWidth::type_auto &&
			element_node->computed_properties.right.type != CSSValueRight::type_auto) // rule #1
		{
			left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
		}
		else if (element_node->computed_properties.left.type == CSSValueLeft::type_auto &&
			element_node->computed_properties.right.type == CSSValueRight::type_auto &&
			element_node->computed_properties.width.type != CSSValueWidth::type_auto) // rule #2
		{
			if (element_node->computed_properties.direction.type == CSSValueDirection::type_ltr)
			{
				CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
				left = offset_x + static_position.left - containing_block.left;
				right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
			}
			else
			{
				CSSActualValue offset_x = static_position_parent ? static_position_parent->formatting_context->get_x() : 0;
				right = offset_x + static_position.right - containing_block.left;
				left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
			}
		}
		else if (element_node->computed_properties.width.type == CSSValueWidth::type_auto &&
			element_node->computed_properties.right.type == CSSValueRight::type_auto &&
			element_node->computed_properties.left.type != CSSValueLeft::type_auto) // rule #3
		{
			right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
		}
		else if (element_node->computed_properties.left.type == CSSValueLeft::type_auto &&
			element_node->computed_properties.width.type != CSSValueWidth::type_auto &&
			element_node->computed_properties.right.type != CSSValueRight::type_auto) // rule #4
		{
			left = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - right;
		}
		else if (element_node->computed_properties.right.type == CSSValueRight::type_auto &&
			element_node->computed_properties.left.type != CSSValueLeft::type_auto &&
			element_node->computed_properties.width.type != CSSValueWidth::type_auto) // rule #6
		{
			right = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - width.value - left;
		}
	}


	CSSUsedValue top = 0.0f;
	if (element_node->computed_properties.top.type == CSSValueTop::type_length)
		top = element_node->computed_properties.top.length.value;
	else if (element_node->computed_properties.top.type == CSSValueTop::type_percentage)
		top = element_node->computed_properties.top.percentage * containing_height.value / 100.0f;

	CSSUsedValue bottom = 0.0f;
	if (element_node->computed_properties.bottom.type == CSSValueBottom::type_length)
		bottom = element_node->computed_properties.bottom.length.value;
	else if (element_node->computed_properties.bottom.type == CSSValueBottom::type_percentage)
		bottom = element_node->computed_properties.bottom.percentage * containing_height.value / 100.0f;

	if (element_node->computed_properties.top.type == CSSValueTop::type_auto &&
		element_node->computed_properties.bottom.type == CSSValueBottom::type_auto &&
		element_node->computed_properties.height.type == CSSValueHeight::type_auto)
	{
		CSSActualValue offset_y = static_position_parent ? static_position_parent->formatting_context->get_y() : 0;
		top = offset_y + static_position.top - containing_block.top;
		bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
	}
	else
	{
		if (element_node->computed_properties.top.type == CSSValueTop::type_auto &&
			element_node->computed_properties.height.type == CSSValueHeight::type_auto &&
			element_node->computed_properties.bottom.type != CSSValueBottom::type_auto) // rule #1
		{
			top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - bottom;
		}
		else if (element_node->computed_properties.top.type == CSSValueTop::type_auto &&
			element_node->computed_properties.bottom.type == CSSValueBottom::type_auto &&
			element_node->computed_properties.height.type != CSSValueHeight::type_auto) // rule #2
		{
			CSSActualValue offset_y = static_position_parent ? static_position_parent->formatting_context->get_y() : 0;
			top = offset_y + static_position.top - containing_block.top;
			bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
		}
		else if (element_node->computed_properties.height.type == CSSValueHeight::type_auto &&
			element_node->computed_properties.bottom.type == CSSValueBottom::type_auto &&
			element_node->computed_properties.top.type != CSSValueTop::type_auto) // rule #3
		{
			bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
		}
		else if (element_node->computed_properties.top.type == CSSValueTop::type_auto &&
			element_node->computed_properties.height.type != CSSValueHeight::type_auto &&
			element_node->computed_properties.bottom.type != CSSValueBottom::type_auto) // rule #4
		{
			top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - bottom;
		}
		else if (element_node->computed_properties.bottom.type == CSSValueBottom::type_auto &&
			element_node->computed_properties.top.type != CSSValueTop::type_auto &&
			element_node->computed_properties.height.type != CSSValueHeight::type_auto) // rule #6
		{
			bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - height.value - top;
		}
	}

	int x = containing_block.left + used_to_actual(left);
	int y = containing_block.top + used_to_actual(top);
	if (element_node->is_absolute() && formatting_context->get_parent())
	{
		x -= formatting_context->get_parent()->get_x();
		y -= formatting_context->get_parent()->get_y();
	}
	set_root_block_position(x, y);
}

void CSSLayoutTreeNode::layout_shrink_to_fit(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSUsedValue available_width)
{
	calc_preferred(graphics, resources);
	CSSUsedValue shrink_to_fit_width;
	if (preferred_width > available_width + 0.1f)
	{
		calc_minimum(graphics, resources);
		shrink_to_fit_width = max(min_width, available_width);
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

void CSSLayoutTreeNode::layout_float(CSSLayoutGraphics *graphics, CSSResourceCache *resources, LayoutStrategy strategy)
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

/*	if (element_node->computed_properties.background_color.type == CSSValueBackgroundColor::type_color &&
		element_node->computed_properties.background_color.color == Colorf::green)
	{
		cl_debug_breakpoint = 0xdeadbabe;
	}*/
}

void CSSLayoutTreeNode::layout_formatting_root_helper(CSSLayoutGraphics *graphics, CSSResourceCache *resources, LayoutStrategy strategy)
{
/*	if (element_node->name.find("#navigation") != std::string::npos)
	{
		Sleep(1);
	}*/

	formatting_context->clear();

	CSSLayoutCursor cursor;
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
		CSSActualValue left_float_height = formatting_context->find_left_clearance();
		CSSActualValue right_float_height = formatting_context->find_right_clearance();
		height.value = max(height.value, actual_to_used(max(left_float_height, right_float_height)));

		if (!css_max_height.use_content)
			height.value = min(height.value, css_max_height.value);
		height.value = max(height.value, css_min_height);
	}

	content_box = Size(used_to_actual(width.value), used_to_actual(height.value));
}

void CSSLayoutTreeNode::layout_normal(CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
/*	if (element_node->name.find("head") != std::string::npos)
	{
		Sleep(1);
	}*/

	calculate_top_down_widths(strategy);

	CSSActualValue old_x = cursor.x;
	CSSUsedValue old_relative_x = cursor.relative_x;
	CSSUsedValue old_relative_y = cursor.relative_y;
	cursor.x += used_to_actual(margin.left) + used_to_actual(border.left) + used_to_actual(padding.left);
	cursor.relative_x += get_local_relative_x();
	cursor.relative_y += get_local_relative_y();

	relative_x = cursor.relative_x;
	relative_y = cursor.relative_y;

	add_margin_top(cursor);

	if (element_node->computed_properties.clear.type == CSSValueClear::type_left || element_node->computed_properties.clear.type == CSSValueClear::type_both)
	{
		CSSActualValue clear_left = formatting_context->find_left_clearance();
		if (cursor.y+cursor.get_total_margin() < clear_left)
			cursor.y = clear_left-cursor.get_total_margin();
	}
	if (element_node->computed_properties.clear.type == CSSValueClear::type_right || element_node->computed_properties.clear.type == CSSValueClear::type_both)
	{
		CSSActualValue clear_right = formatting_context->find_right_clearance();
		if (cursor.y+cursor.get_total_margin() < clear_right)
			cursor.y = clear_right-cursor.get_total_margin();
	}

	if (border.top > 0 || padding.top > 0)
	{
		cursor.apply_margin();
		cursor.y += used_to_actual(border.top) + used_to_actual(padding.top);
	}

	CSSActualValue before_content_y = cursor.y;

	content_box.left = cursor.x;
	content_box.top = cursor.y+cursor.get_total_margin();
	content_box.right = content_box.left+used_to_actual(width.value);
	content_box.bottom = content_box.top+used_to_actual(height.value);

	layout_content(graphics, cursor, strategy);

	if (height.use_content)
	{
		if (cursor.y == before_content_y)
		{
			height.value = actual_to_used(0);
		}
		else
		{
			height.value = actual_to_used(max(0, cursor.y + cursor.get_total_margin() - content_box.top));
		}

		if (!css_max_height.use_content)
			height.value = min(height.value, css_max_height.value);
		height.value = max(height.value, css_min_height);

		content_box.bottom = content_box.top + used_to_actual(height.value);
	}
	else
	{
		if (height.value > 0.0f)
			cursor.apply_margin();
		cursor.y = content_box.top + used_to_actual(height.value);
	}


	if (strategy != normal_strategy)
	{
		content_box.right = content_box.left+used_to_actual(width.value);
	}

	if (border.bottom > 0 || padding.bottom > 0)
	{
		cursor.apply_margin();
		cursor.y += used_to_actual(border.bottom) + used_to_actual(padding.bottom);
	}

	cursor.add_margin(margin.bottom);
	cursor.x = old_x;
	cursor.relative_x = old_relative_x;
	cursor.relative_y = old_relative_y;
}

CSSUsedValue CSSLayoutTreeNode::get_local_relative_x() const
{
	return get_local_relative_x(element_node, containing_width.value);
}

CSSUsedValue CSSLayoutTreeNode::get_local_relative_y() const
{
	return get_local_relative_y(element_node, containing_height.value);
}

CSSUsedValue CSSLayoutTreeNode::get_local_relative_x(const CSSBoxElement *element, CSSUsedValue containing_width)
{
	if (element->computed_properties.position.type == CSSValuePosition::type_relative)
	{
		if (element->computed_properties.left.type == CSSValueLeft::type_length)
			return element->computed_properties.left.length.value;
		else if (element->computed_properties.left.type == CSSValueLeft::type_percentage)
			return element->computed_properties.left.percentage / 100.0f * containing_width;
		else
			return 0.0f;
	}
	else
	{
		return 0.0f;
	}
}

CSSUsedValue CSSLayoutTreeNode::get_local_relative_y(const CSSBoxElement *element, CSSUsedValue containing_height)
{
	if (element->computed_properties.position.type == CSSValuePosition::type_relative)
	{
		if (element->computed_properties.top.type == CSSValueTop::type_length)
			return element->computed_properties.top.length.value;
		else if (element->computed_properties.top.type == CSSValueTop::type_percentage)
			return element->computed_properties.top.percentage / 100.0f * containing_height;
		else
			return 0.0f;
	}
	else
	{
		return 0.0f;
	}
}

bool CSSLayoutTreeNode::add_margin_top(CSSLayoutCursor &cursor)
{
/*	if (element_node->name.find("body") != std::string::npos)
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
		if (element_node->computed_properties.min_height.type == CSSValueMinHeight::type_length)
			min_height = element_node->computed_properties.min_height.length.value;
		else if (element_node->computed_properties.min_height.type == CSSValueMinHeight::type_percentage)
			min_height = element_node->computed_properties.min_height.percentage * containing_height.value / 100.0f;

		if (min_height > 0.0f || border.bottom > 0.0f || padding.bottom > 0.0f)
			return true;

		cursor.add_margin(margin.bottom); // Margins are adjoining
		return false;
	}
}

void CSSLayoutTreeNode::set_formatting_context(CSSBlockFormattingContext *new_formatting_context, bool is_root)
{
	if (formatting_context_root)
		delete formatting_context;
	formatting_context = new_formatting_context;
	formatting_context_root = is_root;
}

void CSSLayoutTreeNode::establish_stacking_context_if_needed(CSSStackingContext *current_stacking_context)
{
	if (element_node->computed_properties.position.type != CSSValuePosition::type_static && element_node->computed_properties.z_index.type != CSSValueZIndex::type_auto)
	{
		stacking_context = new CSSStackingContext(this);
		stacking_context_root = true;
		current_stacking_context->push_back(stacking_context);
	}
	else
	{
		stacking_context = current_stacking_context;
		stacking_context_root = false;
	}
}

CSSActualValue CSSLayoutTreeNode::get_block_width() const
{
	return used_to_actual(margin.left) +
		used_to_actual(border.left) +
		used_to_actual(padding.left) +
		used_to_actual(width.value) +
		used_to_actual(padding.right) +
		used_to_actual(border.right) +
		used_to_actual(margin.right);
}

CSSActualValue CSSLayoutTreeNode::get_block_height() const
{
	return used_to_actual(margin.top) +
		used_to_actual(border.top) +
		used_to_actual(padding.top) +
		used_to_actual(height.value) +
		used_to_actual(padding.bottom) +
		used_to_actual(border.bottom) +
		used_to_actual(margin.bottom);
}

void CSSLayoutTreeNode::render_non_content(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, bool root)
{
	if (element_node->computed_properties.visibility.type == CSSValueVisibility::type_visible &&
		element_node->computed_properties.display.type != CSSValueDisplay::type_table_cell)
	{
		render_background(graphics, resource_cache, root);
		render_border(graphics, resource_cache);

		if (element_node->computed_properties.display.type == CSSValueDisplay::type_list_item &&
			element_node->computed_properties.list_style_type.type != CSSValueListStyleType::type_none)
		{
			std::string bullet = StringHelp::wchar_to_utf8(8226);
			Font font = graphics->get_font(element_node->computed_properties);
			Size offset = graphics->get_text_size(font, bullet);
			offset.width += 8;
			// to do: find baseline of first item
			graphics->draw_text(font, used_to_actual(relative_x) + formatting_context->get_x() + content_box.left-offset.width, used_to_actual(relative_y) + formatting_context->get_y() + content_box.top + (int)(graphics->get_font_metrics(font).get_ascent()), bullet, element_node->computed_properties.color.color);
		}
	}
}

Rect CSSLayoutTreeNode::get_border_box() const
{
	Rect border_rect = get_padding_box();
	border_rect.expand(used_to_actual(border.left), used_to_actual(border.top), used_to_actual(border.right), used_to_actual(border.bottom));
	return border_rect;
}

Rect CSSLayoutTreeNode::get_padding_box() const
{
	Rect padding_rect = content_box;
	if (!formatting_context_root)
		padding_rect.translate(formatting_context->get_x(), formatting_context->get_y());
	else if (formatting_context->get_parent())
		padding_rect.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
	padding_rect.expand(used_to_actual(padding.left), used_to_actual(padding.top), used_to_actual(padding.right), used_to_actual(padding.bottom));
	return padding_rect;
}

void CSSLayoutTreeNode::render_background(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, bool root)
{
	Rect box = content_box;
	if (!formatting_context_root)
		box.translate(formatting_context->get_x(), formatting_context->get_y());
	else if (formatting_context->get_parent())
		box.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
	box.translate(used_to_actual(relative_x), used_to_actual(relative_y));

	CSSBackgroundRenderer renderer(graphics, resource_cache, element_node->computed_properties);
	renderer.set_is_root(root);
	renderer.set_initial_containing_box(Rect(0, 0, used_to_actual(containing_width.value), used_to_actual(containing_height.value))); // Bug: this is wrong except for the root
	renderer.set_content_box(box);
	box.expand(used_to_actual(padding.left), used_to_actual(padding.top), used_to_actual(padding.right), used_to_actual(padding.bottom));
	renderer.set_padding_box(box);
	box.expand(used_to_actual(border.left), used_to_actual(border.top), used_to_actual(border.right), used_to_actual(border.bottom));
	renderer.set_border_box(box);
	renderer.render();
}

void CSSLayoutTreeNode::render_border(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache)
{
	Rect border_box = get_border_box();
	border_box.translate(relative_x, relative_y);

	CSSBorderRenderer renderer(graphics, resource_cache, element_node->computed_properties);
	renderer.set_border_values(border.left, border.top, border.right, border.bottom);
	renderer.set_border_box(border_box);
	renderer.render();
}

}
