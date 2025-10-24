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
#define CL_IPAddress_Platform CL_IPAddress_GetAddr
#else
#include "ip_address_getsock.h"
#define CL_IPAddress_Platform CL_IPAddress_GetSock
#endif

CL_IPAddress::CL_IPAddress()
{
	impl = new CL_IPAddress_Platform;
}

CL_IPAddress::CL_IPAddress(const std::string &port)
{
	impl = new CL_IPAddress_Platform(port);
}

CL_IPAddress::CL_IPAddress(const std::string &hostname, const std::string &port)
{
	impl = new CL_IPAddress_Platform(hostname, port);
}

CL_IPAddress::CL_IPAddress(const CL_IPAddress &copy)
{
	impl = new CL_IPAddress_Platform(copy);
}

void CL_IPAddress::get_addrinfo(int type, sockaddr &addr, int &len, int domain) const
{
	impl->get_addrinfo(type, addr, len, domain);
}

std::string CL_IPAddress::get_address() const
{
	return impl->get_address();
}

std::string CL_IPAddress::get_port() const
{
	return impl->get_port();
}

bool CL_IPAddress::operator == (const CL_IPAddress &other_instance) const
{
	return impl->operator ==(other_instance);
}

bool CL_IPAddress::operator < (const CL_IPAddress &other_instance) const
{
	return impl->operator <(other_instance);
}

bool CL_IPAddress::operator > (const CL_IPAddress &other_instance) const
{
	return impl->operator >(other_instance);
}

void CL_IPAddress::set_address(const std::string &hostname, const std::string &port)
{
	impl->set_address(hostname, port);
}

void CL_IPAddress::set_port(const std::string &new_port)
{
	impl->set_port(new_port);
}

std::string CL_IPAddress::dns_lookup() const
{
	return impl->dns_lookup();
}

bool CL_IPAddress::is_ipv4() const
{
	return impl->is_ipv4();
}

bool CL_IPAddress::is_ipv6() const
{
	return impl->is_ipv6();
}
