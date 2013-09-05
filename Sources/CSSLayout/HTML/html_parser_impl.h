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
