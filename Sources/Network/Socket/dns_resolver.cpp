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
#include "API/Network/Socket/dns_resolver.h"
#include "API/Network/Socket/dns_packet.h"
#include "API/Network/Socket/dns_resource_record.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/file.h"
#include "dns_resolver_impl.h"
#ifdef WIN32
#include <iphlpapi.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DNSResolver Construction:

CL_DNSResolver::CL_DNSResolver()
: impl(new CL_DNSResolver_Impl)
{
#ifdef WIN32
	CL_DataBuffer buffer(16*1024);
	ULONG buffer_length = buffer.get_size();
	DWORD result = GetNetworkParams((PFIXED_INFO) buffer.get_data(), &buffer_length);
	if (result == ERROR_BUFFER_OVERFLOW)
	{
		buffer.set_size(buffer_length * 2);
		buffer_length *= 2;
		result = GetNetworkParams((PFIXED_INFO) buffer.get_data(), &buffer_length);
	}
	if (result != ERROR_SUCCESS)
		throw CL_Exception("Unable to get network parameters");

	PFIXED_INFO fixed_info = (PFIXED_INFO) buffer.get_data();
	IP_ADDR_STRING *cur = &fixed_info->DnsServerList;
	while (cur)
	{
		CL_String dns_server = CL_StringHelp::local8_to_text(cur->IpAddress.String);
		if (!dns_server.empty())
		impl->dns_servers.push_back(CL_SocketName(dns_server, "53"));
		cur = cur->Next;
	}
#else
	// Convert file into lines:
	CL_File file("/etc/resolv.conf", CL_File::open_existing, CL_File::access_read);
	CL_DataBuffer buffer(file.get_size());
	file.read(buffer.get_data(), buffer.get_size());
	char *d = (char *) buffer.get_data();
	int size = buffer.get_size();
	int last_pos = 0;
	std::vector<CL_String8> lines;
	int i;
	for (i=0; i<size; i++)
	{
		if (d[i] == '\n')
		{
			lines.push_back(CL_String8(d+last_pos, i-last_pos));
			last_pos = i+1;
		}
	}
	if (last_pos != size)
		lines.push_back(CL_String8(d+last_pos, size-last_pos));

	std::vector<CL_String8>::size_type lines_index;
	for (lines_index = 0; lines_index < lines.size(); lines_index++)
	{
		CL_String8 &line = lines[lines_index];

		CL_String8::size_type keyword_start = line.find_first_not_of(" \t");
		if (keyword_start == CL_String8::npos)
			continue;
		CL_String8::size_type keyword_end = line.find_first_of(" \t", keyword_start);
		if (keyword_end == CL_String8::npos)
			keyword_end = line.length();

		CL_String8 keyword = line.substr(keyword_start, keyword_end - keyword_start);
		if (keyword != "nameserver")
			continue;

		CL_String8::size_type param1_start = line.find_first_not_of(" \t", keyword_end);
		if (param1_start == CL_String8::npos)
			continue;
		CL_String8::size_type param1_end = line.find_first_of(" \t", param1_start);
		if (param1_end == CL_String8::npos)
			param1_end = line.length();

		CL_String8 dns_server = line.substr(param1_start, param1_end - param1_start);
		impl->dns_servers.push_back(CL_SocketName(dns_server, "53"));

	}
	if (impl->dns_servers.empty())
		throw CL_Exception("No dns servers found in /etc/resolv.conf");
#endif
}

CL_DNSResolver::~CL_DNSResolver()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DNSResolver Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DNSResolver Operations:

std::vector<CL_DNSResourceRecord> CL_DNSResolver::lookup_resource(
		const CL_String &domain_name,
		const CL_String &resource_type,
		int timeout)
{
	CL_String dns_server = impl->dns_servers[0].get_address();

	for (int i=0; i<25; i++)
	{
		CL_DNSPacket packet = perform_query(
			domain_name, resource_type, timeout, dns_server);

		if (packet.is_truncated())
			throw CL_Exception("Unable to lookup DNS resource; truncated DNS answer packet");

		switch (packet.get_response_code())
		{
		case CL_DNSPacket::response_ok:
			break;
		case CL_DNSPacket::response_format_error:
			throw CL_Exception("Unable to lookup DNS resource; format error");
		case CL_DNSPacket::response_server_failure:
			throw CL_Exception("Unable to lookup DNS resource; server failure");
		case CL_DNSPacket::response_name_error:
			throw CL_Exception("Unable to lookup DNS resource; name error");
		case CL_DNSPacket::response_not_implemented:
			throw CL_Exception("Unable to lookup DNS resource; not implemented");
		case CL_DNSPacket::response_refused:
			throw CL_Exception("Unable to lookup DNS resource; refused");
		default:
			throw CL_Exception("Unable to lookup DNS resource; unknown error");
		}

		// Does this DNS server know the answer?
		CL_String domain_name_cname;
		std::vector<CL_DNSResourceRecord> results;
		int j;
		for (j=0; j<packet.get_answer_count(); j++)
		{
			CL_DNSResourceRecord record = packet.get_answer(j);

			if (record.get_name() == domain_name && record.get_type() == "CNAME")
				domain_name_cname = record.get_cname_cname();

			if (record.get_name() == domain_name && record.get_type() == resource_type)
				results.push_back(record);
		}
		for (j=0; j<packet.get_additional_count(); j++)
		{
			CL_DNSResourceRecord record = packet.get_additional(j);

			if (record.get_name() == domain_name && record.get_type() == "CNAME")
				domain_name_cname = record.get_cname_cname();

			if (record.get_name() == domain_name && record.get_type() == resource_type)
				results.push_back(record);
		}
		if (!results.empty())
			return results;

		// Check for CNAME redirected answers:
		if (!domain_name_cname.empty())
		{
			for (j=0; j<packet.get_answer_count(); j++)
			{
				CL_DNSResourceRecord record = packet.get_answer(j);
				if (record.get_name() == domain_name_cname && record.get_type() == resource_type)
					results.push_back(record);
			}
			for (j=0; j<packet.get_additional_count(); j++)
			{
				CL_DNSResourceRecord record = packet.get_additional(j);
				if (record.get_name() == domain_name_cname && record.get_type() == resource_type)
					results.push_back(record);
			}
			if (!results.empty())
				return results;
		}

		// Does it know someone who does?
		if (packet.get_nameserver_count() > 0)
		{
			CL_DNSResourceRecord rr = packet.get_nameserver(0);
			if (rr.get_type() != "NS")
				throw CL_Exception("Unable to lookup DNS resource");
			dns_server = rr.get_ns_nsdname();
			continue;
		}

		// Looks like this resource does not exist.
		throw CL_Exception("DNS resource data not found");
	}

	std::vector<CL_DNSResourceRecord> results;
	return results;
}

CL_DNSPacket CL_DNSResolver::perform_query(
	CL_DNSPacket &packet,
	int timeout,
	const CL_String &dns_server_name)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	int query_id = impl->query_id++;
	if (impl->query_id > 0xffff)
		impl->query_id = 0;
	packet.set_query_id(query_id);
	impl->queries[query_id] = packet;
	mutex_lock.unlock();

	for (int i = 0; i < timeout; i += 1000)
	{
		CL_SocketName dns_server(dns_server_name, "53");
		impl->udp_socket.send(
			packet.get_data().get_data(),
			packet.get_data().get_size(),
			dns_server);
		impl->event_bound.set();

		for (int j = 0; j < 10; j++)
		{
			CL_System::sleep(100);
			mutex_lock.lock();
			std::map<int, CL_DNSPacket>::iterator it;
			it = impl->answers.find(query_id);
			if (it != impl->answers.end())
			{
				CL_DNSPacket result = it->second;
				impl->answers.erase(it);
				impl->queries.erase(impl->queries.find(query_id));
				return result;
			}
			mutex_lock.unlock();
		}
	}

	impl->queries.erase(impl->queries.find(query_id));
	throw CL_Exception("Unable to perform lookup");
	return CL_DNSPacket();
}

CL_DNSPacket CL_DNSResolver::perform_query(
	const CL_String &domain_name,
	const CL_String &resource_type,
	int timeout,
	const CL_String &dns_server_name)
{
	CL_DNSPacket packet(
		0,
		CL_DNSPacket::opcode_query,
		true,
		domain_name,
		CL_DNSResourceRecord::type_to_int(resource_type),
		CL_DNSResourceRecord::class_to_int("IN"));
	return perform_query(packet, timeout, dns_server_name);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DNSResolver Implementation:
