/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/XML/xml_tokenizer.h"
#include "API/Core/XML/xml_token.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "xml_tokenizer_generic.h"
#include <algorithm>
#include <utility>

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenizer construction:

CL_XMLTokenizer::CL_XMLTokenizer()
{
}

CL_XMLTokenizer::CL_XMLTokenizer(const CL_XMLTokenizer &copy) : impl(copy.impl)
{
}

CL_XMLTokenizer::CL_XMLTokenizer(CL_IODevice &input) : impl(new CL_XMLTokenizer_Generic)
{
	impl->input = input;
	impl->size = input.get_size();
	impl->pos = 0;

	CL_DataBuffer buffer(impl->size);
	input.receive(buffer.get_data(), buffer.get_size(), true);

	CL_StringHelp::BOMType bom_type = CL_StringHelp::detect_bom(buffer.get_data(), buffer.get_size());
	switch (bom_type)
	{
	default:
	case CL_StringHelp::bom_none:
		impl->data = CL_StringHelp::utf8_to_text(CL_StringRef8(buffer.get_data(), buffer.get_size(), false));
		break;
	case CL_StringHelp::bom_utf32_be:
	case CL_StringHelp::bom_utf32_le:
		throw CL_Exception("UTF-16 XML files not supported yet");
		break;
	case CL_StringHelp::bom_utf16_be:
	case CL_StringHelp::bom_utf16_le:
		throw CL_Exception("UTF-32 XML files not supported yet");
		break;
	case CL_StringHelp::bom_utf8:
		impl->data = CL_StringHelp::utf8_to_text(CL_StringRef8(buffer.get_data()+3, buffer.get_size()-3, false));
		break;
	}

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

void CL_XMLTokenizer::next(CL_XMLToken *out_token)
{
	out_token->type = CL_XMLToken::NULL_TOKEN;
	out_token->variant = CL_XMLToken::SINGLE;
//	out_token->attributes.clear();
	while (!out_token->attributes.empty())
		out_token->attributes.pop_back();

	if (impl != 0)
	{
		if (impl->next_text_node(out_token))
			return;
		impl->next_tag_node(out_token);
	}
}

CL_XMLToken CL_XMLTokenizer::next()
{
	CL_XMLToken token;
	next(&token);
	return token;
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLTokenizer implementation:

bool CL_XMLTokenizer_Generic::next_text_node(CL_XMLToken *out_token)
{
	CL_String::char_type *data_ptr = data.data();
	while (pos < size && data_ptr[pos] != '<')
	{
		CL_String::size_type start_pos = pos;
		CL_String::size_type end_pos = data.find('<', start_pos);
		if (end_pos == data.npos) end_pos = size;
		pos = end_pos;

		CL_StringRef text, text_orig(data_ptr + start_pos, end_pos-start_pos, false);
		unescape(text, text_orig);
		if (eat_whitespace)
		{
			text = trim_whitespace(text);
			if (text.empty())
				continue;
		}

		out_token->type = CL_XMLToken::TEXT_TOKEN;
		out_token->value = text;
		return true;
	}
	return false;
}

bool CL_XMLTokenizer_Generic::next_tag_node(CL_XMLToken *out_token)
{
	CL_String::char_type *data_ptr = data.data();
	if (pos == size || data_ptr[pos] != '<')
		return false;

	pos++;
	if (pos == size)
		CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

	// Try to early predict what sort of node it might be:
	bool closing = (data_ptr[pos] == '/');
	bool questionMark = (data_ptr[pos] == '?');
	bool exclamationMark = (data_ptr[pos] == '!');

	if (closing || questionMark || exclamationMark)
	{
		pos++;
		if (pos == size)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
	}

	if (exclamationMark) // check for cdata section, comments or doctype
	{
		if (next_exclamation_mark_node(out_token))
			return true;
	}

	// Extract the tag name:
	CL_String::size_type start_pos = pos;
	CL_String::size_type end_pos = data.find_first_of(" \r\n\t?/>", start_pos);
	if (end_pos == data.npos)
		CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
	pos = end_pos;

	out_token->type = questionMark ? CL_XMLToken::PROCESSING_INSTRUCTION_TOKEN : CL_XMLToken::ELEMENT_TOKEN;
	out_token->variant = closing ? CL_XMLToken::END : CL_XMLToken::BEGIN;
	out_token->name = string_allocator.alloc(data_ptr + start_pos, end_pos-start_pos);

	// Check for possible attributes:
	while (true)
	{
		// Strip whitespace:
		pos = data.find_first_not_of(" \r\n\t", pos);
		if (pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

		// End of tag, stop searching for more attributes:
		if (data_ptr[pos] == '/' || data_ptr[pos] == '?' || data_ptr[pos] == '>')
			break;

		// Extract attribute name:
		CL_String::size_type start_pos = pos;
		CL_String::size_type end_pos = data.find_first_of(" \r\n\t=", start_pos);
		if (end_pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		pos = end_pos;

		CL_StringRef attributeName = string_allocator.alloc(data_ptr + start_pos, end_pos-start_pos);

		// Find seperator:
		pos = data.find_first_not_of(" \r\n\t", pos);
		if (pos == data.npos || pos == size-1)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		if (data_ptr[pos++] != '=')
			CL_XMLTokenizer_Generic::throw_exception(cl_format("XML error(s), parser confused at line %1 (tag=%2, attributeName=%3)", get_line_number(), out_token->name, attributeName));

		// Strip whitespace:
		pos = data.find_first_not_of(" \r\n\t", pos);
		if (pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

		// Extract attribute value:
		CL_String::char_type const * first_of = " \r\n\t";
		if (data_ptr[pos] == '"')
		{
			first_of = "\"";
			pos++;
			if (pos == size)
				CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		}
		else
			if (data_ptr[pos] == '\'')
			{
				first_of = "'";
				pos++;
				if (pos == size)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
			}

		start_pos = pos;
		end_pos = data.find_first_of(first_of, start_pos);
		if (end_pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		
		CL_StringRef attributeValue, attributeValueOrig(data_ptr + start_pos, end_pos-start_pos, false);
		unescape(attributeValue, attributeValueOrig);

		pos = end_pos + 1;
		if (pos == size)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

		// Finally apply attribute to token:
		out_token->attributes.push_back(CL_XMLToken::Attribute(attributeName, attributeValue));
	}

	// Check if its singular:
	if (data_ptr[pos] == '/' || data_ptr[pos] == '?')
	{
		out_token->variant = CL_XMLToken::SINGLE;
		pos++;
		if (pos == size)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
	}

	// Data stream should be ending now.
	if (data_ptr[pos] != '>')
		CL_XMLTokenizer_Generic::throw_exception(cl_format("Error in XML stream, line %1 (expected end of tag)", get_line_number()));
	pos++;

	return true;
}

bool CL_XMLTokenizer_Generic::next_exclamation_mark_node(CL_XMLToken *out_token)
{
	CL_String::char_type *data_ptr = data.data();
	if (pos+2 >= size)
		CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
	
	if (data.compare(pos, 2, "--") == 0) // comment block
	{
		CL_String::size_type start_pos = pos+2;
		CL_String::size_type end_pos = data.find("-->", start_pos);
		if (end_pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		pos = end_pos+3;

		CL_StringRef text, text_orig(data_ptr + start_pos, end_pos-start_pos, false);
		unescape(text, text_orig);
		if (eat_whitespace)
			text = trim_whitespace(text);

		out_token->type = CL_XMLToken::COMMENT_TOKEN;
		out_token->variant = CL_XMLToken::SINGLE;
		out_token->value = text;
		return true;
	}

	if (pos+7 >= size)
		CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
	
	if (data.compare(pos, 7, "DOCTYPE") == 0)
	{
		// Strip whitespace:
		pos = data.find_first_not_of(" \r\n\t", pos+7);
		if (pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

		// Find doctype name:				
		CL_String::size_type name_start = pos;
		CL_String::size_type name_end = data.find_first_of(" \r\n\t?/>", name_start);
		if (name_end == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		pos = name_end;
		
		// Strip whitespace:
		pos = data.find_first_not_of(" \r\n\t", pos);
		if (pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

		CL_String::size_type public_start = data.npos;
		CL_String::size_type public_end = data.npos;
		CL_String::size_type system_start = data.npos;
		CL_String::size_type system_end = data.npos;
		CL_String::size_type subset_start = data.npos;
		CL_String::size_type subset_end = data.npos;

		// Look for possible external id:
		if (data_ptr[pos] != '[' && data_ptr[pos] != '>')
		{
			if (pos+6 >= size)
				CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

			if (data.compare(pos, 6, "SYSTEM") == 0)
			{
				pos+=6;
				if (pos == size)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				// Strip whitespace:
				pos = data.find_first_not_of(" \r\n\t", pos);
				if (pos == data.npos)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				// Read system literal:
				CL_String::char_type literal_char = data_ptr[pos];
				if (literal_char != '\'' && literal_char != '"')
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				system_start = pos+1;
				system_end = data.find(literal_char, system_start);
				if (system_end == data.npos)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
				pos = system_end + 1;
				if (pos >= size)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
			}
			else if (data.compare(pos, 6, "PUBLIC") == 0)
			{
				pos+=6;
				if (pos == size)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				// Strip whitespace:
				pos = data.find_first_not_of(" \r\n\t", pos);
				if (pos == data.npos)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				// Read public literal:
				CL_String::char_type literal_char = data_ptr[pos];
				if (literal_char != '\'' && literal_char != '"')
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				public_start = pos+1;
				public_end = data.find(literal_char, public_start);
				if (public_end == data.npos)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
				pos = public_end + 1;
				if (pos >= size)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				// Strip whitespace:
				pos = data.find_first_not_of(" \r\n\t", pos);
				if (pos == data.npos)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				// Read system literal:
				literal_char = data_ptr[pos];
				if (literal_char != '\'' && literal_char != '"')
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");

				system_start = pos+1;
				system_end = data.find(literal_char, system_start);
				if (system_end == data.npos)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
				pos = system_end + 1;
				if (pos >= size)
					CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
			}
			else
				CL_XMLTokenizer_Generic::throw_exception(cl_format("Error in XML stream, line %1 (unknown external identifier type in DOCTYPE)", get_line_number()));
		
			// Strip whitespace:
			pos = data.find_first_not_of(" \r\n\t", pos);
			if (pos == data.npos)
				CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		}
		
		// Look for possible internal subset:
		if (data_ptr[pos] == '[')
		{
			subset_start = pos + 1;
		
			// Search for the end of the internal subset:
			// (to avoid parsing it, we search backwards)
			CL_String::size_type end_pos = data.find('>', pos+1);
			if (end_pos == data.npos)
				CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
			
			subset_end = data.rfind(']', end_pos);
			if (subset_end == data.npos)
				CL_XMLTokenizer_Generic::throw_exception(cl_format("Error in XML stream, line %1 (expected end of internal subset in DOCTYPE)", get_line_number()));
				
			pos = end_pos;
		}
		
		// Expect DOCTYPE tag to end now:
		if (data_ptr[pos] != '>')
			CL_XMLTokenizer_Generic::throw_exception(cl_format("Error in XML stream, line %1 (expected end of DOCTYPE)", get_line_number()));
		pos++;

		out_token->type = CL_XMLToken::DOCUMENT_TYPE_TOKEN;
		return true;
	}
	else if (data.compare(pos, 7, "[CDATA[") == 0)
	{
		CL_String::size_type start_pos = pos+7;
		CL_String::size_type end_pos = data.find("]]>", start_pos);
		if (end_pos == data.npos)
			CL_XMLTokenizer_Generic::throw_exception("Premature end of XML data!");
		pos = end_pos+3;

		CL_StringRef value = string_allocator.alloc(
			data_ptr + start_pos, end_pos-start_pos);

		out_token->type = CL_XMLToken::CDATA_SECTION_TOKEN;
		out_token->variant = CL_XMLToken::SINGLE;
		out_token->value = value;
		return true;
	}
	else
	{
		CL_XMLTokenizer_Generic::throw_exception(cl_format("Error in XML stream at position %1", static_cast<int>(pos)));
		return false;
	}
}

void CL_XMLTokenizer_Generic::throw_exception(const CL_StringRef &str)
{
	throw CL_Exception(str);
}

int CL_XMLTokenizer_Generic::get_line_number()
{
	int line = 1;
	CL_String::size_type tmp_pos = 0;

	CL_String::const_iterator it;
	for( it = data.begin(); it != data.end() && tmp_pos <= pos; ++it, tmp_pos++ )
	{
		if( (*it) == '\n' )
			line++;
	}

	return line;
}

inline void CL_XMLTokenizer_Generic::unescape(CL_StringRef &unescaped, const CL_StringRef &text)
{
	static const CL_StringRef quot("&quot;");
	static const CL_StringRef apos("&apos;");
	static const CL_StringRef lt("&lt;");
	static const CL_StringRef gt("&gt;");
	static const CL_StringRef amp("&amp;");

	unescaped = string_allocator.alloc(text);
	unescape(unescaped, quot, '"');
	unescape(unescaped, apos, '\'');
	unescape(unescaped, lt, '<');
	unescape(unescaped, gt, '>');
	unescape(unescaped, amp, '&');
}

inline void CL_XMLTokenizer_Generic::unescape(CL_StringRef &text, const CL_StringRef &search, CL_String::char_type replace)
{
	CL_StringRef::size_type read_pos = 0;
	CL_StringRef::size_type length = text.length();
	CL_StringRef::size_type search_length = search.length();
	CL_StringRef::char_type *data = text.data();
	while (true)
	{
		CL_StringRef::size_type next_match = text.find(search, read_pos);
		if (next_match == CL_StringRef::npos)
			break;

		CL_StringRef::size_type copy_size = length - (next_match + search_length);
		memcpy(data + next_match + 1, data + next_match + search_length, copy_size * sizeof(CL_StringRef::char_type));
		data[next_match] = replace;
		length -= search_length - 1;
		read_pos = next_match + 1;
		if (read_pos > length)
			break;
	}
	text.set_length(length);
}

inline CL_StringRef CL_XMLTokenizer_Generic::trim_whitespace(const CL_StringRef &text)
{
	CL_StringRef::size_type pos_start = text.find_first_not_of(" \t\r\n");
	if (pos_start == CL_StringRef::npos)
		return CL_StringRef();
	CL_StringRef::size_type pos_end = text.find_last_not_of(" \t\r\n", pos_start);
	if (pos_end == CL_StringRef::npos)
	{
		if (pos_start == 0)
			return text;
		else
			return string_allocator.alloc(text.substr(pos_start));
	}
	else
	{
		return string_allocator.alloc(text.substr(pos_start, pos_end - pos_start + 1));
	}
}
