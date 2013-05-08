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
#include "get_width.h"
#include "CSSLayout/Layout/BoxTree/css_box_text.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"

namespace clan
{

CSSInlineLayoutGetWidth::CSSInlineLayoutGetWidth(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSInlineLayout *tree_node, bool start_of_line)
: graphics(graphics), resources(resources), tree_node(tree_node), start_of_line(start_of_line), width(0)
{
}

bool CSSInlineLayoutGetWidth::node(CSSInlineGeneratedBox *box, size_t text_start, size_t text_end)
{
	CSSBoxText *text = dynamic_cast<CSSBoxText *>(box->box_node);
	CSSBoxElement *element = dynamic_cast<CSSBoxElement *>(box->box_node);
	if (text)
	{
		CSSInlineLayout::adjust_start_of_line_text_range(text, text_start, text_end, start_of_line);
		Font font = graphics->get_font(text->get_properties());
		width += graphics->get_text_size(font, text->processed_text.substr(text_start, text_end - text_start)).width;
	}
	else if (element && !element->is_float())
	{
		if (box->layout_node)
			start_of_line = false;

		if (box->layout_node && (box->layout_node->is_replaced() || box->layout_node->get_element_node()->is_inline_block_level()))
		{
			width += box->layout_node->get_block_width();
		}
		else
		{
			width += used_to_actual(tree_node->get_css_margin_width(element->computed_values.get_margin().margin_width_left, tree_node->containing_width)) + used_to_actual(element->computed_values.get_border().border_width_left.length.value) + used_to_actual(tree_node->get_css_padding_width(element->computed_values.get_padding().padding_width_left, tree_node->containing_width));
		}
	}
	return true;
}

void CSSInlineLayoutGetWidth::close_node(CSSInlineGeneratedBox *box)
{
	CSSBoxElement *element = dynamic_cast<CSSBoxElement *>(box->box_node);
	if (element && !element->is_float() && (box->layout_node == 0 || !(box->layout_node->is_replaced() || box->layout_node->get_element_node()->is_inline_block_level())))
		width += used_to_actual(tree_node->get_css_margin_width(element->computed_values.get_margin().margin_width_right, tree_node->containing_width)) + used_to_actual(element->computed_values.get_border().border_width_right.length.value) + used_to_actual(tree_node->get_css_padding_width(element->computed_values.get_padding().padding_width_right, tree_node->containing_width));
}

}
