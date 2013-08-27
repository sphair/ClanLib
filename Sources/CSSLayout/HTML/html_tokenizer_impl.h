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

#include <string>

namespace clan
{

class HTMLToken;

class HTMLTokenizer_Impl
{
public:
	HTMLTokenizer_Impl();

	void append(const std::string &data);
	void tokenize(HTMLToken &out_token);

	static bool compare(const std::string &a, const std::string &b);

private:
	bool is_tag_begin(size_t p);
	bool is_tag_end(size_t p);
	bool is_tag_single_end(size_t p);
	bool is_end_tag_begin(size_t p);
	bool is_tag_name(size_t p);
	bool is_tag_name_continued(size_t p);
	bool is_comment_begin(size_t p);
	bool is_comment_end(size_t p);
	bool is_whitespace(size_t p);
	bool is_operator_equal(size_t p);
	bool is_string(size_t p);
	bool is_single_quote(size_t p);
	bool is_double_quote(size_t p);
	bool is_unquoted_string(size_t p);
	bool is_string_escape(size_t p);
	bool is_dtd(size_t p);
	
	size_t read_tag(size_t p, HTMLToken &out_token);
	size_t read_comment(size_t p, HTMLToken &out_token);
	size_t read_text(size_t p, HTMLToken &out_token);
	size_t read_dtd(size_t p, HTMLToken &out_token);
	size_t read_style(size_t p, HTMLToken &out_token);
	size_t read_name(size_t p, std::string &out_string);
	size_t read_whitespace(size_t p);
	size_t read_string(size_t p, std::string &out_string);
	size_t read_script_value(size_t p, size_t pvalue, HTMLToken &token);
	size_t read_style_value(size_t p, size_t pvalue, HTMLToken &token);

	struct HTMLEscape
	{
		char *name;
		wchar_t cdata;
	};
	static HTMLEscape escapes[];

	static void unescape(std::string &text);

	std::string data;
	size_t pos;
	bool before_first_tag;

	friend class HTMLTokenizer;
};

}
