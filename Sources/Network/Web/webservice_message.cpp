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

#include "Network/precomp.h"
#include "API/Network/Web/webservice_message.h"
#include "API/Network/Web/webservice_part.h"
#include "API/Network/Web/soap_help.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/databuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceMessage_Impl Class:

class CL_WebserviceMessage_Impl
{
public:
	CL_DomDocument document;

	CL_DomElement envelope, header, body, fault, message;
};

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceMessage Construction:

CL_WebserviceMessage::CL_WebserviceMessage(const CL_StringRef &name, const CL_StringRef &target_namespace)
: impl(new CL_WebserviceMessage_Impl)
{
	impl->document = CL_SoapHelp::create_soap11_message(impl->envelope, impl->body);
	impl->message = impl->document.create_element_ns(target_namespace, name);
	impl->message.set_attribute("xmlns", target_namespace);
	impl->body.append_child(impl->message);
}

CL_WebserviceMessage::CL_WebserviceMessage(const CL_DomDocument &doc)
: impl(new CL_WebserviceMessage_Impl)
{
	impl->document = doc;
	CL_SoapHelp::get_soap11_elements(impl->document, impl->envelope, impl->header, impl->body, impl->fault);
	CL_DomNode child = impl->body.get_first_child();
	while (!child.is_null() && !child.is_element())
		child = child.get_next_sibling();
	impl->message = child.to_element();
}

CL_WebserviceMessage::~CL_WebserviceMessage()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceMessage Attributes:

CL_DomDocument CL_WebserviceMessage::get_document() const
{
	return impl->document;
}

CL_DomElement CL_WebserviceMessage::get_message_element() const
{
	return impl->message;
}

CL_DomElement CL_WebserviceMessage::get_fault_element() const
{
	return impl->fault;
}

CL_String CL_WebserviceMessage::get_name() const
{
	return impl->message.get_local_name();
}

CL_String CL_WebserviceMessage::get_target_namespace() const
{
	return impl->message.get_namespace_uri();
}

CL_WebservicePart CL_WebserviceMessage::get_part(const CL_StringRef &name)
{
	return get_part_ns(impl->message.get_namespace_uri(), name);
}

CL_WebservicePart CL_WebserviceMessage::get_part_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name)
{
	CL_DomNode node = impl->message.named_item_ns(namespace_uri, name);
	if (!node.is_element())
		throw CL_Exception(cl_format("No web service message part named %1", name));
	return CL_WebservicePart(node.to_element());
}

CL_String CL_WebserviceMessage::get_string(const CL_StringRef &name)
{
	CL_WebservicePart part = get_part(name);
	return part.get_value_string();
}

CL_String CL_WebserviceMessage::get_string_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name)
{
	CL_WebservicePart part = get_part_ns(namespace_uri, name);
	return part.get_value_string();
}

int CL_WebserviceMessage::get_int(const CL_StringRef &name)
{
	CL_WebservicePart part = get_part(name);
	return part.get_value_int();
}

int CL_WebserviceMessage::get_int_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name)
{
	CL_WebservicePart part = get_part_ns(namespace_uri, name);
	return part.get_value_int();
}

double CL_WebserviceMessage::get_double(const CL_StringRef &name)
{
	CL_WebservicePart part = get_part(name);
	return part.get_value_double();
}

double CL_WebserviceMessage::get_double_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name)
{
	CL_WebservicePart part = get_part_ns(namespace_uri, name);
	return part.get_value_double();
}

CL_DataBuffer CL_WebserviceMessage::get_base64(const CL_StringRef &name)
{
	CL_WebservicePart part = get_part(name);
	return part.get_value_base64();
}

CL_DataBuffer CL_WebserviceMessage::get_base64_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name)
{
	CL_WebservicePart part = get_part_ns(namespace_uri, name);
	return part.get_value_base64();
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceMessage Operations:

CL_String CL_WebserviceMessage::create_qname(const CL_StringRef &name)
{
	CL_DomString prefix = impl->message.get_prefix();
	if (prefix.empty())
		return name;
	else
		return prefix + ":" + name;
}

CL_WebservicePart CL_WebserviceMessage::append_part(const CL_StringRef &name)
{
	CL_DomElement element = impl->document.create_element_ns(get_target_namespace(), create_qname(name));
	impl->message.append_child(element);
	return element;
}

CL_WebservicePart CL_WebserviceMessage::append_part_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name)
{
	CL_DomElement element = impl->document.create_element_ns(namespace_uri, name);
	impl->message.append_child(element);
	return element;
}

CL_WebservicePart CL_WebserviceMessage::append_string(const CL_StringRef &name, const CL_StringRef &value)
{
	CL_WebservicePart part = append_part(name);
	part.set_value_string(value);
	return part;
}

CL_WebservicePart CL_WebserviceMessage::append_string_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, const CL_StringRef &value)
{
	CL_WebservicePart part = append_part_ns(namespace_uri, name);
	part.set_value_string(value);
	return part;
}

CL_WebservicePart CL_WebserviceMessage::append_int(const CL_StringRef &name, int value)
{
	CL_WebservicePart part = append_part(name);
	part.set_value_int(value);
	return part;
}

CL_WebservicePart CL_WebserviceMessage::append_int_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, int value)
{
	CL_WebservicePart part = append_part_ns(namespace_uri, name);
	part.set_value_int(value);
	return part;
}

CL_WebservicePart CL_WebserviceMessage::append_double(const CL_StringRef &name, double value)
{
	CL_WebservicePart part = append_part(name);
	part.set_value_double(value);
	return part;
}

CL_WebservicePart CL_WebserviceMessage::append_double_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, double value)
{
	CL_WebservicePart part = append_part_ns(namespace_uri, name);
	part.set_value_double(value);
	return part;
}

CL_WebservicePart CL_WebserviceMessage::append_base64(const CL_StringRef &name, const CL_DataBuffer &value)
{
	CL_WebservicePart part = append_part(name);
	part.set_value_base64(value);
	return part;
}

CL_WebservicePart CL_WebserviceMessage::append_base64_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, const CL_DataBuffer &value)
{
	CL_WebservicePart part = append_part_ns(namespace_uri, name);
	part.set_value_base64(value);
	return part;
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceMessage Implementation:
