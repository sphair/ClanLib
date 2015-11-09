/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "API/Core/IOData/iodevice.h"

namespace clan
{
	class XMLTokenizer_Impl
	{
	public:
		XMLTokenizer_Impl() : pos(0), size(0), eat_whitespace(true) { }

		IODevice input;
		std::string::size_type pos, size;
		std::string data;
		bool eat_whitespace;

		static void throw_exception(const std::string &str);
		bool next_text_node(XMLToken *out_token);
		bool next_tag_node(XMLToken *out_token);
		bool next_exclamation_mark_node(XMLToken *out_token);

		// used to get the line number when there is an error in the xml file
		int get_line_number();

		void unescape(std::string &text_out, const std::string &text_in);
		void unescape(std::string &text, const std::string &search, std::string::value_type replace);
		std::string trim_whitespace(const std::string &text);
	};
}
