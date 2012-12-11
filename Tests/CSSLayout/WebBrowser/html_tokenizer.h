
#pragma once

class HTMLToken;

class HTMLTokenizer
{
public:
	HTMLTokenizer();

	void append(const CL_String &data);
	void tokenize(HTMLToken &out_token);

	static bool compare(const CL_String &a, const CL_String &b);

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
	size_t read_name(size_t p, CL_String &out_string);
	size_t read_whitespace(size_t p);
	size_t read_string(size_t p, CL_String &out_string);
	size_t read_script_value(size_t p, size_t pvalue, HTMLToken &token);
	size_t read_style_value(size_t p, size_t pvalue, HTMLToken &token);

	struct HTMLEscape
	{
		CL_String::char_type *name;
		wchar_t cdata;
	};
	static HTMLEscape escapes[];

	static void unescape(CL_String &text);

	CL_String data;
	size_t pos;
	bool before_first_tag;
};
