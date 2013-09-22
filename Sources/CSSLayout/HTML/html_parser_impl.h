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

#pragma once

#include "API/CSSLayout/Layout/css_layout.h"
#include "API/CSSLayout/Layout/css_layout_element.h"
#include <string>

namespace clan
{

class HTMLToken;

class HTMLParser_Impl
{
public:
	HTMLParser_Impl();

	void append(const HTMLToken &token);

private:
	enum InsertionMode
	{
		mode_initial,
		mode_before_html,
		mode_before_head,
		mode_in_head,
		mode_in_head_noscript,
		mode_after_head,
		mode_in_body,
		mode_text,
		mode_in_table,
		mode_in_table_text,
		mode_in_caption,
		mode_in_column_group,
		mode_in_table_body,
		mode_in_row,
		mode_in_cell,
		mode_in_select,
		mode_in_select_in_table,
		mode_in_template,
		mode_after_body,
		mode_in_frameset,
		mode_after_frameset,
		mode_after_after_body,
		mode_after_after_frameset
	};

	struct ActiveFormattingElement
	{
		ActiveFormattingElement() : scope_marker() { }

		HTMLToken token;
		CSSLayoutElement element;
		bool scope_marker;
	};

	void reset_insertion_mode();

	bool is_element_in_scope(const std::vector<std::string> &list);
	bool is_element_in_list_item_scope(const std::vector<std::string> &list);
	bool is_element_in_button_scope(const std::vector<std::string> &list);
	bool is_element_in_table_scope(const std::vector<std::string> &list);
	bool is_element_in_select_scope(const std::vector<std::string> &list);

	void push_active_formatting_element(const HTMLToken &token);
	void push_active_formatting_scope_marker();
	void reconstruct_active_formatting_elements();
	void pop_active_formatting_until_scope_marker();

	void parse_insertion_mode(const HTMLToken &token);
	void parse_foreign_content(const HTMLToken &token);

	CSSLayoutElement get_adjusted_current_node();

	static bool is_html_element(const CSSLayoutElement &element);
	static bool is_mathml_text_integration_point(const CSSLayoutElement &element);
	static bool is_html_integration_point(const CSSLayoutElement &element);

	CSSLayoutElement get_adjusted_insertion_location(CSSLayoutElement override_target, bool foster_parenting);
	CSSLayoutElement create_element_for_token(const HTMLToken &token, CSSLayoutElement intended_parent);
	CSSLayoutElement insert_foreign_element(const HTMLToken &token, CSSLayoutElement adjusted_insertion_location);
	CSSLayoutElement insert_html_element(const HTMLToken &token, CSSLayoutElement adjusted_insertion_location);

	void adjust_mathml_attributes(HTMLToken &token);
	void adjust_svg_attributes(HTMLToken &token);
	void adjust_foreign_attributes(HTMLToken &token);

	void insert_character(const std::string &data, CSSLayoutElement adjusted_insertion_location);

	void raw_text_element_parsing(CSSLayoutElement adjusted_insertion_location);
	void rcdata_element_parsing(CSSLayoutElement adjusted_insertion_location);
	void generate_implied_end_tags(const std::vector<std::string> &exclude_list = std::vector<std::string>());

	void dispatch(HTMLToken &token);

	void initial_insertion_mode(HTMLToken &token);
	void before_html_insertion_mode(HTMLToken &token);
	void before_head_insertion_mode(HTMLToken &token);
	void in_head_insertion_mode(HTMLToken &token);
	void in_head_noscript_insertion_mode(HTMLToken &token);
	void after_head_insertion_mode(HTMLToken &token);
	void in_body_insertion_mode(HTMLToken &token);
	void text_insertion_mode(HTMLToken &token);
	void in_table_insertion_mode(HTMLToken &token);
	void in_table_text_insertion_mode(HTMLToken &token);
	void in_caption_insertion_mode(HTMLToken &token);
	void in_column_group_insertion_mode(HTMLToken &token);
	void in_table_body_insertion_mode(HTMLToken &token);
	void in_row_insertion_mode(HTMLToken &token);
	void in_cell_insertion_mode(HTMLToken &token);
	void in_select_insertion_mode(HTMLToken &token);
	void in_select_in_table_insertion_mode(HTMLToken &token);
	void in_template_insertion_mode(HTMLToken &token);
	void after_body_insertion_mode(HTMLToken &token);
	void in_frameset_insertion_mode(HTMLToken &token);
	void after_frameset_insertion_mode(HTMLToken &token);
	void after_after_frameset_insertion_mode(HTMLToken &token);

	void stopped_parsing();

	InsertionMode insertion_mode;
	InsertionMode original_insertion_mode;
	std::vector<InsertionMode> template_insertion_modes;

	CSSLayout layout;

	CSSLayoutElement context; // fragment case

	std::vector<CSSLayoutElement> open_elements;
	std::vector<ActiveFormattingElement> active_formatting_elements;

	CSSLayoutElement head_element;
	CSSLayoutElement form_element; // The form element pointer points to the last form element that was opened and whose end tag has not yet been seen.

	bool scripting_flag;
	bool frameset_ok_flag;

	friend class HTMLParser;
};

}
