/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "css_used_values.h"
#include "../BoxTree/css_box_properties.h"

CL_CSSUsedValues::CL_CSSUsedValues()
: replaced(false), width(0.0f), height(0.0f), undetermined_width(true), undetermined_height(true),
  min_width(0.0f), preferred_width(0.0f), min_width_calculated(false), preferred_width_calculated(false)
{
}

void CL_CSSUsedValues::set_width(const CL_CSSBoxProperties &properties)
{
	margin.left = get_margin_width(properties.margin_width_left);
	margin.right = get_margin_width(properties.margin_width_right);
	border.left = properties.border_width_left.length.value;
	border.right = properties.border_width_right.length.value;
	padding.left = get_padding_width(properties.padding_width_left);
	padding.right = get_padding_width(properties.padding_width_right);

	width = 0;
	undetermined_width = false;

	if (replaced)
	{
		if (properties.width.type == CL_CSSBoxWidth::type_auto && properties.height.type == CL_CSSBoxHeight::type_auto && intrinsic.has_width)
		{
			width = intrinsic.width;
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto && properties.height.type == CL_CSSBoxHeight::type_auto && intrinsic.has_height && intrinsic.has_ratio)
		{
			width = intrinsic.height / intrinsic.ratio;
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto && properties.height.type == CL_CSSBoxHeight::type_auto && intrinsic.has_ratio)
		{
			if (containing.undetermined_width)
			{
				width = 0;
				undetermined_width = true;
			}
			else
			{
				width = containing.width - margin.left - margin.right - border.left - border.right - padding.left - padding.right;
			}
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto && intrinsic.has_width)
		{
			width = intrinsic.width;
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto)
		{
			width = 300; // bug: Should be 300px (css physical length)
			/*if (width > device.width)
			{
				width = largest_2_1_ratio_rect();
			}*/
		}
	}
	else if (is_inline(properties))
	{
		// width property does not apply.
		width = 0;
		undetermined_width = true;
	}
	else if (is_absolute(properties))
	{
		if (properties.width.type == CL_CSSBoxWidth::type_length)
		{
			width = properties.width.length.value;
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_percentage)
		{
			if (containing.undetermined_width)
			{
				width = 0;
				undetermined_width = true;
			}
			else
			{
				width = properties.width.percentage * containing.width / 100.0f;
			}
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto && properties.left.type != CL_CSSBoxLeft::type_auto && properties.right.type != CL_CSSBoxRight::type_auto)
		{
			if (containing.undetermined_width)
			{
				width = 0;
				undetermined_width = true;
			}
			else
			{
				// To do: Handle the cases where the length property is a percentage. Also determine what the containing box is (the viewport/page-area?)
				width = containing.width - properties.left.length.value - properties.right.length.value - margin.left - margin.right - border.left - border.right - padding.left - padding.right;
			}
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto)
		{
			width = 0.0f;
			undetermined_width = true; // shrink-to-fit
		}
	}
	else if (is_floating(properties) || is_inline_block(properties))
	{
		if (properties.width.type == CL_CSSBoxWidth::type_length)
		{
			width = properties.width.length.value;
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_percentage)
		{
			if (containing.undetermined_width)
			{
				width = 0;
				undetermined_width = true;
			}
			else
			{
				width = properties.width.percentage * containing.width / 100.0f;
			}
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto)
		{
			width = 0.0f;
			undetermined_width = true; // shrink-to-fit
		}
	}
	else if (is_block(properties))
	{
		if (properties.width.type == CL_CSSBoxWidth::type_length)
		{
			width = properties.width.length.value;
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_percentage)
		{
			if (containing.undetermined_width)
			{
				width = 0;
				undetermined_width = true;
			}
			else
			{
				width = properties.width.percentage * containing.width / 100.0f;
			}
		}
		else if (properties.width.type == CL_CSSBoxWidth::type_auto)
		{
			if (containing.undetermined_width)
			{
				width = 0;
				undetermined_width = true;
			}
			else
			{
				width = cl_max(0.0f, containing.width - margin.left - margin.right - border.left - border.right - padding.left - padding.right);
			}
		}
	}

	if (!undetermined_width)
	{
		if (properties.max_width.type == CL_CSSBoxMaxWidth::type_length)
		{
			width = cl_min(width, properties.max_width.length.value);
		}
		else if (properties.max_width.type == CL_CSSBoxMaxWidth::type_percentage && !containing.undetermined_width)
		{
			width = cl_min(width, properties.max_width.percentage * containing.width / 100.0f);
		}

		if (properties.min_width.type == CL_CSSBoxMinWidth::type_length)
		{
			width = cl_max(width, properties.min_width.length.value);
		}
		else if (properties.min_width.type == CL_CSSBoxMinWidth::type_percentage && !containing.undetermined_width)
		{
			width = cl_max(width, properties.min_width.percentage * containing.width / 100.0f);
		}
	}

	calc_noncontent_width(properties);
}

void CL_CSSUsedValues::set_width(const CL_CSSBoxProperties &properties, CL_CSSUsedValue new_width)
{
	width = new_width;
	undetermined_width = false;
	calc_noncontent_width(properties);
}

void CL_CSSUsedValues::calc_noncontent_width(const CL_CSSBoxProperties &properties)
{
	margin.left = get_margin_width(properties.margin_width_left);
	margin.right = get_margin_width(properties.margin_width_right);
	border.left = properties.border_width_left.length.value;
	border.right = properties.border_width_right.length.value;
	padding.left = get_padding_width(properties.padding_width_left);
	padding.right = get_padding_width(properties.padding_width_right);

	if (is_inline(properties) || is_inline_block(properties) || is_floating(properties))
	{
		// Do nothing. Correct values already set by get_margin_width.
	}
	else if (is_block(properties))
	{
		if (properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto && properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.left = cl_max(0.0f, (containing.width-border.left-border.right-padding.left-padding.right-width)/2.0f);
			margin.right = cl_max(0.0f, containing.width-border.left-border.right-padding.left-padding.right-width-margin.left);
		}
		else if (properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.left = cl_max(0.0f, containing.width-margin.right-border.left-border.right-padding.left-padding.right-width);
		}
		else if (properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
		{
			margin.right = cl_max(0.0f, containing.width-margin.left-border.left-border.right-padding.left-padding.right-width);
		}

		if (margin.left + border.left + width + border.right + padding.right + margin.right > containing.width)
		{
			if (properties.direction.type == CL_CSSBoxDirection::type_ltr)
				margin.right = cl_max(0.0f, containing.width-margin.left-border.left-border.right-padding.left-padding.right-width);
			else
				margin.left = cl_max(0.0f, containing.width-margin.right-border.left-border.right-padding.left-padding.right-width);
		}
	}
	else if (is_absolute(properties) && !replaced)
	{
		// Lots of annoying calculations here using 'left', 'right', 'width', 'direction' and the margin+border+padding+width=containing.width constraint
	}
	else if (is_absolute(properties) && replaced)
	{
		if (properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto)
		{
			if (properties.left.type == CL_CSSBoxLeft::type_auto || properties.right.type == CL_CSSBoxRight::type_auto)
			{
				margin.left = 0;
			}
			else
			{
				// solve using margin+border+padding+width=containing.width constraint
			}
		}

		if (properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
		{
			if (properties.left.type == CL_CSSBoxLeft::type_auto || properties.right.type == CL_CSSBoxRight::type_auto)
			{
				margin.right = 0;
			}
			else
			{
				// solve using margin+border+padding+width=containing.width constraint
			}
		}
	}
}

void CL_CSSUsedValues::set_height(const CL_CSSBoxProperties &properties)
{
	height = 0;
	undetermined_height = false;

	if (replaced)
	{
		if (!is_absolute(properties))
		{
			if (properties.height.type == CL_CSSBoxHeight::type_length)
			{
				height = properties.height.length.value;
			}
			else if (properties.height.type == CL_CSSBoxHeight::type_percentage)
			{
				if (containing.undetermined_height)
				{
					height = 0;
					undetermined_height = true;
				}
				else
				{
					height = containing.height * properties.height.percentage / 100.0f;
				}
			}
			else if (properties.height.type == CL_CSSBoxHeight::type_auto)
			{
				height = 0;
				undetermined_height = true;
			}
		}
		else
		{
			height = 0;
			undetermined_height = true;

			// 'top' + 'margin-top' + 'border-top-width' + 'padding-top' + 'height' + 'padding-bottom' + 'border-bottom-width' + 'margin-bottom' + 'bottom' = height of containing block
		}
	}
	else if (is_inline(properties))
	{
		// Height does not apply.
		height = 0;
		undetermined_height = true;
	}
	else if (is_block(properties))
	{
		if (properties.height.type == CL_CSSBoxHeight::type_length)
		{
			height = properties.height.length.value;
		}
		else if (properties.height.type == CL_CSSBoxHeight::type_percentage)
		{
			if (containing.undetermined_height)
			{
				height = 0;
				undetermined_height = true;
			}
			else
			{
				height = containing.height * properties.height.percentage / 100.0f;
			}
		}
		else if (properties.height.type == CL_CSSBoxHeight::type_auto)
		{
			height = 0;
			undetermined_height = true;
		}
	}
	else if (is_absolute(properties))
	{
		height = 0;
		undetermined_height = true;

		// 'top' + 'margin-top' + 'border-top-width' + 'padding-top' + 'height' + 'padding-bottom' + 'border-bottom-width' + 'margin-bottom' + 'bottom' = height of containing block
	}

	if (!undetermined_height)
	{
		if (properties.max_height.type == CL_CSSBoxMaxHeight::type_length)
		{
			height = cl_min(height, properties.max_height.length.value);
		}
		else if (properties.max_height.type == CL_CSSBoxMaxHeight::type_percentage && !containing.undetermined_height)
		{
			height = cl_min(height, properties.max_height.percentage * containing.height / 100.0f);
		}

		if (properties.min_height.type == CL_CSSBoxMinHeight::type_length)
		{
			height = cl_max(height, properties.min_height.length.value);
		}
		else if (properties.min_height.type == CL_CSSBoxMinHeight::type_percentage && !containing.undetermined_height)
		{
			height = cl_max(height, properties.min_height.percentage * containing.height / 100.0f);
		}
	}

	calc_noncontent_height(properties);
}

void CL_CSSUsedValues::set_height(const CL_CSSBoxProperties &properties, CL_CSSUsedValue new_height)
{
	height = new_height;
	undetermined_height = false;
	calc_noncontent_height(properties);
}

void CL_CSSUsedValues::calc_noncontent_height(const CL_CSSBoxProperties &properties)
{
	margin.top = get_margin_width(properties.margin_width_top);
	margin.bottom = get_margin_width(properties.margin_width_bottom);
	border.top = properties.border_width_top.length.value;
	border.bottom = properties.border_width_bottom.length.value;
	padding.top = get_padding_width(properties.padding_width_top);
	padding.bottom = get_padding_width(properties.padding_width_bottom);

	if (is_absolute(properties))
	{
		if (replaced)
		{
			// 'top' + 'margin-top' + 'border-top-width' + 'padding-top' + 'height' + 'padding-bottom' + 'border-bottom-width' + 'margin-bottom' + 'bottom' = height of containing block
		}
		else
		{
			// 'top' + 'margin-top' + 'border-top-width' + 'padding-top' + 'height' + 'padding-bottom' + 'border-bottom-width' + 'margin-bottom' + 'bottom' = height of containing block
		}
	}
}

CL_CSSUsedValue CL_CSSUsedValues::get_margin_width(const CL_CSSBoxMarginWidth &margin_width)
{
	switch (margin_width.type)
	{
	case CL_CSSBoxMarginWidth::type_auto: return 0.0f;
	case CL_CSSBoxMarginWidth::type_length: return margin_width.length.value;
	case CL_CSSBoxMarginWidth::type_percentage: return margin_width.percentage * containing.width / 100.0f;
	default: return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSUsedValues::get_padding_width(const CL_CSSBoxPaddingWidth &padding_width)
{
	switch (padding_width.type)
	{
	case CL_CSSBoxPaddingWidth::type_length: return padding_width.length.value;
	case CL_CSSBoxPaddingWidth::type_percentage: return padding_width.percentage * containing.width / 100.0f;
	default: return 0.0f;
	}
}

bool CL_CSSUsedValues::is_inline(const CL_CSSBoxProperties &properties)
{
	return CL_CSSBoxDisplay::type_inline;
}

bool CL_CSSUsedValues::is_block(const CL_CSSBoxProperties &properties)
{
	switch (properties.display.type)
	{
	case CL_CSSBoxDisplay::type_block:
	case CL_CSSBoxDisplay::type_run_in:
	case CL_CSSBoxDisplay::type_list_item:
	case CL_CSSBoxDisplay::type_table:
	case CL_CSSBoxDisplay::type_table_row_group:
	case CL_CSSBoxDisplay::type_table_header_group:
	case CL_CSSBoxDisplay::type_table_footer_group:
	case CL_CSSBoxDisplay::type_table_row:
	case CL_CSSBoxDisplay::type_table_column_group:
	case CL_CSSBoxDisplay::type_table_column:
	case CL_CSSBoxDisplay::type_table_cell:
	case CL_CSSBoxDisplay::type_table_caption:
		return true;
	default:
		return false;
	}
}

bool CL_CSSUsedValues::is_floating(const CL_CSSBoxProperties &properties)
{
	return properties.float_box.type != CL_CSSBoxFloat::type_none;
}

bool CL_CSSUsedValues::is_inline_block(const CL_CSSBoxProperties &properties)
{
	switch (properties.display.type)
	{
	case CL_CSSBoxDisplay::type_inline_block:
	case CL_CSSBoxDisplay::type_inline_table:
		return true;
	default:
		return false;
	}
}

bool CL_CSSUsedValues::is_absolute(const CL_CSSBoxProperties &properties)
{
	return properties.position.type == CL_CSSBoxPosition::type_absolute;
}
