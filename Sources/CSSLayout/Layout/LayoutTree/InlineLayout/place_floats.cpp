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
#include "place_floats.h"
#include "CSSLayout/LayoutTree/css_block_formatting_context.h"

namespace clan
{

CSSInlineLayoutPlaceFloats::CSSInlineLayoutPlaceFloats(CSSInlineLayout *layout, CSSActualValue x, CSSActualValue y, CSSInlineLayout::LayoutStrategy strategy)
: layout(layout), x(x), y(y), strategy(strategy)
{
}

bool CSSInlineLayoutPlaceFloats::node(CSSInlineGeneratedBox *cur, size_t text_start, size_t text_end)
{
	if (cur->layout_node && !cur->floated && cur->layout_node->get_element_node()->is_float())
	{
		cur->floated = true;
		layout->floats.push_back(cur->layout_node);

		if (cur->layout_node->get_element_node()->computed_properties.clear.type == CSSBoxClear::type_left || cur->layout_node->get_element_node()->computed_properties.clear.type == CSSBoxClear::type_both)
		{
			CSSActualValue clear_left = layout->get_formatting_context()->find_left_clearance();
			y = max(y, clear_left);
		}
		if (cur->layout_node->get_element_node()->computed_properties.clear.type == CSSBoxClear::type_right || cur->layout_node->get_element_node()->computed_properties.clear.type == CSSBoxClear::type_both)
		{
			CSSActualValue clear_right = layout->get_formatting_context()->find_right_clearance();
			y = max(y, clear_right);
		}

		Rect float_box(0, 0, cur->layout_node->get_block_width(), cur->layout_node->get_block_height());
		float_box.translate(x, y);

		if (strategy == CSSInlineLayout::preferred_strategy && layout->width.expanding)
		{
			CSSActualValue w = 1000000;
			if (cur->layout_node->get_element_node()->computed_properties.float_box.type == CSSBoxFloat::type_left)
			{
				float_box = layout->get_formatting_context()->float_left(float_box, x+w);
			}
			else if (cur->layout_node->get_element_node()->computed_properties.float_box.type == CSSBoxFloat::type_right)
			{
				float_box = layout->get_formatting_context()->float_right_shrink_to_fit(float_box, x+w);
			}
			cur->layout_node->set_root_block_position(float_box.left, float_box.top);
		}
		else
		{
			if (cur->layout_node->get_element_node()->computed_properties.float_box.type == CSSBoxFloat::type_left)
			{
				float_box = layout->get_formatting_context()->float_left(float_box, x+used_to_actual(layout->width.value));
			}
			else if (cur->layout_node->get_element_node()->computed_properties.float_box.type == CSSBoxFloat::type_right)
			{
				float_box.translate(layout->width.value-float_box.get_width(), 0);
				float_box = layout->get_formatting_context()->float_right(float_box, x+used_to_actual(layout->width.value));
			}
			cur->layout_node->set_root_block_position(float_box.left, float_box.top);
		}

		if (strategy != CSSInlineLayout::normal_strategy && layout->width.expanding)
			layout->width.value = max(layout->width.value, float_box.right - x);

		return false;
	}
	return true;
}

}
