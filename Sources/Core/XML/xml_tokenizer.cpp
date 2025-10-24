/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/XML/xml_tokenizer.h"
#include "API/Core/XML/xml_token_load.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "xml_tokenizer_generic.h"
#include "API/Core/XML/xml_token_string.h"

#include <algorithm>
#include <utility>
//#include <iterator>

//std::string replace_escapes(std::string str);

//template <typename Iter>
//std::string replace_escapes_fast(Iter begin, Iter end);

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenizer construction:

CL_XMLTokenizer::CL_XMLTokenizer()
{
}

CL_XMLTokenizer::CL_XMLTokenizer(const CL_XMLTokenizer &copy) : impl(copy.impl)
{
}

CL_XMLTokenizer::CL_XMLTokenizer(CL_InputSource *input, bool delete_input) : impl(new CL_XMLTokenizer_Generic)
{
	impl->input = input;
	impl->delete_input = delete_input;
	impl->size = input->size();
	impl->data.resize(impl->size);
	input->read(&impl->data[0], impl->size);
	impl->pos = 0;
}

CL_XMLTokenizer::~CL_XMLTokenizer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenizer attributes:

bool CL_XMLTokenizer::get_eat_whitespace() const
{
	return impl->eat_whitespace;
}

void CL_XMLTokenizer::set_eat_whitespace(bool enable)
{
	impl->eat_whitespace = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenizer operations:

CL_XMLTokenLoad CL_XMLTokenizer::next()
{
	if (impl == 0)
		return CL_XMLTokenLoad();

	if (impl->pos == impl->size)
		return CL_XMLTokenLoad(); // EOF, return null token.

	bool is_need_escape = true;

	if (impl->data[impl->pos] != '<') // Text node
	{
		std::string::size_type start_pos = impl->pos;
		std::string::size_type end_pos = impl->data.find('<', start_pos);
		if (end_pos == impl->data.npos) end_pos = impl->size;
		impl->pos = end_pos;

		CL_XMLTokenString text(&impl->data[start_pos], end_pos-start_pos, is_need_escape);
		if (impl->eat_whitespace)
		{
			text = trim_whitespace(text);
			if (text.empty())
				return next();
		}

		CL_XMLTokenLoad token;
		token.set_type(CL_XMLToken::TEXT_TOKEN);
		token.set_value(text);

		return token;
	}
	else // Tag node
	{
		impl->pos++;
		if (impl->pos == impl->size)
			throw CL_Error("Premature end of XML data!");

		// Try to early predict what sort of node it might be:
		bool closing = false;
		bool questionMark = false;
		bool exclamationMark = false;
		if (impl->data[impl->pos] == '/')
			closing = true;
		else
			if (impl->data[impl->pos] == '?')
				questionMark = true;
			else
				if (impl->data[impl->pos] == '!')
					exclamationMark = true;

		if (closing || questionMark || exclamationMark)
		{
			impl->pos++;
			if (impl->pos == impl->size) throw CL_Error("Premature end of XML data!");
		}

		if (exclamationMark) // check for cdata section or comments
		{
			if (impl->data.compare(impl->pos, 2, "--") == 0) // comment block
			{
				std::string::size_type start_pos = impl->pos+2;
				std::string::size_type end_pos = impl->data.find("-->", start_pos);
				if (end_pos == impl->data.npos)
					throw CL_Error("Premature end of XML data!");
				impl->pos = end_pos+3;

				CL_XMLTokenLoad token;
				token.set_type(CL_XMLToken::COMMENT_TOKEN);
				token.set_variant(CL_XMLToken::SINGLE);
				token.set_value(CL_XMLTokenString(&impl->data[start_pos], end_pos-start_pos, is_need_escape));
				return token;
			}

			if (impl->data.compare(impl->pos, 7, "[CDATA[") != 0)
				throw CL_Error(CL_String::format("Error in XML stream at position %1", static_cast<int>(impl->pos)));
			std::string::size_type start_pos = impl->pos+7;
			std::string::size_type end_pos = impl->data.find("]]>", start_pos);
			if (end_pos == impl->data.npos)
				throw CL_Error("Premature end of XML data!");
			impl->pos = end_pos+3;

			CL_XMLTokenLoad token;
			token.set_type(CL_XMLToken::CDATA_SECTION_TOKEN);
			token.set_variant(CL_XMLToken::SINGLE);
			token.set_value(CL_XMLTokenString(&impl->data[start_pos], end_pos-start_pos, is_need_escape));
			return token;
		}

		// Extract the tag name:
		std::string::size_type start_pos = impl->pos;
		std::string::size_type end_pos = impl->data.find_first_of(" \r\n\t?/>", start_pos);
		if (end_pos == impl->data.npos)
			throw CL_Error("Premature end of XML data!");
		impl->pos = end_pos;

		CL_XMLTokenLoad token;
		token.set_type(questionMark ? CL_XMLToken::PROCESSING_INSTRUCTION_TOKEN : CL_XMLToken::ELEMENT_TOKEN);
		token.set_variant(closing ? CL_XMLToken::END : CL_XMLToken::BEGIN);
		token.set_name(CL_XMLTokenString(&impl->data[start_pos], end_pos-start_pos, is_need_escape));
		//token.set_name(replace_escapes_fast(impl->data.begin() + start_pos, impl->data.begin() + end_pos));

		// Check for possible attributes:
		while (true)
		{
			// Strip whitespace:
			impl->pos = impl->data.find_first_not_of(" \r\n\t", impl->pos);
			if (impl->pos == impl->data.npos)
				throw CL_Error("Premature end of XML data!");

			// End of tag, stop searching for more attributes:
			if (impl->data[impl->pos] == '/' || impl->data[impl->pos] == '?' || impl->data[impl->pos] == '>')
				break;

			// Extract attribute name:
			std::string::size_type start_pos = impl->pos;
			std::string::size_type end_pos = impl->data.find_first_of(" \r\n\t=", start_pos);
			if (end_pos == impl->data.npos)
				throw CL_Error("Premature end of XML data!");
			impl->pos = end_pos;

			CL_XMLTokenString attributeName(&impl->data[start_pos], end_pos-start_pos, is_need_escape);

			// Find seperator:
			impl->pos = impl->data.find_first_not_of(" \r\n\t", impl->pos);
			if (impl->pos == impl->data.npos || impl->pos == impl->size-1)
				throw CL_Error("Premature end of XML data!");
			if (impl->data[impl->pos++] != '=')
				throw CL_Error(CL_String::format("XML error(s), parser confused at line %1 (tag=%2, attributeName=%3)", impl->get_line_number(), token.get_name(), attributeName.to_string()));

			// Strip whitespace:
			impl->pos = impl->data.find_first_not_of(" \r\n\t", impl->pos);
			if (impl->pos == impl->data.npos)
				throw CL_Error("Premature end of XML data!");

			// Extract attribute value:
			char const * first_of = " \r\n\t";
			if (impl->data[impl->pos] == '"')
			{
				first_of = "\"";
				impl->pos++;
				if (impl->pos == impl->size)
					throw CL_Error("Premature end of XML data!");
			}
			else
				if (impl->data[impl->pos] == '\'')
				{
					first_of = "'";
					impl->pos++;
					if (impl->pos == impl->size)
						throw CL_Error("Premature end of XML data!");
				}

			start_pos = impl->pos;
			end_pos = impl->data.find_first_of(first_of, start_pos);
			if (end_pos == impl->data.npos)
				throw CL_Error("Premature end of XML data!");
			
			CL_XMLTokenString attributeValue(CL_XMLTokenString(&impl->data[start_pos], end_pos-start_pos, is_need_escape));

			impl->pos = end_pos + 1;
			if (impl->pos == impl->size)
				throw CL_Error("Premature end of XML data!");

			// Finally apply attribute to token:
			token.set_attribute(attributeName, attributeValue);
		}

		// Check if its singular:
		if (impl->data[impl->pos] == '/' || impl->data[impl->pos] == '?')
		{
			token.set_variant(CL_XMLToken::SINGLE);
			impl->pos++;
			if (impl->pos == impl->size)
				throw CL_Error("Premature end of XML data!");
		}

		// Data stream should be ending now.
		if (impl->data[impl->pos] != '>')
			throw CL_Error(CL_String::format("Error in XML stream, line %1 (expected end of tag)", impl->get_line_number()));
		impl->pos++;

		return token;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenizer implementation:

/*
inline bool try_replace(std::string & str, std::string::size_type pos, std::string const & escape, char const * escape_char)
{
	if (pos + escape.size() <= str.size())
		if (std::equal(escape.begin(), escape.end(), str.begin() + pos))
		{
			str.replace(pos, escape.size(), escape_char);
			return true;
		}
	return false;
}

std::string replace_escapes(std::string str)
{
	std::string::size_type pos;

	static std::string const amp("&amp");
	static std::string const quot("&quot");
	static std::string const apos("&apos");
	static std::string const lt("&lt");
	static std::string const gt("&gt");

	pos = 0;
	while (pos != std::string::npos)
	{
		pos = str.find('&', pos);
		if (pos == std::string::npos)
			break;

		if (	try_replace(str, pos, amp, "&")
			||	try_replace(str, pos, quot, "\"")
			||	try_replace(str, pos, apos, "\'")
			||	try_replace(str, pos, gt, ">")
			||	try_replace(str, pos, lt, "<") )
		{
		}
		pos++;
	}

	return str;
}
*/

//std::string & replace_escapes(std::string & str)
//{
//	std::string::size_type pos;
//
//	static std::string const amp("&amp");
//	static std::string const quot("&quot");
//	static std::string const apos("&apos");
//	static std::string const lt("&lt");
//	static std::string const gt("&gt");
//
//	pos = 0;
//	while (pos != std::string::npos)
//	{
//		pos = str.find('&', pos);
//		if (pos == std::string::npos)
//			break;
//
//		if (	try_replace(str, pos, amp, "&")
//			||	try_replace(str, pos, quot, "\"")
//			||	try_replace(str, pos, apos, "\'")
//			||	try_replace(str, pos, gt, ">")
//			||	try_replace(str, pos, lt, "<") )
//		{
//		}
//		pos++;
//	}
//
//	return str;
//}

/*
template <typename Container, typename Iter>
inline bool append_escape(Container & buff, Iter & begin, Iter end, std::string const & escape, char escape_char)
{
	if (static_cast<ptrdiff_t>(escape.size()) <= std::distance(begin, end))
		if (std::equal(escape.begin(), escape.end(), begin))
		{
			buff.insert(buff.end(), 1, escape_char);
			std::advance(begin, escape.size());
			return true;
		}
	return false;
}

template <typename Iter>
std::string replace_escapes_fast(Iter begin, Iter end)
{
	static std::string const amp("&amp");
	static std::string const quot("&quot");
	static std::string const apos("&apos");
	static std::string const lt("&lt");
	static std::string const gt("&gt");

	std::size_t size = std::distance(begin, end);
//	static std::string str;
//	str.reserve(size);
//	str.resize(0);

	static std::vector<char> buff;
//	buff.reserve(size);
	buff.resize(0);

	while(begin != end)
	{
		Iter pos = std::find(begin, end, '&');
		buff.insert(buff.end(), begin, pos);
		if (pos == end)
			break;

		begin = pos;
		if (	append_escape(buff, begin, end, amp, '&')
			||	append_escape(buff, begin, end, quot, '\"')
			||	append_escape(buff, begin, end, apos, '\'')
			||	append_escape(buff, begin, end, gt, '>')
			||	append_escape(buff, begin, end, lt, '<'))
		{
		}
		else
			++begin;
	}
	return std::string(&buff[0], buff.size());
}
*/
