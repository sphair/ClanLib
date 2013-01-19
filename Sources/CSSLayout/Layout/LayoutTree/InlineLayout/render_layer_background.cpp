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
#include "render_layer_background.h"
#include "CSSLayout/Layout/LayoutTree/css_stacking_context.h"
#include "CSSLayout/Layout/LayoutTree/css_block_formatting_context.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"
#include "CSSLayout/Layout/BoxTree/css_box_text.h"
#include "CSSLayout/Layout/LayoutTree/css_background_renderer.h"
#include "CSSLayout/Layout/LayoutTree/css_border_renderer.h"

namespace clan
{

CSSInlineLayoutRenderLayerBackground::CSSInlineLayoutRenderLayerBackground(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSLayoutTreeNode *tree_node)
: graphics(graphics), resources(resources), tree_node(tree_node)
{
}

bool CSSInlineLayoutRenderLayerBackground::node(CSSInlineGeneratedBox *cur)
{
	CSSBoxElement *element = cur->box_node ? dynamic_cast<CSSBoxElement*>(cur->box_node) : 0;
	if (cur->layout_node)
	{
		//cur->layout_node->render_layer_background(canvas, resources, false);
	}
	else if (element)
	{
		if (element->computed_values.get_box().visibility.type == CSSValueVisibility::type_visible)
		{
			int pos_x = used_to_actual(cur->relative_x) + tree_node->get_formatting_context()->get_x();
			int pos_y = used_to_actual(cur->relative_y) + tree_node->get_formatting_context()->get_y();
			Rect content(pos_x + cur->x, pos_y + cur->y, pos_x + cur->x + cur->width, pos_y + cur->y + cur->ascent + cur->descent);

			Rect padding_box = content;
			padding_box.expand(
				cur->opening ? used_to_actual(tree_node->get_css_padding_width(element->computed_values.get_box().padding_width_left, tree_node->containing_width)) : 0,
				used_to_actual(tree_node->get_css_padding_height(element->computed_values.get_box().padding_width_top, tree_node->containing_height)),
				cur->closing ?  used_to_actual(tree_node->get_css_padding_width(element->computed_values.get_box().padding_width_right, tree_node->containing_width)) : 0,
				used_to_actual(tree_node->get_css_padding_height(element->computed_values.get_box().padding_width_bottom, tree_node->containing_height)));

			Rect border_box = padding_box;
			border_box.expand(
				cur->opening ? used_to_actual(element->computed_values.get_box().border_width_left.length.value) : 0,
				used_to_actual(element->computed_values.get_box().border_width_top.length.value),
				cur->closing ? used_to_actual(element->computed_values.get_box().border_width_right.length.value) : 0,
				used_to_actual(element->computed_values.get_box().border_width_bottom.length.value));

			CSSBackgroundRenderer background(graphics, resources, element->computed_values.get_box());
			background.set_initial_containing_box(Rect(0, 0, used_to_actual(tree_node->containing_width.value), used_to_actual(tree_node->containing_height.value))); // Bug: this is wrong
			background.set_content_box(content);
			background.set_padding_box(padding_box);
			background.set_border_box(border_box);
			background.render();

			CSSBorderRenderer border(graphics, resources, element->computed_values.get_box());
			border.set_border_box(border_box);
			border.set_border_values(
				cur->opening ? element->computed_values.get_box().border_width_left.length.value : 0,
				element->computed_values.get_box().border_width_top.length.value,
				cur->closing ? element->computed_values.get_box().border_width_right.length.value : 0,
				element->computed_values.get_box().border_width_bottom.length.value);
			border.render();
		}
	}

	return true;
}

}
