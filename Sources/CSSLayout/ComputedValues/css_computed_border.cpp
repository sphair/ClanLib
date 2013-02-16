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
#include "API/CSSLayout/ComputedValues/css_computed_border.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"

namespace clan
{

void CSSComputedBorder::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size, const Colorf &computed_color)
{
	if (!parent.is_null())
	{
		const CSSComputedBorder &parent_border = parent.get_border();

		border_color_left.compute(&parent_border.border_color_left, layout, em_size, ex_size, computed_color);
		border_color_top.compute(&parent_border.border_color_top, layout, em_size, ex_size, computed_color);
		border_color_right.compute(&parent_border.border_color_right, layout, em_size, ex_size, computed_color);
		border_color_bottom.compute(&parent_border.border_color_bottom, layout, em_size, ex_size, computed_color);

		border_style_left.compute(&parent_border.border_style_left, layout, em_size, ex_size);
		border_style_top.compute(&parent_border.border_style_top, layout, em_size, ex_size);
		border_style_right.compute(&parent_border.border_style_right, layout, em_size, ex_size);
		border_style_bottom.compute(&parent_border.border_style_bottom, layout, em_size, ex_size);

		border_width_left.compute(&parent_border.border_width_left, layout, em_size, ex_size, border_style_left);
		border_width_top.compute(&parent_border.border_width_top, layout, em_size, ex_size, border_style_top);
		border_width_right.compute(&parent_border.border_width_right, layout, em_size, ex_size, border_style_right);
		border_width_bottom.compute(&parent_border.border_width_bottom, layout, em_size, ex_size, border_style_bottom);

		border_radius_top_right.compute(&parent_border.border_radius_top_right, layout, em_size, ex_size);
		border_radius_bottom_right.compute(&parent_border.border_radius_bottom_right, layout, em_size, ex_size);
		border_radius_bottom_left.compute(&parent_border.border_radius_bottom_left, layout, em_size, ex_size);
		border_radius_top_left.compute(&parent_border.border_radius_top_left, layout, em_size, ex_size);

		border_image_source.compute(&parent_border.border_image_source, layout, em_size, ex_size);
		border_image_slice.compute(&parent_border.border_image_slice, layout, em_size, ex_size);
		border_image_width.compute(&parent_border.border_image_width, layout, em_size, ex_size);
		border_image_outset.compute(&parent_border.border_image_outset, layout, em_size, ex_size);
		border_image_repeat.compute(&parent_border.border_image_repeat, layout, em_size, ex_size);
	}
	else
	{
		border_color_left.compute(nullptr, layout, em_size, ex_size, computed_color);
		border_color_top.compute(nullptr, layout, em_size, ex_size, computed_color);
		border_color_right.compute(nullptr, layout, em_size, ex_size, computed_color);
		border_color_bottom.compute(nullptr, layout, em_size, ex_size, computed_color);

		border_style_left.compute(nullptr, layout, em_size, ex_size);
		border_style_top.compute(nullptr, layout, em_size, ex_size);
		border_style_right.compute(nullptr, layout, em_size, ex_size);
		border_style_bottom.compute(nullptr, layout, em_size, ex_size);

		border_width_left.compute(nullptr, layout, em_size, ex_size, border_style_left);
		border_width_top.compute(nullptr, layout, em_size, ex_size, border_style_top);
		border_width_right.compute(nullptr, layout, em_size, ex_size, border_style_right);
		border_width_bottom.compute(nullptr, layout, em_size, ex_size, border_style_bottom);

		border_radius_top_right.compute(nullptr, layout, em_size, ex_size);
		border_radius_bottom_right.compute(nullptr, layout, em_size, ex_size);
		border_radius_bottom_left.compute(nullptr, layout, em_size, ex_size);
		border_radius_top_left.compute(nullptr, layout, em_size, ex_size);

		border_image_source.compute(nullptr, layout, em_size, ex_size);
		border_image_slice.compute(nullptr, layout, em_size, ex_size);
		border_image_width.compute(nullptr, layout, em_size, ex_size);
		border_image_outset.compute(nullptr, layout, em_size, ex_size);
		border_image_repeat.compute(nullptr, layout, em_size, ex_size);
	}
}

}
