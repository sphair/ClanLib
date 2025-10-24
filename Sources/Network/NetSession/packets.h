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

#pragma once


/*
	From RFC 879 ( http://www.faqs.org/rfcs/rfc879.html ):

	"HOSTS MUST NOT SEND DATAGRAMS LARGER THAN 576 OCTETS UNLESS THEY
	HAVE SPECIFIC KNOWLEDGE THAT THE DESTINATION HOST IS PREPARED TO
	ACCEPT LARGER DATAGRAMS.

	This is a long established rule."
*/

#define CL_MAX_PACKET_SIZE 576

class CL_DataBuffer;

struct CL_HelloPacket
{
	unsigned char protocol_major_version;

	unsigned char protocol_minor_version;

	char client_name[64];

	unsigned char client_major_version;

	unsigned char client_minor_version;

	char game_id[64];

	char user_guid[128];
};

struct CL_HelloAckPacket
{
	char server_name[64];

	unsigned char server_major_version;

	unsigned char server_minor_version;
};

struct CL_GoodbyePacket
{
	unsigned int reason_code;

	char reason_string[256];
};

struct CL_GoodbyeAckPacket
{
};

struct CL_MessagePayload
{
	unsigned short channel;

	unsigned short sequence;

	unsigned short last_reliable_sequence;

	unsigned char flags;

	unsigned short size;

	char *data;

	enum Flags
	{
		flag_reliable = 1,
		flag_ack      = 2
	};
};

struct CL_MessagePacket
{
	/// \brief Number of message payloads in message.
	unsigned char num_payloads;

	/// \brief Message payloads.
	CL_MessagePayload payloads[64];

	/// \brief Space available for payload data.
	/** <p>Size calculated by taking max datagram size (576 bytes) and
	    subtracting message packet header (6 bytes).*/
	char data[570];
};

struct CL_NetSessionPacket
{
	enum Type
	{
		type_hello,
		type_hello_ack,
		type_goodbye,
		type_goodbye_ack,
		type_message
	};

	union Packet
	{
		CL_HelloPacket hello;

		CL_HelloAckPacket hello_ack;

		CL_GoodbyePacket goodbye;

		CL_GoodbyeAckPacket goodbye_ack;

		CL_MessagePacket message;
	};

	Type type;

	Packet packet;
};

/// \brief Create a binary packet based on the CL_NetSessionPacket struct.
void cl_write_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet);

void cl_write_hello_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet);

void cl_write_hello_ack_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet);

void cl_write_goodbye_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet);

void cl_write_goodbye_ack_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet);

void cl_write_message_packet(const CL_NetSessionPacket &packet, CL_DataBuffer &out_packet);

/// \brief Parse a binary packet and fill the CL_NetSessionPacket struct.
void cl_read_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet);

void cl_read_hello_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet);

void cl_read_hello_ack_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet);

void cl_read_goodbye_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet);

void cl_read_goodbye_ack_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet);

void cl_read_message_packet(const CL_DataBuffer &packet, CL_NetSessionPacket &out_packet);

/// \brief Add a payload to a message packet.
/** <p>Returns true if the payload could be added. False if the packet is full.</p>*/
bool cl_add_payload(CL_NetSessionPacket &packet, const CL_MessagePayload &payload);


