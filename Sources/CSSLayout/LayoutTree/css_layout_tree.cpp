/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "css_layout_tree.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_object.h"
#include "../BoxTree/css_box_text.h"
#include "../BoxTree/css_box_node_walker.h"
#include "css_layout_tree_node.h"
#include "css_inline_layout.h"
#include "css_replaced_layout.h"
#include "css_table_layout.h"
#include "css_layout_cursor.h"
#include "css_stacking_context.h"
#include "css_layout_hit_test_result.h"

CL_CSSLayoutTree::CL_CSSLayoutTree()
: root_layout(0), root_stacking_context(0)
{
}

CL_CSSLayoutTree::~CL_CSSLayoutTree()
{
	clear();
}

void CL_CSSLayoutTree::clear()
{
	delete root_stacking_context;
	root_stacking_context = 0;
	delete root_layout;
	root_layout = 0;
}

void CL_CSSLayoutTree::create(CL_CSSBoxElement *element)
{
	clear();
	root_layout = create_layout(element);
}

void CL_CSSLayoutTree::layout(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, const CL_Size &viewport)
{
	delete root_stacking_context;
	root_stacking_context = new CL_CSSStackingContext(root_layout);

	root_layout->containing_width.value = viewport.width;
	root_layout->containing_height.value = viewport.height;
	root_layout->containing_width.expanding = false;
	root_layout->containing_height.use_content = false;

	root_layout->prepare(0, root_stacking_context);
	root_layout->calculate_top_down_widths(CL_CSSLayoutTreeNode::normal_strategy);
	root_layout->calculate_top_down_heights();
	root_layout->layout_formatting_root_helper(graphics, resource_cache, CL_CSSLayoutTreeNode::normal_strategy);
	root_layout->set_root_block_position(0, 0);
	root_layout->layout_absolute_and_fixed_content(graphics, resource_cache, viewport, viewport);
	root_layout->set_component_geometry();

	root_stacking_context->sort();
}

void CL_CSSLayoutTree::render(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache)
{
	root_stacking_context->render(graphics, resource_cache, true);
}

CL_CSSLayoutHitTestResult CL_CSSLayoutTree::hit_test(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, const CL_Point &pos)
{
	return root_layout->hit_test(graphics, resource_cache, pos);
}

CL_Rect CL_CSSLayoutTree::get_cursor_box(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, CL_CSSBoxText *text_node, CL_String::size_type pos)
{
	CL_Rect box;
	root_layout->get_cursor_box(graphics, resources, text_node, pos, box);
	return box;
}

CL_Rect CL_CSSLayoutTree::get_content_box(CL_CSSBoxElement *element)
{
	CL_Rect rect;
	root_layout->find_content_box(element, rect);
	return rect;
}

CL_CSSLayoutTreeNode *CL_CSSLayoutTree::create_layout(CL_CSSBoxElement *element)
{
	if (element->is_block_level() || element->is_inline_block_level())
	{
		if (dynamic_cast<CL_CSSBoxObject*>(element))
			return create_replaced_level_layout(dynamic_cast<CL_CSSBoxObject*>(element));
		else if (element->is_table() || element->is_inline_table())
			return create_table_level_layout(element);
		//else if (element->has_block_level_children())
		//	return create_block_level_layout(element);
		else
			return create_inline_level_layout(element);
	}
	else
	{
		return 0;
	}
}

CL_CSSTableLayout *CL_CSSLayoutTree::create_table_level_layout(CL_CSSBoxElement *element)
{
	// To do: This code needs to apply the rules of 17.2.1 Anonymous table objects
	/*
		<table>
			<caption>
			</caption>
			<thead>
				<tr>
					<td>Foobar</td>
					<td>Foobar</td>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>Foobar</td>
					<td>Foobar</td>
				</tr>
			</tbody>
			<tfoot>
				<tr>
					<td>Foobar</td>
					<td>Foobar</td>
				</tr>
			</tfoot>
			<colgroup>
				<col/>
				<col/>
				<col/>
			</colgroup>
		</table>
	*/

	CL_CSSTableLayout *table = new CL_CSSTableLayout(element);
	// bool in_table_row = false;

	CL_CSSBoxNodeWalker walker(element->get_first_child(), false);
	while (walker.is_node())
	{
		if (walker.is_element())
		{
			if (walker.get_element()->is_table_row())
			{
				table->add_row(walker.get_element());
				walker.next(true);
			}
			else if (walker.get_element()->is_table_row_group())
			{
				walker.next(true);
			}
			else if (walker.get_element()->is_table_header_group())
			{
				walker.next(true);
			}
			else if (walker.get_element()->is_table_footer_group())
			{
				walker.next(true);
			}
			else if (walker.get_element()->is_table_column())
			{
				// 17.2.1: 1.1 All child boxes of a 'table-column' parent are treated as if they had 'display: none'. 
				walker.next(false);
			}
			else if (walker.get_element()->is_table_column_group())
			{
				// 17.2.1: 1.2 If a child C of a 'table-column-group' parent is not a 'table-column' box, then it is treated as if it had 'display: none'. 
				walker.next(true);
			}
			else if (walker.get_element()->is_table_cell())
			{
				CL_CSSLayoutTreeNode *cell_layout = create_layout(walker.get_element());
				if (cell_layout)
					table->add_cell(cell_layout, walker.get_element()->col_span, walker.get_element()->row_span);
				walker.next(false);
			}
			else if (walker.get_element()->is_table_caption())
			{
				CL_CSSLayoutTreeNode *caption_layout = create_layout(walker.get_element());
				if (caption_layout)
					table->add_caption(caption_layout);
				walker.next(false);
			}
			else
			{
				walker.next(true);
			}
		}
		else
		{
			walker.next(true);
		}
	}

	return table;
}

CL_CSSReplacedLayout *CL_CSSLayoutTree::create_replaced_level_layout(CL_CSSBoxObject *object)
{
	return new CL_CSSReplacedLayout(object);
}

CL_CSSInlineLayout *CL_CSSLayoutTree::create_inline_level_layout(CL_CSSBoxElement *element)
{
	CL_CSSInlineLayout *inline_layout = new CL_CSSInlineLayout(element);

	CL_CSSBoxNode *cur = element->get_first_child();
	while (cur)
	{
		CL_CSSInlineGeneratedBox *generated_box = create_inline_generated_box(cur);
		if (generated_box)
			inline_layout->add_box(generated_box);
		cur = cur->get_next_sibling();
	}

	return inline_layout;
}

CL_CSSInlineGeneratedBox *CL_CSSLayoutTree::create_inline_generated_box(CL_CSSBoxNode *cur)
{
	CL_CSSBoxElement *element = dynamic_cast<CL_CSSBoxElement*>(cur);
	if (element && element->is_display_none())
	{
		return 0;
	}
	else
	{
		CL_CSSInlineGeneratedBox *generated_box = new CL_CSSInlineGeneratedBox();
		generated_box->box_node = cur;

		CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(cur);
		if (text)
		{
			generated_box->text_start = 0;
			generated_box->text_end = text->processed_text.length();
		}

		CL_CSSBoxElement *cur_element = dynamic_cast<CL_CSSBoxElement*>(cur);
		if (dynamic_cast<CL_CSSBoxObject*>(cur))
		{
			generated_box->layout_node = create_replaced_level_layout(dynamic_cast<CL_CSSBoxObject*>(element));
		}
		else if (cur_element && (cur_element->is_block_level() || cur_element->is_inline_block_level()))
		{
			generated_box->layout_node = create_layout(cur_element);
		}
		else
		{
			cur = cur->get_first_child();
			while (cur)
			{
				CL_CSSInlineGeneratedBox *child_generated_box = create_inline_generated_box(cur);
				if (child_generated_box)
					generated_box->add_box(child_generated_box);
				cur = cur->get_next_sibling();
			}
		}

		return generated_box;
	}
}
