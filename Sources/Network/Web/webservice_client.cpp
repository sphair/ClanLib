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
#include "API/Network/Web/webservice_client.h"
#include "API/Network/Web/webservice_message.h"
#include "API/Network/Web/webservice_exception.h"
#include "API/Network/Web/http_client_connection.h"
#include "API/Network/Web/url_help.h"
#include "API/Network/Web/soap_help.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Core/System/databuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceClient_Impl Class:

class CL_WebserviceClient_Impl
{
public:
	CL_String url;
	CL_HTTPClientConnection connection;

public:
	CL_DomDocument send_soap(const CL_StringRef &soap_action, const CL_DomDocument &request);
};

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceClient Construction:

CL_WebserviceClient::CL_WebserviceClient()
{
}

CL_WebserviceClient::CL_WebserviceClient(const CL_StringRef &url)
: impl(new CL_WebserviceClient_Impl)
{
	// Establish HTTP connection:
	CL_String protocol = CL_URLHelp::get_protocol(url);
	CL_String address = CL_URLHelp::get_host(url);
	CL_String port = CL_URLHelp::get_port(url);
	if (protocol != "http")
		throw CL_Exception(cl_format("Unsupported protocol: %1", protocol));
	if (port.empty())
		port = "80";
	impl->url = url;
	impl->connection = CL_HTTPClientConnection(CL_SocketName(address, port));
}

CL_WebserviceClient::~CL_WebserviceClient()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceClient Operations:

CL_WebserviceMessage CL_WebserviceClient::invoke(
	const CL_StringRef &soap_action,
	const CL_WebserviceMessage &input)
{
	send_request(soap_action, input);
	return read_response();
}

void CL_WebserviceClient::send_request(
	const CL_StringRef &soap_action,
	const CL_WebserviceMessage &input)
{
	// Send HTTP POST message:

	CL_DomDocument request_doc = input.get_document();
	CL_IODevice_Memory request;
	request_doc.save(request, false);

	CL_String8 request_headers = CL_StringHelp::text_to_local8(cl_format(
		"Host: %1\n"
		"SOAPAction: \"%2\"\n"
		"Content-Type: text/xml; charset=utf-8\n"
		"Content-Length: %3",
		CL_URLHelp::get_host(impl->url),
		soap_action,
		request.get_size()));

	CL_String local_path = CL_URLHelp::get_local_path(impl->url);
	impl->connection.send_post(CL_StringHelp::text_to_local8(local_path), request_headers, request.get_data());
}

CL_WebserviceMessage CL_WebserviceClient::read_response()
{
	CL_String8 response_status_text, response_headers;
	CL_DataBuffer response_data;
	int response_code = impl->connection.receive_response(response_status_text, response_headers, response_data);
	if (response_code == 200)
	{
		CL_IODevice_Memory response(response_data);
		CL_DomDocument response_doc;
		response_doc.load(response, false);
		return CL_WebserviceMessage(response_doc);
	}
	else
	{
		CL_WebserviceException e(CL_StringHelp::local8_to_text(response_status_text));
		e.code = response_code;
		e.status_text = CL_StringHelp::local8_to_text(response_status_text);
		e.headers = CL_StringHelp::local8_to_text(response_headers);
		e.data = response_data;

		try // Try process the SOAP response and extract the fault string
		{
			CL_IODevice_Memory iodevice(response_data);
			CL_DomDocument doc(iodevice, false);
			CL_DomElement soap_envelope, soap_header, soap_body, soap_fault;
			bool is_soap = CL_SoapHelp::get_soap11_elements(doc, soap_envelope, soap_header, soap_body, soap_fault);
			if (is_soap && soap_fault.is_element())
			{
				// faultstring is, for some (to me) unexplainable reason, not namespace qualified.
				CL_DomNode fault_string = soap_fault.named_item("faultstring");
				if (fault_string.is_element())
					e.message = fault_string.to_element().get_text();
			}
		}
		catch (const CL_Exception&) // The response wasn't XML
		{
		}
		throw e;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_WebserviceClient Implementation:
