/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "css_inline_object.h"
#include "css_inline_line_break_opportunity.h"
#include "css_inline_line_box.h"
#include "css_inline_line_box_cursor.h"
#include "css_layout_cursor.h"
#include "css_block_formatting_context.h"
#include "css_block_layout.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_text.h"
#include "../BoxTree/css_box_properties.h"

CL_CSSInlineLayout::CL_CSSInlineLayout(CL_CSSBoxElement *block_element)
: CL_CSSLayoutTreeNode(block_element)
{
}

CL_CSSInlineLayout::~CL_CSSInlineLayout()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i].layout;
	objects.clear();
}

void CL_CSSInlineLayout::render(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	render_non_content(gc, resources);
	int pos_x = formatting_context->get_x();
	int pos_y = formatting_context->get_y();
	for (size_t i = 0; i < line_boxes.size(); i++)
	{
		for (size_t j = 0; j < line_boxes[i].segments.size(); j++)
		{
			CL_CSSInlineLineSegment &segment = line_boxes[i].segments[j];
			if (objects[segment.object_index].layout)
			{
				objects[segment.object_index].layout->render(gc, resources);
			}
			else
			{
				CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(objects[segment.object_index].node);
				const CL_CSSBoxProperties &properties = text->get_properties();
				if (properties.visibility.type == CL_CSSBoxVisibility::type_visible)
				{
					CL_Font &font = resources->get_font(gc, properties);
					CL_FontMetrics metrics = font.get_font_metrics(gc);

					/*CL_Draw::line(
						gc,
						pos_x+line_boxes[i].box.left + segment.left,
						pos_y+line_boxes[i].box.top + line_boxes[i].ascent,
						pos_x+line_boxes[i].box.left + segment.right,
						pos_y+line_boxes[i].box.top + line_boxes[i].ascent, CL_Colorf::black);*/

					size_t sel_start = text->processed_selection_start;
					size_t sel_end = text->processed_selection_end;
					sel_start = cl_max(sel_start, segment.text_start);
					sel_end = cl_min(sel_end, segment.text_end);

					if (sel_start < sel_end)
					{
						CL_Rectf b = line_boxes[i].box;
						b.translate(pos_x, pos_y);
						CL_Draw::fill(gc, b, CL_Colorf::blueviolet);
						font.draw_text(
							gc,
							pos_x+line_boxes[i].box.left + segment.left,
							pos_y+line_boxes[i].box.top + line_boxes[i].ascent - segment.baseline_offset,
							text->processed_text.substr(segment.text_start, segment.text_end-segment.text_start),
							CL_Colorf::white);
					}
					else
					{
						font.draw_text(
							gc,
							pos_x+line_boxes[i].box.left + segment.left,
							pos_y+line_boxes[i].box.top + line_boxes[i].ascent - segment.baseline_offset,
							text->processed_text.substr(segment.text_start, segment.text_end-segment.text_start),
							properties.color.color);
					}
				}
			}
		}
	}
}

bool CL_CSSInlineLayout::is_empty() const
{
	return objects.empty();
}

void CL_CSSInlineLayout::push_back(CL_CSSBoxNode *box_node, CL_CSSLayoutTreeNode *layout_node)
{
	CL_CSSInlineObject object;
	object.node = box_node;
	object.layout = layout_node;
	objects.push_back(object);
}

void CL_CSSInlineLayout::calculate_top_down_sizes()
{
	CL_CSSLayoutTreeNode::calculate_top_down_sizes();
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i].layout)
		{
			objects[i].layout->used.containing.width = used.width;
			objects[i].layout->used.containing.height = used.height;
			objects[i].layout->used.containing.undetermined_width = used.undetermined_width;
			objects[i].layout->used.containing.undetermined_height = used.undetermined_height;
			objects[i].layout->calculate_top_down_sizes();
		}
	}
}

void CL_CSSInlineLayout::layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, LayoutStrategy strategy)
{
	create_line_boxes(gc, layout_cursor, strategy);
	layout_inline_blocks(gc, layout_cursor, strategy);
}

void CL_CSSInlineLayout::layout_inline_blocks(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, LayoutStrategy strategy)
{
	for (size_t i = 0; i < line_boxes.size(); i++)
	{
		for (size_t j = 0; j < line_boxes[i].segments.size(); j++)
		{
			CL_CSSInlineObject &object = objects[line_boxes[i].segments[j].object_index];
			if (object.layout)
			{
				object.layout->set_root_block_position(line_boxes[i].box.left + line_boxes[i].segments[j].left, line_boxes[i].box.top + line_boxes[i].ascent - line_boxes[i].segments[j].ascent - line_boxes[i].segments[j].baseline_offset);
			}
		}
	}
}

bool CL_CSSInlineLayout::stop_at_block_level(CL_CSSInlineLineBoxCursor &cursor, CL_CSSInlineLineBoxCursor &next_linebreak)
{
	if (objects[cursor.object_index].layout && objects[cursor.object_index].layout->get_element_node()->is_block_level())
	{
		return true;
	}
	else
	{
		for (size_t i = cursor.object_index + 1; i < next_linebreak.object_index; i++)
		{
			if (objects[i].layout && objects[i].layout->get_element_node()->is_block_level())
			{
				next_linebreak.object_index = i;
				next_linebreak.text_index = 0;
				next_linebreak.linebreak_index = cursor.linebreak_index;
				break;
			}
		}
		return false;
	}
}

void CL_CSSInlineLayout::create_line_boxes(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, LayoutStrategy strategy)
{
	create_linebreak_opportunities();
	line_boxes.clear();
	int y = layout_cursor.y + layout_cursor.margin_y;
	CL_CSSInlineLineBoxCursor line_start_cursor;
	line_start_cursor.resources = layout_cursor.resources;
	while (line_start_cursor.object_index < objects.size())
	{
		if (objects[line_start_cursor.object_index].layout && objects[line_start_cursor.object_index].layout->get_element_node()->is_block_level())
		{
			// To do: layout block level at the current line box position
			line_start_cursor.object_index++;
		}
		else
		{
			CL_CSSInlineLineBoxCursor cursor = line_start_cursor;
			CL_CSSInlineLineBox line;
			place_line_box(line, layout_cursor, y);

			int x = 0;
			bool start_of_line = true;
			do
			{
				bool is_newline = false;
				CL_CSSInlineLineBoxCursor next_linebreak = find_next_linebreak_opportunity(cursor, is_newline);
				if (stop_at_block_level(cursor, next_linebreak))
					break;
				int width = find_width(gc, cursor, next_linebreak, start_of_line);
				bool fits_on_line = x+width <= line.box.get_width();
				if (!fits_on_line && start_of_line)
				{
					// To do: force line break at the character level
					fits_on_line = true;
				}

				if (strategy == preferred_strategy)
					fits_on_line = true;
				else if (strategy == minimum_strategy)
					fits_on_line = start_of_line;

				if (fits_on_line)
				{
					bool reflow = create_line_segments(gc, layout_cursor, cursor, next_linebreak, line, start_of_line);
					if (reflow)
					{
						cursor = line_start_cursor;
						line = CL_CSSInlineLineBox();
						place_line_box(line, layout_cursor, y);
						x = 0;
						start_of_line = true;
						continue;
					}
					x += width;
				}
				else if (!start_of_line)
				{
					break;
				}
				cursor = next_linebreak;
				if (is_newline)
					break;
			} while (cursor.object_index < objects.size());

			apply_line_box_alignment(line);
			update_line_box_height(gc, cursor.resources, line);
			if (!line.segments.empty())
				layout_cursor.apply_written_width(line.box.left+line.segments.back().right);
			if (!line.segments.empty())
				line_boxes.push_back(line);
			line_start_cursor = cursor;
			y = line.box.bottom;
		}
	}
	if (!line_boxes.empty())
	{
		layout_cursor.apply_margin();
		layout_cursor.y = line_boxes.back().box.bottom;
	}
}

void CL_CSSInlineLayout::place_line_box(CL_CSSInlineLineBox &line, CL_CSSLayoutCursor &layout_cursor, int y)
{
	line.box = formatting_context->find_line_box(layout_cursor.x, layout_cursor.x+used.width, y, 1, 0);
	if (line_boxes.empty())
		apply_text_indent(layout_cursor, line);
}

void CL_CSSInlineLayout::apply_text_indent(CL_CSSLayoutCursor &layout_cursor, CL_CSSInlineLineBox &line)
{
	int text_indent = 0;
	if (element_node->computed_properties.text_indent.type == CL_CSSBoxTextIndent::type_length)
	{
		text_indent = (int)(element_node->computed_properties.text_indent.length.value + 0.5f);
	}
	else if (element_node->computed_properties.text_indent.type == CL_CSSBoxTextIndent::type_percentage)
	{
		text_indent = (int)(used.width * element_node->computed_properties.text_indent.percentage / 100.0f + 0.5f);
	}

	if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
		line.box.left += text_indent;
	else
		line.box.right -= text_indent;
}

void CL_CSSInlineLayout::apply_line_box_alignment(CL_CSSInlineLineBox &line)
{
	if (!line.segments.empty())
	{
		if (element_node->computed_properties.text_align.type == CL_CSSBoxTextAlign::type_right ||
			(element_node->computed_properties.text_align.type == CL_CSSBoxTextAlign::type_get_from_direction_property &&
			element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_rtl))
		{
			int offset = line.box.get_width() - line.segments.back().right;
			for (size_t i = 0; i < line.segments.size(); i++)
			{
				line.segments[i].left += offset;
				line.segments[i].right += offset;
			}
		}
		else if (element_node->computed_properties.text_align.type == CL_CSSBoxTextAlign::type_center)
		{
			int offset = (line.box.get_width() - line.segments.back().right) / 2;
			for (size_t i = 0; i < line.segments.size(); i++)
			{
				line.segments[i].left += offset;
				line.segments[i].right += offset;
			}
		}
		else if (element_node->computed_properties.text_align.type == CL_CSSBoxTextAlign::type_justify)
		{
			// to do: hmm! break segments into words and then justify?
		}
	}
}

void CL_CSSInlineLayout::update_line_box_height(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSInlineLineBox &line)
{
	line.ascent = 0;
	line.descent = 0;
	for (size_t i = 0; i < line.segments.size(); i++)
	{
		line.ascent = cl_max(line.ascent, line.segments[i].ascent);
		line.descent = cl_max(line.descent, line.segments[i].descent);
	}

	int line_height = line.ascent + line.descent;
	if (element_node->computed_properties.line_height.type == CL_CSSBoxLineHeight::type_normal)
	{
		// "Tells user agents to set the used value to a "reasonable" value based
		// on the font of the element. The value has the same meaning as <number>.
		// We recommend a used value for 'normal' between 1.0 to 1.2."
		//
		// Uncertain what value to use here.  The standard says that the
		// line box height should never become less than the height of the 'strut'
		// (the block element font height), but Firefox 3.6 interprets normal to
		// mean it should use the max height of the line segments.
		//
		// The following code uses the value 1.0 for 'normal', thus not producing
		// the same output as Firefox.  Hopefully this is a bug in Firefox and not
		// the standard or my interpretation of it. :-)

		CL_Font &block_font = resources->get_font(gc, element_node->computed_properties);
		float block_line_height = block_font.get_font_metrics(gc).get_height();
		line_height = cl_max(line_height, (int)(block_line_height+0.5f));
	}
	else if (element_node->computed_properties.line_height.type == CL_CSSBoxLineHeight::type_number)
	{
		//CL_Font &block_font = resources->get_font(gc, element_node->computed_properties);
		//float block_line_height = block_font.get_font_metrics(gc).get_height();
		float block_line_height = element_node->computed_properties.font_size.length.value;
		line_height = cl_max(line_height, (int)(element_node->computed_properties.line_height.number * block_line_height + 0.5f));
	}
	else if (element_node->computed_properties.line_height.type == CL_CSSBoxLineHeight::type_length)
	{
		line_height = cl_max(line_height, (int)(element_node->computed_properties.line_height.length.value+0.5f));
	}
	else if (element_node->computed_properties.line_height.type == CL_CSSBoxLineHeight::type_percentage)
	{
		//CL_Font &block_font = resources->get_font(gc, element_node->computed_properties);
		//float block_line_height = block_font.get_font_metrics(gc).get_height();
		float block_line_height = element_node->computed_properties.font_size.length.value;
		line_height = cl_max(line_height, (int)(element_node->computed_properties.line_height.percentage * block_line_height / 100.0f + 0.5f));
	}

	line.box.bottom = line.box.top + line_height;
}

int CL_CSSInlineLayout::find_width(CL_GraphicContext &gc, const CL_CSSInlineLineBoxCursor &start, const CL_CSSInlineLineBoxCursor &end, bool start_of_line)
{
	int width = 0;
	CL_CSSInlineLineBoxCursor cursor = start;
	while (cursor.object_index < end.object_index || cursor.text_index < end.text_index)
	{
		size_t text_start = cursor.text_index;
		size_t text_end = (cursor.object_index < end.object_index) ? CL_String::npos : end.text_index;
		width += find_object_width(gc, cursor.resources, objects[cursor.object_index], text_start, text_end, start_of_line);
		if (cursor.object_index == end.object_index)
			break;

		cursor.object_index++;
		cursor.text_index = 0;
	}
	return width;
}

bool CL_CSSInlineLayout::create_line_segments(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor,  const CL_CSSInlineLineBoxCursor &start, const CL_CSSInlineLineBoxCursor &end, CL_CSSInlineLineBox &line, bool &start_of_line)
{
	CL_CSSInlineLineBoxCursor cursor = start;
	while (cursor.object_index < end.object_index || cursor.text_index < end.text_index)
	{
		size_t text_start = cursor.text_index;
		size_t text_end = (cursor.object_index < end.object_index) ? CL_String::npos : end.text_index;
		create_object_segment(gc, layout_cursor, cursor.object_index, text_start, text_end, line, start_of_line);
		if (cursor.object_index == end.object_index)
			break;

		cursor.object_index++;
		cursor.text_index = 0;
	}
	return false;
}

int CL_CSSInlineLayout::find_object_width(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSInlineObject &object, size_t text_start, size_t text_end, bool &start_of_line)
{
	CL_CSSBoxElement *element = dynamic_cast<CL_CSSBoxElement*>(object.node);
	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(object.node);
	if (element)
		return find_block_width(gc, resources, object, start_of_line);
	else if (text)
		return find_text_width(gc, resources, text, text_start, text_end, start_of_line);
	else
		return 0;
}

int CL_CSSInlineLayout::find_block_width(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSInlineObject &object, bool &start_of_line)
{
	start_of_line = false;
	return object.layout->get_block_width();
}

int CL_CSSInlineLayout::find_text_width(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSBoxText *text, size_t text_start, size_t text_end, bool &start_of_line)
{
	const CL_CSSBoxProperties &properties = text->get_properties();
	if (start_of_line)
	{
		if (properties.white_space.type == CL_CSSBoxWhiteSpace::type_pre || properties.white_space.type == CL_CSSBoxWhiteSpace::type_pre_wrap)
		{
			start_of_line = false;
		}
		else
		{
			text_start = text->processed_text.find_first_not_of(' ', text_start);
			if (text_start < text_end && text_start < text->processed_text.length() && text->processed_text[text_start] != ' ')
				start_of_line = false;
		}
	}
	return resources->get_font(gc, properties).get_text_size(gc, text->processed_text.substr(text_start, text_end-text_start)).width;
}

void CL_CSSInlineLayout::create_object_segment(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, size_t object_index, size_t text_start, size_t text_end, CL_CSSInlineLineBox &line, bool &start_of_line)
{
	CL_CSSBoxElement *element = dynamic_cast<CL_CSSBoxElement*>(objects[object_index].node);
	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(objects[object_index].node);
	if (element)
		create_block_segment(gc, layout_cursor, object_index, line, start_of_line);
	else if (text)
		create_text_segment(gc, layout_cursor, object_index, text_start, text_end, line, start_of_line);
}

void CL_CSSInlineLayout::create_block_segment(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, size_t object_index, CL_CSSInlineLineBox &line, bool &start_of_line)
{
	start_of_line = false;

/*	CL_CSSLayoutCursor block_cursor = layout_cursor;
	block_cursor.x = 0;
	block_cursor.y = 0;
	block_cursor.margin_y = 0;*/
	objects[object_index].layout->layout_formatting_root(gc, layout_cursor);

	CL_CSSInlineLineSegment segment;
	segment.object_index = object_index;
	segment.height = objects[object_index].layout->get_block_height();
	segment.ascent = objects[object_index].layout->used.margin.top + objects[object_index].layout->used.border.top + objects[object_index].layout->used.padding.top;
	segment.ascent += objects[object_index].layout->get_last_line_baseline();
	if (segment.ascent == -1)
	{
		segment.ascent = segment.height;
		segment.descent = 0;
	}
	else
	{
		segment.descent = segment.height-segment.ascent;
	}
	segment.left = line.segments.empty() ? 0 : line.segments.back().right;
	segment.right = segment.left + objects[object_index].layout->get_block_width();
	segment.baseline_offset = 0; // find_baseline_offset(gc, layout_cursor.resources, objects[object_index].node);
	line.segments.push_back(segment);
}

void CL_CSSInlineLayout::create_text_segment(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, size_t object_index, size_t text_start, size_t text_end, CL_CSSInlineLineBox &line, bool &start_of_line)
{
	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(objects[object_index].node);
	const CL_CSSBoxProperties &properties = text->get_properties();
	if (start_of_line)
	{
		if (properties.white_space.type == CL_CSSBoxWhiteSpace::type_pre || properties.white_space.type == CL_CSSBoxWhiteSpace::type_pre_wrap)
		{
			start_of_line = false;
		}
		else
		{
			text_start = text->processed_text.find_first_not_of(' ', text_start);
			if (text_start < text_end && text_start < text->processed_text.length() && text->processed_text[text_start] != ' ')
				start_of_line = false;
		}
	}

	CL_Font &font = layout_cursor.resources->get_font(gc, properties);
	CL_Size text_size = font.get_text_size(gc, text->processed_text.substr(text_start, text_end-text_start));
	if (text_size.width > 0)
	{
		CL_FontMetrics metrics = font.get_font_metrics(gc);
		CL_CSSInlineLineSegment segment;
		segment.object_index = object_index;
		segment.text_start = text_start;
		segment.text_end = text_end;
		segment.ascent = (int)(metrics.get_ascent()+0.5f);
		segment.descent = (int)(metrics.get_descent()+0.5f);
		segment.height = (int)(metrics.get_height()+0.5f);
		segment.external_leading = (int)(metrics.get_external_leading()+0.5f);
		segment.left = line.segments.empty() ? 0 : line.segments.back().right;
		segment.right = segment.left + text_size.width;
		segment.baseline_offset = find_baseline_offset(gc, layout_cursor.resources, text->get_parent_element());

		if (properties.line_height.type == CL_CSSBoxLineHeight::type_number)
		{
			segment.height = cl_max(segment.height, (int)(properties.line_height.number * segment.height + 0.5f));
		}
		else if (properties.line_height.type == CL_CSSBoxLineHeight::type_length)
		{
			segment.height = cl_max(segment.height, (int)(properties.line_height.length.value+0.5f));
		}
		else if (properties.line_height.type == CL_CSSBoxLineHeight::type_percentage)
		{
			segment.height = cl_max(segment.height, (int)(properties.line_height.percentage * segment.height / 100.0f + 0.5f));
		}

		line.segments.push_back(segment);
	}
}

int CL_CSSInlineLayout::find_baseline_offset(CL_GraphicContext &gc, CL_CSSResourceCache *resources, const CL_CSSBoxElement *element)
{
	int offset = 0;

	const CL_CSSBoxElement *parent = dynamic_cast<const CL_CSSBoxElement*>(element->get_parent());
	if (parent)
	{
		if (element->computed_properties.display.type == CL_CSSBoxDisplay::type_inline)
			offset = find_baseline_offset(gc, resources, parent);

		if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_baseline)
		{
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_middle)
		{
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_sub)
		{
			offset -= (int)(resources->get_font(gc, parent->computed_properties).get_font_metrics(gc).get_descent()+0.5f);
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_super)
		{
			offset += (int)(resources->get_font(gc, parent->computed_properties).get_font_metrics(gc).get_ascent()/2.0f+0.5f);
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_text_top)
		{
			offset += (int)(resources->get_font(gc, parent->computed_properties).get_font_metrics(gc).get_ascent()+0.5f);
			offset -= (int)(resources->get_font(gc, element->computed_properties).get_font_metrics(gc).get_ascent()+0.5f);
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_text_bottom)
		{
			offset -= (int)(resources->get_font(gc, parent->computed_properties).get_font_metrics(gc).get_descent()+0.5f);
			offset += (int)(resources->get_font(gc, element->computed_properties).get_font_metrics(gc).get_descent()+0.5f);
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_percentage)
		{
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_length)
		{
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_top)
		{
		}
		else if (element->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_bottom)
		{
		}
	}
	return offset;
}

CL_CSSInlineLineBoxCursor CL_CSSInlineLayout::find_next_linebreak_opportunity(CL_CSSInlineLineBoxCursor cursor, bool &out_is_newline)
{
	if (linebreak_opportunities.size() > cursor.linebreak_index)
	{
		cursor.object_index = linebreak_opportunities[cursor.linebreak_index].object_index;
		cursor.text_index = linebreak_opportunities[cursor.linebreak_index].text_offset;
		out_is_newline = linebreak_opportunities[cursor.linebreak_index].newline;
		cursor.linebreak_index++;
	}
	else
	{
		cursor.object_index = objects.size();
		cursor.text_index = 0;
		out_is_newline = false;
	}
	return cursor;
}

void CL_CSSInlineLayout::create_linebreak_opportunities()
{
	linebreak_opportunities.clear();
	bool prev_space = true;
	for (size_t i = 0; i < objects.size(); i++)
	{
		CL_CSSBoxNode *node = objects[i].node;
		CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(node);
		if (text)
		{
			for (size_t j = 0; j < text->processed_text.size(); j++)
			{
				if (text->processed_text[j] == ' ')
				{
					if (!prev_space)
					{
						linebreak_opportunities.push_back(CL_CSSInlineLineBreakOpportunity(i, j));
						prev_space = true;
					}
				}
				else if (text->processed_text[j] == '\n')
				{
					linebreak_opportunities.push_back(CL_CSSInlineLineBreakOpportunity(i, j+1, true));
					prev_space = true;
				}
				else
				{
					prev_space = false;
				}
			}
		}
	}
}

bool CL_CSSInlineLayout::margin_collapses()
{
	return CL_CSSLayoutTreeNode::margin_collapses() && is_empty();
}

void CL_CSSInlineLayout::set_component_geometry()
{
	for (size_t i = 0; i < line_boxes.size(); i++)
	{
		for (size_t j = 0; j < line_boxes[i].segments.size(); j++)
		{
			CL_CSSInlineObject &object = objects[line_boxes[i].segments[j].object_index];
			if (object.layout)
				object.layout->set_component_geometry();
		}
	}
}

int CL_CSSInlineLayout::get_first_line_baseline()
{
	if (line_boxes.empty())
	{
		return -1;
	}
	else
	{
		int baseline = line_boxes.front().box.top + line_boxes.front().ascent;
		if (formatting_context_root)
			baseline += formatting_context->get_local_y();
		return baseline;
	}
}

int CL_CSSInlineLayout::get_last_line_baseline()
{
	if (line_boxes.empty())
	{
		return -1;
	}
	else
	{
		int baseline = line_boxes.back().box.top + line_boxes.back().ascent;
		if (formatting_context_root)
			baseline += formatting_context->get_local_y();
		return baseline;
	}
}

bool CL_CSSInlineLayout::contains_node(const CL_CSSBoxNode *node) const
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i].node == node)
			return true;
	}
	return false;
}

CL_Rect CL_CSSInlineLayout::get_cursor_box(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSBoxText *text_node, CL_String::size_type pos) const
{
	for (size_t i = 0; i < line_boxes.size(); i++)
	{
		for (size_t j = 0; j < line_boxes[i].segments.size(); j++)
		{
			const CL_CSSInlineLineSegment &segment = line_boxes[i].segments[j];
			if (objects[segment.object_index].node == text_node && segment.text_start <= pos && segment.text_end >= pos)
			{
				int start_x = formatting_context->get_x() + line_boxes[i].box.left + segment.left;
				int baseline = formatting_context->get_y() + line_boxes[i].box.top + line_boxes[i].ascent - segment.baseline_offset;
				int top = baseline - segment.ascent;
				int bottom = baseline + segment.descent;
				CL_Font &font = resources->get_font(gc, text_node->get_properties());
				CL_StringRef text = text_node->processed_text.substr(segment.text_start, pos-segment.text_start);
				CL_Size size = font.get_text_size(gc, text);
				return CL_Rect(start_x+size.width, top, start_x+size.width+1, bottom);
			}
		}
	}
	return CL_Rect();
}

CL_CSSLayoutHitTestResult CL_CSSInlineLayout::hit_test(const CL_Point &pos) const
{
	CL_CSSLayoutHitTestResult result;
/*	for (size_t i = 0; i < line_boxes.size(); i++)
	{
		for (size_t j = 0; j < line_boxes[i].segments.size(); j++)
		{
			const CL_CSSInlineLineSegment &segment = line_boxes[i].segments[j];
		}
	}*/
	return result;
}

void CL_CSSInlineLayout::prepare_children()
{
	for (size_t i = 0; i < objects.size(); i++)
		if (objects[i].layout)
			objects[i].layout->prepare(formatting_context, stacking_context);
}

bool CL_CSSInlineLayout::find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect)
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i].layout)
		{
			if (objects[i].layout->get_element_node() == element)
			{
				CL_Rect rect = objects[i].layout->content_box;
				rect.translate(formatting_context->get_x(), formatting_context->get_y());
				out_rect = rect;
				return true;
			}
			else
			{
				if (objects[i].layout->find_content_box(element, out_rect))
					return true;
			}
		}
	}

	return false;
}
