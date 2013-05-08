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

#include "CSSLayout/precomp.h"
#include "layout_inline_blocks_and_floats.h"

namespace clan
{

CSSInlineLayoutLayoutInlineBlocksAndFloats::CSSInlineLayoutLayoutInlineBlocksAndFloats(CSSInlineLayout *layout, CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSInlineLayout::LayoutStrategy strategy)
: layout(layout), graphics(graphics), resources(resources), strategy(strategy)
{
}

bool CSSInlineLayoutLayoutInlineBlocksAndFloats::node(CSSInlineGeneratedBox *cur)
{
	cur->relative_x = cur->parent->relative_x;
	cur->relative_y = cur->parent->relative_y;
	CSSBoxElement *element = dynamic_cast<CSSBoxElement*>(cur->box_node);
	if (element)
	{
		cur->relative_x += layout->get_local_relative_x(element, layout->width.value);
		cur->relative_y += layout->get_local_relative_y(element, layout->height.value);
	}

	if (cur->layout_node && (cur->layout_node->get_element_node()->is_float() || cur->layout_node->get_element_node()->is_inline_block_level() || cur->layout_node->is_replaced()))
	{
		/*if (cur->layout_node->get_element_node()->name.find("abc") != std::string::npos)
		{
			Sleep(1);
		}*/
		cur->floated = false;
		cur->layout_node->relative_x = cur->relative_x;
		cur->layout_node->relative_y = cur->relative_y;
		cur->layout_node->containing_width = layout->width;
		cur->layout_node->layout_float(graphics, resources, strategy);

		if (strategy == CSSInlineLayout::minimum_strategy && layout->width.expanding)
			layout->width.value = max(layout->width.value, cur->layout_node->get_block_width());
	}
	return true;
}

}
