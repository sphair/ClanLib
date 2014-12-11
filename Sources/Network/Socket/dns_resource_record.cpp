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

#include "Network/precomp.h"
#include "API/Network/Socket/dns_resource_record.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "dns_resource_record_impl.h"
#ifndef WIN32
#include <netinet/in.h>
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DNSResourceRecord Construction:

DNSResourceRecord::DNSResourceRecord()
: impl(std::make_shared<DNSResourceRecord_Impl>())
{
}

DNSResourceRecord::~DNSResourceRecord()
{
}

/////////////////////////////////////////////////////////////////////////////
// DNSResourceRecord Attributes:

std::string DNSResourceRecord::get_name() const
{
	return read_domain_name(impl->packet.get_data(), impl->record_offset);
}

std::string DNSResourceRecord::get_type() const
{
	const DataBuffer &data = impl->packet.get_data();
	int type_offset = find_domain_name_end(data, impl->record_offset);
	int rr_data_offset = type_offset + 2 + 2 + 4 + 2;
	if (data.get_size() < rr_data_offset)
		throw Exception("Premature end of DNS resource record");
	unsigned short rr_type = ntohs(*(unsigned short *) (data.get_data() + type_offset));
	//unsigned short rr_class = ntohs(*(unsigned short *) (data.get_data() + type_offset+2));
	//unsigned int rr_ttl = ntohl(*(unsigned int *) (data.get_data() + type_offset+4));
	//unsigned short rr_data_length = ntohs(*(unsigned short *) (data.get_data() + type_offset+8));
	return type_from_int(rr_type);
}

std::string DNSResourceRecord::get_class() const
{
	const DataBuffer &data = impl->packet.get_data();
	int type_offset = find_domain_name_end(data, impl->record_offset);
	int rr_data_offset = type_offset + 2 + 2 + 4 + 2;
	if (data.get_size() < rr_data_offset)
		throw Exception("Premature end of DNS resource record");
	//unsigned short rr_type = ntohs(*(unsigned short *) (data.get_data() + type_offset));
	unsigned short rr_class = ntohs(*(unsigned short *) (data.get_data() + type_offset+2));
	//unsigned int rr_ttl = ntohl(*(unsigned int *) (data.get_data() + type_offset+4));
	//unsigned short rr_data_length = ntohs(*(unsigned short *) (data.get_data() + type_offset+8));
	return class_from_int(rr_class);
}

int DNSResourceRecord::get_ttl() const
{
	const DataBuffer &data = impl->packet.get_data();
	int type_offset = find_domain_name_end(data, impl->record_offset);
	int rr_data_offset = type_offset + 2 + 2 + 4 + 2;
	if (data.get_size() < rr_data_offset)
		throw Exception("Premature end of DNS resource record");
	//unsigned short rr_type = ntohs(*(unsigned short *) (data.get_data() + type_offset));
	//unsigned short rr_class = ntohs(*(unsigned short *) (data.get_data() + type_offset+2));
	unsigned int rr_ttl = ntohl(*(unsigned int *) (data.get_data() + type_offset+4));
	//unsigned short rr_data_length = ntohs(*(unsigned short *) (data.get_data() + type_offset+8));
	return rr_ttl;
}

const DNSPacket &DNSResourceRecord::get_packet() const
{
	return impl->packet;
}

int DNSResourceRecord::get_record_offset() const
{
	return impl->record_offset;
}

int DNSResourceRecord::get_rdata_offset() const
{
	return impl->rdata_offset;
}

int DNSResourceRecord::get_rdata_length() const
{
	return impl->rdata_length;
}

std::string DNSResourceRecord::get_cname_cname() const
{
	return read_domain_name(impl->packet.get_data(), impl->rdata_offset);
}

int DNSResourceRecord::get_mx_preference() const
{
	const DataBuffer &data = impl->packet.get_data();
	if (data.get_size() < impl->rdata_offset + 2)
		throw Exception("Premature end of resource data section");
	short *preference = (short *) (data.get_data() + impl->rdata_offset);
	return ntohs(*preference);
}

std::string DNSResourceRecord::get_mx_exchange() const
{
	return read_domain_name(impl->packet.get_data(), impl->rdata_offset + 2);
}

std::string DNSResourceRecord::get_ns_nsdname() const
{
	return read_domain_name(impl->packet.get_data(), impl->rdata_offset);
}

std::string DNSResourceRecord::get_ptr_ptrdname() const
{
	return read_domain_name(impl->packet.get_data(), impl->rdata_offset);
}

std::string DNSResourceRecord::get_soa_mname() const
{
	return read_domain_name(impl->packet.get_data(), impl->rdata_offset);
}

std::string DNSResourceRecord::get_soa_rname() const
{
	int pos = find_domain_name_end(impl->packet.get_data(), impl->rdata_offset);
	return read_domain_name(impl->packet.get_data(), pos);
}

unsigned int DNSResourceRecord::get_soa_serial() const
{
	int pos = find_domain_name_end(impl->packet.get_data(), impl->rdata_offset);
	pos = find_domain_name_end(impl->packet.get_data(), pos);
	if (impl->packet.get_data().get_size() < pos + 4)
		throw Exception("Premature end of resource data section");
	unsigned int *serial = (unsigned int *) (impl->packet.get_data().get_data() + pos);
	return ntohl(*serial);
}

int DNSResourceRecord::get_soa_refresh() const
{
	int pos = find_domain_name_end(impl->packet.get_data(), impl->rdata_offset);
	pos = find_domain_name_end(impl->packet.get_data(), pos) + 4;
	if (impl->packet.get_data().get_size() < pos + 4)
		throw Exception("Premature end of resource data section");
	int *refresh = (int *) (impl->packet.get_data().get_data() + pos);
	return ntohl(*refresh);
}

int DNSResourceRecord::get_soa_retry() const
{
	int pos = find_domain_name_end(impl->packet.get_data(), impl->rdata_offset);
	pos = find_domain_name_end(impl->packet.get_data(), pos) + 8;
	if (impl->packet.get_data().get_size() < pos + 4)
		throw Exception("Premature end of resource data section");
	int *retry = (int *) (impl->packet.get_data().get_data() + pos);
	return ntohl(*retry);
}

int DNSResourceRecord::get_soa_expire() const
{
	int pos = find_domain_name_end(impl->packet.get_data(), impl->rdata_offset);
	pos = find_domain_name_end(impl->packet.get_data(), pos) + 12;
	if (impl->packet.get_data().get_size() < pos + 4)
		throw Exception("Premature end of resource data section");
	int *expire = (int *) (impl->packet.get_data().get_data() + pos);
	return ntohl(*expire);
}

unsigned int DNSResourceRecord::get_soa_minimum() const
{
	int pos = find_domain_name_end(impl->packet.get_data(), impl->rdata_offset);
	pos = find_domain_name_end(impl->packet.get_data(), pos) + 16;
	if (impl->packet.get_data().get_size() < pos + 4)
		throw Exception("Premature end of resource data section");
	unsigned int *minimum = (unsigned int *) (impl->packet.get_data().get_data() + pos);
	return ntohl(*minimum);
}

unsigned int DNSResourceRecord::get_a_address() const
{
	if (impl->packet.get_data().get_size() < impl->rdata_offset)
		throw Exception("Premature end of resource data section");
	unsigned int *address = (unsigned int *) (impl->packet.get_data().get_data() + impl->rdata_offset);
	return *address;
}

std::string DNSResourceRecord::get_a_address_str() const
{
	unsigned long addr_long = (unsigned long) ntohl(get_a_address());
	std::string str_addr = string_format(
		"%1.%2.%3.%4",
		int((addr_long & 0xff000000) >> 24),
		int((addr_long & 0x00ff0000) >> 16),
		int((addr_long & 0x0000ff00) >> 8),
		int((addr_long & 0x000000ff)));
	return str_addr;
}

unsigned int DNSResourceRecord::get_wks_address() const
{
	if (impl->packet.get_data().get_size() < impl->rdata_offset + 4)
		throw Exception("Premature end of resource data section");
	unsigned int *address = (unsigned int *) (impl->packet.get_data().get_data() + impl->rdata_offset);
	return *address;
}

std::string DNSResourceRecord::get_wks_address_str() const
{
	unsigned long addr_long = (unsigned long) ntohl(get_wks_address());
	std::string str_addr = string_format(
		"%1.%2.%3.%4",
		int((addr_long & 0xff000000) >> 24),
		int((addr_long & 0x00ff0000) >> 16),
		int((addr_long & 0x0000ff00) >> 8),
		int((addr_long & 0x000000ff)));
	return str_addr;
}

unsigned char DNSResourceRecord::get_wks_protocol() const
{
	if (impl->packet.get_data().get_size() < impl->rdata_offset + 5)
		throw Exception("Premature end of resource data section");
	unsigned char *protocol = (unsigned char *) (impl->packet.get_data().get_data() + impl->rdata_offset + 4);
	return *protocol;
}

DataBuffer DNSResourceRecord::get_wks_bit_map() const
{
	int len = impl->rdata_length - 5;
	if (len < 0)
		throw Exception("Premature end of resource data section");
	DataBuffer bit_map(len);
	memcpy(bit_map.get_data(), impl->packet.get_data().get_data() + impl->rdata_offset + 5, len);
	return bit_map;
}

/////////////////////////////////////////////////////////////////////////////
// DNSResourceRecord Operation:

void DNSResourceRecord::set_record(DNSPacket new_packet, int new_record_offset)
{
	// Verify size sanity of generic record fields:

	const DataBuffer &new_data = new_packet.get_data();
	int type_offset = find_domain_name_end(new_data, new_record_offset);
	int rr_data_offset = type_offset + 2 + 2 + 4 + 2;
	if (new_data.get_size() < rr_data_offset)
		throw Exception("Premature end of DNS resource record");

	//unsigned short rr_type = ntohs(*(unsigned short *) (new_data.get_data() + type_offset));
	//unsigned short rr_class = ntohs(*(unsigned short *) (new_data.get_data() + type_offset+2));
	//unsigned int rr_ttl = ntohl(*(unsigned int *) (new_data.get_data() + type_offset+4));
	unsigned short rr_data_length = ntohs(*(unsigned short *) (new_data.get_data() + type_offset+8));

	if (new_data.get_size() < rr_data_offset + rr_data_length)
		throw Exception("Premature end of DNS resource record RDATA section");

	// Generic record fields seem sane. Store record:

	impl->packet = new_packet;
	impl->rdata_offset = rr_data_offset;
	impl->rdata_length = rr_data_length;
	impl->record_offset = new_record_offset;
}

struct RRType
{
	const char *name;
	int value;
	const char *description;
};

static RRType rr_types[] =
{
	{"A",     1,   "a host address"},
	{"NS",    2,   "an authoritative name server"},
	{"MD",    3,   "a mail destination (Obsolete - use MX)"},
	{"MF",    4,   "a mail forwarder (Obsolete - use MX)"},
	{"CNAME", 5,   "the canonical name for an alias"},
	{"SOA",   6,   "marks the start of a zone of authority"},
	{"MB",    7,   "a mailbox domain name (EXPERIMENTAL)"},
	{"MG",    8,   "a mail group member (EXPERIMENTAL)"},
	{"MR",    9,   "a mail rename domain name (EXPERIMENTAL)"},
	{"NULL",  10,  "a null RR (EXPERIMENTAL)"},
	{"WKS",   11,  "a well known service description"},
	{"PTR",   12,  "a domain name pointer"},
	{"HINFO", 13,  "host information"},
	{"MINFO", 14,  "mailbox or mail list information"},
	{"MX",    15,  "mail exchange"},
	{"TXT",   16,  "text strings"},
	{"AXFR",  252, "qtype: A request for a transfer of an entire zone"},
	{"MAILB", 253, "qtype: A request for mailbox-related records (MB, MG or MR)"},
	{"MAILA", 254, "qtype: A request for mail agent RRs (Obsolete - see MX)"},
	{"*",     255, "qtype: A request for all records"},
	{nullptr,       0,   nullptr}
};

struct ClassType
{
	const char *name;
	int value;
	const char *description;
};

static ClassType class_types[] =
{
	{"IN",    1,   "the Internet"},
	{"CS",    2,   "the CSNET class (Obsolete)"},
	{"CH",    3,   "the CHAOS class"},
	{"HS",    4,   "Hesiod [Dyer 87]"},
	{"*",     255, "qclass: Any class"},
	{nullptr,       0,   nullptr}
};

int DNSResourceRecord::type_to_int(const std::string &qtype)
{
	std::string qtype_local8 = StringHelp::text_to_local8(qtype);
	for (int index = 0; rr_types[index].name != nullptr; index++)
	{
		if (rr_types[index].name == qtype_local8)
			return rr_types[index].value;
	}

	int value = StringHelp::local8_to_int(qtype_local8);
	if (value <= 0)
		throw Exception("Unknown DNS resource record type " + qtype);
	return value;
}

std::string DNSResourceRecord::type_from_int(int qtype)
{
	for (int index = 0; rr_types[index].name != nullptr; index++)
	{
		if (rr_types[index].value == qtype)
			return StringHelp::local8_to_text(rr_types[index].name);
	}
	return StringHelp::int_to_text(qtype);
}

std::string DNSResourceRecord::type_description(const std::string &qtype)
{
	std::string qtype_local8 = StringHelp::text_to_local8(qtype);
	for (int index = 0; rr_types[index].name != nullptr; index++)
	{
		if (rr_types[index].name == qtype_local8)
			return StringHelp::local8_to_text(rr_types[index].description);
	}
	return "Unknown type " + qtype;
}

std::string DNSResourceRecord::type_description(int qtype)
{
	for (int index = 0; rr_types[index].name != nullptr; index++)
	{
		if (rr_types[index].value == qtype)
			return StringHelp::local8_to_text(rr_types[index].description);
	}
	return "Unknown type " + StringHelp::int_to_text(qtype);
}

int DNSResourceRecord::class_to_int(const std::string &qclass)
{
	std::string qclass_local8 = StringHelp::text_to_local8(qclass);
	for (int index = 0; class_types[index].name != nullptr; index++)
	{
		if (class_types[index].name == qclass_local8)
			return class_types[index].value;
	}

	int value = StringHelp::local8_to_int(qclass_local8);
	if (value <= 0)
		throw Exception("Unknown DNS resource record type " + qclass);
	return value;
}

std::string DNSResourceRecord::class_from_int(int qclass)
{
	for (int index = 0; class_types[index].name != nullptr; index++)
	{
		if (class_types[index].value == qclass)
			return StringHelp::local8_to_text(class_types[index].name);
	}
	return StringHelp::int_to_text(qclass);
}

std::string DNSResourceRecord::class_description(const std::string &qclass)
{
	std::string qclass_local8 = StringHelp::text_to_local8(qclass);
	for (int index = 0; class_types[index].name != nullptr; index++)
	{
		if (class_types[index].name == qclass_local8)
			return StringHelp::local8_to_text(class_types[index].description);
	}
	return "Unknown class " + qclass;
}

std::string DNSResourceRecord::class_description(int qclass)
{
	for (int index = 0; class_types[index].name != nullptr; index++)
	{
		if (class_types[index].value == qclass)
			return StringHelp::local8_to_text(class_types[index].description);
	}
	return "Unknown class " + StringHelp::int_to_text(qclass);
}

int DNSResourceRecord::find_domain_name_end(const DataBuffer &packet, int offset)
{
	const unsigned char *data = (const unsigned char *) packet.get_data();
	while (true)
	{
		if (offset >= packet.get_size())
			throw Exception("Premature end of resource data domain name");

		if (data[offset] > 0 && data[offset] < 64)
		{
			offset += data[offset] + 1;
		}
		else if ((data[offset] & 0xc0) == 0xc0)
		{
			offset += 2;
			break;
		}
		else if (data[offset] == 0)
		{
			offset++;
			break;
		}
		else
		{
			throw Exception("Malformed resource data domain name (find_domain_name_end)");
		}
	}
	return offset;
}

std::string DNSResourceRecord::read_domain_name(const DataBuffer &packet, int offset)
{
	std::string name;
	const unsigned char *data = (const unsigned char *) packet.get_data();
	while (true)
	{
		if (offset >= packet.get_size())
			throw Exception("Premature end of resource data domain name");

		if (data[offset] > 0 && data[offset] < 64)
		{
			if (offset+1+data[offset] >= packet.get_size())
				throw Exception("Premature end of resource data domain name");

			if (!name.empty())
				name.append(".");

			name.append(StringHelp::local8_to_text(std::string((const char *) data+offset+1, data[offset])));
			offset += data[offset] + 1;

			if (name.length() > 512)
				throw Exception("Domain name exceeds 512 characters!");
		}
		else if ((data[offset] & 0xc0) == 0xc0)
		{
			if (offset+1 >= packet.get_size())
				throw Exception("Premature end of resource data domain name");

			offset = (int(data[offset] & 0x3f) << 8) + data[offset+1];
		}
		else if (data[offset] == 0)
		{
			break;
		}
		else
		{
			throw Exception("Malformed resource data domain name");
		}
	}
	return name;
}

/////////////////////////////////////////////////////////////////////////////
// DNSResourceRecord Implementation:

}
