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

#pragma once

#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include <algorithm>

namespace clan
{

// \brief Computed values are first converted to used values (we use a floating point representation for this coordinate space)
typedef float CSSUsedValue;

// \brief Used values are then converted to actual pixel sizes before rendering
typedef int CSSActualValue;

// \brief Used values used in box size calculations
class GUICSSUsedValues
{
public:
	GUICSSUsedValues() : width(0.0f), height(0.0f), width_undetermined(false), height_undetermined(false), min_width(0.0f), max_width(1e5f), min_height(0.0f), max_height(1e5f)/*, left(0.0f), top(0.0f)*/ { }

	struct LTRB
	{
		LTRB() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CSSUsedValue left, right, top, bottom;
	};

	LTRB margin;
	LTRB border;
	LTRB padding;

	CSSUsedValue width;
	CSSUsedValue height;

	bool width_undetermined;
	bool height_undetermined;

	CSSUsedValue min_width;
	CSSUsedValue max_width;

	CSSUsedValue min_height;
	CSSUsedValue max_height;

//	CSSUsedValue left;
//	CSSUsedValue top;
};

// \brief Calculates box used values without applying any constraints or auto rules
class GUICSSInitialUsedValues
{
public:
	static void visit(GUICSSUsedValues &values, const CSSComputedValues &properties, GUICSSUsedValues &containing_box)
	{
		const CSSComputedBox &box_properties = properties.get_box();
		const CSSComputedMargin &margin_properties = properties.get_margin();
		const CSSComputedBorder &border_properties = properties.get_border();
		const CSSComputedPadding &padding_properties = properties.get_padding();

		values.margin.left = get_css_margin_width(margin_properties.margin_width_left, containing_box.width, containing_box.width_undetermined);
		values.margin.right = get_css_margin_width(margin_properties.margin_width_right, containing_box.width, containing_box.width_undetermined);
		values.border.left = get_css_border_size(border_properties.border_width_left);
		values.border.right = get_css_border_size(border_properties.border_width_right);
		values.padding.left = get_css_padding_width(padding_properties.padding_width_left, containing_box.width, containing_box.width_undetermined);
		values.padding.right = get_css_padding_width(padding_properties.padding_width_right, containing_box.width, containing_box.width_undetermined);

		values.margin.top = get_css_margin_height(margin_properties.margin_width_top, containing_box.height, containing_box.height_undetermined);
		values.margin.bottom = get_css_margin_height(margin_properties.margin_width_bottom, containing_box.height, containing_box.height_undetermined);
		values.border.top = get_css_border_size(border_properties.border_width_top);
		values.border.bottom = get_css_border_size(border_properties.border_width_bottom);
		values.padding.top = get_css_padding_height(padding_properties.padding_width_top, containing_box.height, containing_box.height_undetermined);
		values.padding.bottom = get_css_padding_height(padding_properties.padding_width_bottom, containing_box.height, containing_box.height_undetermined);

		values.min_width = get_css_min_width(box_properties.min_width, containing_box.width, containing_box.width_undetermined);
		values.max_width = get_css_max_width(box_properties.max_width, containing_box.width, containing_box.width_undetermined);

		values.min_height = get_css_min_height(box_properties.min_height, containing_box.height, containing_box.height_undetermined);
		values.max_height = get_css_max_height(box_properties.max_height, containing_box.height, containing_box.height_undetermined);

		get_css_width(values, box_properties, containing_box);
		get_css_height(values, box_properties, containing_box);
	}

private:
	static void get_css_width(GUICSSUsedValues &values, const CSSComputedBox &properties, GUICSSUsedValues &containing_box)
	{
		switch (properties.width.type)
		{
		case CSSValueWidth::type_auto:
			values.width = 0.0f;
			values.width_undetermined = true;
			break;
		case CSSValueWidth::type_length:
			values.width = properties.width.length.value;
			values.width_undetermined = false;
			break;
		case CSSValueWidth::type_percentage:
			if (containing_box.width_undetermined)
			{
				values.width = 0.0f;
				values.width_undetermined = true;
			}
			else
			{
				values.width = properties.width.percentage * containing_box.width / 100.0f;
				values.width_undetermined = false;
			}
			break;
		}
	}

	static void get_css_height(GUICSSUsedValues &values, const CSSComputedBox &properties, GUICSSUsedValues &containing_box)
	{
		switch (properties.height.type)
		{
		case CSSValueHeight::type_auto:
			values.height = 0.0f;
			values.height_undetermined = true;
			break;
		case CSSValueHeight::type_length:
			values.height = properties.height.length.value;
			values.height_undetermined = false;
			break;
		case CSSValueHeight::type_percentage:
			if (containing_box.height_undetermined)
			{
				values.height = 0.0f;
				values.height_undetermined = true;
			}
			else
			{
				values.height = properties.height.percentage * containing_box.height / 100.0f;
				values.height_undetermined = false;
			}
			break;
		}
	}

	static CSSUsedValue get_css_margin_width(const CSSValueMarginWidth &margin_width, float containing_used_width, bool containing_width_undetermined)
	{
		switch (margin_width.type)
		{
		case CSSValueMarginWidth::type_auto: return 0.0f;
		case CSSValueMarginWidth::type_length: return margin_width.length.value;
		case CSSValueMarginWidth::type_percentage: return containing_width_undetermined ? 0.0f : margin_width.percentage * containing_used_width / 100.0f;
		default: return 0.0f;
		}
	}

	static CSSUsedValue get_css_padding_width(const CSSValuePaddingWidth &padding_width, float containing_used_width, bool containing_width_undetermined)
	{
		switch (padding_width.type)
		{
		case CSSValuePaddingWidth::type_length: return padding_width.length.value;
		case CSSValuePaddingWidth::type_percentage: return containing_width_undetermined ? 0.0f : padding_width.percentage * containing_used_width / 100.0f;
		default: return 0.0f;
		}
	}

	static CSSUsedValue get_css_margin_height(const CSSValueMarginWidth &margin_width, float containing_used_height, bool containing_height_undetermined)
	{
		switch (margin_width.type)
		{
		case CSSValueMarginWidth::type_auto: return 0.0f;
		case CSSValueMarginWidth::type_length: return margin_width.length.value;
		case CSSValueMarginWidth::type_percentage: return containing_height_undetermined ? 0.0f : margin_width.percentage * containing_used_height / 100.0f;
		default: return 0.0f;
		}
	}

	static CSSUsedValue get_css_padding_height(const CSSValuePaddingWidth &padding_width, float containing_used_height, bool containing_height_undetermined)
	{
		switch (padding_width.type)
		{
		case CSSValuePaddingWidth::type_length: return padding_width.length.value;
		case CSSValuePaddingWidth::type_percentage: return containing_height_undetermined ? 0.0f : padding_width.percentage * containing_used_height / 100.0f;
		default: return 0.0f;
		}
	}

	static CSSUsedValue get_css_border_size(const CSSValueBorderWidth &border_width)
	{
		return border_width.length.value;
	}

	static CSSUsedValue get_css_min_width(const CSSValueMinWidth &min_width, float containing_used_width, bool containing_width_undetermined)
	{
		switch (min_width.type)
		{
		case CSSValueMinWidth::type_length: return min_width.length.value;
		case CSSValueMinWidth::type_percentage: return containing_width_undetermined ? 0.0f : min_width.percentage * containing_used_width / 100.0f;
		case CSSValueMinWidth::type_auto:
		default: return 0.0f;
		}
	}

	static CSSUsedValue get_css_max_width(const CSSValueMaxWidth &max_width, float containing_used_width, bool containing_width_undetermined)
	{
		switch (max_width.type)
		{
		case CSSValueMaxWidth::type_none: return 1e5f;
		case CSSValueMaxWidth::type_length: return max_width.length.value;
		case CSSValueMaxWidth::type_percentage: return containing_width_undetermined ? 1e5f : max_width.percentage * containing_used_width / 100.0f;
		default: return 0.0f;
		}
	}

	static CSSUsedValue get_css_min_height(const CSSValueMinHeight &min_height, float containing_used_height, bool containing_height_expanding)
	{
		switch (min_height.type)
		{
		case CSSValueMinHeight::type_length: return min_height.length.value;
		case CSSValueMinHeight::type_percentage: return containing_height_expanding ? 0.0f : min_height.percentage * containing_used_height / 100.0f;
		case CSSValueMinHeight::type_auto:
		default: return 0.0f;
		}
	}

	static CSSUsedValue get_css_max_height(const CSSValueMaxHeight &max_height, float containing_used_height, bool containing_height_expanding)
	{
		switch (max_height.type)
		{
		case CSSValueMaxHeight::type_none: return 1e5f;
		case CSSValueMaxHeight::type_length: return max_height.length.value;
		case CSSValueMaxHeight::type_percentage: return containing_height_expanding ? 1e5f : max_height.percentage * containing_used_height / 100.0f;
		default: return 0.0f;
		}
	}
};

// \brief Apply min/max constraints on box
class GUICSSApplyMinMaxConstraints
{
public:
	static void visit(GUICSSUsedValues &values, const CSSComputedBox &properties, GUICSSUsedValues &containing_box)
	{
		values.width = std::min(values.width, values.max_width);
		values.width = std::max(values.width, values.min_width);

		values.height = std::min(values.height, values.max_height);
		values.height = std::max(values.height, values.min_height);
	}
};

/*
	CSSClanBoxUsedValueSizes() : width(0.0f), height(0.0f), width_undetermined(false), height_undetermined(false) { }

	enum LayoutStrategy
	{
		normal_strategy,
		preferred_strategy,
		minimum_strategy
	};



	void calculate_absolute_widths(const CSSComputedBox &properties, LayoutStrategy strategy)
	{
		margin.left = get_css_margin_width(properties.margin_width_left, containing_width);
		margin.right = get_css_margin_width(properties.margin_width_right, containing_width);
		border.left = get_css_border_width(properties.border_width_left);
		border.right = get_css_border_width(properties.border_width_right);
		padding.left = get_css_padding_width(properties.padding_width_left, containing_width);
		padding.right = get_css_padding_width(properties.padding_width_right, containing_width);

		CSSUsedValue left = 0.0f;
		if (properties.left.type == CSSValueLeft::type_length)
			left = properties.left.length.value;
		else if (properties.left.type == CSSValueLeft::type_percentage)
			left = properties.left.percentage * containing_width.value / 100.0f;

		CSSUsedValue right = 0.0f;
		if (properties.right.type == CSSValueRight::type_length)
			right = properties.right.length.value;
		else if (properties.right.type == CSSValueRight::type_percentage)
			right = properties.right.percentage * containing_width.value / 100.0f;

		width = 0.0f;
		width_undetermined = false;
		if (properties.width.type == CSSValueWidth::type_length)
			width = properties.width.length.value;
		else
			width = properties.width.percentage * containing_width.value / 100.0f;

		if (properties.left.type == CSSValueLeft::type_auto &&
			properties.right.type == CSSValueRight::type_auto &&
			properties.width.type == CSSValueWidth::type_auto)
		{
			width = 0.0f;
			width_undetermined = true;
		}
		else if (properties.left.type != CSSValueLeft::type_auto &&
			properties.right.type != CSSValueRight::type_auto &&
			properties.width.type != CSSValueWidth::type_auto)
		{
			if (properties.margin_width_left.type == CSSValueMarginWidth::type_auto &&
				properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
			{
				CSSUsedValue space_left = containing_width.value - border.left - border.right - padding.left - padding.right - width - left - right;
				if (space_left < 0.0f)
				{
					if (properties.direction.type == CSSValueDirection::type_ltr)
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
			else if (properties.margin_width_left.type == CSSValueMarginWidth::type_auto)
			{
				margin.left = containing_width.value - border.left - border.right - padding.left - padding.right - width - margin.right - left - right;
			}
			else if (properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
			{
				margin.right = containing_width.value - border.left - border.right - padding.left - padding.right - width - margin.left - left - right;
			}
		}
		else if (properties.left.type == CSSValueLeft::type_auto &&
			properties.width.type == CSSValueWidth::type_auto &&
			properties.right.type != CSSValueRight::type_auto) // rule #1
		{
			width = 0.0f;
			width_undetermined = true;
		}
		else if (properties.width.type == CSSValueWidth::type_auto &&
			properties.right.type == CSSValueRight::type_auto &&
			properties.left.type != CSSValueLeft::type_auto) // rule #3
		{
			width = 0.0f;
			width_undetermined = true;
		}
		else if (properties.width.type == CSSValueWidth::type_auto &&
			properties.left.type != CSSValueLeft::type_auto &&
			properties.right.type != CSSValueRight::type_auto) // rule #5
		{
			width = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - left - right;
		}

		if (width_undetermined && strategy == normal_strategy)
		{
			width = actual_to_used(
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
		if (properties.max_width.type != CSSValueMaxWidth::type_none)
		{
			if (properties.max_width.type == CSSValueMaxWidth::type_length)
			{
				css_max_width.value = properties.max_width.length.value;
				css_max_width.expanding = false;
			}
			else if (properties.max_width.type == CSSValueMaxWidth::type_percentage && !containing_width.expanding)
			{
				css_max_width.value = properties.max_width.percentage * containing_width.value / 100.0f;
				css_max_width.expanding = false;
			}
		}

		if (!css_max_width.expanding && width > css_max_width.value)
			apply_absolute_widths_constraint(css_max_width.value, left, right);

		if (width < css_min_width)
			apply_absolute_widths_constraint(min_width, left, right, containing_width);
	}

	void apply_absolute_widths_constraint(const CSSComputedBox &properties, CSSUsedValue constraint_width, CSSUsedValue left, CSSUsedValue right, CSSUsedValue containing_width)
	{
		width = constraint_width;
		if (properties.left.type != CSSValueLeft::type_auto &&
			properties.right.type != CSSValueRight::type_auto)
		{
			if (properties.margin_width_left.type == CSSValueMarginWidth::type_auto &&
				properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
			{
				CSSUsedValue space_left = containing_width - border.left - border.right - padding.left - padding.right - width - left - right;
				if (space_left < 0.0f)
				{
					if (properties.direction.type == CSSValueDirection::type_ltr)
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
			else if (properties.margin_width_left.type == CSSValueMarginWidth::type_auto)
			{
				margin.left = containing_width - border.left - border.right - padding.left - padding.right - width - margin.right - left - right;
			}
			else if (properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
			{
				margin.right = containing_width - border.left - border.right - padding.left - padding.right - width - margin.left - left - right;
			}
		}
	}

	void calculate_absolute_heights(const CSSComputedBox &properties)
	{
		margin.top = get_css_margin_height(properties.margin_width_top, containing_height);
		margin.bottom = get_css_margin_height(properties.margin_width_bottom, containing_height);
		border.top = get_css_border_width(properties.border_width_top);
		border.bottom = get_css_border_width(properties.border_width_bottom);
		padding.top = get_css_padding_height(properties.padding_width_top, containing_height);
		padding.bottom = get_css_padding_height(properties.padding_width_bottom, containing_height);

		CSSUsedValue top = 0.0f;
		if (properties.top.type == CSSValueTop::type_length)
			top = properties.top.length.value;
		else if (properties.top.type == CSSValueTop::type_percentage)
			top = properties.top.percentage * containing_height.value / 100.0f;

		CSSUsedValue bottom = 0.0f;
		if (properties.bottom.type == CSSValueBottom::type_length)
			bottom = properties.bottom.length.value;
		else if (properties.bottom.type == CSSValueBottom::type_percentage)
			bottom = properties.bottom.percentage * containing_height.value / 100.0f;

		height = 0.0f;
		height_undetermined = false;
		if (properties.height.type == CSSValueHeight::type_length)
			height = properties.height.length.value;
		else
			height = properties.height.percentage * containing_height.value / 100.0f;

		if (properties.top.type == CSSValueTop::type_auto &&
			properties.bottom.type == CSSValueBottom::type_auto &&
			properties.height.type == CSSValueHeight::type_auto)
		{
			height = 0.0f;
			height_undetermined = true;
		}
		else if (properties.top.type != CSSValueTop::type_auto &&
			properties.bottom.type != CSSValueBottom::type_auto &&
			properties.height.type != CSSValueHeight::type_auto)
		{
			if (properties.margin_width_top.type == CSSValueMarginWidth::type_auto &&
				properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
			{
				CSSUsedValue space_left = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - top - bottom;
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
			else if (properties.margin_width_top.type == CSSValueMarginWidth::type_auto)
			{
				margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.bottom - top - bottom;
			}
			else if (properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
			{
				margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.top - top - bottom;
			}
		}
		else if (properties.top.type == CSSValueTop::type_auto &&
			properties.height.type == CSSValueHeight::type_auto &&
			properties.bottom.type != CSSValueBottom::type_auto) // rule #1
		{
			height = 0.0f;
			height_undetermined = true;
		}
		else if (properties.height.type == CSSValueHeight::type_auto &&
			properties.bottom.type == CSSValueBottom::type_auto &&
			properties.top.type != CSSValueTop::type_auto) // rule #3
		{
			height = 0.0f;
			height_undetermined = true;
		}
		else if (properties.height.type == CSSValueHeight::type_auto &&
			properties.top.type != CSSValueTop::type_auto &&
			properties.bottom.type != CSSValueBottom::type_auto) // rule #5
		{
			height = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - top - bottom;
		}

		if (!css_max_height.use_content && height > css_max_height.value)
			apply_absolute_heights_constraint(css_max_height.value, top, bottom);

		if (height < css_min_height)
			apply_absolute_heights_constraint(css_min_height, top, bottom);
	}

	void apply_absolute_heights_constraint(const CSSComputedBox &properties, CSSUsedValue constraint_height, CSSUsedValue top, CSSUsedValue bottom)
	{
		height = constraint_height;
		if (properties.top.type != CSSValueTop::type_auto &&
			properties.bottom.type != CSSValueBottom::type_auto)
		{
			if (properties.margin_width_top.type == CSSValueMarginWidth::type_auto &&
				properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
			{
				CSSUsedValue space_left = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - top - bottom;
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
			else if (properties.margin_width_top.type == CSSValueMarginWidth::type_auto)
			{
				margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.bottom - top - bottom;
			}
			else if (properties.margin_width_bottom.type == CSSValueMarginWidth::type_auto)
			{
				margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.top - top - bottom;
			}
		}
	}

	void calculate_static_widths(const CSSComputedBox &properties, LayoutStrategy strategy)
	{
		bool is_table_cell = properties.display.type == CSSValueDisplay::type_table_cell;
		bool is_float = properties.float_box.type != CSSValueFloat::type_none;
		//bool is_overflow_visible = (element_node->get_parent() == 0) || element_node->is_overflow_visible();

		margin.left = get_css_margin_width(properties.margin_width_left, containing_width);
		margin.right = get_css_margin_width(properties.margin_width_right, containing_width);
		border.left = get_css_border_width(properties.border_width_left);
		border.right = get_css_border_width(properties.border_width_right);
		padding.left = get_css_padding_width(properties.padding_width_left, containing_width);
		padding.right = get_css_padding_width(properties.padding_width_right, containing_width);

		if (properties.width.type == CSSValueWidth::type_length)
		{
			width = properties.width.length.value;
			width_undetermined = false;
		}
		else if (properties.width.type == CSSValueWidth::type_percentage)
		{
			if (containing_width.expanding)
			{
				width = 0.0f;
				width_undetermined = true;
			}
			else
			{
				width = properties.width.percentage * containing_width.value / 100.0f;
				width_undetermined = false;
			}
		}
		else if (properties.width.type == CSSValueWidth::type_auto)
		{
			if (containing_width.expanding || is_table_cell || is_float) // || !is_overflow_visible
			{
				width = 0.0f;
				width_undetermined = true;
			}
			else
			{
				width = max(0.0f, containing_width.value - margin.left - margin.right - border.left - border.right - padding.left - padding.right);
				width_undetermined = false;
			}
		}
		else
		{
			// Unreachable code (unless something is seriously wrong elsewhere).
			width = 0.0f;
			width_undetermined = false;
		}

		if (width_undetermined)
		{
			if (strategy == normal_strategy)
				width = actual_to_used(
					used_to_actual(containing_width.value) -
					used_to_actual(margin.left) -
					used_to_actual(margin.right) -
					used_to_actual(border.left) -
					used_to_actual(border.right) -
					used_to_actual(padding.left) -
					used_to_actual(padding.right));

			if (!css_max_width.expanding)
				width = min(width, css_max_width.value);
			width = max(width, css_min_width);
		}
		else
		{
			if (!css_max_width.expanding)
				width = min(width, css_max_width.value);
			width = max(width, css_min_width);

			if (!containing_width.expanding && !is_float)
			{
				if (properties.margin_width_left.type == CSSValueMarginWidth::type_auto && properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
				{
					margin.left = max(0.0f, (containing_width.value-border.left-border.right-padding.left-padding.right-width)/2.0f);
					margin.right = max(0.0f, containing_width.value-border.left-border.right-padding.left-padding.right-width-margin.left);
				}
				else if (properties.margin_width_left.type == CSSValueMarginWidth::type_auto)
				{
					margin.left = max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width);
				}
				else if (properties.margin_width_right.type == CSSValueMarginWidth::type_auto)
				{
					margin.right = max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width);
				}

				if (margin.left + border.left + width + border.right + padding.right + margin.right > containing_width.value)
				{
					if (properties.direction.type == CSSValueDirection::type_ltr)
						margin.right = max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width);
					else
						margin.left = max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width);
				}
			}
		}
	}

	void calculate_static_heights(const CSSComputedBox &properties)
	{
		margin.top = get_css_margin_height(properties.margin_width_top, containing_height);
		margin.bottom = get_css_margin_height(properties.margin_width_bottom, containing_height);
		border.top = get_css_border_width(properties.border_width_top);
		border.bottom = get_css_border_width(properties.border_width_bottom);
		padding.top = get_css_padding_height(properties.padding_width_top, containing_height);
		padding.bottom = get_css_padding_height(properties.padding_width_bottom, containing_height);

		if (properties.height.type == CSSValueHeight::type_length)
		{
			height = properties.height.length.value;
			height_undetermined = false;
		}
		else if (properties.height.type == CSSValueHeight::type_percentage)
		{
			if (containing_height.use_content)
			{
				height = 0.0f;
				height_undetermined = true;
			}
			else
			{
				height = containing_height.value * properties.height.percentage / 100.0f;
				height_undetermined = false;
			}
		}
		else if (properties.height.type == CSSValueHeight::type_auto)
		{
			height = 0.0f;
			height_undetermined = true;
		}
		else
		{
			// Unreachable code (unless something is seriously wrong elsewhere).
			height = 0.0f;
			height_undetermined = false;
		}

		if (!css_max_height.use_content)
			height = min(height, css_max_width.value);
		height = max(height, css_min_width);
	}
*/

}
