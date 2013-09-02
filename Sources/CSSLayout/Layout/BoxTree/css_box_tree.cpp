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
#include "css_box_tree.h"
#include "css_box_element.h"
#include "css_box_text.h"
#include "css_box_node_walker.h"
#include "css_box_select_node.h"
#include "API/CSSLayout/CSSDocument/css_property.h"
#include "API/CSSLayout/CSSDocument/css_property_value.h"

namespace clan
{

CSSBoxTree::CSSBoxTree()
: root_element(0)
{
}

CSSBoxTree::~CSSBoxTree()
{
	clear();
}

void CSSBoxTree::set_root_element(CSSBoxElement *new_root_element)
{
	clear();
	root_element = new_root_element;
}

void CSSBoxTree::prepare(CSSResourceCache *resource_cache)
{
	compute(resource_cache);
}

void CSSBoxTree::compute(CSSResourceCache *cache)
{
	bool collapse_space = false;
	compute(cache, 0, collapse_space);
}

void CSSBoxTree::compute(CSSResourceCache *cache, CSSBoxNode *node, bool &collapse_space)
{
	if (node == 0)
		node = root_element;

	CSSBoxElement *element = dynamic_cast<CSSBoxElement *>(node);
	if (element)
	{
		element->computed_values = CSSComputedValues(cache);

		if (element->get_parent())
			element->computed_values.set_parent(static_cast<CSSBoxElement*>(element->get_parent())->computed_values);

		CSSBoxSelectNode select_node(element);
		CSSSelectResult select_result = css.select(&select_node);
		element->computed_values.set_specified_values(select_result);
	}

	CSSBoxText *text = dynamic_cast<CSSBoxText*>(node);
	if (text)
	{
		const CSSComputedTextInherit &text_properties = text->get_properties().get_text_inherit();

		bool normal = text_properties.white_space.type == CSSValueWhiteSpace::type_normal;
		bool pre_line = text_properties.white_space.type == CSSValueWhiteSpace::type_pre_line;
		bool nowrap = text_properties.white_space.type == CSSValueWhiteSpace::type_nowrap;
		//bool pre = text_properties.white_space.type == CSSValueWhiteSpace::type_pre;
		//bool pre_wrap = text_properties.white_space.type == CSSValueWhiteSpace::type_pre_wrap;

		text->processed_text = normalize_newlines(text->text);

		if (normal || nowrap || pre_line)
			text->processed_text = remove_whitespace_around_linefeed(text->processed_text); // step #1

		//if (pre || prewrap)
		//	treat_spaces_as_nbsp(text->processed_text); // step #2
		//if (prewrap)
		//	add_linebreak_opportunity(text.length());

		if (normal || nowrap)
			text->processed_text =convert_linefeed_to_space(text->processed_text); // step #3

		if (normal || nowrap || pre_line)
		{
			bool prev_ended_with_space = false;
			bool prev_normal = false;
			bool prev_nowrap = false;
			bool prev_preline = false;

			text->processed_text = convert_tab_to_space(text->processed_text); // step #4.1
			text->processed_text = collapse_spaces(text->processed_text, collapse_space);

			text->processed_text_collapse_next = collapse_space;
		}
		else
		{
			text->processed_text_collapse_next = false;
		}
	}

	CSSBoxNode *child = node->get_first_child();
	while (child)
	{
		compute(cache, child, collapse_space);
		child = child->get_next_sibling();
	}
}

std::string CSSBoxTree::normalize_newlines(const std::string &text)
{
	// "The CSS 'white-space' processing model assumes all newlines have been normalized to line feeds."

	std::string result;
	result.reserve(text.length());

	size_t length = text.length();
	for (size_t i = 0; i < length; i++)
	{
		switch (text[i])
		{
		case '\r':
			if (i + 1 != length && text[i + 1] == '\n')
				i++;
			result.push_back('\n');
			break;
		default:
			result.push_back(text[i]);
			break;
		}
	}

	return result;
}

std::string CSSBoxTree::remove_whitespace_around_linefeed(const std::string &text)
{
	// "Each tab (U+0009), carriage return (U+000D), or space (U+0020) character surrounding a linefeed (U+000A) character is removed"

	std::string result;
	result.reserve(text.length());

	bool removing = false;
	size_t length = text.length();
	for (size_t i = 0; i < length; i++)
	{
		if (!removing)
		{
			switch (text[i])
			{
			case '\n':
				while (!result.empty() && (result.back() == ' ' || result.back() == '\t'))
					result.pop_back();
				result.push_back('\n');
				removing = true;
				break;

			default:
				result.push_back(text[i]);
			}
		}
		else
		{
			switch (text[i])
			{
			case ' ':
			case '\t':
				break;
			case '\n':
				result.push_back('\n');
				break;
			default:
				result.push_back(text[i]);
				removing = false;
			}
		}
	}

	return result;
}

std::string CSSBoxTree::convert_linefeed_to_space(const std::string &text)
{
	// "linefeed characters are transformed for rendering purpose into one of the following characters:
	// a space character, a zero width space character (U+200B), or no character (i.e., not rendered),
	// according to UA-specific algorithms based on the content script"

	// our UA-specific algorithm is to always convert it to a space character. :)

	std::string result = text;
	for (size_t i = 0; i < result.length(); i++)
		if (result[i] == '\n')
			result[i] = ' ';
	return result;
}

std::string CSSBoxTree::convert_tab_to_space(const std::string &text)
{
	std::string result = text;
	for (size_t i = 0; i < result.length(); i++)
		if (result[i] == '\t')
			result[i] = ' ';
	return result;
}

std::string CSSBoxTree::collapse_spaces(const std::string &text, bool &collapsing)
{
	std::string result;
	result.reserve(text.length());

	size_t length = text.length();
	for (size_t i = 0; i < length; i++)
	{
		bool is_space = text[i] == ' ';
		if (!is_space || !collapsing)
			result.push_back(text[i]);
		collapsing = is_space;
	}

	return result;
}

void CSSBoxTree::clear()
{
	delete root_element;
	root_element = 0;
}

#ifdef NEEDS_PORTING

void CSSBoxTree::filter_table(CSSResourceCache *resource_cache)
{
/*
	// Remove irrelevant boxes:
	if (parent.display.type == CSSValueDisplay::type_table_column)
	{
		cur.display.type = CSSValueDisplay::type_none;
	}
	else if (parent.display.type == CSSValueDisplay::type_table_column_group &&
		cur.display.type != CSSValueDisplay::type_table_column)
	{
		cur.display.type = CSSValueDisplay::type_none;
	}
	else if (cur.is_text() &&
		cur.only_contains_whitespace() &&
		cur.prev_sibling().is_null_or_proper_table_descendant() &&
		cur.next_sibling().is_null_or_proper_table_descendant())
	{
		cur.display.type = CSSValueDisplay::type_none;
	}
	// To do: If a box B is an anonymous inline containing only white space, and is between two immediate
	// siblings each of which is either an internal table box or a 'table-caption' box then B is treated
	// as if it had 'display: none'. 
*/

	// Generate missing child wrappers:

	CSSBoxNodeWalker walker(root_element->get_first_child(), true);
	while (walker.is_node())
	{
		if (walker.is_element())
		{
			CSSBoxElement *cur = walker.get_element();
			CSSBoxElement *parent = static_cast<CSSBoxElement *>(walker.get_element()->get_parent());

			if ((parent->computed_values.get_box().display.type == CSSValueDisplay::type_table ||
				parent->computed_values.get_box().display.type == CSSValueDisplay::type_inline_table) &&
				cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_caption &&
				cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_row_group &&
				cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_header_group &&
				cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_footer_group &&
				cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_row)
			{
				// generate an anonymous 'table-row' box around C and all consecutive siblings of C that are not proper table children

				CSSBoxElement *anonymous = new CSSBoxElement();
				anonymous->properties.display.type = CSSValueDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CSSBoxElement*>(next);
					if (cur &&
						cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_caption &&
						cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_row_group &&
						cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_header_group &&
						cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_footer_group &&
						cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_row)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
			if ((parent->computed_values.get_box().display.type == CSSValueDisplay::type_table_row_group ||
				parent->computed_values.get_box().display.type == CSSValueDisplay::type_table_header_group ||
				parent->computed_values.get_box().display.type == CSSValueDisplay::type_table_footer_group) &&
				cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_row)
			{
				// generate an anonymous 'table-row' box around C and all consecutive siblings of C that are not 'table-row' boxes

				CSSBoxElement *anonymous = new CSSBoxElement();
				anonymous->properties.display.type = CSSValueDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CSSBoxElement*>(next);
					if (cur && cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_row)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
			if (parent->computed_values.get_box().display.type == CSSValueDisplay::type_table_row &&
				cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_cell)
			{
				// generate an anonymous 'table-cell' box around C and all consecutive siblings of C that are not 'table-cell' boxes

				CSSBoxElement *anonymous = new CSSBoxElement();
				anonymous->properties.display.type = CSSValueDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CSSBoxElement*>(next);
					if (cur && cur->computed_values.get_box().display.type != CSSValueDisplay::type_table_cell)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
		}
		walker.next(true);
	}

	// Generate missing parents:

	walker = CSSBoxNodeWalker(root_element->get_first_child(), true);
	while (walker.is_node())
	{
		if (walker.is_element())
		{
			CSSBoxElement *cur = walker.get_element();
			CSSBoxElement *parent = static_cast<CSSBoxElement *>(walker.get_element()->get_parent());

			if (cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_cell && parent->computed_values.get_box().display.type != CSSValueDisplay::type_table_row)
			{
				// generate an anonymous 'table-row' box around C and all consecutive siblings of C that are 'table-cell' boxes

				CSSBoxElement *anonymous = new CSSBoxElement();
				anonymous->properties.display.type = CSSValueDisplay::type_table_row;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CSSBoxElement*>(next);
					if (cur && cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_cell)
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}

			bool table_row_misparented =
				cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_row && 
				parent->computed_values.get_box().display.type != CSSValueDisplay::type_table &&
				parent->computed_values.get_box().display.type != CSSValueDisplay::type_inline_table;

			bool table_column_misparented = 
				cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_column &&
				parent->computed_values.get_box().display.type != CSSValueDisplay::type_table_column_group &&
				parent->computed_values.get_box().display.type != CSSValueDisplay::type_table &&
				parent->computed_values.get_box().display.type != CSSValueDisplay::type_inline_table;

			bool table_group_column_caption_misparented =
				(cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_row_group ||
				cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_header_group ||
				cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_footer_group ||
				cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_column_group ||
				cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_caption) &&
				parent->computed_values.get_box().display.type != CSSValueDisplay::type_table &&
				parent->computed_values.get_box().display.type != CSSValueDisplay::type_inline_table;

			if (table_row_misparented || table_column_misparented || table_group_column_caption_misparented)
			{
				// generate an anonymous 'table' or 'inline-table' box T around C and all consecutive siblings of C that are proper table children
				// If C's parent is an 'inline' box, then T must be an 'inline-table' box; otherwise it must be a 'table' box

				CSSBoxElement *anonymous = new CSSBoxElement();
				anonymous->properties.display.type = parent->is_block_level() ? CSSValueDisplay::type_table : CSSValueDisplay::type_inline_table;
				anonymous->name = "anonymous";
				parent->insert(anonymous, cur);
				cur->remove();
				anonymous->push_back(cur);

				CSSBoxNode *next = anonymous->get_next_sibling();
				while (next)
				{
					cur = dynamic_cast<CSSBoxElement*>(next);
					if (cur &&
						(cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_caption ||
						cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_row_group ||
						cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_header_group ||
						cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_footer_group ||
						cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_row ||
						cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_column ||
						cur->computed_values.get_box().display.type == CSSValueDisplay::type_table_column_group))
					{
						next = next->get_next_sibling();
						cur->remove();
						anonymous->push_back(cur);
					}
					else
					{
						break;
					}
				}

				compute_element(anonymous, resource_cache);
			}
		}
		walker.next(true);
	}
}

#endif

}
