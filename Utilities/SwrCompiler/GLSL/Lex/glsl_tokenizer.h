
#pragma once

#include "glsl_token.h"
#include <map>

class GlslTokenizer
{
public:
	GlslTokenizer(std::string data);

	GlslToken read_token();
	void read_token(GlslToken &out_token);
	void read_token(GlslToken &out_token, bool skip_whitespace_and_comments);
	GlslToken peek_token(bool skip_whitespace_and_comments);
	void save_position() { saved_pos = pos; }
	void restore_position() { pos = saved_pos; }

private:
	std::string read_digit_sequence(bool is_octal, bool is_hex);

	std::string data;
	size_t pos;
	size_t saved_pos;

	std::map<std::string, GlslToken::Keyword> keywords;
};
