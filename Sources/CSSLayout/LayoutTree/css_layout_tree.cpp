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
#include "css_layout_tree.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_object.h"
#include "../BoxTree/css_box_text.h"
#include "../BoxTree/css_box_node_walker.h"
#include "css_layout_tree_node.h"
#include "css_block_layout.h"
#include "css_inline_layout.h"
#include "css_inline_object.h"
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
	root_layout = create_block_level_layout(element);
}

void CL_CSSLayoutTree::layout(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache, const CL_Size &viewport)
{
	delete root_stacking_context;
	root_stacking_context = new CL_CSSStackingContext(root_layout);

	root_layout->used.containing.width = viewport.width;
	root_layout->used.containing.height = viewport.height;
	root_layout->used.containing.undetermined_width = false;
	root_layout->used.containing.undetermined_height = false;

	CL_CSSLayoutCursor cursor;
	cursor.resources = resource_cache;
	root_layout->prepare(0, root_stacking_context);
	root_layout->layout_formatting_root(gc, cursor);
	root_layout->set_root_block_position(0, 0);
	root_layout->set_component_geometry();

	root_stacking_context->sort();
}

void CL_CSSLayoutTree::render(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache)
{
	root_stacking_context->render(gc, resource_cache);
}

CL_CSSInlineLayout *CL_CSSLayoutTree::find_inline_layout(CL_CSSBoxText *text_node)
{
	return root_layout->find_inline_layout(text_node);
}

CL_CSSLayoutHitTestResult CL_CSSLayoutTree::hit_test(const CL_Point &pos)
{
	return root_layout->hit_test(pos);
}

CL_Rect CL_CSSLayoutTree::get_cursor_box(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSBoxText *text_node, CL_String::size_type pos)
{
	CL_CSSInlineLayout *inline_layout = root_layout->find_inline_layout(text_node);
	if (inline_layout)
	{
		return inline_layout->get_cursor_box(gc, resources, text_node, pos);
	}
	else
	{
		return CL_Rect();
	}
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
		else if (element->has_block_level_children())
			return create_block_level_layout(element);
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
	CL_CSSTableLayout *table = new CL_CSSTableLayout(element);

	CL_CSSBoxNode *cur = element->get_first_child();
	while (cur)
	{
		CL_CSSBoxElement *child_element = dynamic_cast<CL_CSSBoxElement*>(cur);
		if (child_element && child_element->is_table_row())
		{
			table->add_row(child_element);
			CL_CSSBoxNode *cur_cell = cur->get_first_child();
			while (cur_cell)
			{
				CL_CSSBoxElement *cell_element = dynamic_cast<CL_CSSBoxElement*>(cur_cell);
				if (cell_element && cell_element->is_table_cell())
				{
					CL_CSSLayoutTreeNode *cell_layout = create_layout(cell_element);
					if (cell_layout)
						table->add_cell(cell_layout);
					cur_cell = cur_cell->get_next_sibling();
				}
			}
			cur = cur->get_next_sibling();
		}
	}

	return table;
}

CL_CSSReplacedLayout *CL_CSSLayoutTree::create_replaced_level_layout(CL_CSSBoxObject *object)
{
	return new CL_CSSReplacedLayout(object);
}

CL_CSSBlockLayout *CL_CSSLayoutTree::create_block_level_layout(CL_CSSBoxElement *element)
{
	CL_CSSBlockLayout *layout = new CL_CSSBlockLayout(element);
	CL_CSSBoxNode *cur = element->get_first_child();
	while (cur)
	{
		CL_CSSBoxElement *child_element = dynamic_cast<CL_CSSBoxElement*>(cur);
		if (child_element)
		{
			CL_CSSLayoutTreeNode *child_layout = create_layout(child_element);
			if (child_layout)
				layout->children.push_back(child_layout);
		}
		cur = cur->get_next_sibling();
	}
	return layout;
}

CL_CSSInlineLayout *CL_CSSLayoutTree::create_inline_level_layout(CL_CSSBoxElement *element)
{
	CL_CSSInlineLayout *inline_layout = new CL_CSSInlineLayout(element);
	CL_CSSBoxNodeWalker cur_node(element->get_first_child());
	while (cur_node.is_node())
	{
		if (cur_node.is_element())
		{
			if (cur_node.get_element()->is_block_level() || cur_node.get_element()->is_inline_block_level())
			{
				inline_layout->push_back(cur_node.get(), create_layout(cur_node.get_element()));
				cur_node.next(false);
			}
			else
			{
				cur_node.next(true);
			}
		}
		else if (cur_node.is_text())
		{
			inline_layout->push_back(cur_node.get());
			cur_node.next(true);
		}
	}
	return inline_layout;
}
