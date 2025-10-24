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

#ifndef ip_address_getaddr_header
#define ip_address_getaddr_header

#include "ip_address_generic.h"

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#define _inline inline
	#include <ws2tcpip.h>
	#include <tpipv6.h>
	#include <windows.h>
#endif

//: Socket name; container class for an IP address and port.
class CL_IPAddress_GetAddr : public CL_IPAddress_Generic
{
//! Construction:
public:
	//: Constructs an IP address using INADDR_ANY and port 0.
	CL_IPAddress_GetAddr();

	//: Constructs an IP address using INADDR_ANY and the specified port.
	CL_IPAddress_GetAddr(const std::string &port);

	//: Constructs an IP address using the passed address.
//	CL_IPAddress(const std::string &hostname);

	//: Construct an IP address using the hostname and the port.
	CL_IPAddress_GetAddr(const std::string &hostname, const std::string &port);

	//: Copy constructor.
	CL_IPAddress_GetAddr(const CL_IPAddress &copy);

	virtual ~CL_IPAddress_GetAddr() {}

//! Attributes:
public:
	//: Returns the internal addrinfo struct
	void get_addrinfo(int type, sockaddr &addr, int &len, int domain) const;

	//: Returns the IP address in network byte order.
	std::string get_address() const;

	//: Returns the IP port in host byte order.
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
	//: Set the IP address using a hostname.
	//param hostname: Can be an IPv4 dotted-quad, hostname or a valid IPv6 address.  
	// The port number cannot be passed here.
	// Port number is reassigned to default(any port)


	//: Sets both the address and the port.
	void set_address(const std::string &hostname, const std::string &port);

	//: Set the IP port.
	void set_port(const std::string &new_port);

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

private:
	addrinfo hints;
	addrinfo *info;
	
	//: Will merge two addrinfo lists into one.
	void merge_lists(addrinfo *first, addrinfo *second);
};


#endif
