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

#include "Core/precomp.h"
#include "API/Core/XML/xml_writer.h"
#include "API/Core/XML/xml_token.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "xml_writer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_XMLWriter construction:

CL_XMLWriter::CL_XMLWriter()
{
}

CL_XMLWriter::CL_XMLWriter(const CL_XMLWriter &copy) : impl(copy.impl)
{
}

CL_XMLWriter::CL_XMLWriter(CL_IODevice &output) : impl(new CL_XMLWriter_Generic)
{
	impl->output = output;
	impl->str.reserve(4096);
	impl->escaped_string.reserve(4096);
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

void CL_XMLWriter::write(const CL_XMLToken &token)
{
	if (!impl) return;
	
	// We are reusing a CL_String here to build up a capacity that fits
	// all strings we write.
	CL_String &str = impl->str;
	str.clear();

	if (token.variant == CL_XMLToken::END)
	{
		impl->indent--;
	}

	if (impl->insert_whitespace)
	{
		str.append(impl->indent, L'\t');
	}

	switch (token.type)
	{
	case CL_XMLToken::NULL_TOKEN:
		return; // should this throw exception instead?
		
	case CL_XMLToken::ELEMENT_TOKEN:
		if (token.variant == CL_XMLToken::END)
		{
			str.append("</");
			str.append(impl->insert_escapes_fast(token.name));
			str.append(">");
		}
		else
		{
			str.append("<");
			str.append(impl->insert_escapes_fast(token.name));

			int size = (int) token.attributes.size();
			for (int i=0; i<size; i++)
			{
				str.append(" ");
				str.append(token.attributes[i].first);
				str.append("=\"");
				str.append(impl->insert_escapes_fast(token.attributes[i].second));
				str.append("\"");
			}

			if (token.variant == CL_XMLToken::SINGLE)
				str.append("/>");
			else
				str.append(">");
		}
		break;
		
	case CL_XMLToken::TEXT_TOKEN:
		str.append(impl->insert_escapes_fast(token.value));
		break;
		
	case CL_XMLToken::CDATA_SECTION_TOKEN:
		str.append("<![CDATA[");
		str.append(token.value);
		str.append("]]>");
		break;

	case CL_XMLToken::COMMENT_TOKEN:
		str.append("<!--");
		str.append(token.value);
		str.append("-->");
		break;

	case CL_XMLToken::ENTITY_REFERENCE_TOKEN:
	case CL_XMLToken::ENTITY_TOKEN:
	case CL_XMLToken::PROCESSING_INSTRUCTION_TOKEN:
	case CL_XMLToken::DOCUMENT_TYPE_TOKEN:
	case CL_XMLToken::NOTATION_TOKEN:
		return; // not implemented yet.
	}

	if (impl->insert_whitespace)
	{
#ifdef WIN32
		str.append("\r\n");
#else
		str.append("\n");
#endif
	}

	if (token.variant == CL_XMLToken::BEGIN)
	{
		impl->indent++;
	}

	impl->output.send(str.data(), str.size());
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_XMLWriter implementation:

CL_StringRef CL_XMLWriter_Generic::insert_escapes_fast(const CL_StringRef &str)
{
	static CL_StringRef const amp("&amp;");
	static CL_StringRef const quot("&quot;");
	static CL_StringRef const apos("&apos;");
	static CL_StringRef const lt("&lt;");
	static CL_StringRef const gt("&gt;");

	escaped_string = str;
	CL_StringRef::size_type pos = 0;
	while (pos < escaped_string.size())
	{
		switch(escaped_string[pos])
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
