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
#include "css_layout_tree.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_object.h"
#include "../BoxTree/css_box_text.h"
#include "../BoxTree/css_box_node_walker.h"
#include "css_layout_tree_node.h"
#include "InlineLayout/css_inline_layout.h"
#include "css_replaced_layout.h"
#include "css_table_layout.h"
#include "css_layout_cursor.h"
#include "css_stacking_context.h"
#include "css_layout_hit_test_result.h"

namespace clan
{

CSSLayoutTree::CSSLayoutTree()
: root_layout(0), root_stacking_context(0)
{
}

CSSLayoutTree::~CSSLayoutTree()
{
	clear();
}

void CSSLayoutTree::clear()
{
	delete root_stacking_context;
	root_stacking_context = 0;
	delete root_layout;
	root_layout = 0;
}

void CSSLayoutTree::create(CSSBoxElement *element)
{
	clear();
	root_layout = create_layout(element);
}

void CSSLayoutTree::layout(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Size &viewport)
{
	delete root_stacking_context;
	root_stacking_context = new CSSStackingContext(root_layout);

	root_layout->containing_width.value = viewport.width;
	root_layout->containing_height.value = viewport.height;
	root_layout->containing_width.expanding = false;
	root_layout->containing_height.use_content = false;

	root_layout->prepare(0, root_stacking_context);
	root_layout->calculate_top_down_widths(CSSLayoutTreeNode::normal_strategy);
	root_layout->calculate_top_down_heights();
	root_layout->layout_formatting_root_helper(graphics, resource_cache, CSSLayoutTreeNode::normal_strategy);
	root_layout->set_root_block_position(0, 0);
	root_layout->layout_absolute_and_fixed_content(graphics, resource_cache, viewport, viewport);
	root_layout->set_component_geometry();

	root_stacking_context->sort();
}

void CSSLayoutTree::render(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache)
{
	root_stacking_context->render(graphics, resource_cache, true);
}

CSSLayoutHitTestResult CSSLayoutTree::hit_test(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Point &pos)
{
	return root_layout->hit_test(graphics, resource_cache, pos);
}

Rect CSSLayoutTree::get_cursor_box(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSBoxText *text_node, std::string::size_type pos)
{
	Rect box;
	root_layout->get_cursor_box(graphics, resources, text_node, pos, box);
	return box;
}

Rect CSSLayoutTree::get_content_box(CSSBoxElement *element)
{
	Rect rect;
	root_layout->find_content_box(element, rect);
	return rect;
}

CSSLayoutTreeNode *CSSLayoutTree::create_layout(CSSBoxElement *element)
{
	if (element->is_block_level() || element->is_inline_block_level())
	{
		if (dynamic_cast<CSSBoxObject*>(element))
			return create_replaced_level_layout(dynamic_cast<CSSBoxObject*>(element));
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

CSSTableLayout *CSSLayoutTree::create_table_level_layout(CSSBoxElement *element)
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

	CSSTableLayout *table = new CSSTableLayout(element);
	// bool in_table_row = false;

	CSSBoxNodeWalker walker(element->get_first_child(), false);
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
				CSSLayoutTreeNode *cell_layout = create_layout(walker.get_element());
				if (cell_layout)
					table->add_cell(cell_layout, walker.get_element()->col_span, walker.get_element()->row_span);
				walker.next(false);
			}
			else if (walker.get_element()->is_table_caption())
			{
				CSSLayoutTreeNode *caption_layout = create_layout(walker.get_element());
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

CSSReplacedLayout *CSSLayoutTree::create_replaced_level_layout(CSSBoxObject *object)
{
	return new CSSReplacedLayout(object);
}

CSSInlineLayout *CSSLayoutTree::create_inline_level_layout(CSSBoxElement *element)
{
	CSSInlineLayout *inline_layout = new CSSInlineLayout(element);

	CSSBoxNode *cur = element->get_first_child();
	while (cur)
	{
		CSSInlineGeneratedBox *generated_box = create_inline_generated_box(cur);
		if (generated_box)
			inline_layout->add_box(generated_box);
		cur = cur->get_next_sibling();
	}

	return inline_layout;
}

CSSInlineGeneratedBox *CSSLayoutTree::create_inline_generated_box(CSSBoxNode *cur)
{
	CSSBoxElement *element = dynamic_cast<CSSBoxElement*>(cur);
	if (element && element->is_display_none())
	{
		return 0;
	}
	else
	{
		CSSInlineGeneratedBox *generated_box = new CSSInlineGeneratedBox();
		generated_box->box_node = cur;

		CSSBoxText *text = dynamic_cast<CSSBoxText*>(cur);
		if (text)
		{
			generated_box->text_start = 0;
			generated_box->text_end = text->processed_text.length();
		}

		CSSBoxElement *cur_element = dynamic_cast<CSSBoxElement*>(cur);
		if (dynamic_cast<CSSBoxObject*>(cur))
		{
			generated_box->layout_node = create_replaced_level_layout(dynamic_cast<CSSBoxObject*>(element));
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
				CSSInlineGeneratedBox *child_generated_box = create_inline_generated_box(cur);
				if (child_generated_box)
					generated_box->add_box(child_generated_box);
				cur = cur->get_next_sibling();
			}
		}

		return generated_box;
	}
}

}
