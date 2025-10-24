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
#include "packets.h"
#ifndef WIN32
#include <netinet/in.h>
#endif

void cl_write_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet)
{
	out_packet.set_size(0);
	switch (packet.type)
	{
	case CL_NetSessionPacket::type_hello:
		cl_write_hello_packet(packet, out_packet);
		break;
	case CL_NetSessionPacket::type_hello_ack:
		cl_write_hello_ack_packet(packet, out_packet);
		break;
	case CL_NetSessionPacket::type_goodbye:
		cl_write_goodbye_packet(packet, out_packet);
		break;
	case CL_NetSessionPacket::type_goodbye_ack:
		cl_write_goodbye_ack_packet(packet, out_packet);
		break;
	case CL_NetSessionPacket::type_message:
		cl_write_message_packet(packet, out_packet);
		break;
	}
}

void cl_write_hello_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet)
{
	out_packet.set_size(265);
	char *bin = out_packet.get_data();
	unsigned char *bin_opcode = (unsigned char *) (bin + 4);
	unsigned char *bin_protocol_major = (unsigned char *) (bin + 5);
	unsigned char *bin_protocol_minor = (unsigned char *) (bin + 6);
	unsigned char *bin_client_name = (unsigned char *) (bin + 7);
	unsigned char *bin_client_major = (unsigned char *) (bin + 71);
	unsigned char *bin_client_minor = (unsigned char *) (bin + 72);
	unsigned char *bin_game_id = (unsigned char *) (bin + 73);
	unsigned char *bin_user_guid = (unsigned char *) (bin + 137);
	memcpy(bin, "clan", 4);
	*bin_opcode = 0;
	*bin_protocol_major = packet.packet.hello.protocol_major_version;
	*bin_protocol_minor = packet.packet.hello.protocol_minor_version;
	memcpy(bin_client_name, packet.packet.hello.client_name, 64);
	*bin_client_major = packet.packet.hello.client_major_version;
	*bin_client_minor = packet.packet.hello.client_minor_version;
	memcpy(bin_game_id, packet.packet.hello.game_id, 64);
	memcpy(bin_user_guid, packet.packet.hello.user_guid, 128);
}

void cl_write_hello_ack_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet)
{
	out_packet.set_size(71);
	char *bin = out_packet.get_data();
	unsigned char *bin_opcode = (unsigned char *) (bin + 4);
	unsigned char *bin_server_name = (unsigned char *) (bin + 5);
	unsigned char *bin_server_major = (unsigned char *) (bin + 69);
	unsigned char *bin_server_minor = (unsigned char *) (bin + 70);
	memcpy(bin, "clan", 4);
	*bin_opcode = 1;
	memcpy(bin_server_name, packet.packet.hello_ack.server_name, 64);
	*bin_server_major = packet.packet.hello_ack.server_major_version;
	*bin_server_minor = packet.packet.hello_ack.server_minor_version;
}

void cl_write_goodbye_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet)
{
	out_packet.set_size(265);
	char *bin = out_packet.get_data();
	unsigned char *bin_opcode = (unsigned char *) (bin + 4);
	unsigned int *bin_reason_code = (unsigned int *) (bin + 5);
	unsigned char *bin_reason_string = (unsigned char *) (bin + 9);
	memcpy(bin, "clan", 4);
	*bin_opcode = 2;
	*bin_reason_code = htonl(packet.packet.goodbye.reason_code);
	memcpy(bin_reason_string, packet.packet.goodbye.reason_string, 256);
}

void cl_write_goodbye_ack_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet)
{
	out_packet.set_size(5);
	char *bin = out_packet.get_data();
	unsigned char *bin_opcode = (unsigned char *) (bin + 4);
	memcpy(bin, "clan", 4);
	*bin_opcode = 3;
}

void cl_write_message_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet)
{
	out_packet.set_size(6);
	char *bin = out_packet.get_data();
	unsigned char *bin_opcode = (unsigned char *) (bin + 4);
	unsigned char *bin_num_payloads = (unsigned char *) (bin + 5);
	memcpy(bin, "clan", 4);
	*bin_opcode = 4;
	*bin_num_payloads = packet.packet.message.num_payloads;

	unsigned int i;
	int needed_size = 6 + 9*packet.packet.message.num_payloads;
	for (i=0; i<packet.packet.message.num_payloads; i++)
		needed_size += packet.packet.message.payloads[i].size;
	out_packet.set_size(needed_size);

	int pos = 6;
	for (i=0; i<packet.packet.message.num_payloads; i++)
	{
		unsigned short *bin_channel = (unsigned short *) (bin + pos);
		unsigned short *bin_sequence = (unsigned short *) (bin + pos + 2);
		unsigned short *bin_last_reliable_sequence = (unsigned short *) (bin + pos + 4);
		unsigned char *bin_flags = (unsigned char *) (bin + pos + 6);
		unsigned short *bin_size = (unsigned short *) (bin + pos + 7);
		char *bin_data = bin + pos + 9;

		const CL_MessagePayload &payload = packet.packet.message.payloads[i];
		*bin_channel = htons(payload.channel);
		*bin_sequence = htons(payload.sequence);
		*bin_last_reliable_sequence = htons(payload.last_reliable_sequence);
		*bin_flags = payload.flags;
		*bin_size = htons(payload.size);
		memcpy(bin_data, payload.data, payload.size);

		pos += 9 + payload.size;
	}
}

void cl_read_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet)
{
	const char *bin = packet.get_data();
	int length = packet.get_size();
	if (length < 5)
		throw CL_Exception(cl_text("Packet size too small"));
	if (memcmp(bin, "clan", 4) != 0)
		throw CL_Exception(cl_text("Not a ClanLib netsession packet"));
	const unsigned char *bin_opcode = (const unsigned char *) (bin + 4);
	switch (*bin_opcode)
	{
	case 0:
		cl_read_hello_packet(packet, out_packet);
		break;
	case 1:
		cl_read_hello_ack_packet(packet, out_packet);
		break;
	case 2:
		cl_read_goodbye_packet(packet, out_packet);
		break;
	case 3:
		cl_read_goodbye_ack_packet(packet, out_packet);
		break;
	case 4:
		cl_read_message_packet(packet, out_packet);
		break;
	default:
		throw CL_Exception(cl_text("Unknown ClanLib netsession packet opcode"));
	}
}

void cl_read_hello_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet)
{
	const char *bin = packet.get_data();
	int length = packet.get_size();
	if (length != 265)
		throw CL_Exception(cl_text("Invalid packet size for hello"));
	if (memcmp(bin, "clan", 4) != 0)
		throw CL_Exception(cl_text("Not a ClanLib netsession packet"));
	const unsigned char *bin_opcode = (const unsigned char *) (bin + 4);
	const unsigned char *bin_protocol_major = (const unsigned char *) (bin + 5);
	const unsigned char *bin_protocol_minor = (const unsigned char *) (bin + 6);
	const unsigned char *bin_client_name = (const unsigned char *) (bin + 7);
	const unsigned char *bin_client_major = (const unsigned char *) (bin + 71);
	const unsigned char *bin_client_minor = (const unsigned char *) (bin + 72);
	const unsigned char *bin_game_id = (const unsigned char *) (bin + 73);
	const unsigned char *bin_user_guid = (const unsigned char *) (bin + 137);
	if (*bin_opcode != 0)
		throw CL_Exception(cl_text("Invalid packet opcode for hello"));
	memset(&out_packet, 0, sizeof(CL_NetSessionPacket));
	out_packet.type = CL_NetSessionPacket::type_hello;
	out_packet.packet.hello.protocol_major_version = *bin_protocol_major;
	out_packet.packet.hello.protocol_minor_version = *bin_protocol_minor;
	memcpy(out_packet.packet.hello.client_name, bin_client_name, 64);
	out_packet.packet.hello.client_major_version = *bin_client_major;
	out_packet.packet.hello.client_minor_version = *bin_client_minor;
	memcpy(out_packet.packet.hello.game_id, bin_game_id, 64);
	memcpy(out_packet.packet.hello.user_guid, bin_user_guid, 128);
	out_packet.packet.hello.client_name[63] = 0;
	out_packet.packet.hello.game_id[63] = 0;
}

void cl_read_hello_ack_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet)
{
	const char *bin = packet.get_data();
	int length = packet.get_size();
	if (length != 71)
		throw CL_Exception(cl_text("Invalid packet size for hello ack"));
	if (memcmp(bin, "clan", 4) != 0)
		throw CL_Exception(cl_text("Not a ClanLib netsession packet"));
	const unsigned char *bin_opcode = (const unsigned char *) (bin + 4);
	const unsigned char *bin_server_name = (const unsigned char *) (bin + 5);
	const unsigned char *bin_server_major = (const unsigned char *) (bin + 69);
	const unsigned char *bin_server_minor = (const unsigned char *) (bin + 70);
	if (*bin_opcode != 1)
		throw CL_Exception(cl_text("Invalid packet opcode for hello ack"));
	memset(&out_packet, 0, sizeof(CL_NetSessionPacket));
	out_packet.type = CL_NetSessionPacket::type_hello_ack;
	memcpy(out_packet.packet.hello_ack.server_name, bin_server_name, 64);
	out_packet.packet.hello_ack.server_major_version = *bin_server_major;
	out_packet.packet.hello_ack.server_minor_version = *bin_server_minor;
	out_packet.packet.hello_ack.server_name[63] = 0;
}

void cl_read_goodbye_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet)
{
	const char *bin = packet.get_data();
	int length = packet.get_size();
	if (length != 265)
		throw CL_Exception(cl_text("Invalid packet size for goodbye"));
	if (memcmp(bin, "clan", 4) != 0)
		throw CL_Exception(cl_text("Not a ClanLib netsession packet"));
	const unsigned char *bin_opcode = (const unsigned char *) (bin + 4);
	const unsigned int *bin_reason_code = (const unsigned int *) (bin + 5);
	const unsigned char *bin_reason_string = (const unsigned char *) (bin + 9);
	if (*bin_opcode != 2)
		throw CL_Exception(cl_text("Invalid packet opcode for goodbye"));
	memset(&out_packet, 0, sizeof(CL_NetSessionPacket));
	out_packet.type = CL_NetSessionPacket::type_goodbye;
	out_packet.packet.goodbye.reason_code = ntohl(*bin_reason_code);
	memcpy(out_packet.packet.goodbye.reason_string, bin_reason_string, 256);
	out_packet.packet.goodbye.reason_string[255] = 0;
}

void cl_read_goodbye_ack_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet)
{
	const char *bin = packet.get_data();
	int length = packet.get_size();
	if (length != 5)
		throw CL_Exception(cl_text("Invalid packet size for goodbye"));
	if (memcmp(bin, "clan", 4) != 0)
		throw CL_Exception(cl_text("Not a ClanLib netsession packet"));
	const unsigned char *bin_opcode = (const unsigned char *) (bin + 4);
	if (*bin_opcode != 3)
		throw CL_Exception(cl_text("Invalid packet opcode for goodbye ack"));
	memset(&out_packet, 0, sizeof(CL_NetSessionPacket));
	out_packet.type = CL_NetSessionPacket::type_goodbye_ack;
}

void cl_read_message_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet)
{
	const char *bin = packet.get_data();
	int length = packet.get_size();
	if (length < 6)
		throw CL_Exception(cl_text("Invalid packet size for message"));
	if (memcmp(bin, "clan", 4) != 0)
		throw CL_Exception(cl_text("Not a ClanLib netsession packet"));
	const unsigned char *bin_opcode = (const unsigned char *) (bin + 4);
	const unsigned char *bin_num_payloads = (const unsigned char *) (bin + 5);
	if (*bin_opcode != 4)
		throw CL_Exception(cl_text("Invalid packet opcode for message"));
	memset(&out_packet, 0, sizeof(CL_NetSessionPacket));
	out_packet.type = CL_NetSessionPacket::type_message;
	out_packet.packet.message.num_payloads = 0;
//	unsigned char num_payloads = *bin_num_payloads;

	int pos = 6;
	for (int i=0; i<out_packet.packet.message.num_payloads; i++)
	{
		if (pos + 9 > length)
			throw CL_Exception(cl_text("Premature end of message packet"));

		const unsigned short *bin_channel = (const unsigned short *) (bin + pos);
		const unsigned short *bin_sequence = (const unsigned short *) (bin + pos + 2);
		const unsigned short *bin_last_reliable_sequence = (const unsigned short *) (bin + pos + 4);
		const unsigned char *bin_flags = (const unsigned char *) (bin + pos + 6);
		const unsigned short *bin_size = (const unsigned short *) (bin + pos + 7);
		const char *bin_data = bin + pos + 9;

		CL_MessagePayload payload;
		payload.channel = ntohs(*bin_channel);
		payload.sequence = ntohs(*bin_sequence);
		payload.last_reliable_sequence = ntohs(*bin_last_reliable_sequence);
		payload.flags = *bin_flags;
		payload.size = ntohs(*bin_size);
		payload.data = (char *) bin_data;

		if (pos + payload.size > length)
			throw CL_Exception(cl_text("Premature end of payload"));

		bool result = cl_add_payload(out_packet, payload);
		if (result == false)
			throw CL_Exception(cl_text("Insufficient space for all payloads"));

		pos += 9 + payload.size;
	}
}

bool cl_add_payload(CL_NetSessionPacket &packet, const CL_MessagePayload &payload)
{
	CL_MessagePacket &message = packet.packet.message;

	// Is there still space in our struct?

	if (message.num_payloads >= 64)
		return false;

	// Calculate if there's still space in datagram packet:

	char *end_pos = message.data + 570 - 9*(message.num_payloads+1);
	char *pos = message.data;
	if (message.num_payloads > 0)
	{
		pos =
			message.payloads[message.num_payloads-1].data +
			message.payloads[message.num_payloads-1].size;
	}
	if (pos + payload.size >= end_pos)
		return false;

	// Ok, there's space for the payload. Insert it.

	int payload_pos = message.num_payloads;
	memcpy(&message.payloads[payload_pos], &payload, sizeof(CL_MessagePayload));
	message.payloads[payload_pos].data = pos;
	memcpy(pos, payload.data, payload.size);
	message.num_payloads++;

	return true;
}
