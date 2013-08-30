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
#include "css_inline_layout.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_cursor.h"
#include "CSSLayout/Layout/LayoutTree/css_stacking_context.h"
#include "CSSLayout/Layout/LayoutTree/css_block_formatting_context.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"
#include "CSSLayout/Layout/LayoutTree/css_background_renderer.h"
#include "CSSLayout/Layout/LayoutTree/css_border_renderer.h"
#include "CSSLayout/Layout/BoxTree/css_box_text.h"
#include "CSSLayout/css_resource_cache.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"
#include "add_content_margin_top.h"
#include "calculate_top_down_heights.h"
#include "create_linebreak_opportunities.h"
#include "layout_absolute_and_fixed_content.h"
#include "layout_inline_blocks_and_floats.h"
#include "layout_is_empty.h"
#include "layout_line.h"
#include "prepare_children.h"
#include "render_layer_floats.h"
#include "render_layer_positioned.h"
#include "render_layer_inline.h"
#include "render_layer_non_inline.h"
#include "render_layer_background.h"
#include "set_component_geometry.h"
#include "get_cursor_box.h"
#include "is_empty_line.h"
#include "get_width.h"
#include "place_floats.h"
#include "align_line.h"
#include "find_content_box.h"
#include "generate_line.h"

namespace clan
{

CSSInlineLayout::CSSInlineLayout(CSSBoxElement *element)
: CSSLayoutTreeNode(element)
{
}

void CSSInlineLayout::add_box(CSSInlineGeneratedBox *box)
{
	boxes.add_box(box);
}

void CSSInlineLayout::set_component_geometry()
{
	CSSInlineLayoutSetComponentGeometry visitor;
	boxes.descendants(&visitor);
}

int CSSInlineLayout::get_first_line_baseline()
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		CSSInlineGeneratedBox *cur = lines[i];
		if (cur->layout_node)
		{
			int baseline = cur->layout_node->get_first_line_baseline();
			if (baseline != -1)
			{
				if (cur->layout_node->is_formatting_context_root())
					baseline += cur->layout_node->get_formatting_context()->get_local_y();
				return baseline;
			}
		}
		else
		{
			return cur->y + cur->ascent;
		}
	}
	return -1;
}

int CSSInlineLayout::get_last_line_baseline()
{
	for (size_t i = lines.size(); i > 0; i++)
	{
		CSSInlineGeneratedBox *cur = lines[i-1];
		if (cur->layout_node)
		{
			int baseline = cur->layout_node->get_last_line_baseline();
			if (baseline != -1)
			{
				if (cur->layout_node->is_formatting_context_root())
					baseline += cur->layout_node->get_formatting_context()->get_local_y();
				return baseline;
			}
		}
		else
		{
			return cur->y + cur->ascent;
		}
	}
	return -1;
}

void CSSInlineLayout::prepare_children()
{
	CSSInlineLayoutPrepareChildren visitor(formatting_context, stacking_context);
	boxes.descendants(&visitor);
}

void CSSInlineLayout::calculate_content_top_down_heights()
{
	CSSInlineLayoutCalculateTopDownHeights visitor(height);
	boxes.descendants(&visitor);
}

bool CSSInlineLayout::add_content_margin_top(CSSLayoutCursor &cursor)
{
	CSSInlineLayoutAddContentMarginTop visitor(cursor);
	boxes.descendants(&visitor);
	if (visitor.result)
	{
		return true;
	}
	else if (height.use_content)
	{
		return false;
	}
	else
	{
		return height.value != 0.0f;
	}
}

bool CSSInlineLayout::is_empty() const
{
	CSSInlineLayoutIsEmpty visitor;
	boxes.descendants(&visitor);
	if (!visitor.result)
	{
		return false;
	}
	if (height.use_content)
	{
		return true;
	}
	else
	{
		return height.value == 0.0f;
	}
}

void CSSInlineLayout::layout_content(CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	lines.clear();
	layout_inline_blocks_and_floats(graphics, cursor.resources, strategy);
	create_linebreak_opportunities();

	CSSInlinePosition line_start_pos = begin();
	size_t next_linebreak_opportunity = 0;
	while (line_start_pos != end())
	{
		if (line_start_pos.is_block_level() && !line_start_pos.is_float())
		{
			CSSLayoutTreeNode *layout_node = line_start_pos.box->layout_node;

			if (layout_node->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_absolute &&
				layout_node->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_fixed)
			{
				generate_block_line(line_start_pos);
				layout_block_line(lines.back(), graphics, cursor, strategy);
			}
			else
			{
				// We layout absolute or fixed elements later since they may rely on the calculated height of the normal flow.

				layout_node->static_position_parent = this;
				layout_node->static_position.left = actual_to_used(cursor.x);
				layout_node->static_position.top = actual_to_used(cursor.y + cursor.get_total_margin());
				layout_node->static_position.right = layout_node->static_position.left;
				layout_node->static_position.bottom = layout_node->static_position.top;
				layout_node->relative_x = line_start_pos.box->relative_x;
				layout_node->relative_y = line_start_pos.box->relative_y;
				layout_node->containing_width = width;
				layout_node->calc_preferred(graphics, cursor.resources);
				layout_node->calculate_top_down_widths(normal_strategy);
				layout_node->calculate_top_down_heights();
				if (layout_node->width.expanding)
					layout_node->width.value = layout_node->preferred_width;
				layout_node->layout_formatting_root_helper(graphics, cursor.resources, normal_strategy);
				layout_node->static_position.right = layout_node->static_position.left + layout_node->width.value;
				layout_node->static_position.bottom = layout_node->static_position.top + layout_node->height.value;
			}

			if (line_start_pos == linebreak_opportunities[next_linebreak_opportunity].pos)
				next_linebreak_opportunity++;

			line_start_pos.text_pos = 0;
			if (line_start_pos.box->first_child)
			{
				line_start_pos.box = line_start_pos.box->first_child;
			}
			else if (line_start_pos.box->next_sibling)
			{
				line_start_pos.box = line_start_pos.box->next_sibling;
			}
			else
			{
				while (line_start_pos.box && !line_start_pos.box->next_sibling)
				{
					line_start_pos.box = line_start_pos.box->parent;
				}
				if (line_start_pos.box)
				{
					line_start_pos.box = line_start_pos.box->next_sibling;
				}
			}
		}
		else
		{
			CSSActualValue y = used_to_actual(cursor.y + cursor.get_total_margin());
			Rect line_box;

			CSSInlinePosition line_end_pos = line_start_pos;
			CSSActualValue available_width = used_to_actual(width.value);
			CSSActualValue used_width = 0;
			bool start_of_line = true;
			bool restart = false;
			while (next_linebreak_opportunity < linebreak_opportunities.size())
			{
				CSSInlinePosition linebreak_pos = linebreak_opportunities[next_linebreak_opportunity].pos;
				bool is_newline = linebreak_opportunities[next_linebreak_opportunity].newline;

				bool next_start_of_line = start_of_line;
				CSSActualValue segment_width = get_width(graphics, cursor.resources, line_end_pos, linebreak_pos, next_start_of_line);

				if (start_of_line)
				{
					CSSActualValue text_indent = 0;
					if (lines.empty())
					{
						if (element_node->computed_values.get_text_inherit().text_indent.type == CSSValueTextIndent::type_length)
						{
							text_indent = used_to_actual(element_node->computed_values.get_text_inherit().text_indent.length.value);
						}
						else if (element_node->computed_values.get_text_inherit().text_indent.type == CSSValueTextIndent::type_percentage)
						{
							text_indent = used_to_actual(width.value * element_node->computed_values.get_text_inherit().text_indent.percentage / 100.0f);
						}
					}

					CSSActualValue w = used_to_actual(width.value);
					if (width.expanding && strategy == preferred_strategy)
					{
						w = 1000000;
						if (!css_max_height.use_content)
							w = min(w, used_to_actual(css_max_height.value));
						w = max(w, used_to_actual(css_min_height));
					}
					line_box = formatting_context->find_line_box(cursor.x + text_indent, cursor.x + w, y, 1, segment_width);
					available_width = line_box.get_width();
				}

				bool fits_on_line = used_width + segment_width <= available_width;
				if (!fits_on_line && start_of_line)
				{
					// To do: force line break at the character level
					fits_on_line = true;
				}

				if (width.expanding)
				{
					if (strategy == preferred_strategy)
						fits_on_line = true;
					else if (strategy == minimum_strategy)
						fits_on_line = start_of_line;
				}

				if (!fits_on_line)
					break;

				if (place_floats(line_start_pos, linebreak_pos, cursor.x, y, strategy))
				{
					restart = true;
					break;
				}
				used_width += segment_width;
				line_end_pos = linebreak_pos;
				next_linebreak_opportunity++;
				start_of_line = next_start_of_line;
				if (is_newline || (line_end_pos.is_block_level() && !line_end_pos.is_float()))
					break;
			}
			if (restart)
				continue;

			if (!is_empty_line(line_start_pos, line_end_pos))
			{
				generate_line(line_start_pos, line_end_pos);
				layout_line(lines.back(), line_box, graphics, cursor.resources);
				cursor.apply_margin();
				cursor.y = actual_to_used(line_box.bottom);

				if (strategy != normal_strategy && width.expanding)
					width.value = max(width.value, actual_to_used(line_box.left+used_width-cursor.x));
			}

			line_start_pos = line_end_pos;
		}
	}

	for (size_t i = 0; i < lines.size(); i++)
	{
		bool last_line = (i+1 == lines.size());
		if (!lines[i]->is_block_line())
			align_line(lines[i], graphics, cursor.resources, last_line);
	}

	if (lines.empty() && height.value > 0.0f)
		cursor.apply_margin();
}

void CSSInlineLayout::layout_absolute_and_fixed_content(CSSLayoutGraphics *graphics, CSSResourceCache *resources, Rect containing_block, const Size &viewport_size)
{
	if (get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static)
	{
		Rect rect = get_padding_box();
		containing_block = rect;
	}

	CSSInlineLayoutLayoutAbsoluteAndFixedContent visitor(graphics, resources, containing_block, viewport_size);
	boxes.descendants(&visitor);
}

bool CSSInlineLayout::find_content_box(CSSBoxElement *search_element, Rect &out_rect)
{
	if (get_element_node() == search_element)
	{
		Rect box = content_box;
		box.translate(used_to_actual(relative_x), used_to_actual(relative_y));
		if (!formatting_context_root)
			content_box.translate(formatting_context->get_x(), formatting_context->get_y());
		else if (formatting_context->get_parent())
			content_box.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
		out_rect = box;
		return true;
	}

	CSSInlineLayoutFindContentBox visitor(search_element, formatting_context);
	if (!boxes.self_and_descendants(&visitor))
	{
		out_rect = visitor.box;
		return true;
	}
	else
	{
		return false;
	}
}

bool CSSInlineLayout::get_cursor_box(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSBoxText *text_node, std::string::size_type pos, Rect &out_box)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		CSSInlineLayoutGetCursorBox visitor(graphics, resources, stacking_context, formatting_context, text_node, pos);
		if (!lines[i]->self_and_descendants(&visitor))
		{
			out_box = visitor.get_box();
			return true;
		}
	}

	for (size_t i = 0; i < floats.size(); i++)
	{
		if (floats[i]->get_cursor_box(graphics, resources, text_node, pos, out_box))
			return true;
	}

	return false;
}

void CSSInlineLayout::render_layer_background(CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool root)
{
	render_non_content(graphics, resources, root);

	for (size_t i = 0; i < lines.size(); i++)
	{
		CSSInlineLayoutRenderLayerBackground visitor(graphics, resources, this);
		lines[i]->self_and_descendants(&visitor);
	}
}

void CSSInlineLayout::render_layer_non_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		CSSInlineLayoutRenderLayerNonInline visitor(graphics, resources, stacking_context);
		lines[i]->self_and_descendants(&visitor);
	}
}

void CSSInlineLayout::render_layer_floats(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	CSSInlineLayoutRenderLayerFloats visitor(graphics, resources, stacking_context);
	boxes.descendants(&visitor);
}

void CSSInlineLayout::render_layer_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		CSSInlineLayoutRenderLayerInline visitor(graphics, resources, stacking_context, formatting_context);
		lines[i]->self_and_descendants(&visitor);
	}
}

void CSSInlineLayout::render_layer_positioned(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	CSSInlineLayoutRenderLayerPositioned visitor(graphics, resources, stacking_context);
	boxes.descendants(&visitor);
}

void CSSInlineLayout::create_linebreak_opportunities()
{
	CSSInlineLayoutCreateLinebreakOpportunities visitor(linebreak_opportunities);
	boxes.descendants(&visitor);
	linebreak_opportunities.push_back(CSSInlineLineBreakOpportunity(0, 0));
}

CSSInlinePosition CSSInlineLayout::begin() const
{
	return CSSInlinePosition(boxes.first_child, 0);
}

CSSInlinePosition CSSInlineLayout::end() const
{
	return CSSInlinePosition();
}

void CSSInlineLayout::generate_block_line(CSSInlinePosition pos)
{
	std::unique_ptr<CSSInlineGeneratedBox> line(new CSSInlineGeneratedBox());
	line->box_node = pos.box->box_node;
	line->layout_node = pos.box->layout_node;
	line->relative_x = pos.box->relative_x;
	line->relative_y = pos.box->relative_y;
	lines.push_back(line.get());
	line.release();
}

void CSSInlineLayout::generate_line(CSSInlinePosition start, CSSInlinePosition end)
{
	CSSInlineLayoutGenerateLine visitor(start.box, boxes.relative_x, boxes.relative_y);
	CSSInlinePosition::foreach(start, end, &visitor);
	lines.push_back(visitor.line.get());
	visitor.line.release();
}

bool CSSInlineLayout::place_floats(CSSInlinePosition start, CSSInlinePosition end, CSSActualValue x, CSSActualValue y, LayoutStrategy strategy)
{
	CSSInlineLayoutPlaceFloats visitor(this, x, y, strategy);
	return !CSSInlinePosition::foreach(start, end, &visitor);
}

bool CSSInlineLayout::is_empty_line(CSSInlinePosition start, CSSInlinePosition end) const
{
	CSSInlineLayoutIsEmptyLine visitor;
	return CSSInlinePosition::foreach(start, end, &visitor);
}

CSSActualValue CSSInlineLayout::get_width(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSInlinePosition start, CSSInlinePosition end, bool &start_of_line)
{
	CSSInlineLayoutGetWidth visitor(graphics, resources, this, start_of_line);
	CSSInlinePosition::foreach(start, end, &visitor);
	start_of_line = visitor.get_start_of_line();
	return visitor.get_width();
}

void CSSInlineLayout::layout_inline_blocks_and_floats(CSSLayoutGraphics *graphics, CSSResourceCache *resources, LayoutStrategy strategy)
{
	floats.clear();
	boxes.relative_x = relative_x;
	boxes.relative_y = relative_y;
	CSSInlineLayoutLayoutInlineBlocksAndFloats visitor(this, graphics, resources, strategy);
	boxes.descendants(&visitor);
}

void CSSInlineLayout::layout_line(CSSInlineGeneratedBox *line, Rect &line_box, CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	CSSInlineLayoutLayoutLine visitor(this, line, line_box, graphics, resources);
	line->self_and_descendants(&visitor);

	switch (get_element_node()->computed_values.get_font().line_height.type)
	{
	default:
	case CSSValueLineHeight::type_normal:
		break;
	case CSSValueLineHeight::type_length:
		line->height = max(line->height, used_to_actual(get_element_node()->computed_values.get_font().line_height.length.value));
		break;
	case CSSValueLineHeight::type_number:
		line->height = max(line->height, used_to_actual(get_element_node()->computed_values.get_font().line_height.number * get_element_node()->computed_values.get_font().font_size.length.value));
		break;
	}

	//CSSActualValue line_width = x - line_box.left;
	//CSSActualValue extra_total = max(0, line_box.get_width() - line_width);
	line->width = line_box.get_width();
	line_box.bottom = line->y + line->height;
}

void CSSInlineLayout::align_line(CSSInlineGeneratedBox *line, CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool last_line)
{
	//CSSInlineLayoutAlignLine visitor(this, line, graphics, resources, last_line);
	//line->descendants(&visitor);
}

void CSSInlineLayout::layout_block_line(CSSInlineGeneratedBox *line, CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	if (line->layout_node->get_element_node()->is_overflow_visible() && !line->layout_node->get_element_node()->is_table() && !line->layout_node->is_replaced())
	{
		line->layout_node->containing_width = width;
		line->layout_node->layout_normal(graphics, cursor, strategy);
		if (strategy != normal_strategy && width.expanding)
			width.value = max(width.value, actual_to_used(line->layout_node->get_block_width()));
	}
	else
	{
		int box_y = used_to_actual(cursor.y+cursor.get_total_margin());
		if (line->layout_node->get_element_node()->computed_values.get_box().clear.type == CSSValueClear::type_left || line->layout_node->get_element_node()->computed_values.get_box().clear.type == CSSValueClear::type_both)
		{
			int clear_left = formatting_context->find_left_clearance();
			box_y = max(box_y, clear_left);
		}
		if (line->layout_node->get_element_node()->computed_values.get_box().clear.type == CSSValueClear::type_right || line->layout_node->get_element_node()->computed_values.get_box().clear.type == CSSValueClear::type_both)
		{
			int clear_right = formatting_context->find_right_clearance();
			box_y = max(box_y, clear_right);
		}

		line->layout_node->containing_width = width;
		if (strategy == normal_strategy)
		{
			CSSActualValue available_width = formatting_context->find_line_box(cursor.x, cursor.x + used_to_actual(width.value), box_y, 1, 0).get_width();
			line->layout_node->containing_width.value = available_width;
		}
		line->layout_node->calculate_top_down_widths(strategy);
		line->layout_node->calculate_top_down_heights();
		line->layout_node->layout_formatting_root_helper(graphics, cursor.resources, strategy);

		Rect float_box(0, 0, line->layout_node->get_block_width(), line->layout_node->get_block_height());
		float_box.translate(cursor.x, box_y);
		if (strategy == preferred_strategy && width.expanding)
		{
			float_box = formatting_context->place_left(float_box, cursor.x+1000000);
			cursor.apply_margin();
			cursor.y = float_box.bottom;
		}
		else
		{
			float_box = formatting_context->place_left(float_box, cursor.x+used_to_actual(width.value));
			cursor.apply_margin();
			cursor.y = float_box.bottom;
		}

		if (strategy != normal_strategy && width.expanding)
			width.value = max(width.value, actual_to_used(float_box.right - cursor.x));

		line->layout_node->set_root_block_position(float_box.left, float_box.top);
	}
}

void CSSInlineLayout::adjust_start_of_line_text_range(CSSBoxText *text, size_t &text_start, size_t &text_end, bool &start_of_line)
{
	if (start_of_line && text_start < text_end)
	{
		const CSSComputedTextInherit &properties = text->get_properties().get_text_inherit();
		if (properties.white_space.type == CSSValueWhiteSpace::type_pre || properties.white_space.type == CSSValueWhiteSpace::type_pre_wrap)
		{
			start_of_line = false;
		}
		else
		{
			text_start = text->processed_text.find_first_not_of(' ', text_start);
			if (text_start == std::string::npos || text_start >= text_end)
			{
				text_start = text_end;
			}
			else if (text->processed_text[text_start] != ' ')
			{
				start_of_line = false;
			}
		}
	}
}

}
