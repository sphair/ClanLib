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

#include "ip_address_getaddr.h"
#include "API/Core/System/error.h"
#include "API/Core/System/mutex.h"
#include "API/Network/Socket/socket.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress construction:

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr() : CL_IPAddress_Generic(), info(0)
{
	// The error handling here isn't the best, ie it leaks memory
	memset(&hints,0,sizeof(hints));
	
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	int error = getaddrinfo(NULL, "0", &hints, &info);
	if(error)
		throw CL_Error(gai_strerror(error));

	hints.ai_socktype = SOCK_DGRAM;
	addrinfo *foo;
	error = getaddrinfo(NULL, "0", &hints, &foo);
	if(error)
		throw CL_Error(gai_strerror(error));
	
	addrinfo *tmp = info;
	while(tmp->ai_next)
	{
		tmp = tmp->ai_next;
	}
	tmp->ai_next = &foo[0];

	// Both lists have to merged by use because getaddrinfo can't return both at once.
}

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr(const std::string &port) :CL_IPAddress_Generic(), info(0)
{
	set_port(port);
}

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr(const std::string &hostname, const std::string &port) : CL_IPAddress_Generic(), info(0)
{
	set_address(hostname, port);
}

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr(const CL_IPAddress &copy) :CL_IPAddress_Generic(), info(0)
{
	set_address(copy.get_address(), copy.get_port());
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress attributes:

void CL_IPAddress_GetAddr::get_addrinfo(int type, sockaddr &addr, int &len, int domain) const
{
	addrinfo *tmp = info;

	while(tmp != NULL)
	{
		if(type == CL_Socket::tcp && (tmp->ai_socktype == SOCK_STREAM) && (tmp->ai_family == domain))
		{
			len = tmp->ai_addrlen;
			memcpy(&addr, tmp->ai_addr, tmp->ai_addrlen);
			return;
		}
		else if(type == CL_Socket::udp && (tmp->ai_socktype == SOCK_DGRAM) && (tmp->ai_family == domain))
		{
			len = tmp->ai_addrlen;
			memcpy(&addr,tmp->ai_addr,len);
			return;
		}
		tmp = tmp->ai_next;
	}
	
	throw CL_Error("CL_Socket: Could not find appropriate socket");
}

bool CL_IPAddress_GetAddr::operator == (const CL_IPAddress &other) const
{
	if(get_address() == other.get_address() && get_port() == other.get_port())
		return true;
	else
		return false;
}

bool CL_IPAddress_GetAddr::operator < (const CL_IPAddress &other) const
{
	if(get_address() == other.get_address())
		return atoi(get_port().c_str()) < atoi(other.get_port().c_str());
	else
		return get_address() < other.get_address();
}

bool CL_IPAddress_GetAddr::operator > (const CL_IPAddress &other) const
{
	if(get_address() == other.get_address())
		return atoi(get_port().c_str()) > atoi(other.get_port().c_str());
	else
		return get_address() > other.get_address();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress operations:

void CL_IPAddress_GetAddr::set_address(const std::string &hostname, const std::string &port)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = 0; // For now
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (info)
	{
		freeaddrinfo(info);
		info = NULL;
	}

	int error = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &info);

	if(error)
		throw CL_Error(gai_strerror(error));

	addrinfo *udp;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_DGRAM;
	error = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &udp);

	if( error )
		throw CL_Error(gai_strerror(error));

	addrinfo *tmp = &info[0];

	while(tmp->ai_next != NULL)
	{
		tmp = tmp->ai_next;
	}

	tmp->ai_next = udp;
/*
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = 0; // For now
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if(info)
	{
		freeaddrinfo(info);
		info = NULL;
	}

	int error = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &info);

	if(error)
		throw CL_Error(gai_strerror(error));
	*/
}

void CL_IPAddress_GetAddr::set_port(const std::string &port)
{
	addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if(info)
	{
		freeaddrinfo(info);
		info = NULL;
	}

	int error = getaddrinfo(NULL, port.c_str(), &hints, &info);

	if(error)
		throw CL_Error(gai_strerror(error));

	addrinfo *udp;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_DGRAM;
	error = getaddrinfo(NULL, port.c_str(), &hints, &udp);

	if( error )
		throw CL_Error(gai_strerror(error));

	addrinfo *tmp = &info[0];

	while(tmp->ai_next != NULL)
	{
		tmp = tmp->ai_next;
	}

	tmp->ai_next = udp;
}

std::string CL_IPAddress_GetAddr::dns_lookup() const
{
	char *name = new char[NI_MAXHOST];
	int error = getnameinfo(info[0].ai_addr, info[0].ai_addrlen, name, NI_MAXHOST, NULL, 0, 0);

	if(error)
	{
		delete[] name;
		throw CL_Error(gai_strerror(error));
	}
	
	std::string ret(name);
	delete[] name;
	
	return ret;
}

std::string CL_IPAddress_GetAddr::get_address() const
{
	char *addr = new char[NI_MAXHOST]; //Kinda ridiculous to allocate 1025 bytes for an address
	memset(addr, 0, NI_MAXHOST);

	int error = getnameinfo(info[0].ai_addr, info[0].ai_addrlen, addr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

	if(error)
	{
		delete[] addr;
		//const char *err = gai_strerror(error);
		throw CL_Error((std::string)"Error in CL_IPAddress::get_address: " + (std::string)gai_strerror(error));
	}
	
	std::string ret(addr);
	delete[] addr;
	
	return ret;
}

std::string CL_IPAddress_GetAddr::get_port() const
{
	char *port = new char[NI_MAXSERV];
	memset(port, 0, NI_MAXSERV);
	
	int error = getnameinfo(info[0].ai_addr, info[0].ai_addrlen, NULL, 0, port, NI_MAXSERV, NI_NUMERICSERV);
	
	if(error)
	{
		delete[] port;
		throw CL_Error((std::string)"Error in CL_IPAddress::get_address: " + (std::string)gai_strerror(error));
	}

	std::string ret(port);
	delete[] port;
	
	return ret;
}

bool CL_IPAddress_GetAddr::is_ipv4() const
{
	addrinfo *tmp  = info;
	
	while(tmp != NULL)
	{
		if(tmp->ai_family == PF_INET)
			return true;
		tmp = tmp->ai_next;
	}
	
	return false;
}

bool CL_IPAddress_GetAddr::is_ipv6() const
{
	addrinfo *tmp  = info;
	
	while(tmp != NULL)
	{
		if(tmp->ai_family == PF_INET6)
			return true;
		tmp = tmp->ai_next;
	}
	
	return false;
}

#endif
