/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

namespace clan
{
	class StyleToken;

	class StyleTokenizer_Impl
	{
	public:
		StyleTokenizer_Impl(const std::string &text);
		void read(StyleToken &out_token);
		void peek(StyleToken &out_token);

	private:
		void read_atkeyword(StyleToken &out_token);
		void read_hash(StyleToken &out_token);
		void read_cdo(StyleToken &out_token);
		void read_cdc(StyleToken &out_token);
		void read_comment(StyleToken &out_token);
		void read_uri(StyleToken &out_token);
		void read_function(StyleToken &out_token);
		void read_whitespace(StyleToken &out_token);
		void read_includes(StyleToken &out_token);
		void read_dashmatch(StyleToken &out_token);
		void read_number_type(StyleToken &out_token);
		size_t read_ident(size_t p, std::string &out_ident);
		size_t read_name(size_t p, std::string &out_ident);
		inline size_t read_nmstart(size_t p, std::string::value_type &out_c);
		inline size_t read_nmchar(size_t p, std::string::value_type &out_c);
		size_t read_string(size_t p, std::string &out_str, std::string::value_type str_char) const;
		size_t read_invalid(size_t p) const;
		size_t read_uri_nonquoted_string(size_t p, std::string &out_str) const;
		inline static bool is_whitespace(std::string::value_type c);

		std::string doc;
		size_t pos;
	};
}
