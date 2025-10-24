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

//! clanNetwork="Sockets"
//! header=network.h

#ifndef header_ip_address
#define header_ip_address

#ifdef CL_API_DLL
#ifdef CL_NETWORK_EXPORT
#define CL_API_NETWORK __declspec(dllexport)
#else
#define CL_API_NETWORK __declspec(dllimport)
#endif
#else
#define CL_API_NETWORK
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>

class CL_IPAddress_Generic;

//: Socket name; container class for an IP address and port.
//- !group=Network/Sockets!
//- !header=network.h!
class CL_API_NETWORK CL_IPAddress
{
//! Construction:
public:
	//: Constructs an IP address using INADDR_ANY and port 0.
	CL_IPAddress();

	//: Constructs an IP address using INADDR_ANY and the specified port.
	CL_IPAddress(const std::string &port);

	//: Construct an IP address using the hostname and the port.
	CL_IPAddress(const std::string &hostname, const std::string &port);

	//: Copy constructor.
	CL_IPAddress(const CL_IPAddress &copy);

//! Attributes:
public:
	//: Returns the internal addrinfo struct
	void get_addrinfo(int type, struct sockaddr &addr, int &len, int domain) const;

	//: Returns the IP address as a string.
	std::string get_address() const;

	//: Returns the IP port as a string.
	std::string get_port() const;

	//: Returns true if objects are the same.
	bool operator == (const CL_IPAddress &other_instance) const;

	//: Returns true if the other address is less.
	//- <p>This is used for sorting
	//- purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//- the address based on lowest IP number address.</p>
	bool operator < (const CL_IPAddress &other_instance) const;

	//: Returns true if the other address is greater.
	//- <p>This is used for sorting
	//- purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//- the address based on lowest IP number address.</p>
	bool operator > (const CL_IPAddress &other_instance) const;

//! Operations:
public:

	//: Set the IP address using a hostname and port.
	//param hostname: Can be an IPv4 dotted-quad, hostname or a valid IPv6 address.
	//param port: Port number.
	void set_address(const std::string &hostname, const std::string &port);

	//: Set the IP port.  Note, this will reset the hostname to the default value of any host.
	void set_port(const std::string &port);

	//: Do a DNS lookup on the IP address.
	std::string dns_lookup() const;
	
	//: Check to see if the address has an IPv4 mapping
	//: is_ipv4() and is_ipv6() are not mutally exclusive
	//: An address can have both IPv4 and IPv6 mappings
	bool is_ipv4() const;
	
	//: Check to see if the address has an IPv6 mapping
	//: is_ipv4() and is_ipv6() are not mutally exclusive
	//: An address can have both IPv4 and IPv6 mappings
	bool is_ipv6() const;

//! Implementation:
private:
	CL_IPAddress_Generic *impl;
};

#endif
