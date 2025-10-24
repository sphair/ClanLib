/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_text.h"
#include "network_data.h"

CL_NetGameEvent CL_NetGameNetworkData::receive_data(CL_TCPConnection connection)
{
	// Receive the message:

	int size = connection.read_uint16();
	if (size > packet_limit)
		throw CL_Exception("Incoming message too big");
	CL_DataBuffer buffer(size);
	connection.read(buffer.get_data(), buffer.get_size());

	CL_IODevice_Memory iodevice_memory(buffer);
	CL_DomDocument doc;
	doc.load(iodevice_memory);

	// Read the XML message:
	CL_NetGameEvent e(doc.get_document_element().get_tag_name());
	CL_DomNode cur = doc.get_document_element().get_first_child();
	while (!cur.is_null())
	{
		if (cur.is_element())
		{
			CL_DomElement argument_element = cur.to_element();
			if (argument_element.get_tag_name() == cl_text("null"))
			{
				e.add_argument(CL_NetGameEventValue());
			}
			else if (argument_element.get_tag_name() == cl_text("integer"))
			{
				e.add_argument(CL_NetGameEventValue(CL_NetGameEventValue::integer, argument_element.get_text()));
			}
			else if (argument_element.get_tag_name() == cl_text("number"))
			{
				e.add_argument(CL_NetGameEventValue(CL_NetGameEventValue::number, argument_element.get_text()));
			}
			else if (argument_element.get_tag_name() == cl_text("boolean"))
			{
				e.add_argument(CL_NetGameEventValue(CL_NetGameEventValue::boolean, argument_element.get_text()));
			}
			else if (argument_element.get_tag_name() == cl_text("string"))
			{
				e.add_argument(CL_NetGameEventValue(CL_NetGameEventValue::string, argument_element.get_text()));
			}
		}
		cur = cur.get_next_sibling();
	}
	return e;
}

void CL_NetGameNetworkData::send_data(CL_TCPConnection connection, const CL_NetGameEvent &e)
{
	// Create XML message to send:

	CL_DomDocument doc;
	CL_DomElement event_element = doc.create_element(e.get_name());
	for (unsigned int i = 0; i < e.get_argument_count(); i++)
	{
		CL_NetGameEventValue value = e.get_argument(i);
		CL_DomText value_text = doc.create_text_node(value.get_value());
		CL_DomElement value_element;
		switch (value.get_type())
		{
		case CL_NetGameEventValue::null:
			value_element = doc.create_element(cl_text("null"));
			break;
		case CL_NetGameEventValue::integer:
			value_element = doc.create_element(cl_text("integer"));
			value_element.append_child(value_text);
			break;
		case CL_NetGameEventValue::number:
			value_element = doc.create_element(cl_text("number"));
			value_element.append_child(value_text);
			break;
		case CL_NetGameEventValue::boolean:
			value_element = doc.create_element(cl_text("boolean"));
			value_element.append_child(value_text);
			break;
		case CL_NetGameEventValue::string:
			value_element = doc.create_element(cl_text("string"));
			value_element.append_child(value_text);
			break;
		default:
			throw CL_Exception(cl_text("Unknown game event value type"));
		}
		event_element.append_child(value_element);
	}
	doc.append_child(event_element);

	// Send the message:

	CL_IODevice_Memory iodevice_memory;
	doc.save(iodevice_memory, false);
	CL_DataBuffer buffer = iodevice_memory.get_data();
	if (buffer.get_size() > packet_limit)
		throw CL_Exception(cl_text("Outgoing message too big"));

	connection.write_uint16(buffer.get_size());
	connection.write(buffer.get_data(), buffer.get_size());
}
