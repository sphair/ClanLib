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

#include "Network/precomp.h"
#include "API/Network/Web/webservice_part.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_text.h"
#include "API/Core/Math/base64_decoder.h"
#include "API/Core/Math/base64_encoder.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/databuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_WebservicePart_Impl Class:

class CL_WebservicePart_Impl
{
public:
	CL_DomElement element;
};

/////////////////////////////////////////////////////////////////////////////
// CL_WebservicePart Construction:

CL_WebservicePart::CL_WebservicePart(const CL_DomElement &part_element)
: impl(new CL_WebservicePart_Impl)
{
	impl->element = part_element;
}

CL_WebservicePart::~CL_WebservicePart()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebservicePart Attributes:

CL_String CL_WebservicePart::get_name() const
{
	return impl->element.get_local_name();
}

CL_String CL_WebservicePart::get_target_namespace() const
{
	return impl->element.get_namespace_uri();
}

CL_DomElement CL_WebservicePart::get_element() const
{
	return impl->element;
}

CL_String CL_WebservicePart::get_value_string() const
{
	CL_DomNode child = impl->element.get_first_child();
	if (!child.is_text())
		return CL_String();
	return child.get_node_value();
}

int CL_WebservicePart::get_value_int() const
{
	CL_DomNode child = impl->element.get_first_child();
	if (!child.is_text())
		return 0;
	return CL_StringHelp::text_to_int(child.get_node_value());
}

double CL_WebservicePart::get_value_double() const
{
	CL_DomNode child = impl->element.get_first_child();
	if (!child.is_text())
		return 0.0;
	return CL_StringHelp::text_to_double(child.get_node_value());
}

CL_DataBuffer CL_WebservicePart::get_value_base64() const
{
	CL_DomNode child = impl->element.get_first_child();
	if (!child.is_text())
		return CL_DataBuffer();
	return CL_Base64Decoder::decode(CL_StringHelp::text_to_local8(child.get_node_value()));
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebservicePart Operations:

void CL_WebservicePart::set_value_string(const CL_StringRef &value)
{
	CL_DomNode child = impl->element.get_first_child();
	if (!child.is_text())
	{
		child = impl->element.get_owner_document().create_text_node(value);
		CL_DomNode first_subchild = impl->element.get_first_child();
		impl->element.insert_before(child, first_subchild);
	}
	else
	{
		child.set_node_value(value);
	}
}

void CL_WebservicePart::set_value_int(int value)
{
	set_value_string(CL_StringHelp::int_to_text(value));
}

void CL_WebservicePart::set_value_double(double value)
{
	set_value_string(CL_StringHelp::double_to_text(value));
}

void CL_WebservicePart::set_value_base64(const CL_DataBuffer &value)
{
	set_value_string(CL_StringHelp::local8_to_text(CL_Base64Encoder::encode(value)));
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebservicePart Implementation:
