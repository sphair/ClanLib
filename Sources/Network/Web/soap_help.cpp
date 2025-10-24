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
#include "API/Network/Web/soap_help.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_text.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoapHelp Construction:

/////////////////////////////////////////////////////////////////////////////
// CL_SoapHelp Attributes:

CL_StringRef CL_SoapHelp::soap11_ns = "http://schemas.xmlsoap.org/soap/envelope/";

CL_StringRef CL_SoapHelp::soap12_ns = "http://www.w3.org/2003/05/soap-envelope";

CL_StringRef CL_SoapHelp::xsi_ns = "http://www.w3.org/2001/XMLSchema-instance";

CL_StringRef CL_SoapHelp::xsd_ns = "http://www.w3.org/2001/XMLSchema";

CL_StringRef CL_SoapHelp::xml_ns = "http://www.w3.org/XML/1998/namespace";

/////////////////////////////////////////////////////////////////////////////
// CL_SoapHelp Operations:

CL_DomDocument CL_SoapHelp::create_soap11_sender_fault(const CL_StringRef &reason, const CL_StringRef &lang)
{
	CL_DomElement envelope_element, body_element;
	CL_DomDocument response = create_soap11_message(envelope_element, body_element);
	CL_DomElement fault_element = response.create_element_ns(soap11_ns, "soap:Fault");
	body_element.append_child(fault_element);
	CL_DomElement code_element = response.create_element_ns(soap11_ns, "soap:Code");
	fault_element.append_child(code_element);
	CL_DomElement code_value_element = response.create_element_ns(soap11_ns, "soap:Value");
	code_value_element.append_child(response.create_text_node("soap:Sender"));
	code_element.append_child(code_value_element);
	CL_DomElement reason_element = response.create_element_ns(soap11_ns, "soap:Reason");
	fault_element.append_child(reason_element);
	CL_DomElement reason_text_element = response.create_element_ns(soap11_ns, "soap:Text");
	reason_text_element.set_attribute_ns(xml_ns, "xml:lang", lang);
	reason_text_element.append_child(response.create_text_node(reason));
	reason_element.append_child(reason_text_element);
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap12_sender_fault(const CL_StringRef &reason, const CL_StringRef &lang)
{
	CL_DomElement envelope_element, body_element;
	CL_DomDocument response = create_soap12_message(envelope_element, body_element);
	CL_DomElement fault_element = response.create_element_ns(soap12_ns, "soap:Fault");
	body_element.append_child(fault_element);
	CL_DomElement code_element = response.create_element_ns(soap12_ns, "soap:Code");
	fault_element.append_child(code_element);
	CL_DomElement code_value_element = response.create_element_ns(soap12_ns, "soap:Value");
	code_value_element.append_child(response.create_text_node("soap:Sender"));
	code_element.append_child(code_value_element);
	CL_DomElement reason_element = response.create_element_ns(soap12_ns, "soap:Reason");
	fault_element.append_child(reason_element);
	CL_DomElement reason_text_element = response.create_element_ns(soap12_ns, "soap:Text");
	reason_text_element.set_attribute_ns(xml_ns, "xml:lang", lang);
	reason_text_element.append_child(response.create_text_node(reason));
	reason_element.append_child(reason_text_element);
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap11_receiver_fault(const CL_StringRef &reason, const CL_StringRef &lang)
{
	CL_DomElement envelope_element, body_element;
	CL_DomDocument response = create_soap11_message(envelope_element, body_element);
	CL_DomElement fault_element = response.create_element_ns(soap11_ns, "soap:Fault");
	body_element.append_child(fault_element);
	CL_DomElement code_element = response.create_element_ns(soap11_ns, "soap:Code");
	fault_element.append_child(code_element);
	CL_DomElement code_value_element = response.create_element_ns(soap11_ns, "soap:Value");
	code_value_element.append_child(response.create_text_node("soap:Receiver"));
	code_element.append_child(code_value_element);
	CL_DomElement reason_element = response.create_element_ns(soap11_ns, "soap:Reason");
	fault_element.append_child(reason_element);
	CL_DomElement reason_text_element = response.create_element_ns(soap11_ns, "soap:Text");
	reason_text_element.set_attribute_ns(xml_ns, "xml:lang", lang);
	reason_text_element.append_child(response.create_text_node(reason));
	reason_element.append_child(reason_text_element);
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap12_receiver_fault(const CL_StringRef &reason, const CL_StringRef &lang)
{
	CL_DomElement envelope_element, body_element;
	CL_DomDocument response = create_soap12_message(envelope_element, body_element);
	CL_DomElement fault_element = response.create_element_ns(soap12_ns, "soap:Fault");
	body_element.append_child(fault_element);
	CL_DomElement code_element = response.create_element_ns(soap12_ns, "soap:Code");
	fault_element.append_child(code_element);
	CL_DomElement code_value_element = response.create_element_ns(soap12_ns, "soap:Value");
	code_value_element.append_child(response.create_text_node("soap:Receiver"));
	code_element.append_child(code_value_element);
	CL_DomElement reason_element = response.create_element_ns(soap12_ns, "soap:Reason");
	fault_element.append_child(reason_element);
	CL_DomElement reason_text_element = response.create_element_ns(soap12_ns, "soap:Text");
	reason_text_element.set_attribute_ns(xml_ns, "xml:lang", lang);
	reason_text_element.append_child(response.create_text_node(reason));
	reason_element.append_child(reason_text_element);
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap11_version_mismatch(const CL_StringRef &reason, const CL_StringRef &lang, bool supports_soap11, bool supports_soap12)
{
	CL_DomElement envelope_element, header_element, body_element;
	CL_DomDocument response = create_soap11_message(envelope_element, header_element, body_element);
	CL_DomElement upgrade_element = response.create_element_ns(soap11_ns, "soap:Upgrade");
	header_element.append_child(upgrade_element);
	if (supports_soap12)
	{
		CL_DomElement supported12_element = response.create_element_ns(soap11_ns, "soap:SupportedEnvelope");
		supported12_element.set_attribute_ns(xml_ns, "xml:qname", "soap12:Envelope");
		supported12_element.set_attribute("xmlns:soap12", soap12_ns);
		upgrade_element.append_child(supported12_element);
	}
	if (supports_soap11)
	{
		CL_DomElement supported11_element = response.create_element_ns(soap11_ns, "soap:SupportedEnvelope");
		supported11_element.set_attribute_ns(xml_ns, "xml:qname", "soap11:Envelope");
		supported11_element.set_attribute("xmlns:soap11", soap11_ns);
		upgrade_element.append_child(supported11_element);
	}
	CL_DomElement fault_element = response.create_element_ns(soap11_ns, "soap:Fault");
	body_element.append_child(fault_element);
	CL_DomElement code_element = response.create_element_ns(soap11_ns, "soap:Code");
	fault_element.append_child(code_element);
	CL_DomElement code_value_element = response.create_element_ns(soap11_ns, "soap:Value");
	code_value_element.append_child(response.create_text_node("soap:VersionMismatch"));
	code_element.append_child(code_value_element);
	CL_DomElement reason_element = response.create_element_ns(soap11_ns, "soap:Reason");
	fault_element.append_child(reason_element);
	CL_DomElement reason_text_element = response.create_element_ns(soap11_ns, "soap:Text");
	reason_text_element.set_attribute_ns(xml_ns, "xml:lang", lang);
	reason_text_element.append_child(response.create_text_node(reason));
	reason_element.append_child(reason_text_element);
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap12_version_mismatch(const CL_StringRef &reason, const CL_StringRef &lang, bool supports_soap11, bool supports_soap12)
{
	CL_DomElement envelope_element, header_element, body_element;
	CL_DomDocument response = create_soap12_message(envelope_element, header_element, body_element);
	CL_DomElement upgrade_element = response.create_element_ns(soap12_ns, "soap:Upgrade");
	header_element.append_child(upgrade_element);
	if (supports_soap12)
	{
		CL_DomElement supported12_element = response.create_element_ns(soap12_ns, "soap:SupportedEnvelope");
		supported12_element.set_attribute_ns(xml_ns, "xml:qname", "soap12:Envelope");
		supported12_element.set_attribute("xmlns:soap12", soap12_ns);
		upgrade_element.append_child(supported12_element);
	}
	if (supports_soap11)
	{
		CL_DomElement supported11_element = response.create_element_ns(soap12_ns, "soap:SupportedEnvelope");
		supported11_element.set_attribute_ns(xml_ns, "xml:qname", "soap11:Envelope");
		supported11_element.set_attribute("xmlns:soap11", soap11_ns);
		upgrade_element.append_child(supported11_element);
	}
	CL_DomElement fault_element = response.create_element_ns(soap12_ns, "soap:Fault");
	body_element.append_child(fault_element);
	CL_DomElement code_element = response.create_element_ns(soap12_ns, "soap:Code");
	fault_element.append_child(code_element);
	CL_DomElement code_value_element = response.create_element_ns(soap12_ns, "soap:Value");
	code_value_element.append_child(response.create_text_node("soap:VersionMismatch"));
	code_element.append_child(code_value_element);
	CL_DomElement reason_element = response.create_element_ns(soap12_ns, "soap:Reason");
	fault_element.append_child(reason_element);
	CL_DomElement reason_text_element = response.create_element_ns(soap12_ns, "soap:Text");
	reason_text_element.set_attribute_ns(xml_ns, "xml:lang", lang);
	reason_text_element.append_child(response.create_text_node(reason));
	reason_element.append_child(reason_text_element);
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap11_message(
	CL_DomElement &out_envelope,
	CL_DomElement &out_header,
	CL_DomElement &out_body)
{
	CL_DomDocument response;
	CL_DomElement document_element = response.create_element_ns(soap11_ns, "soap:Envelope");
	document_element.set_attribute("xmlns:soap", soap11_ns);
	document_element.set_attribute("xmlns:xsi", xsi_ns);
	document_element.set_attribute("xmlns:xsd", xsd_ns);
	CL_DomElement header_element = response.create_element_ns(soap11_ns, "soap:Header");
	document_element.append_child(header_element);
	CL_DomElement body_element = response.create_element_ns(soap11_ns, "soap:Body");
	document_element.append_child(body_element);
	response.append_child(document_element);

	out_envelope = document_element;
	out_header = header_element;
	out_body = body_element;
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap11_message(
	CL_DomElement &out_envelope,
	CL_DomElement &out_body)
{
	CL_DomDocument response;
	CL_DomElement document_element = response.create_element_ns(soap11_ns, "soap:Envelope");
	document_element.set_attribute("xmlns:soap", soap11_ns);
	document_element.set_attribute("xmlns:xsi", xsi_ns);
	document_element.set_attribute("xmlns:xsd", xsd_ns);
	CL_DomElement body_element = response.create_element_ns(soap11_ns, "soap:Body");
	document_element.append_child(body_element);
	response.append_child(document_element);

	out_envelope = document_element;
	out_body = body_element;
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap12_message(
	CL_DomElement &out_envelope,
	CL_DomElement &out_header,
	CL_DomElement &out_body)
{
	CL_DomDocument response;
	CL_DomElement document_element = response.create_element_ns(soap12_ns, "soap:Envelope");
	document_element.set_attribute("xmlns:soap", soap12_ns);
	document_element.set_attribute("xmlns:xsi", xsi_ns);
	document_element.set_attribute("xmlns:xsd", xsd_ns);
	CL_DomElement header_element = response.create_element_ns(soap12_ns, "soap:Header");
	document_element.append_child(header_element);
	CL_DomElement body_element = response.create_element_ns(soap12_ns, "soap:Body");
	document_element.append_child(body_element);
	response.append_child(document_element);

	out_envelope = document_element;
	out_header = header_element;
	out_body = body_element;
	return response;
}

CL_DomDocument CL_SoapHelp::create_soap12_message(
	CL_DomElement &out_envelope,
	CL_DomElement &out_body)
{
	CL_DomDocument response;
	CL_DomElement document_element = response.create_element_ns(soap12_ns, "soap:Envelope");
	document_element.set_attribute("xmlns:soap", soap12_ns);
	document_element.set_attribute("xmlns:xsi", xsi_ns);
	document_element.set_attribute("xmlns:xsd", xsd_ns);
	CL_DomElement body_element = response.create_element_ns(soap12_ns, "soap:Body");
	document_element.append_child(body_element);
	response.append_child(document_element);

	out_envelope = document_element;
	out_body = body_element;
	return response;
}

bool CL_SoapHelp::get_soap11_elements(
	CL_DomDocument &document,
	CL_DomElement &out_envelope,
	CL_DomElement &out_header,
	CL_DomElement &out_body,
	CL_DomElement &out_fault)
{
	CL_DomElement envelope = document.named_item_ns(soap11_ns, "Envelope").to_element();
	if (envelope.is_null())
	{
		out_envelope = CL_DomElement();
		out_header = CL_DomElement();
		out_body = CL_DomElement();
		out_fault = CL_DomElement();
		return false;
	}

	out_envelope = envelope;
	out_header = envelope.named_item_ns(soap11_ns, "Header").to_element();
	out_body = envelope.named_item_ns(soap11_ns, "Body").to_element();
	if (out_body.is_element())
		out_fault = out_body.named_item_ns(soap11_ns, "Fault").to_element();
	else
		out_fault = CL_DomElement();
	return true;
}

bool CL_SoapHelp::get_soap12_elements(
	CL_DomDocument &document,
	CL_DomElement &out_envelope,
	CL_DomElement &out_header,
	CL_DomElement &out_body,
	CL_DomElement &out_fault)
{
	CL_DomElement envelope = document.named_item_ns(soap12_ns, "Envelope").to_element();
	if (envelope.is_null())
	{
		out_envelope = CL_DomElement();
		out_header = CL_DomElement();
		out_body = CL_DomElement();
		out_fault = CL_DomElement();
		return false;
	}

	out_envelope = envelope;
	out_header = envelope.named_item_ns(soap12_ns, "Header").to_element();
	out_body = envelope.named_item_ns(soap12_ns, "Body").to_element();
	if (out_body.is_element())
		out_fault = out_body.named_item_ns(soap12_ns, "Fault").to_element();
	else
		out_fault = CL_DomElement();
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoapHelp Implementation:
