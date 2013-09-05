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
#include "API/CSSLayout/HTML/html_parser.h"
#include "API/CSSLayout/HTML/html_token.h"
#include "html_parser_impl.h"

namespace clan
{

HTMLParser::HTMLParser()
	: impl(new HTMLParser_Impl())
{
}

void HTMLParser::append(const HTMLToken &token)
{
	impl->append(token);
}

/////////////////////////////////////////////////////////////////////////////

HTMLParser_Impl::HTMLParser_Impl()
	: insertion_mode(mode_initial), original_insertion_mode(mode_initial), scripting_flag(false), frameset_ok_flag(false)
{
}

void HTMLParser_Impl::append(const HTMLToken &token)
{
	// http://www.w3.org/html/wg/drafts/html/master/syntax.html#parsing

	// The "before html" insertion mode creates the html root element node, which is then added to the stack.

	// In the fragment case, the stack of open elements is initialized to contain an html element that is created as part of that
	// algorithm. (The fragment case skips the "before html" insertion mode.)

	// The scope markers are inserted when entering applet elements, buttons, object elements, marquees, table cells, and table
	// captions, and are used to prevent formatting from "leaking" into applet elements, buttons, object elements, marquees, and tables.

	// To do: §8.2.5 Tree construction
}

void HTMLParser_Impl::reset_insertion_mode()
{
	for (size_t i = open_elements.size(); i > 0; i++)
	{
		CSSLayoutElement node = open_elements[i - 1];

		bool last = false;
		if (i == 1)
		{
			last = true;
			if (!context.is_null())
				node = context;
		}

		std::string node_name = node.get_name();

		if (node_name == "select")
		{
			if (!last)
			{
				CSSLayoutElement ancestor = node;
				while (!ancestor.get_parent().is_null())
				{
					ancestor = ancestor.get_parent().to_element();
					if (ancestor.get_name() == "template")
					{
						break;
					}
					else if (ancestor.get_name() == "table")
					{
						insertion_mode = mode_in_select_in_table;
						return;
					}
				}
				insertion_mode = mode_in_select;
				return;
			}
		}
		else if (!last && (node_name == "td" || node_name == "th"))
		{
			insertion_mode = mode_in_cell;
			return;
		}
		else if (node_name == "tr")
		{
			insertion_mode = mode_in_row;
			return;
		}
		else if (node_name == "tbody" || node_name == "thead" || node_name == "tfoot")
		{
			insertion_mode = mode_in_table_body;
			return;
		}
		else if (node_name == "caption")
		{
			insertion_mode = mode_in_caption;
			return;
		}
		else if (node_name == "colgroup")
		{
			insertion_mode = mode_in_column_group;
			return;
		}
		else if (node_name == "table")
		{
			insertion_mode = mode_in_table;
			return;
		}
		else if (node_name == "template")
		{
			insertion_mode = template_insertion_modes.back();
			return;
		}
		else if (node_name == "head")
		{
			if (last)
				insertion_mode = mode_in_body;
			else
				insertion_mode = mode_in_head;
			return;
		}
		else if (node_name == "body")
		{
			insertion_mode = mode_in_body;
			return;
		}
		else if (node_name == "frameset")
		{
			insertion_mode = mode_in_frameset;
			return;
		}
		else if (node_name == "html")
		{
			insertion_mode = mode_before_head;
			return;
		}
		else if (last)
		{
			insertion_mode = mode_in_body;
			return;
		}
	}
}

bool HTMLParser_Impl::is_element_in_scope(const std::vector<std::string> &list)
{
	for (size_t i = open_elements.size(); i > 0; i++)
	{
		const CSSLayoutElement &node = open_elements[i - 1];
		std::string name = node.get_name();

		if (name == "applet" || name == "caption" || name == "html" || name == "table" || name == "td" || name == "th" || name == "marquee" || name == "object" || name == "template" ||
			name == "mi" || name == "mo" || name == "mn" || name == "ms" || name == "mtext" || name == "annotation-xml" ||
			name == "foreignObject" || name == "desc" || name == "title")
		{
			return true;
		}

		for (size_t j = 0; j < list.size(); j++)
			if (name == list[j])
				return false;
	}
	return true;
}

bool HTMLParser_Impl::is_element_in_list_item_scope(const std::vector<std::string> &list)
{
	for (size_t i = open_elements.size(); i > 0; i++)
	{
		const CSSLayoutElement &node = open_elements[i - 1];
		std::string name = node.get_name();

		if (name == "applet" || name == "caption" || name == "html" || name == "table" || name == "td" || name == "th" || name == "marquee" || name == "object" || name == "template" ||
			name == "mi" || name == "mo" || name == "mn" || name == "ms" || name == "mtext" || name == "annotation-xml" ||
			name == "foreignObject" || name == "desc" || name == "title" ||
			name == "ol" || name == "ul")
		{
			return true;
		}

		for (size_t j = 0; j < list.size(); j++)
			if (name == list[j])
				return false;
	}
	return true;
}

bool HTMLParser_Impl::is_element_in_button_scope(const std::vector<std::string> &list)
{
	for (size_t i = open_elements.size(); i > 0; i++)
	{
		const CSSLayoutElement &node = open_elements[i - 1];
		std::string name = node.get_name();

		if (name == "applet" || name == "caption" || name == "html" || name == "table" || name == "td" || name == "th" || name == "marquee" || name == "object" || name == "template" ||
			name == "mi" || name == "mo" || name == "mn" || name == "ms" || name == "mtext" || name == "annotation-xml" ||
			name == "foreignObject" || name == "desc" || name == "title" ||
			name == "button")
		{
			return true;
		}

		for (size_t j = 0; j < list.size(); j++)
			if (name == list[j])
				return false;
	}
	return true;
}

bool HTMLParser_Impl::is_element_in_table_scope(const std::vector<std::string> &list)
{
	for (size_t i = open_elements.size(); i > 0; i++)
	{
		const CSSLayoutElement &node = open_elements[i - 1];
		std::string name = node.get_name();

		if (name == "html" || name == "table" || name == "template")
		{
			return true;
		}

		for (size_t j = 0; j < list.size(); j++)
			if (name == list[j])
				return false;
	}
	return true;
}

bool HTMLParser_Impl::is_element_in_select_scope(const std::vector<std::string> &list)
{
	for (size_t i = open_elements.size(); i > 0; i++)
	{
		const CSSLayoutElement &node = open_elements[i - 1];
		std::string name = node.get_name();

		if (name != "optgroup" && name != "option")
		{
			return true;
		}

		for (size_t j = 0; j < list.size(); j++)
			if (name == list[j])
				return false;
	}
	return true;
}

void HTMLParser_Impl::push_active_formatting_element(const HTMLToken &token)
{
	// See §8.2.3.3 The list of active formatting elements
}

void HTMLParser_Impl::push_active_formatting_scope_marker()
{
	// See §8.2.3.3 The list of active formatting elements
}

void HTMLParser_Impl::reconstruct_active_formatting_elements()
{
	// See §8.2.3.3 The list of active formatting elements
}

void HTMLParser_Impl::pop_active_formatting_until_scope_marker()
{
	// See §8.2.3.3 The list of active formatting elements
}

}
