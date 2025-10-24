/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifdef USE_GETADDR

// msvc got a strange bug that makes #ifndef USE_GETADDR sometimes not work!!
// thats why we check for USE_GETADDR and not the opposite.
// -- mbn 20 dec 2002

#else

#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "ip_address_getsock.h"

#include <cstdlib>

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#include <windows.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress construction:

CL_IPAddress_GetSock::CL_IPAddress_GetSock()
: ip(INADDR_ANY), port(0)
{
}

CL_IPAddress_GetSock::CL_IPAddress_GetSock(const std::string &new_port)
: ip(INADDR_ANY)
{
	port = CL_String::to_int(new_port);
}

CL_IPAddress_GetSock::CL_IPAddress_GetSock(const std::string &hostname, const std::string &new_port)
{
	port = CL_String::to_int(new_port);

	unsigned int addr = 0;

	addr = inet_addr(hostname.c_str());
	if (addr == INADDR_NONE)
	{
		hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) throw CL_Error("Could not lookup DNS name");

		addr = *((unsigned int*) host->h_addr_list[0]);
	}

	ip = addr;
}

CL_IPAddress_GetSock::CL_IPAddress_GetSock(const CL_IPAddress &copy)
{
	set_address(copy.get_address(),copy.get_port());
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress attributes:

void CL_IPAddress_GetSock::get_addrinfo(int type, sockaddr &addr, int &len, int domain) const
{
	// Domain ignored since it will always be ipv4
	sockaddr_in ret;
	ret.sin_family = AF_INET;
	ret.sin_port = htons(port);
	ret.sin_addr.s_addr = ip;
	len = sizeof(sockaddr_in);
	memcpy(&addr, &ret, len);
}

std::string CL_IPAddress_GetSock::get_address() const
{
	struct in_addr foo;
	foo.s_addr = ip;
	char *addr = inet_ntoa(foo);
	return std::string(addr);	
}

std::string CL_IPAddress_GetSock::get_port() const
{
	return CL_String::from_int(port);
}

bool CL_IPAddress_GetSock::operator == (const CL_IPAddress &other) const
{
	return (get_address() == other.get_address()) && (get_port() == other.get_port());
}

bool CL_IPAddress_GetSock::operator < (const CL_IPAddress &other) const
{
	if (get_address() == other.get_address())
		return atoi(get_port().c_str()) < atoi(other.get_port().c_str());
	return get_address() < other.get_address();
}

bool CL_IPAddress_GetSock::operator > (const CL_IPAddress &other) const
{
	if (get_address() == other.get_address())
		return atoi(get_port().c_str()) > atoi(other.get_port().c_str());
	return get_address() > other.get_address();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress operations:


void CL_IPAddress_GetSock::set_address(const std::string &hostname)
{
	unsigned int addr = 0;

	addr = inet_addr(hostname.c_str());
	if (addr == INADDR_NONE)
	{
		hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) throw CL_Error("Could not lookup DNS name");

		addr = *((unsigned int*) host->h_addr_list[0]);
	}

	ip = addr;
}


void CL_IPAddress_GetSock::set_address(const std::string &hostname, const std::string &port)
{
	set_port(port);
	set_address(hostname);
}

void CL_IPAddress_GetSock::set_port(const std::string &new_port)
{
	port = atoi(new_port.c_str());
	ip = INADDR_ANY;
}

std::string CL_IPAddress_GetSock::dns_lookup() const
{
	hostent *host = gethostbyaddr((const char *) &ip, sizeof(ip), AF_INET);
	if (host == NULL) throw CL_Error("Unable to lookup IP address");
	return std::string(host->h_name);
}

bool CL_IPAddress_GetSock::is_ipv4() const
{
	return true;
}

bool CL_IPAddress_GetSock::is_ipv6() const
{
	return false;
}

#endif
