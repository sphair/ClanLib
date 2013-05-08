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
#include "layout_absolute_and_fixed_content.h"

namespace clan
{

CSSInlineLayoutLayoutAbsoluteAndFixedContent::CSSInlineLayoutLayoutAbsoluteAndFixedContent(CSSLayoutGraphics *graphics, CSSResourceCache *resources, Rect containing_block, const Size &viewport_size)
: graphics(graphics), resources(resources), containing_block(containing_block), viewport_size(viewport_size)
{
}

bool CSSInlineLayoutLayoutAbsoluteAndFixedContent::node(CSSInlineGeneratedBox *cur)
{
	if (cur->layout_node)
	{
		if (cur->layout_node->get_element_node()->computed_values.get_box().position.type == CSSValuePosition::type_absolute ||
			cur->layout_node->get_element_node()->computed_values.get_box().position.type == CSSValuePosition::type_fixed)
		{
			cur->layout_node->relative_x = cur->relative_x;
			cur->layout_node->relative_y = cur->relative_y;
			cur->layout_node->layout_absolute_or_fixed(graphics, resources, containing_block, viewport_size);
		}

		cur->layout_node->layout_absolute_and_fixed_content(graphics, resources, containing_block, viewport_size);
	}
	return true;
}

}
