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

#include "XML/precomp.h"
#include "API/XML/xml_tokenizer.h"
#include "API/XML/xml_token.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "xml_tokenizer_generic.h"
#include <algorithm>
#include <utility>

namespace clan
{
	XMLTokenizer::XMLTokenizer()
	{
	}

	XMLTokenizer::XMLTokenizer(const XMLTokenizer &copy) : impl(copy.impl)
	{
	}

	XMLTokenizer::XMLTokenizer(IODevice &input) : impl(std::make_shared<XMLTokenizer_Impl>())
	{
		impl->input = input;
		impl->size = input.get_size();
		impl->pos = 0;

		DataBuffer buffer(impl->size);
		input.receive(buffer.get_data(), buffer.get_size(), true);

		StringHelp::BOMType bom_type = StringHelp::detect_bom(buffer.get_data(), buffer.get_size());
		switch (bom_type)
		{
		default:
		case StringHelp::bom_none:
			impl->data = StringHelp::utf8_to_text(std::string(buffer.get_data(), buffer.get_size()));
			break;
		case StringHelp::bom_utf32_be:
		case StringHelp::bom_utf32_le:
			throw Exception("UTF-16 XML files not supported yet");
			break;
		case StringHelp::bom_utf16_be:
		case StringHelp::bom_utf16_le:
			throw Exception("UTF-32 XML files not supported yet");
			break;
		case StringHelp::bom_utf8:
			impl->data = StringHelp::utf8_to_text(std::string(buffer.get_data() + 3, buffer.get_size() - 3));
			break;
		}

	}

	XMLTokenizer::~XMLTokenizer()
	{
	}

	bool XMLTokenizer::get_eat_whitespace() const
	{
		return impl->eat_whitespace;
	}

	void XMLTokenizer::set_eat_whitespace(bool enable)
	{
		impl->eat_whitespace = enable;
	}

	void XMLTokenizer::next(XMLToken *out_token)
	{
		out_token->type = XMLToken::NULL_TOKEN;
		out_token->variant = XMLToken::SINGLE;
		//	out_token->attributes.clear();
		while (!out_token->attributes.empty())
			out_token->attributes.pop_back();

		if (impl)
		{
			if (impl->next_text_node(out_token))
				return;
			impl->next_tag_node(out_token);
		}
	}

	XMLToken XMLTokenizer::next()
	{
		XMLToken token;
		next(&token);
		return token;
	}

	/////////////////////////////////////////////////////////////////////////////

	bool XMLTokenizer_Impl::next_text_node(XMLToken *out_token)
	{
		while (pos < size && data[pos] != '<')
		{
			std::string::size_type start_pos = pos;
			std::string::size_type end_pos = data.find('<', start_pos);
			if (end_pos == data.npos) end_pos = size;
			pos = end_pos;

			std::string text;
			std::string text_orig = data.substr(start_pos, end_pos - start_pos);
			unescape(text, text_orig);
			if (eat_whitespace)
			{
				text = trim_whitespace(text);
				if (text.empty())
					continue;
			}

			out_token->type = XMLToken::TEXT_TOKEN;
			out_token->value = text;
			return true;
		}
		return false;
	}

	bool XMLTokenizer_Impl::next_tag_node(XMLToken *out_token)
	{
		if (pos == size || data[pos] != '<')
			return false;

		pos++;
		if (pos == size)
			XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

		// Try to early predict what sort of node it might be:
		bool closing = (data[pos] == '/');
		bool questionMark = (data[pos] == '?');
		bool exclamationMark = (data[pos] == '!');

		if (closing || questionMark || exclamationMark)
		{
			pos++;
			if (pos == size)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
		}

		if (exclamationMark) // check for cdata section, comments or doctype
		{
			if (next_exclamation_mark_node(out_token))
				return true;
		}

		// Extract the tag name:
		std::string::size_type start_pos = pos;
		std::string::size_type end_pos = data.find_first_of(" \r\n\t?/>", start_pos);
		if (end_pos == data.npos)
			XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
		pos = end_pos;

		out_token->type = questionMark ? XMLToken::PROCESSING_INSTRUCTION_TOKEN : XMLToken::ELEMENT_TOKEN;
		out_token->variant = closing ? XMLToken::END : XMLToken::BEGIN;
		out_token->name = data.substr(start_pos, end_pos - start_pos);

		if (out_token->type == XMLToken::PROCESSING_INSTRUCTION_TOKEN)
		{
			// Strip whitespace:
			pos = data.find_first_not_of(" \r\n\t", pos);
			if (pos == data.npos)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

			end_pos = data.find_first_of("?", pos);
			if (end_pos == data.npos)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
			out_token->value = data.substr(pos, end_pos - pos);
			pos = end_pos;
		}
		else // out_token->type == XMLToken::ELEMENT_TOKEN
		{
			// Check for possible attributes:
			while (true)
			{
				// Strip whitespace:
				pos = data.find_first_not_of(" \r\n\t", pos);
				if (pos == data.npos)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

				// End of tag, stop searching for more attributes:
				if (data[pos] == '/' || data[pos] == '?' || data[pos] == '>')
					break;

				// Extract attribute name:
				std::string::size_type start_pos = pos;
				std::string::size_type end_pos = data.find_first_of(" \r\n\t=", start_pos);
				if (end_pos == data.npos)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
				pos = end_pos;

				std::string attributeName = data.substr(start_pos, end_pos - start_pos);

				// Find seperator:
				pos = data.find_first_not_of(" \r\n\t", pos);
				if (pos == data.npos || pos == size - 1)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
				if (data[pos++] != '=')
					XMLTokenizer_Impl::throw_exception(string_format("XML error(s), parser confused at line %1 (tag=%2, attributeName=%3)", get_line_number(), out_token->name, attributeName));

				// Strip whitespace:
				pos = data.find_first_not_of(" \r\n\t", pos);
				if (pos == data.npos)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

				// Extract attribute value:
				std::string::value_type const * first_of = " \r\n\t";
				if (data[pos] == '"')
				{
					first_of = "\"";
					pos++;
					if (pos == size)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
				}
				else
					if (data[pos] == '\'')
					{
						first_of = "'";
						pos++;
						if (pos == size)
							XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
					}

				start_pos = pos;
				end_pos = data.find_first_of(first_of, start_pos);
				if (end_pos == data.npos)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

				std::string attributeValue;
				std::string attributeValueOrig = data.substr(start_pos, end_pos - start_pos);
				unescape(attributeValue, attributeValueOrig);

				pos = end_pos + 1;
				if (pos == size)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

				// Finally apply attribute to token:
				out_token->attributes.push_back(XMLToken::Attribute(attributeName, attributeValue));
			}
		}

		// Check if its singular:
		if (data[pos] == '/' || data[pos] == '?')
		{
			out_token->variant = XMLToken::SINGLE;
			pos++;
			if (pos == size)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
		}

		// Data stream should be ending now.
		if (data[pos] != '>')
			XMLTokenizer_Impl::throw_exception(string_format("Error in XML stream, line %1 (expected end of tag)", get_line_number()));
		pos++;

		return true;
	}

	bool XMLTokenizer_Impl::next_exclamation_mark_node(XMLToken *out_token)
	{
		if (pos + 2 >= size)
			XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

		if (data.compare(pos, 2, "--") == 0) // comment block
		{
			std::string::size_type start_pos = pos + 2;
			std::string::size_type end_pos = data.find("-->", start_pos);
			if (end_pos == data.npos)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
			pos = end_pos + 3;

			std::string text;
			std::string text_orig = data.substr(start_pos, end_pos - start_pos);
			unescape(text, text_orig);
			if (eat_whitespace)
				text = trim_whitespace(text);

			out_token->type = XMLToken::COMMENT_TOKEN;
			out_token->variant = XMLToken::SINGLE;
			out_token->value = text;
			return true;
		}

		if (pos + 7 >= size)
			XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

		if (data.compare(pos, 7, "DOCTYPE") == 0)
		{
			// Strip whitespace:
			pos = data.find_first_not_of(" \r\n\t", pos + 7);
			if (pos == data.npos)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

			// Find doctype name:				
			std::string::size_type name_start = pos;
			std::string::size_type name_end = data.find_first_of(" \r\n\t?/>", name_start);
			if (name_end == data.npos)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
			pos = name_end;

			// Strip whitespace:
			pos = data.find_first_not_of(" \r\n\t", pos);
			if (pos == data.npos)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

			std::string::size_type public_start = data.npos;
			std::string::size_type public_end = data.npos;
			std::string::size_type system_start = data.npos;
			std::string::size_type system_end = data.npos;
			std::string::size_type subset_start = data.npos;
			std::string::size_type subset_end = data.npos;

			// Look for possible external id:
			if (data[pos] != '[' && data[pos] != '>')
			{
				if (pos + 6 >= size)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

				if (data.compare(pos, 6, "SYSTEM") == 0)
				{
					pos += 6;
					if (pos == size)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					// Strip whitespace:
					pos = data.find_first_not_of(" \r\n\t", pos);
					if (pos == data.npos)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					// Read system literal:
					std::string::value_type literal_char = data[pos];
					if (literal_char != '\'' && literal_char != '"')
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					system_start = pos + 1;
					system_end = data.find(literal_char, system_start);
					if (system_end == data.npos)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
					pos = system_end + 1;
					if (pos >= size)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
				}
				else if (data.compare(pos, 6, "PUBLIC") == 0)
				{
					pos += 6;
					if (pos == size)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					// Strip whitespace:
					pos = data.find_first_not_of(" \r\n\t", pos);
					if (pos == data.npos)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					// Read public literal:
					std::string::value_type literal_char = data[pos];
					if (literal_char != '\'' && literal_char != '"')
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					public_start = pos + 1;
					public_end = data.find(literal_char, public_start);
					if (public_end == data.npos)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
					pos = public_end + 1;
					if (pos >= size)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					// Strip whitespace:
					pos = data.find_first_not_of(" \r\n\t", pos);
					if (pos == data.npos)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					// Read system literal:
					literal_char = data[pos];
					if (literal_char != '\'' && literal_char != '"')
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

					system_start = pos + 1;
					system_end = data.find(literal_char, system_start);
					if (system_end == data.npos)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
					pos = system_end + 1;
					if (pos >= size)
						XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
				}
				else
					XMLTokenizer_Impl::throw_exception(string_format("Error in XML stream, line %1 (unknown external identifier type in DOCTYPE)", get_line_number()));

				// Strip whitespace:
				pos = data.find_first_not_of(" \r\n\t", pos);
				if (pos == data.npos)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
			}

			// Look for possible internal subset:
			if (data[pos] == '[')
			{
				subset_start = pos + 1;

				// Search for the end of the internal subset:
				// (to avoid parsing it, we search backwards)
				std::string::size_type end_pos = data.find('>', pos + 1);
				if (end_pos == data.npos)
					XMLTokenizer_Impl::throw_exception("Premature end of XML data!");

				subset_end = data.rfind(']', end_pos);
				if (subset_end == data.npos)
					XMLTokenizer_Impl::throw_exception(string_format("Error in XML stream, line %1 (expected end of internal subset in DOCTYPE)", get_line_number()));

				pos = end_pos;
			}

			// Expect DOCTYPE tag to end now:
			if (data[pos] != '>')
				XMLTokenizer_Impl::throw_exception(string_format("Error in XML stream, line %1 (expected end of DOCTYPE)", get_line_number()));
			pos++;

			out_token->type = XMLToken::DOCUMENT_TYPE_TOKEN;
			return true;
		}
		else if (data.compare(pos, 7, "[CDATA[") == 0)
		{
			std::string::size_type start_pos = pos + 7;
			std::string::size_type end_pos = data.find("]]>", start_pos);
			if (end_pos == data.npos)
				XMLTokenizer_Impl::throw_exception("Premature end of XML data!");
			pos = end_pos + 3;

			std::string value = data.substr(start_pos, end_pos - start_pos);

			out_token->type = XMLToken::CDATA_SECTION_TOKEN;
			out_token->variant = XMLToken::SINGLE;
			out_token->value = value;
			return true;
		}
		else
		{
			XMLTokenizer_Impl::throw_exception(string_format("Error in XML stream at position %1", static_cast<int>(pos)));
			return false;
		}
	}

	void XMLTokenizer_Impl::throw_exception(const std::string &str)
	{
		throw Exception(str);
	}

	int XMLTokenizer_Impl::get_line_number()
	{
		int line = 1;
		std::string::size_type tmp_pos = 0;

		std::string::const_iterator it;
		for (it = data.begin(); it != data.end() && tmp_pos <= pos; ++it, tmp_pos++)
		{
			if ((*it) == '\n')
				line++;
		}

		return line;
	}

	inline void XMLTokenizer_Impl::unescape(std::string &unescaped, const std::string &text)
	{
		static const std::string quot("&quot;");
		static const std::string apos("&apos;");
		static const std::string lt("&lt;");
		static const std::string gt("&gt;");
		static const std::string amp("&amp;");

		unescaped = text;
		unescape(unescaped, quot, '"');
		unescape(unescaped, apos, '\'');
		unescape(unescaped, lt, '<');
		unescape(unescaped, gt, '>');
		unescape(unescaped, amp, '&');
	}

	inline void XMLTokenizer_Impl::unescape(std::string &text, const std::string &search, std::string::value_type replace)
	{
		std::string::size_type read_pos = 0;
		std::string::size_type length = text.length();
		std::string::size_type search_length = search.length();
		while (true)
		{
			std::string::size_type next_match = text.find(search, read_pos);
			if (next_match == std::string::npos)
				break;

			std::string::size_type copy_size = length - (next_match + search_length);
			if (copy_size > 0)
				memmove(&text[next_match + 1], &text[next_match + search_length], copy_size * sizeof(std::string::value_type));
			text[next_match] = replace;
			length -= search_length - 1;
			read_pos = next_match + 1;
			if (read_pos > length)
				break;
		}
		text.resize(length);
	}

	inline std::string XMLTokenizer_Impl::trim_whitespace(const std::string &text)
	{
		std::string::size_type pos_start = text.find_first_not_of(" \t\r\n");
		if (pos_start == std::string::npos)
			return std::string();
		std::string::size_type pos_end = text.find_last_not_of(" \t\r\n");
		if (pos_end == std::string::npos)
			return std::string();
		return text.substr(pos_start, pos_end - pos_start + 1);
	}
}
