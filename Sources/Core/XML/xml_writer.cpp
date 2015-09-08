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

#include "Core/precomp.h"
#include "API/Core/XML/xml_writer.h"
#include "API/Core/XML/xml_token.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "xml_writer_generic.h"

namespace clan
{
	XMLWriter::XMLWriter()
	{
	}

	XMLWriter::XMLWriter(const XMLWriter &copy) : impl(copy.impl)
	{
	}

	XMLWriter::XMLWriter(IODevice &output) : impl(std::make_shared<XMLWriter_Impl>())
	{
		impl->output = output;
		impl->str.reserve(4096);
		impl->escaped_string.reserve(4096);
	}

	XMLWriter::~XMLWriter()
	{
	}

	bool XMLWriter::get_insert_whitespace() const
	{
		return impl->insert_whitespace;
	}

	void XMLWriter::set_insert_whitespace(bool enable)
	{
		impl->insert_whitespace = enable;
	}

	void XMLWriter::write(const XMLToken &token)
	{
		if (!impl) return;

		// We are reusing a std::string here to build up a capacity that fits
		// all strings we write.
		std::string &str = impl->str;
		str.clear();

		if (token.variant == XMLToken::END)
		{
			impl->indent--;
		}

#ifdef WIN32
#define NEWLINE_STR "\r\n"
#else
#define NEWLINE_STR "\n"
#endif

		switch (token.type)
		{
		case XMLToken::NULL_TOKEN:
			return; // should this throw exception instead?

		case XMLToken::ELEMENT_TOKEN:
			if (token.variant == XMLToken::BEGIN || token.variant == XMLToken::SINGLE)
			{
				if (impl->insert_whitespace)
				{
					if (!impl->first_token)
						str.append(NEWLINE_STR);
					str.append(impl->indent, L'\t');
				}

				str.append("<");
				str.append(impl->insert_escapes_fast(token.name));

				int size = (int)token.attributes.size();
				for (int i = 0; i < size; i++)
				{
					str.append(" ");
					str.append(token.attributes[i].first);
					str.append("=\"");
					str.append(impl->insert_escapes_fast(token.attributes[i].second));
					str.append("\"");
				}

				if (token.variant == XMLToken::SINGLE)
				{
					str.append("/>");
					impl->single_line_tag = false;
				}
				else
				{
					str.append(">");
					impl->single_line_tag = true;
				}
			}
			else if (token.variant == XMLToken::END)
			{
				if (impl->insert_whitespace && !impl->single_line_tag)
				{
					str.append(NEWLINE_STR);
					str.append(impl->indent, L'\t');
				}
				str.append("</");
				str.append(impl->insert_escapes_fast(token.name));
				str.append(">");
				impl->single_line_tag = false;
			}
			break;

		case XMLToken::TEXT_TOKEN:
			str.append(impl->insert_escapes_fast(token.value));
			break;

		case XMLToken::CDATA_SECTION_TOKEN:
			str.append("<![CDATA[");
			str.append(token.value);
			str.append("]]>");
			break;

		case XMLToken::COMMENT_TOKEN:
			if (impl->insert_whitespace)
			{
				if (!impl->first_token)
					str.append(NEWLINE_STR);
				str.append(impl->indent, L'\t');
			}
			str.append("<!--");
			str.append(token.value);
			str.append("-->");
			break;

		case XMLToken::PROCESSING_INSTRUCTION_TOKEN:
			if (impl->insert_whitespace && !impl->first_token)
				str.append(NEWLINE_STR);

			str.append("<?");
			str.append(token.name);
			if (!token.value.empty())
			{
				str.append(" ");
				str.append(token.value);
			}
			str.append("?>");
			break;

		case XMLToken::ENTITY_REFERENCE_TOKEN:
		case XMLToken::ENTITY_TOKEN:
		case XMLToken::DOCUMENT_TYPE_TOKEN:
		case XMLToken::NOTATION_TOKEN:
			return; // not implemented yet.
		}

		impl->first_token = false;

		if (token.variant == XMLToken::BEGIN)
		{
			impl->indent++;
		}

		impl->output.send(str.data(), str.size());
	}

	/////////////////////////////////////////////////////////////////////////////

	std::string XMLWriter_Impl::insert_escapes_fast(const std::string &str)
	{
		static std::string const amp("&amp;");
		static std::string const quot("&quot;");
		static std::string const apos("&apos;");
		static std::string const lt("&lt;");
		static std::string const gt("&gt;");

		escaped_string = str;
		std::string::size_type pos = 0;
		while (pos < escaped_string.size())
		{
			switch (escaped_string[pos])
			{
			case '&':
				escaped_string.replace(pos, 1, amp);
				pos += amp.size();
				break;
			case '\'':
				escaped_string.replace(pos, 1, apos);
				pos += apos.size();
				break;
			case '\"':
				escaped_string.replace(pos, 1, quot);
				pos += quot.size();
				break;
			case '<':
				escaped_string.replace(pos, 1, lt);
				pos += lt.size();
				break;
			case '>':
				escaped_string.replace(pos, 1, gt);
				pos += gt.size();
				break;
			default:
				++pos;
				break;
			}
		}
		return escaped_string;
	}
}
