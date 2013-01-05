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
#include "render_layer_floats.h"

namespace clan
{

CSSInlineLayoutRenderLayerFloats::CSSInlineLayoutRenderLayerFloats(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSStackingContext *stacking_context)
: graphics(graphics), resources(resources), stacking_context(stacking_context)
{
}

bool CSSInlineLayoutRenderLayerFloats::node(CSSInlineGeneratedBox *cur)
{
	if (cur->layout_node)
	{
		CSSLayoutTreeNode *object_node = cur->layout_node;
		bool is_same_stacking_context = (stacking_context == object_node->get_stacking_context());
		bool is_positioned = (object_node->get_element_node()->computed_properties.position.type != CSSBoxPosition::type_static);
		bool is_float = object_node->get_element_node()->is_float();
		if (is_same_stacking_context && !is_positioned)
		{
			if (is_float)
			{
				object_node->render_layer_background(graphics, resources, false);
				object_node->render_layer_non_inline(graphics, resources);
				object_node->render_layer_floats(graphics, resources);
				object_node->render_layer_inline(graphics, resources);
			}
			else
			{
				object_node->render_layer_floats(graphics, resources);
			}
		}
	}
	return true;
}

}
