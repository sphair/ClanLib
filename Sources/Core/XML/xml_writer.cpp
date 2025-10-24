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
#include "API/Core/XML/xml_writer.h"
#include "API/Core/XML/xml_token_save.h"
#include "API/Core/System/clanstring.h"
#include "xml_writer_generic.h"

//static std::string insert_escapes(std::string str);
static std::string insert_escapes_fast(std::string str);

/////////////////////////////////////////////////////////////////////////////
// CL_XMLWriter construction:

CL_XMLWriter::CL_XMLWriter()
{
}

CL_XMLWriter::CL_XMLWriter(const CL_XMLWriter &copy) : impl(copy.impl)
{
}

CL_XMLWriter::CL_XMLWriter(CL_OutputSource *output, bool delete_output) : impl(new CL_XMLWriter_Generic)
{
	impl->output = output;
	impl->delete_output = delete_output;
}

CL_XMLWriter::~CL_XMLWriter()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLWriter attributes:

bool CL_XMLWriter::get_insert_whitespace() const
{
	return impl->insert_whitespace;
}

void CL_XMLWriter::set_insert_whitespace(bool enable)
{
	impl->insert_whitespace = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_XMLWriter operations:

void CL_XMLWriter::write(const CL_XMLTokenSave &token)
{
	if (!impl) return;

	std::string str;
	switch (token.get_type())
	{
	case CL_XMLToken::NULL_TOKEN:
		return; // should this throw exception instead?
		
	case CL_XMLToken::ELEMENT_TOKEN:
		if (token.get_variant() == CL_XMLToken::END)
		{
			str = CL_String::format("</%1>", insert_escapes_fast(token.get_name()));
		}
		else
		{
			str = CL_String::format("<%1", insert_escapes_fast(token.get_name()));

			int size = token.get_attributes_number();
			for (int i=0; i<size; i++)
			{
				std::pair<std::string, std::string> attribute(token.get_attribute(i));
				str.append(CL_String::format(" %1=\"%2\"", insert_escapes_fast(attribute.first), insert_escapes_fast(attribute.second)));
			}

			if (token.get_variant() == CL_XMLToken::SINGLE)
				str.append("/>");
			else
				str.append(">");
		}
		break;
		
	case CL_XMLToken::TEXT_TOKEN:
		str = insert_escapes_fast(token.get_value());
		break;
		
	case CL_XMLToken::CDATA_SECTION_TOKEN:
		str = CL_String::format("<![CDATA[%1]]>", token.get_value());
		break;

	case CL_XMLToken::COMMENT_TOKEN:
		str = CL_String::format("<!--%1-->", token.get_value());
		break;

	case CL_XMLToken::ENTITY_REFERENCE_TOKEN:
	case CL_XMLToken::ENTITY_TOKEN:
	case CL_XMLToken::PROCESSING_INSTRUCTION_TOKEN:
	case CL_XMLToken::DOCUMENT_TYPE_TOKEN:
	case CL_XMLToken::NOTATION_TOKEN:
		return; // not implemented yet.
	}

	if (token.get_variant() == CL_XMLToken::END)
	{
		impl->indent--;
	}

	if (impl->insert_whitespace)
	{
		std::string indent_tabs(impl->indent, '\t');
		impl->output->write(indent_tabs.data(), indent_tabs.size());
	}

	impl->output->write(str.data(), str.size());

	if (impl->insert_whitespace)
	{
#ifdef WIN32
		impl->output->write("\r\n", 2);
#else
		impl->output->write("\n", 1);
#endif
	}

	if (token.get_variant() == CL_XMLToken::BEGIN)
	{
		impl->indent++;
	}
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_XMLWriter implementation:

#if 0
static std::string insert_escapes(std::string str)
{
	std::string::size_type pos;

	pos = 0;
	while (pos != std::string::npos)
	{
		pos = str.find("&", pos);
		if (pos == std::string::npos) break;
		str.replace( pos, 1, "&amp;" );
		pos++;
	}
	
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = str.find("\"", pos);
		if (pos == std::string::npos) break;
		str.replace( pos, 1, "&quot;" );
		pos++;
	}

	pos = 0;
	while (pos != std::string::npos)
	{
		pos = str.find("\'", pos);
		if (pos == std::string::npos) break;
		str.replace( pos, 1, "&apos;" );
		pos++;
	}

	pos = 0;
	while (pos != std::string::npos)
	{
		pos = str.find("<", pos);
		if (pos == std::string::npos) break;
		str.replace( pos, 1, "&lt;" );
		pos++;
	}

	pos = 0;
	while (pos != std::string::npos)
	{
		pos = str.find(">", pos);
		if (pos == std::string::npos) break;
		str.replace( pos, 1, "&gt;" );
		pos++;
	}

	return str;
}
#endif 

static std::string insert_escapes_fast(std::string str)
{
	static std::string const amp("&amp;");
	static std::string const quot("&quot;");
	static std::string const apos("&apos;");
	static std::string const lt("&lt;");
	static std::string const gt("&gt;");

	std::string::size_type pos = 0;
	while (pos < str.size())
	{
		switch(str[pos])
		{
		case '&':
			str.replace(pos, 1, amp);
			pos += amp.size();
			break;
		case '\'':
			str.replace(pos, 1, apos);
			pos += apos.size();
			break;
		case '\"':
			str.replace(pos, 1, quot);
			pos += quot.size();
			break;
		case '<':
			str.replace(pos, 1, lt);
			pos += lt.size();
			break;
		case '>':
			str.replace(pos, 1, gt);
			pos += gt.size();
			break;
		default:
			++pos;
			break;
		}
	}
	return str;
}
