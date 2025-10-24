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
#include "css_box_tree.h"
#include "css_box_element.h"
#include "css_box_text.h"
#include "css_whitespace_eraser.h"
#include "API/CSSLayout/css_property2.h"
#include "API/CSSLayout/css_property_list2.h"

CL_CSSBoxTree::CL_CSSBoxTree()
: root_element(0), selection_start(0), selection_end(0), selection_start_text_offset(0), selection_end_text_offset(0)
{
}

CL_CSSBoxTree::~CL_CSSBoxTree()
{
	clear();
}

void CL_CSSBoxTree::set_root_element(CL_CSSBoxElement *new_root_element)
{
	clear();
	root_element = new_root_element;
}

void CL_CSSBoxTree::create(const CL_DomNode &node)
{
	clear();
	CL_CSSBoxNode *root_node = create_node(node);
	root_element = dynamic_cast<CL_CSSBoxElement*>(root_node);
	if (!root_element)
	{
		delete root_node;
		throw CL_Exception("XML contained no document element!");
	}
}

void CL_CSSBoxTree::prepare(CL_CSSResourceCache *resource_cache)
{
	clean();
	compute_element(root_element, resource_cache);
	convert_run_in_blocks(root_element);
	CL_CSSWhitespaceEraser::remove_whitespace(root_element);
	create_anonymous_blocks(root_element, resource_cache);
}

void CL_CSSBoxTree::clean(CL_CSSBoxNode *node)
{
	if (node == 0)
		node = root_element;

	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(node);
	if (text)
		text->processed_text = text->text;
	
	CL_CSSBoxNode *child = node->get_first_child();
	while (child)
	{
		clean(child);
		child = child->get_next_sibling();
	}
}

void CL_CSSBoxTree::clear()
{
	delete root_element;
	root_element = 0;
}

CL_CSSBoxNode *CL_CSSBoxTree::create_node(const CL_DomNode &node)
{
	if (node.is_element())
	{
		CL_CSSBoxElement *box_element = new CL_CSSBoxElement();
		box_element->name = node.get_node_name();
		box_element->properties = get_css_properties(node.to_element());
		create_pseudo_element(box_element, node.to_element(), "before");
		CL_DomNode cur = node.get_first_child();
		while (!cur.is_null())
		{
			CL_CSSBoxNode *box_child = create_node(cur);
			if (box_child)
				box_element->push_back(box_child);
			cur = cur.get_next_sibling();
		}
		create_pseudo_element(box_element, node.to_element(), "after");
		return box_element;
	}
	else if (node.is_text())
	{
		CL_CSSBoxText *box_text = new CL_CSSBoxText();
		box_text->set_text(node.get_node_value());
		return box_text;
	}
	else
	{
		return 0;
	}
}

void CL_CSSBoxTree::create_pseudo_element(CL_CSSBoxElement *box_element, const CL_DomElement &dom_element, const CL_String &pseudo_element)
{
	CL_CSSBoxProperties properties = get_css_properties(dom_element, pseudo_element);
	if (properties.content.type != CL_CSSBoxContent::type_none && properties.content.type != CL_CSSBoxContent::type_normal)
	{
		CL_CSSBoxElement *before_element = new CL_CSSBoxElement();
		before_element->name = cl_format("%1:%2", dom_element.get_node_name(), pseudo_element);
		before_element->properties = properties;

		CL_CSSBoxText *box_text = new CL_CSSBoxText();
		box_text->set_text(properties.content.str);

		before_element->push_back(box_text);
		box_element->push_back(before_element);
	}
}

void CL_CSSBoxTree::apply_properties(CL_CSSBoxElement *node, const CL_CSSPropertyList2 &css_properties)
{
	for (size_t i = css_properties.size(); i > 0; i--)
		property_parsers.parse(node->properties, css_properties[i-1]);
}

CL_CSSBoxProperties CL_CSSBoxTree::get_css_properties(const CL_DomElement &element, const CL_String &pseudo_element)
{
	CL_CSSBoxProperties properties;
	CL_CSSPropertyList2 css_properties = css.select(element, pseudo_element);
	for (size_t i = css_properties.size(); i > 0; i--)
		property_parsers.parse(properties, css_properties[i-1]);
	return properties;
}

void CL_CSSBoxTree::compute_element(CL_CSSBoxElement *element, CL_CSSResourceCache *resource_cache)
{
	CL_CSSBoxProperties *parent_properties = 0;
	CL_CSSBoxNode *parent_node = element->get_parent();
	if (parent_node)
		parent_properties = &dynamic_cast<CL_CSSBoxElement*>(parent_node)->computed_properties;

	element->computed_properties = element->properties;
	element->computed_properties.compute(parent_properties, resource_cache);

	CL_CSSBoxNode *cur = element->get_first_child();
	while (cur)
	{
		CL_CSSBoxElement *cur_element = dynamic_cast<CL_CSSBoxElement*>(cur);
		if (cur_element)
			compute_element(cur_element, resource_cache);
		cur = cur->get_next_sibling();
	}
}

void CL_CSSBoxTree::create_anonymous_blocks(CL_CSSBoxElement *element, CL_CSSResourceCache *resource_cache)
{
	if (element->is_table() || element->is_inline_table())
	{
		filter_table(element, resource_cache);
	}
	else if (element->is_block_level() && element->has_block_level_children())
	{
		CL_CSSBoxNode *cur = element->get_first_child();
		CL_CSSBoxElement *anonymous = 0;
		while (cur)
		{
			CL_CSSBoxElement *child_element = dynamic_cast<CL_CSSBoxElement*>(cur);
			CL_CSSBoxText *child_text = dynamic_cast<CL_CSSBoxText*>(cur);
			if (child_text)
			{
				if (!anonymous)
				{
					anonymous = new CL_CSSBoxElement();
					anonymous->properties.display.type = CL_CSSBoxDisplay::type_block;
					anonymous->name = "anonymous";
					element->insert(anonymous, cur);
					compute_element(anonymous, resource_cache);
				}
				cur->remove();
				anonymous->push_back(cur);
				cur = anonymous;
			}
			else if (child_element)
			{
				if (child_element->is_block_level())
				{
					anonymous = 0;
					create_anonymous_blocks(child_element, resource_cache);
				}
				else
				{
					if (!anonymous)
					{
						anonymous = new CL_CSSBoxElement();
						anonymous->properties.display.type = CL_CSSBoxDisplay::type_block;
						anonymous->name = "anonymous";
						element->insert(anonymous, cur);
						compute_element(anonymous, resource_cache);
					}
					cur->remove();
					anonymous->push_back(cur);
					cur = anonymous;
				}
			}

			cur = cur->get_next_sibling();
		}
	}
}

void CL_CSSBoxTree::convert_run_in_blocks(CL_CSSBoxElement *element)
{
	// This code is pretty broken.  But then again, who cares?  There's not
	// a single browser out there yet that managed to implement run-in boxes properly,
	// so I doubt they are being used much..

	if (element->computed_properties.display.type == CL_CSSBoxDisplay::type_run_in)
	{
		if (element->has_block_level_children())
		{
			element->properties.display.type = CL_CSSBoxDisplay::type_block;
			element->computed_properties.display.type = CL_CSSBoxDisplay::type_block;
		}
		else
		{
			element->properties.display.type = CL_CSSBoxDisplay::type_block;
			element->computed_properties.display.type = CL_CSSBoxDisplay::type_block;

			CL_CSSBoxNode *next = element->get_next_sibling();
			while (next)
			{
				CL_CSSBoxText *next_text = dynamic_cast<CL_CSSBoxText*>(next);
				CL_CSSBoxElement *next_element = dynamic_cast<CL_CSSBoxElement*>(next);
				if (next_text)
				{
					if (next_text->processed_text.find_first_not_of(" \t\r\n") != CL_String::npos)
						break;
				}
				else if (next_element)
				{
					if (next_element->computed_properties.float_box.type != CL_CSSBoxFloat::type_none ||
						next_element->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
						next_element->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
					{
						// Skip floating and absolute positioned boxes
					}
					else if (next_element->computed_properties.display.type == CL_CSSBoxDisplay::type_block)
					{
						element->properties.display.type = CL_CSSBoxDisplay::type_inline;
						element->computed_properties.display.type = CL_CSSBoxDisplay::type_inline;
						element->remove();
						next_element->push_front(element);
						break;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				next = next->get_next_sibling();
			}
		}
	}
	else
	{
		CL_CSSBoxNode *cur = element->get_first_child();
		while (cur)
		{
			CL_CSSBoxElement *cur_element = dynamic_cast<CL_CSSBoxElement*>(cur);
			if (cur_element)
				convert_run_in_blocks(cur_element);
			cur = cur->get_next_sibling();
		}
	}
}

void CL_CSSBoxTree::filter_table(CL_CSSBoxElement *element, CL_CSSResourceCache *resource_cache)
{
	CL_CSSBoxNode *cur = element->get_first_child();
	while (cur)
	{
		CL_CSSBoxElement *child_element = dynamic_cast<CL_CSSBoxElement*>(cur);
		if (child_element && child_element->is_table_row())
		{
			CL_CSSBoxNode *cur_cell = cur->get_first_child();
			while (cur_cell)
			{
				CL_CSSBoxElement *cell_element = dynamic_cast<CL_CSSBoxElement*>(cur_cell);
				if (cell_element && cell_element->is_table_cell())
				{
					create_anonymous_blocks(cell_element, resource_cache);
					cur_cell = cur_cell->get_next_sibling();
				}
				else
				{
					CL_CSSBoxNode *d = cur_cell;
					cur_cell = cur_cell->get_next_sibling();
					d->remove();
				}
			}
			cur = cur->get_next_sibling();
		}
		else
		{
			CL_CSSBoxNode *d = cur;
			cur = cur->get_next_sibling();
			d->remove();
		}
	}
}

void CL_CSSBoxTree::set_selection(CL_CSSBoxNode *start, size_t start_text_offset, CL_CSSBoxNode *end, size_t end_text_offset)
{
	apply_selection(selection_start, selection_start, selection_start_text_offset, selection_end, selection_end_text_offset, true);

	selection_start = start;
	selection_start_text_offset = start_text_offset;
	selection_end = end;
	selection_end_text_offset = end_text_offset;

	apply_selection(selection_start, selection_start, selection_start_text_offset, selection_end, selection_end_text_offset, false);
}

void CL_CSSBoxTree::apply_selection(CL_CSSBoxNode *node, CL_CSSBoxNode *start, size_t start_offset, CL_CSSBoxNode *end, size_t end_offset, bool clear, bool in_selection)
{
	if (node == start)
		in_selection = true;

	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(node);
	if (text)
	{
		if (clear)
		{
			text->selection_start = 0;
			text->processed_selection_start = 0;
			text->selection_end = 0;
			text->processed_selection_end = 0;
		}
		else if (in_selection)
		{
			text->selection_start = 0;
			text->processed_selection_start = 0;
			text->selection_end = text->text.length();
			text->processed_selection_end = text->processed_text.length();

			if (text == start)
			{
				text->selection_start = start_offset;
				text->processed_selection_start = start_offset;
			}
			else if (text == end)
			{
				text->selection_end = end_offset;
				text->processed_selection_end = end_offset;
			}
		}
	}

	CL_CSSBoxNode *cur = node->get_first_child();
	while (cur)
	{
		apply_selection(cur, start, start_offset, end, end_offset, clear, in_selection);
		cur = cur->get_next_sibling();
	}

	if (node == end)
		in_selection = false;
}
