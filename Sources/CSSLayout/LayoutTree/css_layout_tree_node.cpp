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
#include "css_layout_tree_node.h"
#include "css_block_formatting_context.h"
#include "css_stacking_context.h"
#include "css_layout_cursor.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "clan_image_stretch.h"

CL_CSSLayoutTreeNode::CL_CSSLayoutTreeNode(CL_CSSBoxElement *element_node)
: element_node(element_node), formatting_context(0), formatting_context_root(false), stacking_context(0), stacking_context_root(false)
{
}

CL_CSSLayoutTreeNode::~CL_CSSLayoutTreeNode()
{
	if (formatting_context_root)
		delete formatting_context;
//	if (stacking_context_root)
//		delete stacking_context;
}

void CL_CSSLayoutTreeNode::prepare(CL_CSSBlockFormattingContext *current_formatting_context, CL_CSSStackingContext *current_stacking_context)
{
	if (current_formatting_context == 0 || element_node->is_inline_block_level() || element_node->is_float() || element_node->is_table() || element_node->is_table_cell() || is_replaced())
		set_formatting_context(new CL_CSSBlockFormattingContext(current_formatting_context), true);
	else
		set_formatting_context(current_formatting_context, false);
	establish_stacking_context_if_needed(current_stacking_context);
	prepare_children();
}

void CL_CSSLayoutTreeNode::calculate_top_down_sizes()
{
	used.set_width(element_node->computed_properties);
	used.set_height(element_node->computed_properties);
	//used.calc_block_offsets(element_node->computed_properties);
}

void CL_CSSLayoutTreeNode::set_auto_width(CL_CSSUsedValue width)
{
	used.set_width(element_node->computed_properties, width);
}

void CL_CSSLayoutTreeNode::set_root_block_position(int x, int y)
{
	if (!formatting_context_root)
		throw CL_Exception("CL_CSSLayoutTreeNode::set_root_block_position misuse");
	content_box = CL_Rect(CL_Point(x+used.margin.left+used.border.left+used.padding.left, y+used.margin.top+used.border.top+used.padding.top), content_box.get_size());
	formatting_context->set_position(content_box.left, content_box.top);
}

void CL_CSSLayoutTreeNode::calc_preferred(CL_GraphicContext &gc, CL_CSSLayoutCursor &parent_flow)
{
	if (!used.preferred_width_calculated)
	{
		calculate_top_down_sizes();
		layout_formatting_root_helper(gc, parent_flow, preferred_strategy);
	}
}

void CL_CSSLayoutTreeNode::calc_minimum(CL_GraphicContext &gc, CL_CSSLayoutCursor &parent_flow)
{
	if (!used.min_width_calculated)
	{
		calculate_top_down_sizes();
		layout_formatting_root_helper(gc, parent_flow, minimum_strategy);
	}
}

void CL_CSSLayoutTreeNode::layout_minimum(CL_GraphicContext &gc, CL_CSSLayoutCursor &parent_flow)
{
	calc_minimum(gc, parent_flow);
	set_auto_width(used.min_width);
	layout_formatting_root_helper(gc, parent_flow, normal_strategy);
}

void CL_CSSLayoutTreeNode::layout_preferred(CL_GraphicContext &gc, CL_CSSLayoutCursor &parent_flow)
{
	calc_preferred(gc, parent_flow);
	set_auto_width(used.preferred_width);
	layout_formatting_root_helper(gc, parent_flow, normal_strategy);
}

void CL_CSSLayoutTreeNode::layout_shrink_to_fit(CL_GraphicContext &gc, CL_CSSLayoutCursor &parent_flow)
{
	calc_preferred(gc, parent_flow);
	CL_CSSUsedValue available_width = used.containing.width;
	if (used.preferred_width > available_width + 0.1f)
	{
		calc_minimum(gc, parent_flow);
		set_auto_width(cl_max(used.min_width, available_width));
	}
	else
	{
		set_auto_width(used.preferred_width);
	}
	layout_formatting_root_helper(gc, parent_flow, normal_strategy);
}

void CL_CSSLayoutTreeNode::layout_formatting_root(CL_GraphicContext &gc, CL_CSSLayoutCursor &parent_flow, LayoutStrategy strategy)
{
	calculate_top_down_sizes();
	if (element_node->is_shrink_to_fit())
	{
		if (strategy == preferred_strategy)
			layout_preferred(gc, parent_flow);
		else if (strategy == minimum_strategy)
			layout_minimum(gc, parent_flow);
		else
			layout_shrink_to_fit(gc, parent_flow);
	}
	else
	{
		layout_formatting_root_helper(gc, parent_flow, normal_strategy);
	}

/*	if (element_node->computed_properties.background_color.type == CL_CSSBoxBackgroundColor::type_color &&
		element_node->computed_properties.background_color.color == CL_Colorf::green)
	{
		cl_debug_breakpoint = 0xdeadbabe;
	}*/
}

void CL_CSSLayoutTreeNode::layout_formatting_root_helper(CL_GraphicContext &gc, CL_CSSLayoutCursor &parent_flow, LayoutStrategy strategy)
{
	formatting_context->clear();

	CL_CSSLayoutCursor cursor;
	cursor.x = 0;
	cursor.y = 0;
	cursor.resources = parent_flow.resources;
	add_content_margin_top(cursor);
	layout_content(gc, cursor, strategy);
	cursor.apply_margin();

	if (strategy != normal_strategy && element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
	{
		if (strategy == preferred_strategy)
		{
			used.preferred_width = cursor.max_written_width;
			used.preferred_width_calculated = true;
		}
		else if (strategy == minimum_strategy)
		{
			used.min_width = cursor.max_written_width;
			used.min_width_calculated = true;
		}
		//used.width = cursor.max_written_width+0.1f; // temp hack to test some rounding issues elsewhere.
	}

	if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		used.height = cursor.y;
		// rombust - These were "int", but it is written to a float, so I am using CL_CSSUsedValue instead.
		CL_CSSUsedValue left_float_height = formatting_context->find_left_clearance();
		CL_CSSUsedValue right_float_height = formatting_context->find_right_clearance();
		used.height = cl_max(used.height, cl_max(left_float_height, right_float_height));
	}

	content_box = CL_Size(used.width, used.height);
}

void CL_CSSLayoutTreeNode::layout_normal(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	float old_x = cursor.x;

	cursor.x += used.margin.left + used.border.left + used.padding.left;
	add_margin_top(cursor);

	if (used.border.top > 0 || used.padding.top > 0)
		cursor.apply_margin();

	cursor.y += used.border.top+used.padding.top;

	if (element_node->computed_properties.clear.type == CL_CSSBoxClear::type_left || element_node->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		int clear_left = formatting_context->find_left_clearance();
		if (cursor.y+cursor.margin_y < clear_left)
			cursor.y = clear_left-cursor.margin_y;
	}
	if (element_node->computed_properties.clear.type == CL_CSSBoxClear::type_right || element_node->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		int clear_right = formatting_context->find_right_clearance();
		if (cursor.y+cursor.margin_y < clear_right)
			cursor.y = clear_right-cursor.margin_y;
	}

	content_box.left = cursor.x;
	content_box.top = cursor.y+cursor.margin_y;
	content_box.right = content_box.left+used.width;
	content_box.bottom = content_box.top+used.height;
	cursor.apply_written_width(content_box.right);

	layout_content(gc, cursor, strategy);

	if (used.border.bottom > 0 || used.padding.bottom > 0)
		cursor.apply_margin();

	if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		used.height = cl_max(0.0f, cursor.y - content_box.top);
		content_box.bottom = cursor.y;
	}
	else
	{
		float inner_margin_y = cursor.y+cursor.margin_y;
		cursor.y = content_box.top + used.height;
		cursor.margin_y = cl_max(0.0f, inner_margin_y-cursor.y);
	}

	cursor.y += used.border.bottom+used.padding.bottom;
	cursor.add_margin(used.margin.bottom);

	cursor.x = old_x;
}

bool CL_CSSLayoutTreeNode::add_margin_top(CL_CSSLayoutCursor &cursor)
{
	if (element_node->computed_properties.float_box.type != CL_CSSBoxFloat::type_none ||
		element_node->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
		element_node->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
	{
		return false;
	}
	else
	{
		cursor.add_margin(used.margin.top);
		if (margin_collapses())
		{
			if (add_content_margin_top(cursor))
				return true;

			if (used.border.bottom != 0 || used.padding.bottom != 0)
				return true;

			cursor.add_margin(used.margin.bottom);
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool CL_CSSLayoutTreeNode::margin_collapses()
{
	return used.border.top == 0 && used.padding.top == 0 && used.height == 0;
}

void CL_CSSLayoutTreeNode::set_formatting_context(CL_CSSBlockFormattingContext *new_formatting_context, bool is_root)
{
	if (formatting_context_root)
		delete formatting_context;
	formatting_context = new_formatting_context;
	formatting_context_root = is_root;
}

void CL_CSSLayoutTreeNode::establish_stacking_context_if_needed(CL_CSSStackingContext *current_stacking_context)
{
	if (element_node->computed_properties.position.type != CL_CSSBoxPosition::type_static && element_node->computed_properties.z_index.type != CL_CSSBoxZIndex::type_auto)
	{
		stacking_context = new CL_CSSStackingContext(this);
		stacking_context_root = true;
		stacking_context->push_back(current_stacking_context);
	}
	else
	{
		stacking_context = current_stacking_context;
		stacking_context_root = false;
	}
}

int CL_CSSLayoutTreeNode::get_block_width() const
{
	return (int)(used.margin.left + used.border.left + used.padding.left + used.width + used.padding.right + used.border.right + used.margin.right + 0.5f);
}

int CL_CSSLayoutTreeNode::get_block_height() const
{
	return (int)(used.margin.top + used.border.top + used.padding.top + used.height + used.padding.bottom + used.border.bottom + used.margin.bottom + 0.5f);
}

void CL_CSSLayoutTreeNode::render_non_content(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache)
{
	if (element_node->computed_properties.visibility.type == CL_CSSBoxVisibility::type_visible &&
		element_node->computed_properties.display.type != CL_CSSBoxDisplay::type_table_cell)
	{
		render_background(gc, resource_cache);
		render_border(gc);

		if (element_node->computed_properties.display.type == CL_CSSBoxDisplay::type_list_item &&
			element_node->computed_properties.list_style_type.type != CL_CSSBoxListStyleType::type_none)
		{
			CL_String bullet = CL_StringHelp::wchar_to_utf8(8226);
			CL_Font &font = resource_cache->get_font(gc, element_node->computed_properties);
			CL_Size offset = font.get_text_size(gc, bullet);
			offset.width += 8;
			// to do: find baseline of first item
			font.draw_text(gc, formatting_context->get_x() + content_box.left-offset.width, formatting_context->get_y() + content_box.top + (int)(font.get_font_metrics(gc).get_ascent()), bullet, element_node->computed_properties.color.color);
		}
	}
}

CL_Rect CL_CSSLayoutTreeNode::get_border_box() const
{
	CL_Rect border_rect = content_box;
	if (!formatting_context_root)
		border_rect.translate(formatting_context->get_x(), formatting_context->get_y());
	else if (formatting_context->get_parent())
		border_rect.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
	border_rect.expand(used.padding.left, used.padding.top, used.padding.right, used.padding.bottom);
	border_rect.expand(used.border.left, used.border.top, used.border.right, used.border.bottom);
	return border_rect;
}

void CL_CSSLayoutTreeNode::render_background(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache)
{
	CL_Rect border_box = get_border_box();
	if (element_node->computed_properties.background_color.type == CL_CSSBoxBackgroundColor::type_color)
		CL_Draw::fill(gc, border_box, element_node->computed_properties.background_color.color);

	if (element_node->computed_properties.background_image.type == CL_CSSBoxBackgroundImage::type_uri)
	{
		CL_Image &image = resource_cache->get_image(gc, element_node->computed_properties.background_image.url);
		if (!image.is_null())
		{
			if (element_node->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_repeat)
			{
				image.draw(gc, border_box);
			}
			else if (element_node->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_clan_stretch)
			{
				int sizing_left = (int)(element_node->computed_properties.clan_background_border_left.length.value+0.5f);
				int sizing_top = (int)(element_node->computed_properties.clan_background_border_top.length.value+0.5f);
				int sizing_right = (int)(element_node->computed_properties.clan_background_border_right.length.value+0.5f);
				int sizing_bottom = (int)(element_node->computed_properties.clan_background_border_bottom.length.value+0.5f);
				CL_ClanImageStretch::draw_image(gc, border_box, image, sizing_left, sizing_top, sizing_right, sizing_bottom);
			}
		}
	}
}

void CL_CSSLayoutTreeNode::render_border(CL_GraphicContext &gc)
{
	CL_Rect border_box = get_border_box();
	if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)border_box.top,
			(float)border_box.right,
			(float)(border_box.top+used.border.top),
			element_node->computed_properties.border_color_top.color);
	}
	if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)(border_box.bottom-used.border.bottom),
			(float)border_box.right,
			(float)border_box.bottom,
			element_node->computed_properties.border_color_bottom.color);
	}
	if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)(border_box.top+used.border.top),
			(float)(border_box.left+used.border.left),
			(float)(border_box.bottom-used.border.bottom),
			element_node->computed_properties.border_color_left.color);
	}
	if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)(border_box.right-used.border.right),
			(float)(border_box.top+used.border.top),
			(float)border_box.right,
			(float)(border_box.bottom-used.border.bottom),
			element_node->computed_properties.border_color_right.color);
	}
}
