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
#include "layout_line.h"
#include "CSSLayout/Layout/BoxTree/css_box_text.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"

namespace clan
{

CSSInlineLayoutLayoutLine::CSSInlineLayoutLayoutLine(CSSInlineLayout *layout, CSSInlineGeneratedBox *line, Rect &line_box, CSSLayoutGraphics *graphics, CSSResourceCache *resources)
: layout(layout), line(line), line_box(line_box), graphics(graphics), resources(resources)
{
	x = line_box.left;
	baseline_offset = 0;
	line->y = line_box.top;
}

bool CSSInlineLayoutLayoutLine::node(CSSInlineGeneratedBox *cur)
{
	cur->y = line->y;

	CSSBoxText *text = dynamic_cast<CSSBoxText *>(cur->box_node);
	CSSBoxElement *element = dynamic_cast<CSSBoxElement *>(cur->box_node);
	if (text)
	{
		cur->x = x;
		const CSSComputedBox &properties = text->get_properties();
		Font font = graphics->get_font(properties);
		FontMetrics metrics = graphics->get_font_metrics(font);
		cur->ascent = used_to_actual(metrics.get_ascent());
		cur->descent = used_to_actual(metrics.get_descent());
		switch (properties.line_height.type)
		{
		default:
		case CSSValueLineHeight::type_normal:
			cur->height = cur->ascent + cur->descent;
			break;
		case CSSValueLineHeight::type_length:
			cur->height = used_to_actual(properties.line_height.length.value);
			break;
		case CSSValueLineHeight::type_number:
			cur->height = used_to_actual(properties.line_height.number * properties.font_size.length.value);
			break;
		}
		cur->baseline_offset = baseline_offset;
		cur->parent->ascent = max(cur->parent->ascent, cur->ascent);
		cur->parent->descent = max(cur->parent->descent, cur->descent);
		cur->parent->height = max(cur->parent->height, cur->height);
		std::string t = text->processed_text.substr(cur->text_start, cur->text_end - cur->text_start);
		cur->width = graphics->get_text_size(font, t).width;
		x += cur->width;
	}
	else if (element)
	{
		if (cur->layout_node && (cur->layout_node->is_replaced() || cur->layout_node->get_element_node()->is_inline_block_level()))
		{
			cur->height = cur->layout_node->get_block_height();
			cur->ascent = cur->height; // To do: fetch baseline from layout node
			cur->parent->ascent = max(cur->parent->ascent, cur->ascent);
			cur->parent->height = max(cur->parent->height, cur->height);
			cur->x = x;
			x += cur->layout_node->get_block_width();
		}
		else
		{
			if (cur->opening)
				x += used_to_actual(layout->get_css_margin_width(element->computed_values.get_box().margin_width_left, layout->containing_width)) + used_to_actual(element->computed_values.get_box().border_width_left.length.value) + used_to_actual(layout->get_css_padding_width(element->computed_values.get_box().padding_width_left, layout->containing_width));
			cur->x = x;
			cur->height = used_to_actual(element->computed_values.get_box().line_height.length.value);
			switch (element->computed_values.get_box().line_height.type)
			{
			default:
			case CSSValueLineHeight::type_normal:
				break;
			case CSSValueLineHeight::type_length:
				cur->height = used_to_actual(element->computed_values.get_box().line_height.length.value);
				break;
			case CSSValueLineHeight::type_number:
				cur->height = used_to_actual(element->computed_values.get_box().line_height.number * element->computed_values.get_box().font_size.length.value);
				break;
			}
		}
	}
	return true;
}

void CSSInlineLayoutLayoutLine::close_node(CSSInlineGeneratedBox *cur)
{
	CSSBoxElement *element = dynamic_cast<CSSBoxElement *>(cur->box_node);
	cur->width = x - cur->x;
	if (cur->parent)
	{
		cur->parent->ascent = max(cur->parent->ascent, cur->ascent);
		cur->parent->descent = max(cur->parent->descent, cur->descent);
		cur->parent->height = max(cur->parent->height, cur->height);
	}
	if (element && cur->closing && (cur->layout_node == 0 || !(cur->layout_node->is_replaced() || cur->layout_node->get_element_node()->is_inline_block_level())))
	{
		x += used_to_actual(layout->get_css_margin_width(element->computed_values.get_box().margin_width_right, layout->containing_width)) + used_to_actual(element->computed_values.get_box().border_width_right.length.value) + used_to_actual(layout->get_css_padding_width(element->computed_values.get_box().padding_width_right, layout->containing_width));
	}
}

}
