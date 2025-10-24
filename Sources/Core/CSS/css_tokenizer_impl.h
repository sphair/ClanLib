/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_CSSToken;
class CL_IODevice;

class CL_CSSTokenizer_Impl
{
public:
	CL_CSSTokenizer_Impl(CL_IODevice &device);
	CL_CSSTokenizer_Impl(const CL_String &text);
	void read(CL_CSSToken &out_token);
	void peek(CL_CSSToken &out_token);

private:
	void read_atkeyword(CL_CSSToken &out_token);
	void read_hash(CL_CSSToken &out_token);
	void read_cdo(CL_CSSToken &out_token);
	void read_cdc(CL_CSSToken &out_token);
	void read_comment(CL_CSSToken &out_token);
	void read_uri(CL_CSSToken &out_token);
	void read_function(CL_CSSToken &out_token);
	void read_whitespace(CL_CSSToken &out_token);
	void read_includes(CL_CSSToken &out_token);
	void read_dashmatch(CL_CSSToken &out_token);
	void read_number_type(CL_CSSToken &out_token);
	size_t read_ident(size_t p, CL_String &out_ident);
	size_t read_name(size_t p, CL_String &out_ident);
	inline size_t read_nmstart(size_t p, CL_String::char_type &out_c);
	inline size_t read_nmchar(size_t p, CL_String::char_type &out_c);
	size_t read_string(size_t p, CL_String &out_str, CL_String::char_type str_char) const;
	size_t read_invalid(size_t p) const;
	size_t read_uri_nonquoted_string(size_t p, CL_String &out_str) const;
	inline static bool is_whitespace(CL_String::char_type c);

	CL_String doc;
	size_t pos;
};
