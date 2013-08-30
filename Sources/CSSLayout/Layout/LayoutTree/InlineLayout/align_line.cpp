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
#include "align_line.h"
#include "word_count.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"

namespace clan
{

CSSInlineLayoutAlignLine::CSSInlineLayoutAlignLine(CSSInlineLayout *layout, CSSInlineGeneratedBox *line, CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool last_line)
	: graphics(graphics), resources(resources)
{
	segments_width = line->last_child->x + line->last_child->width - line->first_child->x;
	extra_total = max(0, line->width - segments_width);
	offset_x = 0;
	word_count = 0;
	word_index = 0;
	if (layout->get_element_node()->computed_values.get_text_inherit().text_align.type == CSSValueTextAlign::type_right)
	{
		offset_x = extra_total;
	}
	else if (layout->get_element_node()->computed_values.get_text_inherit().text_align.type == CSSValueTextAlign::type_center)
	{
		offset_x = extra_total / 2;
	}
	else if (layout->get_element_node()->computed_values.get_text_inherit().text_align.type == CSSValueTextAlign::type_justify && !last_line)
	{
		word_count = find_word_count(line);
	}

	start_of_line = true;
	prev_space = true;
	baseline_y = line->y + (line->height - line->ascent - line->descent) / 2 + line->ascent;
}

bool CSSInlineLayoutAlignLine::node(CSSInlineGeneratedBox *cur)
{
	cur->x += offset_x;
	cur->y = baseline_y - cur->ascent;
	if (cur->layout_node && (cur->layout_node->is_replaced() || cur->layout_node->get_element_node()->is_inline_block_level()))
	{
		cur->layout_node->set_root_block_position(cur->x, cur->y);
	}

	if (word_count != 0)
	{
		CSSBoxNode *node = cur->box_node;
		CSSBoxText *text = dynamic_cast<CSSBoxText*>(node);
		if (text)
		{
			for (size_t j = cur->text_start; j < cur->text_end; j++)
			{
				if (text->processed_text[j] == ' ')
				{
					prev_space = true;
				}
				else
				{
					if (prev_space && !start_of_line)
					{
						split_text(cur, j, graphics, resources);
						cur = cur->next_sibling;

						CSSActualValue v1 = extra_total * word_index / word_count;
						CSSActualValue v2 = extra_total * (word_index + 1) / word_count;
						CSSActualValue word_offset_x = v2-v1;
						word_index++;

						cur->x += word_offset_x;
						if (cur->parent)
							expand_box(cur->parent, word_offset_x);
						offset_x += word_offset_x;

						prev_space = false;
						start_of_line = false;
						break;
					}
					else
					{
						prev_space = false;
						start_of_line = false;
					}
				}
			}
		}
	}
	return true;
}

int CSSInlineLayoutAlignLine::find_word_count(CSSInlineGeneratedBox *line)
{
	CSSInlineLayoutWordCount word_count;
	line->self_and_descendants(&word_count);
	return word_count.get_word_count();
}

void CSSInlineLayoutAlignLine::expand_box(CSSInlineGeneratedBox *box, CSSActualValue extra)
{
	while (box->parent)
	{
		box->width += extra;
		box = box->parent;
	}
}

void CSSInlineLayoutAlignLine::split_text(CSSInlineGeneratedBox *box, size_t text_pos, CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	// Duplicate box:
	{
		std::unique_ptr<CSSInlineGeneratedBox> box2(new CSSInlineGeneratedBox());
		box2->box_node = box->box_node;
		box2->relative_x = box->relative_x;
		box2->relative_y = box->relative_y;
		box2->x = box->x;
		box2->y = box->y;
		box2->width = box->width;
		box2->height = box->height;
		box2->ascent = box->ascent;
		box2->descent = box->descent;
		box2->baseline_offset = box->baseline_offset;
		box2->text_start = text_pos;
		box2->text_end = box->text_end;
		box->text_end = text_pos;

		box2->parent = box->parent;
		box2->next_sibling = box->next_sibling;
		box->next_sibling = box2.get();
		if (box->parent && box->parent->last_child == box)
			box->parent->last_child = box2.get();
		box2.release();
	}

	CSSInlineGeneratedBox *box2 = box->next_sibling;

	CSSBoxText *text = dynamic_cast<CSSBoxText *>(box->box_node);

	Font font = graphics->get_font(text->get_properties());

	box->width = graphics->get_text_size(font, text->processed_text.substr(box->text_start, box->text_end-box->text_start)).width;
	box2->width = graphics->get_text_size(font, text->processed_text.substr(box2->text_start, box2->text_end-box2->text_start)).width;
	box2->x += box->width;
}

}
