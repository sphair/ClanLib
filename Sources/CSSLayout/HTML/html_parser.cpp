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

	// §8.2.5 Tree construction:

	CSSLayoutElement adjusted_current_node = get_adjusted_current_node();
	if (adjusted_current_node.is_null())
	{
		parse_insertion_mode(token);
	}
	else if (is_html_element(adjusted_current_node))
	{
		parse_insertion_mode(token);
	}
	else if (is_mathml_text_integration_point(adjusted_current_node))
	{
		if (token.type == HTMLToken::type_tag_begin)
		{
			if ((token.value == "mglyph" || token.value == "malignmark"))
				parse_foreign_content(token);
			else
				parse_insertion_mode(token);
		}
		else if (token.type == HTMLToken::type_text)
		{
			parse_insertion_mode(token);
		}
		else
		{
			parse_foreign_content(token);
		}
	}
	else if (adjusted_current_node.get_name() == "annotation-xml" && token.type == HTMLToken::type_tag_begin && token.value == "svg")
	{
		parse_insertion_mode(token);
	}
	else if (is_html_integration_point(adjusted_current_node))
	{
		if (token.type == HTMLToken::type_tag_begin || token.type == HTMLToken::type_text)
		{
			parse_insertion_mode(token);
		}
		else
		{
			parse_foreign_content(token);
		}
	}
	else
	{
		parse_foreign_content(token);
	}
}

bool HTMLParser_Impl::is_html_element(const CSSLayoutElement &element)
{
	// To do: check if element is any of the known HTML elements
	return true;
}

bool HTMLParser_Impl::is_mathml_text_integration_point(const CSSLayoutElement &element)
{
	std::string name = element.get_name();
	return name == "mi" || name == "mo" || name == "mn" || name == "ms" || name == "mtext";
}

bool HTMLParser_Impl::is_html_integration_point(const CSSLayoutElement &element)
{
	// To do:
	// An annotation-xml element in the MathML namespace whose start tag token had an attribute with the name "encoding" whose value was an ASCII case-insensitive match for the string "text/html"
	// An annotation-xml element in the MathML namespace whose start tag token had an attribute with the name "encoding" whose value was an ASCII case-insensitive match for the string "application/xhtml+xml"
	// A foreignObject element in the SVG namespace
	// A desc element in the SVG namespace
	// A title element in the SVG namespace
	return false;
}

CSSLayoutElement HTMLParser_Impl::get_adjusted_current_node()
{
	if (open_elements.size() == 1 && !context.is_null())
		return context;
	else if (!open_elements.empty())
		return open_elements.back();
	else
		return CSSLayoutElement();
}

void HTMLParser_Impl::parse_insertion_mode(const HTMLToken &token)
{
}

void HTMLParser_Impl::parse_foreign_content(const HTMLToken &token)
{
	// §8.2.5.5 The rules for parsing tokens in foreign content
}

CSSLayoutElement HTMLParser_Impl::get_adjusted_insertion_location(CSSLayoutElement override_target, bool foster_parenting)
{
	// §8.2.5.1 Creating and inserting nodes:
/*
	CSSLayoutElement target = override_target;
	if (target.is_null() && !open_elements.empty())
		target = open_elements.back();

	if (foster_parenting && (target.get_name() == "table" || target.get_name() == "tbody" || target.get_name() == "tfoot" || target.get_name() == "thead" || target.get_name() == "tr"))
	{
		CSSLayoutElement last_template = get_last_template();
		CSSLayoutElement last_table = get_last_table();
	}
*/
	return CSSLayoutElement();
}

CSSLayoutElement HTMLParser_Impl::create_element_for_token(const HTMLToken &token, CSSLayoutElement intended_parent)
{
	return CSSLayoutElement();
}

CSSLayoutElement HTMLParser_Impl::insert_foreign_element(const HTMLToken &token, CSSLayoutElement adjusted_insertion_location)
{
	return CSSLayoutElement();
}

CSSLayoutElement HTMLParser_Impl::insert_html_element(const HTMLToken &token, CSSLayoutElement adjusted_insertion_location)
{
	return CSSLayoutElement();
}

void HTMLParser_Impl::adjust_mathml_attributes(HTMLToken &token)
{
}

void HTMLParser_Impl::adjust_svg_attributes(HTMLToken &token)
{
}

void HTMLParser_Impl::adjust_foreign_attributes(HTMLToken &token)
{
}

void HTMLParser_Impl::insert_character(const std::string &data, CSSLayoutElement adjusted_insertion_location)
{
}

void HTMLParser_Impl::raw_text_element_parsing(CSSLayoutElement adjusted_insertion_location)
{
	// §8.2.5.2 Parsing elements that contain only text

	// "These algorithms are always invoked in response to a start tag token."
}

void HTMLParser_Impl::rcdata_element_parsing(CSSLayoutElement adjusted_insertion_location)
{
	// §8.2.5.2 Parsing elements that contain only text

	// "These algorithms are always invoked in response to a start tag token."
}

void HTMLParser_Impl::generate_implied_end_tags(const std::vector<std::string> &exclude_list)
{
	// §8.2.5.3 Closing elements that have implied end tags
}

void HTMLParser_Impl::dispatch(HTMLToken &token)
{
	// §8.2.5 Tree construction
	// tree construction dispatcher
}

void HTMLParser_Impl::initial_insertion_mode(HTMLToken &token)
{
	if (token.type == HTMLToken::type_text)
	{
		// Ignore: character token that is one of U+0009 CHARACTER TABULATION, "LF" (U+000A), "FF" (U+000C), "CR" (U+000D), or U+0020 SPACE
		std::size_t pos = token.value.find_first_not_of("\t\r\n\f ");
		if (pos != 0 && pos != std::string::npos)
			token.value = token.value.substr(pos);
		if (token.value.empty())
			return;
	}

	if (token.type == HTMLToken::type_comment)
	{
		// bool foster_parenting = false;
		// CSSLayoutComment comment = layout.create_comment(token.value);
		// layout.append_child(comment);

		// "mutation observers do fire, as required by the DOM specification."
	}
	else if (token.type == HTMLToken::type_dtd)
	{
		//std::string name, public_id, system_id;
		//layout.append_child(layout.create_document_type(name, public_id, system_id);

		insertion_mode = mode_before_html;
	}
	else
	{
		// To do: "If the document is not an iframe srcdoc document, then this is a parse error; set the Document to quirks mode."
		insertion_mode = mode_before_html;
		dispatch(token);
	}
}

void HTMLParser_Impl::before_html_insertion_mode(HTMLToken &token)
{
	if (token.type == HTMLToken::type_text)
	{
		// Ignore: character token that is one of U+0009 CHARACTER TABULATION, "LF" (U+000A), "FF" (U+000C), "CR" (U+000D), or U+0020 SPACE
		std::size_t pos = token.value.find_first_not_of("\t\r\n\f ");
		if (pos != 0 && pos != std::string::npos)
			token.value = token.value.substr(pos);
		if (token.value.empty())
			return;
	}

	if (token.type == HTMLToken::type_comment)
	{
		// CSSLayoutComment comment = layout.create_comment(token.value);
		// layout.append_child(comment);

		// "mutation observers do fire, as required by the DOM specification."
	}
	else if (token.type == HTMLToken::type_dtd)
	{
		// Parse error. Ignore token.
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "html")
	{
		CSSLayoutElement html = layout.create_element(token.value/*, html_namespace*/);
		//layout.append_child(html);
		layout.set_document_element(html);
		open_elements.push_back(html);

		// To do: handle 'manifest' attribute

		insertion_mode = mode_before_head;
	}
	else if (token.type == HTMLToken::type_tag_end && (token.value != "head" && token.value != "body" && token.value != "html" && token.value != "br"))
	{
		// Parse error. Ignore token.
	}
	else
	{
		CSSLayoutElement html = layout.create_element("html"/*, html_namespace*/);
		//layout.append_child(html);
		layout.set_document_element(html);
		open_elements.push_back(html);

		insertion_mode = mode_before_head;
		dispatch(token);
	}
}

void HTMLParser_Impl::before_head_insertion_mode(HTMLToken &token)
{
	if (token.type == HTMLToken::type_text)
	{
		// Ignore: character token that is one of U+0009 CHARACTER TABULATION, "LF" (U+000A), "FF" (U+000C), "CR" (U+000D), or U+0020 SPACE
		std::size_t pos = token.value.find_first_not_of("\t\r\n\f ");
		if (pos != 0 && pos != std::string::npos)
			token.value = token.value.substr(pos);
		if (token.value.empty())
			return;
	}

	if (token.type == HTMLToken::type_comment)
	{
		// CSSLayoutComment comment = layout.create_comment(token.value);
		//get_adjusted_insertion_location(CSSLayoutElement(), false).append_child(comment);

		// "mutation observers do fire, as required by the DOM specification."
	}
	else if (token.type == HTMLToken::type_dtd)
	{
		// Parse error. Ignore token.
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "html")
	{
		in_body_insertion_mode(token);
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "head")
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		head_element = insert_html_element(token, insert_location.get_parent().to_element());

		insertion_mode = mode_in_head;
	}
	else if (token.type == HTMLToken::type_tag_end && (token.value != "head" && token.value != "body" && token.value != "html" && token.value != "br"))
	{
		// Parse error. Ignore token.
	}
	else
	{
		HTMLToken head_token;
		head_token.type = HTMLToken::type_tag_begin;
		head_token.value = "head";

		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		head_element = insert_html_element(head_token, insert_location.get_parent().to_element());

		insertion_mode = mode_in_head;
		dispatch(token);
	}
}

void HTMLParser_Impl::in_head_insertion_mode(HTMLToken &token)
{
	if (token.type == HTMLToken::type_text)
	{
		// Ignore: character token that is one of U+0009 CHARACTER TABULATION, "LF" (U+000A), "FF" (U+000C), "CR" (U+000D), or U+0020 SPACE
		std::size_t pos = token.value.find_first_not_of("\t\r\n\f ");
		if (pos != 0 && pos != std::string::npos)
		{
			CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
			insert_character(token.value.substr(0, pos), insert_location);
			token.value = token.value.substr(pos);
		}
		if (token.value.empty())
			return;
	}

	if (token.type == HTMLToken::type_comment)
	{
		// CSSLayoutComment comment = layout.create_comment(token.value);
		//get_adjusted_insertion_location(CSSLayoutElement(), false).append_child(comment);

		// "mutation observers do fire, as required by the DOM specification."
	}
	else if (token.type == HTMLToken::type_dtd)
	{
		// Parse error. Ignore token.
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "html")
	{
		in_body_insertion_mode(token);
	}
	else if (token.type == HTMLToken::type_tag_begin && (token.value == "base" || token.value == "basefont" || token.value == "bgsound" || token.value == "link"))
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(token, insert_location.get_parent().to_element());
		open_elements.pop_back();

		// To do: "Acknowledge the token's self-closing flag, if it is set."
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "meta")
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(token, insert_location.get_parent().to_element());
		open_elements.pop_back();

		// To do: "Acknowledge the token's self-closing flag, if it is set."

		// To do: Deal with the charset attribute
		// To do: Deal with http-equiv="Content-Type"
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "title")
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(token, insert_location.get_parent().to_element());

		// To do: Follow the generic RCDATA element parsing algorithm.
	}
	else if ((token.type == HTMLToken::type_tag_begin && token.value == "noscript" && scripting_flag) ||
		(token.type == HTMLToken::type_tag_begin && token.value == "noframes") ||
		(token.type == HTMLToken::type_tag_begin && token.value == "style"))
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(token, insert_location.get_parent().to_element());

		// To do: Follow the generic raw text element parsing algorithm.
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "noscript" && !scripting_flag)
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(token, insert_location.get_parent().to_element());

		insertion_mode = mode_in_head_noscript;
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "script")
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		CSSLayoutElement element = create_element_for_token(token, insert_location.get_parent().to_element());

		// To do: Mark the element as being "parser-inserted" and unset the element's "force-async" flag
		// To do: If the parser was originally created for the HTML fragment parsing algorithm, then mark the script element as "already started". (fragment case)
		// To do: Insert the newly created element at the adjusted insertion location
		open_elements.push_back(element);
		// To do: Switch the tokenizer to the script data state.
		original_insertion_mode = insertion_mode;
		insertion_mode = mode_text;
	}
	else if (token.type == HTMLToken::type_tag_end && token.value == "head")
	{
		open_elements.pop_back();
		insertion_mode = mode_after_head;
	}
	else if ((token.type == HTMLToken::type_tag_begin && token.value == "head") ||
		(token.type == HTMLToken::type_tag_end && (token.value != "body" && token.value != "html" && token.value != "br")))
	{
		// Parse error. Ignore token.
	}
	else
	{
		open_elements.pop_back();
		insertion_mode = mode_after_head;
		dispatch(token);
	}
}

void HTMLParser_Impl::in_head_noscript_insertion_mode(HTMLToken &token)
{
	if (token.type == HTMLToken::type_text)
	{
		// Ignore: character token that is one of U+0009 CHARACTER TABULATION, "LF" (U+000A), "FF" (U+000C), "CR" (U+000D), or U+0020 SPACE
		std::size_t pos = token.value.find_first_not_of("\t\r\n\f ");
		if (pos != 0 && pos != std::string::npos)
		{
			CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
			insert_character(token.value.substr(0, pos), insert_location);
			token.value = token.value.substr(pos);
		}
		if (token.value.empty())
			return;
	}

	if (token.type == HTMLToken::type_dtd)
	{
		// Parse error. Ignore token.
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "html")
	{
		in_body_insertion_mode(token);
	}
	else if (token.type == HTMLToken::type_tag_end && token.value == "noscript")
	{
		open_elements.pop_back();
		insertion_mode = mode_in_head;
	}
	else if (token.type == HTMLToken::type_comment)
	{
		in_head_insertion_mode(token);
	}
	else if (token.type == HTMLToken::type_tag_begin && (token.value == "basefont" || token.value == "bgsound" || token.value == "link" || token.value == "meta" || token.value == "noframes" || token.value == "style"))
	{
		in_head_insertion_mode(token);
	}
	else if ((token.type == HTMLToken::type_tag_begin && (token.value == "head" || token.value == "noscript")) ||
		(token.type == HTMLToken::type_tag_end && token.value != "br"))
	{
		// Parse error. Ignore token.
	}
	else
	{
		// Parse error.

		open_elements.pop_back();
		insertion_mode = mode_in_head;
		dispatch(token);
	}
}

void HTMLParser_Impl::after_head_insertion_mode(HTMLToken &token)
{
	if (token.type == HTMLToken::type_text)
	{
		// Ignore: character token that is one of U+0009 CHARACTER TABULATION, "LF" (U+000A), "FF" (U+000C), "CR" (U+000D), or U+0020 SPACE
		std::size_t pos = token.value.find_first_not_of("\t\r\n\f ");
		if (pos != 0 && pos != std::string::npos)
		{
			CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
			insert_character(token.value.substr(0, pos), insert_location);
			token.value = token.value.substr(pos);
		}
		if (token.value.empty())
			return;
	}

	if (token.type == HTMLToken::type_comment)
	{
		// CSSLayoutComment comment = layout.create_comment(token.value);
		//get_adjusted_insertion_location(CSSLayoutElement(), false).append_child(comment);

		// "mutation observers do fire, as required by the DOM specification."
	}
	else if (token.type == HTMLToken::type_dtd)
	{
		// Parse error. Ignore token.
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "html")
	{
		in_body_insertion_mode(token);
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "body")
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(token, insert_location.get_parent().to_element());

		frameset_ok_flag = false;

		insertion_mode = mode_in_body;
	}
	else if (token.type == HTMLToken::type_tag_begin && token.value == "frameset")
	{
		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(token, insert_location.get_parent().to_element());

		insertion_mode = mode_in_frameset;
	}
	else if (token.type == HTMLToken::type_tag_begin && (token.value == "base" || token.value == "basefont" || token.value == "bgsound" ||
		token.value == "link" || token.value == "meta" || token.value == "noframes" || token.value == "script" || token.value == "style" ||
		token.value == "title"))
	{
		// Parse error.

		open_elements.push_back(head_element);

		in_head_insertion_mode(token);

		for (size_t i = open_elements.size(); i != 0; i--)
		{
			if (open_elements[i -1] == head_element)
			{
				open_elements.erase(open_elements.begin() + (i - 1));
				break;
			}
		}
	}
	else if ((token.type == HTMLToken::type_tag_begin && token.value == "head") ||
		(token.type == HTMLToken::type_tag_end && (token.value != "body" && token.value != "html" && token.value != "br")))
	{
		// Parse error. Ignore token.
	}
	else
	{
		HTMLToken html_token;
		html_token.type = HTMLToken::type_tag_begin;
		html_token.value = "body";

		CSSLayoutElement insert_location = get_adjusted_insertion_location(CSSLayoutElement(), false);
		insert_html_element(html_token, insert_location.get_parent().to_element());

		insertion_mode = mode_in_body;
		dispatch(token);
	}
}

void HTMLParser_Impl::in_body_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::text_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_table_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_table_text_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_caption_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_column_group_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_table_body_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_row_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_cell_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_select_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_select_in_table_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_template_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::after_body_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::in_frameset_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::after_frameset_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::after_after_frameset_insertion_mode(HTMLToken &token)
{
}

void HTMLParser_Impl::stopped_parsing()
{
	// §8.2.6 The end
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
