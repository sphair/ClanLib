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
#include "css_block_layout.h"
#include "css_block_formatting_context.h"
#include "css_layout_cursor.h"
#include "css_inline_layout.h"
#include "css_stacking_context.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_text.h"
#include "../css_resource_cache.h"

CL_CSSBlockLayout::CL_CSSBlockLayout(CL_CSSBoxElement *element_node)
: CL_CSSLayoutTreeNode(element_node)
{
}

CL_CSSBlockLayout::~CL_CSSBlockLayout()
{
	for (size_t i = 0; i < children.size(); i++)
		delete children[i];
}

void CL_CSSBlockLayout::render(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache)
{
	render_non_content(gc, resource_cache);

	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_static ||
			children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_relative)
		{
			if (stacking_context == children[i]->get_stacking_context())
				children[i]->render(gc, resource_cache);
		}
	}
}

void CL_CSSBlockLayout::calculate_top_down_sizes()
{
	CL_CSSLayoutTreeNode::calculate_top_down_sizes();
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->used.containing.width = used.width;
		children[i]->used.containing.height = used.height;
		children[i]->used.containing.undetermined_width = used.undetermined_width;
		children[i]->used.containing.undetermined_height = used.undetermined_height;
		children[i]->calculate_top_down_sizes();
	}
}

void CL_CSSBlockLayout::set_auto_width(CL_CSSUsedValue width)
{
	CL_CSSLayoutTreeNode::set_auto_width(width);
	for (size_t i = 0; i < children.size(); i++)
	{
		CL_CSSUsedValue spacing = children[i]->used.margin.left + children[i]->used.border.left + children[i]->used.padding.left + children[i]->used.padding.right + children[i]->used.border.right + children[i]->used.margin.right;
		children[i]->set_auto_width(cl_max(0.0f, width-spacing));
	}
}

void CL_CSSBlockLayout::layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
			children[i]->get_element_node()->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
		{
			// To do: layout absolute and fixed elements later since they may rely on the calculated height of the normal flow.
		}
		else
		{
			if (children[i]->get_element_node()->is_float() ||
				children[i]->get_element_node()->is_table() ||
				children[i]->is_replaced())
			{
				if (children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_left || children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_both)
				{
					int clear_left = formatting_context->find_left_clearance();
					if (cursor.y+cursor.margin_y < clear_left)
						cursor.y = clear_left-cursor.margin_y;
				}
				if (children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_right || children[i]->get_element_node()->computed_properties.clear.type == CL_CSSBoxClear::type_both)
				{
					int clear_right = formatting_context->find_right_clearance();
					if (cursor.y+cursor.margin_y < clear_right)
						cursor.y = clear_right-cursor.margin_y;
				}


				children[i]->layout_formatting_root(gc, cursor, strategy);

				CL_Rect float_box(0, 0, children[i]->get_block_width(), children[i]->get_block_height());
				float_box.translate(cursor.x, cursor.y+cursor.margin_y);
				/*if (strategy == preferred_strategy && used.undetermined_width)
				{
					if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_left)
					{
						float_box = formatting_context->float_left(float_box, cursor.x+1000000, this);
					}
					else if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_right)
					{
						// This is wrong.. the width must be dynamically expanded as needed
						float_box.translate(used.width-float_box.get_width(), 0);
						float_box = formatting_context->float_right(float_box, cursor.x+used.width, this);
					}
					else
					{
						float_box = formatting_context->place_left(float_box, cursor.x+1000000);
						cursor.apply_margin();
						cursor.y = float_box.bottom;
					}
				}
				else*/
				{
					if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_left)
					{
						float_box = formatting_context->float_left(float_box, cursor.x+used.width, this);
					}
					else if (children[i]->get_element_node()->computed_properties.float_box.type == CL_CSSBoxFloat::type_right)
					{
						float_box.translate(used.width-float_box.get_width(), 0);
						float_box = formatting_context->float_right(float_box, cursor.x+used.width, this);
					}
					else
					{
						cursor.apply_margin();
						float_box = formatting_context->place_left(float_box, cursor.x+used.width);
						cursor.y = float_box.bottom;
					}
				}

				cursor.apply_written_width(float_box.right);
				children[i]->set_root_block_position(float_box.left+children[i]->used.offset.x, float_box.top+children[i]->used.offset.y);
			}
			else
			{
				children[i]->layout_normal(gc, cursor, strategy);
			}
		}
	}
}

bool CL_CSSBlockLayout::add_content_margin_top(CL_CSSLayoutCursor &cursor)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->add_margin_top(cursor))
			return true;
	}
	return false;
}

void CL_CSSBlockLayout::set_component_geometry()
{
	for (size_t i = 0; i < children.size(); i++)
		children[i]->set_component_geometry();
}

int CL_CSSBlockLayout::get_first_line_baseline()
{
	int first_line_baseline = -1;
	for (size_t i = 0; i < children.size(); i++)
	{
		int baseline = children[i]->get_first_line_baseline();
		if (first_line_baseline == -1)
			first_line_baseline = baseline;
		else if (baseline != -1)
			first_line_baseline = cl_min(first_line_baseline, baseline);
	}
	if (first_line_baseline != -1 && formatting_context_root)
		first_line_baseline += formatting_context->get_local_y();
	return first_line_baseline;
}

int CL_CSSBlockLayout::get_last_line_baseline()
{
	int last_line_baseline = -1;
	for (size_t i = 0; i < children.size(); i++)
	{
		int baseline = children[i]->get_last_line_baseline();
		if (last_line_baseline == -1)
			last_line_baseline = baseline;
		else if (baseline != -1)
			last_line_baseline = cl_max(last_line_baseline, baseline);
	}
	if (last_line_baseline != -1 && formatting_context_root)
		last_line_baseline += formatting_context->get_local_y();
	return last_line_baseline;
}

CL_CSSInlineLayout *CL_CSSBlockLayout::find_inline_layout(CL_CSSBoxText *text_node)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		CL_CSSBlockLayout *block_child = dynamic_cast<CL_CSSBlockLayout*>(children[i]);
		CL_CSSInlineLayout *inline_child = dynamic_cast<CL_CSSInlineLayout*>(children[i]);
		if (block_child)
		{
			CL_CSSInlineLayout *inline_layout = block_child->find_inline_layout(text_node);
			if (inline_layout)
				return inline_layout;
		}
		else if (inline_child)
		{
			if (inline_child->contains_node(text_node))
				return inline_child;
		}
	}
	return 0;
}

CL_CSSLayoutHitTestResult CL_CSSBlockLayout::hit_test(const CL_Point &pos) const
{
	for (size_t i = 0; i < children.size(); i++)
	{
		CL_CSSLayoutHitTestResult result = children[i]->hit_test(pos);
		if (result.node)
			return result;
	}
	return CL_CSSLayoutHitTestResult();
}

void CL_CSSBlockLayout::prepare_children()
{
	for (size_t i = 0; i < children.size(); i++)
		children[i]->prepare(formatting_context, stacking_context);
}

bool CL_CSSBlockLayout::find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->get_element_node() == element)
		{
			CL_Rect rect = children[i]->content_box;
			rect.translate(formatting_context->get_x(), formatting_context->get_y());
			out_rect = rect;
			return true;
		}
		else
		{
			if (children[i]->find_content_box(element, out_rect))
				return true;
		}
	}
	return false;
}
