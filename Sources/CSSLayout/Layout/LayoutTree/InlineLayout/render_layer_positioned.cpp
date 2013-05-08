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
#include "render_layer_positioned.h"
#include "CSSLayout/Layout/LayoutTree/css_stacking_context.h"

namespace clan
{

CSSInlineLayoutRenderLayerPositioned::CSSInlineLayoutRenderLayerPositioned(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSStackingContext *stacking_context)
: graphics(graphics), resources(resources), stacking_context(stacking_context)
{
}

bool CSSInlineLayoutRenderLayerPositioned::node(CSSInlineGeneratedBox *cur)
{
	if (cur->layout_node)
	{
		CSSLayoutTreeNode *object_node = cur->layout_node;
		bool is_same_stacking_context = (stacking_context == object_node->get_stacking_context());
		bool is_positioned = (object_node->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
		int level = object_node->get_stacking_context()->get_level();
		if (is_same_stacking_context)
		{
			if (is_positioned)
			{
				object_node->render_layer_background(graphics, resources, false);
				object_node->render_layer_non_inline(graphics, resources);
				object_node->render_layer_floats(graphics, resources);
				object_node->render_layer_inline(graphics, resources);
			}
			object_node->render_layer_positioned(graphics, resources);
		}
		else if (!is_same_stacking_context && level == 0)
		{
			object_node->get_stacking_context()->render(graphics, resources);
		}
	}
	return true;
}

}
