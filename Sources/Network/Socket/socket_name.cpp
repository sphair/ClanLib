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
#include "API/Network/Socket/socket_name.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "socket_name_impl.h"
#ifdef WIN32
typedef unsigned long in_addr_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifndef WIN32
#include "stdlib.h"
#include "string.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_SocketName construction:

CL_SocketName::CL_SocketName()
: impl(new CL_SocketName_Impl)
{
}

CL_SocketName::CL_SocketName(const CL_String &port)
: impl(new CL_SocketName_Impl)
{
	impl->port = port;
}

CL_SocketName::CL_SocketName(const CL_String &address, const CL_String &port)
: impl(new CL_SocketName_Impl)
{
	impl->address = address;
	impl->port = port;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SocketName attributes:

CL_String CL_SocketName::get_address() const
{
	return impl->address;
}
	
CL_String CL_SocketName::get_port() const
{
	return impl->port;
}

bool CL_SocketName::operator == (const CL_SocketName &other_instance) const
{
	if (CL_StringHelp::compare(other_instance.impl->address, impl->address, true) != 0)
		return false;
	if (CL_StringHelp::compare(other_instance.impl->port, impl->port, true) != 0)
		return false;
	return true;
}

bool CL_SocketName::operator < (const CL_SocketName &other_instance) const
{
	int difference = CL_StringHelp::compare(other_instance.impl->address, impl->address, true);
	if (difference == 0)
		difference = CL_StringHelp::compare(other_instance.impl->port, impl->port, true);
	return difference < 0;
}

bool CL_SocketName::operator > (const CL_SocketName &other_instance) const
{
	int difference = CL_StringHelp::compare(other_instance.impl->address, impl->address, true);
	if (difference == 0)
		difference = CL_StringHelp::compare(other_instance.impl->port, impl->port, true);
	return difference > 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SocketName operations:

void CL_SocketName::set_name(const CL_String &hostname, const CL_String &port)
{
	impl->address = hostname;
	impl->port = port;
}
	
void CL_SocketName::set_address(const CL_String &address)
{
	impl->address = address;
}

void CL_SocketName::set_port(const CL_String &port)
{
	impl->port = port;
}

CL_String CL_SocketName::lookup_ipv4() const
{
	in_addr_t ipv4_address = inet_addr(CL_StringHelp::text_to_local8(impl->address).c_str());
	if (ipv4_address == INADDR_NONE)
	{
		hostent *host = gethostbyname(CL_StringHelp::text_to_local8(impl->address).c_str());
		if (host == 0)
			throw CL_Exception("Could not lookup DNS name");
		
		ipv4_address = *((in_addr_t *) host->h_addr_list[0]);
	}

	return CL_String();
}

CL_String CL_SocketName::lookup_hostname() const
{
	in_addr_t ipv4_address = inet_addr(CL_StringHelp::text_to_local8(impl->address).c_str());
	if (ipv4_address != INADDR_NONE)
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(CL_StringHelp::text_to_int(impl->port));
		addr.sin_addr.s_addr = ipv4_address;

		hostent *host = gethostbyaddr((const char *) &addr, sizeof(sockaddr_in), AF_INET);
		if (host == 0)
			throw CL_Exception("Could not lookup DNS name");
		
		return CL_StringHelp::local8_to_text(host->h_name);
	}

	return impl->address;
}
	
CL_SocketName CL_SocketName::to_ipv4()
{
	return CL_SocketName(lookup_ipv4(), impl->port);
}
	
CL_SocketName CL_SocketName::to_hostname()
{
	return CL_SocketName(lookup_hostname(), impl->port);
}

void CL_SocketName::to_sockaddr(int domain, struct sockaddr *out_addr, int len) const
{
	if (domain != AF_INET)
		throw CL_Exception("Only AF_INET domain supported for CL_SocketName::to_sockaddr");

	if (len < sizeof(sockaddr_in))
		throw CL_Exception("Insufficient buffer for sockaddr_in structure");

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(CL_StringHelp::text_to_int(impl->port));
	if (impl->address.empty())
	{
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(CL_StringHelp::text_to_local8(impl->address).c_str());
		if (addr.sin_addr.s_addr == INADDR_NONE)
		{
			hostent *host = gethostbyname(CL_StringHelp::text_to_local8(impl->address).c_str());
			if (host == 0)
				throw CL_Exception("Could not lookup DNS name");
		
			addr.sin_addr.s_addr = *((in_addr_t *) host->h_addr_list[0]);
		}
	}

	memcpy(out_addr, &addr, sizeof(sockaddr_in));
}

void CL_SocketName::from_sockaddr(int domain, struct sockaddr *addr, int len)
{
	if (domain != AF_INET)
		throw CL_Exception("Only AF_INET domain supported for CL_SocketName::to_sockaddr");

	if (len < sizeof(sockaddr_in))
		throw CL_Exception("Insufficient buffer for sockaddr_in structure");

	sockaddr_in *addr_in = (sockaddr_in *) addr;
	if (addr_in->sin_family != AF_INET)
		throw CL_Exception("Unexpected sin_family in sockaddr");

	unsigned long addr_long = (unsigned long) ntohl(addr_in->sin_addr.s_addr);
	CL_String port = CL_StringHelp::int_to_text(ntohs(addr_in->sin_port));
	
	if (addr_in->sin_addr.s_addr == INADDR_ANY)
	{
		impl->address = CL_String();
	}
	else
	{
		CL_String str_addr = cl_format(
			"%1.%2.%3.%4",
			int((addr_long & 0xff000000) >> 24),
			int((addr_long & 0x00ff0000) >> 16),
			int((addr_long & 0x0000ff00) >> 8),
			int((addr_long & 0x000000ff)));

		impl->address = str_addr;
	}

	impl->port = port;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SocketName implementation:
