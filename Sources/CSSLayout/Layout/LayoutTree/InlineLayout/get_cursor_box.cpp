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
#include "get_cursor_box.h"
#include "CSSLayout/Layout/BoxTree/css_box_text.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"
#include "CSSLayout/Layout/LayoutTree/css_block_formatting_context.h"

namespace clan
{

CSSInlineLayoutGetCursorBox::CSSInlineLayoutGetCursorBox(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSStackingContext *stacking_context, CSSBlockFormattingContext *formatting_context, CSSBoxText *text_node, std::string::size_type pos)
: graphics(graphics), resources(resources), stacking_context(stacking_context), formatting_context(formatting_context), text_node(text_node), pos(pos)
{
}

bool CSSInlineLayoutGetCursorBox::node(CSSInlineGeneratedBox *cur)
{
	CSSBoxNode *node = cur->box_node;
	CSSBoxText *text = dynamic_cast<CSSBoxText*>(node);
	if (text && text == text_node && cur->text_start <= pos && cur->text_end >= pos)
	{
		const CSSComputedBox &properties = text->get_properties();
		if (properties.visibility.type == CSSValueVisibility::type_visible)
		{
			Font font = graphics->get_font(properties);
			FontMetrics metrics = graphics->get_font_metrics(font);
			int pos_x = used_to_actual(cur->relative_x) + formatting_context->get_x();
			int pos_y = used_to_actual(cur->relative_y) + formatting_context->get_y();
			Size size = graphics->get_text_size(font, text->processed_text.substr(cur->text_start, clamp(pos, cur->text_start, cur->text_end) - cur->text_start));
			box = Rect(Point(pos_x + cur->x + size.width, pos_y + cur->y), Size(1, (int)metrics.get_height()));
			return false;
		}
	}
	else if (cur->layout_node)
	{
		CSSLayoutTreeNode *object_node = cur->layout_node;
		bool is_same_stacking_context = (stacking_context == object_node->get_stacking_context());
		bool is_positioned = (object_node->get_element_node()->computed_properties.position.type != CSSValuePosition::type_static);
		if (is_same_stacking_context && !is_positioned)
		{
			if (object_node->get_cursor_box(graphics, resources, text_node, pos, box))
				return false;
		}
	}
	return true;
}

}
