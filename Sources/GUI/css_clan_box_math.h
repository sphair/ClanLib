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

#pragma once

#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

class CSSClanBoxMath
{
public:
	std::vector<float> used_min_lengths;
	std::vector<float> used_max_lengths;
	std::vector<float> used_expand_weights;
	std::vector<float> used_shrink_weights;
	std::vector<float> used_lengths;

	void adjust(float used_content_length)
	{
		clamp_lengths();
		float available_space = used_content_length - find_sum(used_lengths);
		if (available_space > 0.0f)
			expand_lengths(available_space);
		else if (available_space < -0.0f)
			shrink_lengths(available_space);
	}

	void clamp_lengths()
	{
		for (size_t i = 0; i < used_lengths.size(); i++)
		{
			used_lengths[i] = clamp(used_lengths[i], used_min_lengths[i], used_max_lengths[i]);
		}
	}

	void expand_lengths(float available_space)
	{
		float weight_sum = find_sum(used_expand_weights);
		while (weight_sum != 0.0f)
		{
			float new_weight_sum = 0.0f;
			float expand_length_sum = 0.0f;
			for (size_t i = 0; i < used_lengths.size(); i++)
			{
				float expand_length = available_space * used_expand_weights[i] / weight_sum;
				if (used_lengths[i] + expand_length >= used_max_lengths[i])
				{
					expand_length = used_max_lengths[i] - used_lengths[i];
					used_lengths[i] = used_max_lengths[i];
				}
				else
				{
					new_weight_sum += used_expand_weights[i];
					used_lengths[i] += expand_length;
				}
				expand_length_sum += expand_length;
			}
			available_space -= expand_length_sum;
			if (weight_sum == new_weight_sum)
				break;
			weight_sum = new_weight_sum;
		}
	}

	void shrink_lengths(float available_space)
	{
		float weight_sum = find_sum(used_shrink_weights);
		while (weight_sum != 0.0f)
		{
			float new_weight_sum = 0.0f;
			float shrink_length_sum = 0.0f;
			for (size_t i = 0; i < used_lengths.size(); i++)
			{
				float shrink_length = available_space * used_shrink_weights[i] / weight_sum;
				if (used_lengths[i] + shrink_length <= used_min_lengths[i])
				{
					shrink_length = used_min_lengths[i] - used_lengths[i];
					used_lengths[i] = used_min_lengths[i];
				}
				else
				{
					new_weight_sum += used_shrink_weights[i];
					used_lengths[i] += shrink_length;
				}
				shrink_length_sum += shrink_length;
			}
			available_space -= shrink_length_sum;
			if (weight_sum == new_weight_sum)
				break;
			weight_sum = new_weight_sum;
		}
	}

	float find_sum(const std::vector<float> &vec)
	{
		float sum = 0.0f;
		for (size_t i = 0; i < vec.size(); i++)
			sum += vec[i];
		return sum;
	}
};

class CSSClanBoxUsedValueSizes
{
public:
	static float get_css_margin_width(const CSSBoxMarginWidth &margin_width, float containing_used_width, bool containing_width_expanding)
	{
		switch (margin_width.type)
		{
		case CSSBoxMarginWidth::type_auto: return 0.0f;
		case CSSBoxMarginWidth::type_length: return margin_width.length.value;
		case CSSBoxMarginWidth::type_percentage: return containing_width_expanding ? 0.0f : margin_width.percentage * containing_used_width / 100.0f;
		default: return 0.0f;
		}
	}

	static float get_css_padding_width(const CSSBoxPaddingWidth &padding_width, float containing_used_width, bool containing_width_expanding)
	{
		switch (padding_width.type)
		{
		case CSSBoxPaddingWidth::type_length: return padding_width.length.value;
		case CSSBoxPaddingWidth::type_percentage: return containing_width_expanding ? 0.0f : padding_width.percentage * containing_used_width / 100.0f;
		default: return 0.0f;
		}
	}

	static float get_css_margin_height(const CSSBoxMarginWidth &margin_width, float containing_used_height, bool containing_height_use_content)
	{
		switch (margin_width.type)
		{
		case CSSBoxMarginWidth::type_auto: return 0.0f;
		case CSSBoxMarginWidth::type_length: return margin_width.length.value;
		case CSSBoxMarginWidth::type_percentage: return containing_height_use_content ? 0.0f : margin_width.percentage * containing_used_height / 100.0f;
		default: return 0.0f;
		}
	}

	static float get_css_padding_height(const CSSBoxPaddingWidth &padding_width, float containing_used_height, bool containing_height_use_content)
	{
		switch (padding_width.type)
		{
		case CSSBoxPaddingWidth::type_length: return padding_width.length.value;
		case CSSBoxPaddingWidth::type_percentage: return containing_height_use_content ? 0.0f : padding_width.percentage * containing_used_height / 100.0f;
		default: return 0.0f;
		}
	}

	static float get_css_border_width(const CSSBoxBorderWidth &border_width)
	{
		return border_width.length.value;
	}
/*
	CSSClanBoxUsedValueSizes() : width(0.0f), height(0.0f), width_expanding(false), height_use_content(false) { }

	typedef float CSSUsedValue;
	typedef int CSSActualValue;

	enum LayoutStrategy
	{
		normal_strategy,
		preferred_strategy,
		minimum_strategy
	};

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

	bool width_expanding;
	bool height_use_content;

	void calculate_absolute_widths(const CSSBoxProperties &properties, LayoutStrategy strategy)
	{
		margin.left = get_css_margin_width(properties.margin_width_left, containing_width);
		margin.right = get_css_margin_width(properties.margin_width_right, containing_width);
		border.left = get_css_border_width(properties.border_width_left);
		border.right = get_css_border_width(properties.border_width_right);
		padding.left = get_css_padding_width(properties.padding_width_left, containing_width);
		padding.right = get_css_padding_width(properties.padding_width_right, containing_width);

		CSSUsedValue left = 0.0f;
		if (properties.left.type == CSSBoxLeft::type_length)
			left = properties.left.length.value;
		else if (properties.left.type == CSSBoxLeft::type_percentage)
			left = properties.left.percentage * containing_width.value / 100.0f;

		CSSUsedValue right = 0.0f;
		if (properties.right.type == CSSBoxRight::type_length)
			right = properties.right.length.value;
		else if (properties.right.type == CSSBoxRight::type_percentage)
			right = properties.right.percentage * containing_width.value / 100.0f;

		width = 0.0f;
		width_expanding = false;
		if (properties.width.type == CSSBoxWidth::type_length)
			width = properties.width.length.value;
		else
			width = properties.width.percentage * containing_width.value / 100.0f;

		if (properties.left.type == CSSBoxLeft::type_auto &&
			properties.right.type == CSSBoxRight::type_auto &&
			properties.width.type == CSSBoxWidth::type_auto)
		{
			width = 0.0f;
			width_expanding = true;
		}
		else if (properties.left.type != CSSBoxLeft::type_auto &&
			properties.right.type != CSSBoxRight::type_auto &&
			properties.width.type != CSSBoxWidth::type_auto)
		{
			if (properties.margin_width_left.type == CSSBoxMarginWidth::type_auto &&
				properties.margin_width_right.type == CSSBoxMarginWidth::type_auto)
			{
				CSSUsedValue space_left = containing_width.value - border.left - border.right - padding.left - padding.right - width - left - right;
				if (space_left < 0.0f)
				{
					if (properties.direction.type == CSSBoxDirection::type_ltr)
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
			else if (properties.margin_width_left.type == CSSBoxMarginWidth::type_auto)
			{
				margin.left = containing_width.value - border.left - border.right - padding.left - padding.right - width - margin.right - left - right;
			}
			else if (properties.margin_width_right.type == CSSBoxMarginWidth::type_auto)
			{
				margin.right = containing_width.value - border.left - border.right - padding.left - padding.right - width - margin.left - left - right;
			}
		}
		else if (properties.left.type == CSSBoxLeft::type_auto &&
			properties.width.type == CSSBoxWidth::type_auto &&
			properties.right.type != CSSBoxRight::type_auto) // rule #1
		{
			width = 0.0f;
			width_expanding = true;
		}
		else if (properties.width.type == CSSBoxWidth::type_auto &&
			properties.right.type == CSSBoxRight::type_auto &&
			properties.left.type != CSSBoxLeft::type_auto) // rule #3
		{
			width = 0.0f;
			width_expanding = true;
		}
		else if (properties.width.type == CSSBoxWidth::type_auto &&
			properties.left.type != CSSBoxLeft::type_auto &&
			properties.right.type != CSSBoxRight::type_auto) // rule #5
		{
			width = containing_width.value - border.left - border.right - padding.left - padding.right - margin.left - margin.right - left - right;
		}

		if (width_expanding && strategy == normal_strategy)
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
		if (properties.max_width.type != CSSBoxMaxWidth::type_none)
		{
			if (properties.max_width.type == CSSBoxMaxWidth::type_length)
			{
				css_max_width.value = properties.max_width.length.value;
				css_max_width.expanding = false;
			}
			else if (properties.max_width.type == CSSBoxMaxWidth::type_percentage && !containing_width.expanding)
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

	void apply_absolute_widths_constraint(const CSSBoxProperties &properties, CSSUsedValue constraint_width, CSSUsedValue left, CSSUsedValue right, CSSUsedValue containing_width)
	{
		width = constraint_width;
		if (properties.left.type != CSSBoxLeft::type_auto &&
			properties.right.type != CSSBoxRight::type_auto)
		{
			if (properties.margin_width_left.type == CSSBoxMarginWidth::type_auto &&
				properties.margin_width_right.type == CSSBoxMarginWidth::type_auto)
			{
				CSSUsedValue space_left = containing_width - border.left - border.right - padding.left - padding.right - width - left - right;
				if (space_left < 0.0f)
				{
					if (properties.direction.type == CSSBoxDirection::type_ltr)
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
			else if (properties.margin_width_left.type == CSSBoxMarginWidth::type_auto)
			{
				margin.left = containing_width - border.left - border.right - padding.left - padding.right - width - margin.right - left - right;
			}
			else if (properties.margin_width_right.type == CSSBoxMarginWidth::type_auto)
			{
				margin.right = containing_width - border.left - border.right - padding.left - padding.right - width - margin.left - left - right;
			}
		}
	}

	void calculate_absolute_heights(const CSSBoxProperties &properties)
	{
		margin.top = get_css_margin_height(properties.margin_width_top, containing_height);
		margin.bottom = get_css_margin_height(properties.margin_width_bottom, containing_height);
		border.top = get_css_border_width(properties.border_width_top);
		border.bottom = get_css_border_width(properties.border_width_bottom);
		padding.top = get_css_padding_height(properties.padding_width_top, containing_height);
		padding.bottom = get_css_padding_height(properties.padding_width_bottom, containing_height);

		CSSUsedValue top = 0.0f;
		if (properties.top.type == CSSBoxTop::type_length)
			top = properties.top.length.value;
		else if (properties.top.type == CSSBoxTop::type_percentage)
			top = properties.top.percentage * containing_height.value / 100.0f;

		CSSUsedValue bottom = 0.0f;
		if (properties.bottom.type == CSSBoxBottom::type_length)
			bottom = properties.bottom.length.value;
		else if (properties.bottom.type == CSSBoxBottom::type_percentage)
			bottom = properties.bottom.percentage * containing_height.value / 100.0f;

		height = 0.0f;
		height_use_content = false;
		if (properties.height.type == CSSBoxHeight::type_length)
			height = properties.height.length.value;
		else
			height = properties.height.percentage * containing_height.value / 100.0f;

		if (properties.top.type == CSSBoxTop::type_auto &&
			properties.bottom.type == CSSBoxBottom::type_auto &&
			properties.height.type == CSSBoxHeight::type_auto)
		{
			height = 0.0f;
			height_use_content = true;
		}
		else if (properties.top.type != CSSBoxTop::type_auto &&
			properties.bottom.type != CSSBoxBottom::type_auto &&
			properties.height.type != CSSBoxHeight::type_auto)
		{
			if (properties.margin_width_top.type == CSSBoxMarginWidth::type_auto &&
				properties.margin_width_bottom.type == CSSBoxMarginWidth::type_auto)
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
			else if (properties.margin_width_top.type == CSSBoxMarginWidth::type_auto)
			{
				margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.bottom - top - bottom;
			}
			else if (properties.margin_width_bottom.type == CSSBoxMarginWidth::type_auto)
			{
				margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.top - top - bottom;
			}
		}
		else if (properties.top.type == CSSBoxTop::type_auto &&
			properties.height.type == CSSBoxHeight::type_auto &&
			properties.bottom.type != CSSBoxBottom::type_auto) // rule #1
		{
			height = 0.0f;
			height_use_content = true;
		}
		else if (properties.height.type == CSSBoxHeight::type_auto &&
			properties.bottom.type == CSSBoxBottom::type_auto &&
			properties.top.type != CSSBoxTop::type_auto) // rule #3
		{
			height = 0.0f;
			height_use_content = true;
		}
		else if (properties.height.type == CSSBoxHeight::type_auto &&
			properties.top.type != CSSBoxTop::type_auto &&
			properties.bottom.type != CSSBoxBottom::type_auto) // rule #5
		{
			height = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - margin.top - margin.bottom - top - bottom;
		}

		if (!css_max_height.use_content && height > css_max_height.value)
			apply_absolute_heights_constraint(css_max_height.value, top, bottom);

		if (height < css_min_height)
			apply_absolute_heights_constraint(css_min_height, top, bottom);
	}

	void apply_absolute_heights_constraint(const CSSBoxProperties &properties, CSSUsedValue constraint_height, CSSUsedValue top, CSSUsedValue bottom)
	{
		height = constraint_height;
		if (properties.top.type != CSSBoxTop::type_auto &&
			properties.bottom.type != CSSBoxBottom::type_auto)
		{
			if (properties.margin_width_top.type == CSSBoxMarginWidth::type_auto &&
				properties.margin_width_bottom.type == CSSBoxMarginWidth::type_auto)
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
			else if (properties.margin_width_top.type == CSSBoxMarginWidth::type_auto)
			{
				margin.top = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.bottom - top - bottom;
			}
			else if (properties.margin_width_bottom.type == CSSBoxMarginWidth::type_auto)
			{
				margin.bottom = containing_height.value - border.top - border.bottom - padding.top - padding.bottom - height - margin.top - top - bottom;
			}
		}
	}

	void calculate_static_widths(const CSSBoxProperties &properties, LayoutStrategy strategy)
	{
		bool is_table_cell = properties.display.type == CSSBoxDisplay::type_table_cell;
		bool is_float = properties.float_box.type != CSSBoxFloat::type_none;
		//bool is_overflow_visible = (element_node->get_parent() == 0) || element_node->is_overflow_visible();

		margin.left = get_css_margin_width(properties.margin_width_left, containing_width);
		margin.right = get_css_margin_width(properties.margin_width_right, containing_width);
		border.left = get_css_border_width(properties.border_width_left);
		border.right = get_css_border_width(properties.border_width_right);
		padding.left = get_css_padding_width(properties.padding_width_left, containing_width);
		padding.right = get_css_padding_width(properties.padding_width_right, containing_width);

		if (properties.width.type == CSSBoxWidth::type_length)
		{
			width = properties.width.length.value;
			width_expanding = false;
		}
		else if (properties.width.type == CSSBoxWidth::type_percentage)
		{
			if (containing_width.expanding)
			{
				width = 0.0f;
				width_expanding = true;
			}
			else
			{
				width = properties.width.percentage * containing_width.value / 100.0f;
				width_expanding = false;
			}
		}
		else if (properties.width.type == CSSBoxWidth::type_auto)
		{
			if (containing_width.expanding || is_table_cell || is_float) // || !is_overflow_visible
			{
				width = 0.0f;
				width_expanding = true;
			}
			else
			{
				width = max(0.0f, containing_width.value - margin.left - margin.right - border.left - border.right - padding.left - padding.right);
				width_expanding = false;
			}
		}
		else
		{
			// Unreachable code (unless something is seriously wrong elsewhere).
			width = 0.0f;
			width_expanding = false;
		}

		if (width_expanding)
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
				if (properties.margin_width_left.type == CSSBoxMarginWidth::type_auto && properties.margin_width_right.type == CSSBoxMarginWidth::type_auto)
				{
					margin.left = max(0.0f, (containing_width.value-border.left-border.right-padding.left-padding.right-width)/2.0f);
					margin.right = max(0.0f, containing_width.value-border.left-border.right-padding.left-padding.right-width-margin.left);
				}
				else if (properties.margin_width_left.type == CSSBoxMarginWidth::type_auto)
				{
					margin.left = max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width);
				}
				else if (properties.margin_width_right.type == CSSBoxMarginWidth::type_auto)
				{
					margin.right = max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width);
				}

				if (margin.left + border.left + width + border.right + padding.right + margin.right > containing_width.value)
				{
					if (properties.direction.type == CSSBoxDirection::type_ltr)
						margin.right = max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width);
					else
						margin.left = max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width);
				}
			}
		}
	}

	void calculate_static_heights(const CSSBoxProperties &properties)
	{
		margin.top = get_css_margin_height(properties.margin_width_top, containing_height);
		margin.bottom = get_css_margin_height(properties.margin_width_bottom, containing_height);
		border.top = get_css_border_width(properties.border_width_top);
		border.bottom = get_css_border_width(properties.border_width_bottom);
		padding.top = get_css_padding_height(properties.padding_width_top, containing_height);
		padding.bottom = get_css_padding_height(properties.padding_width_bottom, containing_height);

		if (properties.height.type == CSSBoxHeight::type_length)
		{
			height = properties.height.length.value;
			height_use_content = false;
		}
		else if (properties.height.type == CSSBoxHeight::type_percentage)
		{
			if (containing_height.use_content)
			{
				height = 0.0f;
				height_use_content = true;
			}
			else
			{
				height = containing_height.value * properties.height.percentage / 100.0f;
				height_use_content = false;
			}
		}
		else if (properties.height.type == CSSBoxHeight::type_auto)
		{
			height = 0.0f;
			height_use_content = true;
		}
		else
		{
			// Unreachable code (unless something is seriously wrong elsewhere).
			height = 0.0f;
			height_use_content = false;
		}

		if (!css_max_height.use_content)
			height = min(height, css_max_width.value);
		height = max(height, css_min_width);
	}
*/
};

}
