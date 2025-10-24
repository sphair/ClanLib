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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#ifndef WIN32
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h> // needed for ioctl ( on linux and bsd )
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>    // needed for struct ifreq and struct ifconf
#define INVALID_SOCKET -1
#endif
#include <cstring>

#include "socket_generic.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"

// No socklen_t on MacOSX
#if defined(__APPLE__) && ( __GNUC__ < 4 ) && ( __GNUC_MINOR__ < 3 )
#define socklen_t int
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Socket construction:


// This is really bad to use.  It doesn't, and can't, properly set the domain variable
// which is used by GetAddr.  
CL_Socket::CL_Socket()
: impl(0)
{
}

CL_Socket::CL_Socket(int socket)
: impl(new CL_Socket_Generic)
{
	impl->add_ref();
	impl->sock = socket;

	input.socket = impl;
	output.socket = impl;
}

CL_Socket::CL_Socket(CL_Socket::Type type, Protocol protocol)
: impl(new CL_Socket_Generic)
{
	impl->add_ref();

	int domain;
	int in_type = SOCK_STREAM, in_proto = 0;

	switch (type)
	{
	case tcp: in_type = SOCK_STREAM; break;
	case udp: in_type = SOCK_DGRAM; break;
	}

	switch(protocol)
	{
	case ipv4:
		domain = PF_INET;
		break;
	case ipv6: 
		domain = PF_INET6; 
		break;
	default:
		throw CL_Error("CL_Socket::CL_Socket failed: Invalid protocol specified");
		domain = 0;
		break;
	}
	
	impl->sock = socket(domain, in_type, in_proto);
	impl->domain = domain;
	impl->protocol = in_proto;
	proto = type;
	if (impl->sock < 0) throw CL_Error("CL_Socket::CL_Socket failed: " + CL_Socket_Generic::get_error_message());

	input.socket = impl;
	output.socket = impl;
}

CL_Socket::CL_Socket(const CL_Socket &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
	input.socket = impl;
	output.socket = impl;
}

CL_Socket::~CL_Socket()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket attributes:

int CL_Socket::get_socket() const
{
	return impl->sock;
}

int CL_Socket::get_max_msg_size() const
{
/*
	From RFC 879 ( http://www.faqs.org/rfcs/rfc879.html ):

	"HOSTS MUST NOT SEND DATAGRAMS LARGER THAN 576 OCTETS UNLESS THEY
	HAVE SPECIFIC KNOWLEDGE THAT THE DESTINATION HOST IS PREPARED TO
	ACCEPT LARGER DATAGRAMS.

	This is a long established rule."
*/         
	return 576;
}

CL_EventTrigger *CL_Socket::get_read_trigger() const
{
	if (impl->read == NULL) impl->read = impl->create_read_trigger();
	return impl->read;
}

CL_EventTrigger *CL_Socket::get_write_trigger() const
{
	if (impl->write == NULL) impl->write = impl->create_write_trigger();
	return impl->write;
}

CL_EventTrigger *CL_Socket::get_exception_trigger() const
{
	if (impl->exception == NULL) impl->exception = impl->create_exception_trigger();
	return impl->exception;
}

CL_IPAddress CL_Socket::get_source_address() const
{
//TODO:
// Fix this garbage
// IPv4 specific hack

	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	int size = sizeof(sockaddr_in);

	int result = getsockname(impl->sock, (sockaddr *) &addr_in, (socklen_t *)&size);
	if (result != 0) throw CL_Error("CL_Socket::get_source_address() failed.");

	return CL_Socket_Generic::create_ip_address(addr_in);
}

CL_IPAddress CL_Socket::get_dest_address() const
{
//TODO:
// Fix this garbage
// IPv4 specific hack

	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	int size = sizeof(sockaddr_in);

	int result = getpeername(impl->sock, (sockaddr *) &addr_in, (socklen_t *)&size);
	if (result != 0) throw CL_Error("CL_Socket::get_dest_address() failed.");

	return CL_Socket_Generic::create_ip_address(addr_in);
}

std::list<CL_IPAddress> CL_Socket::get_broadcast_addresses(const std::string &port) const
{
// Should be IPv6 safe

	std::list<CL_IPAddress> broadcast_addresses;

	// if not a IPv4 socket or not a UDP socket
	if(impl->domain != PF_INET)
		throw CL_Error("CL_Socket::get_broadcast_addresses() failed. Broadcasting is for IPv4 only");
	if(proto != udp)
		throw CL_Error("CL_Socket::get_broadcast_addresses() failed. Broadcasting is for UDP only");

#ifdef WIN32
	// win32 seems to always use 255.255.255.255 as its broadcast address
	broadcast_addresses.push_back(CL_IPAddress("255.255.255.255", port));
#else
	// UNIX
#ifdef SIOCGIFBRDADDR
	// get interfaces
	struct ifconf ifc;
	int len = 100 * sizeof(struct ifreq), lastlen = 0,status;
	char *buf = 0;
	for (;;)
	{
		buf = new char[len];
		ifc.ifc_len = len;
		ifc.ifc_buf = buf;
		status = ioctl(impl->sock, SIOCGIFCONF, &ifc);
		if (0 == status)
		{
			if(lastlen == ifc.ifc_len) {
				if(lastlen == 0) delete [] buf;
				break;
			}
			lastlen = ifc.ifc_len;
		}
		else
		{
			CL_Log::log("debug", "CL_Socket::get_broadcast_addresses(): ioctl error trying to get network interfaces");
			lastlen = 0;
			break;
		}
		len += 10 * sizeof(struct ifreq);
		delete [] buf;
	}

	if (lastlen != 0)
	{
		// found interfaces!
		char *ptr = buf;
		char *end_ptr = buf + ifc.ifc_len;
		std::string lastname;
		while (ptr < end_ptr)
		{
			struct ifreq *ifr = (struct ifreq *) ptr;

			// point to next item
			ptr += sizeof (ifr->ifr_name) + sizeof (struct sockaddr);

			// if it isn't IPv4, skip
			if (ifr->ifr_addr.sa_family != AF_INET) continue;

			// make aliases show up as the same as the interface
			char *cptr;
			if ((cptr = strchr(ifr->ifr_name, ':')) != 0) *cptr = '\0';

			// if we have processed this interface before, skip
			if (lastname == ifr->ifr_name) continue;
			// save the interface name
			lastname = ifr->ifr_name;

			// get interface flags
			struct ifreq ifrcopy = *ifr;
			status = ioctl(impl->sock, SIOCGIFFLAGS, &ifrcopy);
			int flags = ifrcopy.ifr_flags;

			// if could not get flags or interface not up or no broadcast address, skip interface
			if (0 != status || !(flags & IFF_UP) || !(flags & IFF_BROADCAST)) continue;

			// get interface broadcast address
			status = ioctl(impl->sock, SIOCGIFBRDADDR, &ifrcopy);
			if (0 == status)
			{
				broadcast_addresses.push_back(CL_IPAddress(inet_ntoa(((sockaddr_in*)&ifrcopy.ifr_broadaddr)->sin_addr),port));
			}
			else
			{
				CL_Log::log("debug", "CL_Socket::get_broadcast_addresses(): Can't get broadcast address for interface %1", lastname);
			}
		}
		delete [] buf;
	}

	if(broadcast_addresses.empty())
	{
		CL_Log::log("debug", "CL_Socket::get_broadcast_addresses(): Can't get broadcast address. Using 255.255.255.255");
		broadcast_addresses.push_back(CL_IPAddress("255.255.255.255", port));
	}
#else // SIOCGIFBRDADDR
	// default to 255.255.255.255 if we do not support getting broadcast addresses
	broadcast_addresses.push_back(CL_IPAddress("255.255.255.255", port));
#endif // SIOCGIFBRDADDR
#endif // WIN32
	return broadcast_addresses;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket operations:

CL_Socket &CL_Socket::operator =(const CL_Socket &other)
{
	if (impl) impl->release_ref();
	impl = other.impl;
	if (impl) impl->add_ref();
	input.socket = impl;
	output.socket = impl;
	return *this;
}

void CL_Socket::set_nonblocking(bool nonblocking)
{
#ifdef WIN32
	u_long argp = nonblocking ? 1 : 0;
	ioctlsocket(impl->sock, FIONBIO, &argp);
#else
	fcntl(impl->sock, F_SETFL, O_NONBLOCK, nonblocking ? 1 : 0);
#endif
}

void CL_Socket::set_nodelay(bool nodelay)
{
	static int sol_tcp = -1;
	if (sol_tcp == -1) sol_tcp = getprotobyname("tcp")->p_proto;

	int value = nodelay ? 1 : 0;
	setsockopt(impl->sock, sol_tcp, TCP_NODELAY, (const char *) &value, sizeof(int));
}

void CL_Socket::set_broadcasting(bool broadcasting)
{
	// if not a IPv4 socket or not a UDP socket
	if(impl->domain != PF_INET)
		throw CL_Error("CL_Socket::set_broadcasting() failed. Broadcasting is for IPv4 only");
	if(proto != udp)
		throw CL_Error("CL_Socket::set_broadcasting() failed. Broadcasting is for UDP only");

	int on = broadcasting ? 1 : 0;
	setsockopt(impl->sock, SOL_SOCKET, SO_BROADCAST, (const char *) &on, sizeof(on));
}

int CL_Socket::send(const void *data, int size)
{
	int result = ::send(
		impl->sock,
		(const char *) data,
		size,
		0);
	if (result < 0) throw CL_Error("CL_Socket::send failed");

	if (impl->write) impl->write->start_listen();
	return result;
}

int CL_Socket::send(const void *data, int size, const CL_IPAddress &dest)
{
	sockaddr addr;
	int len;

	dest.get_addrinfo(proto, addr, len, impl->domain);
	
	int result = ::sendto(
		impl->sock,
		(const char *) data,
		size,
		0,
		&addr,
		len);
	if (result < 0) throw CL_Error("CL_Socket::send failed");

	if (impl->write) impl->write->start_listen();
	return result;
}

int CL_Socket::send(const std::string &string)
{
	return send(string.c_str(), string.size());
}

int CL_Socket::recv(void *data, int size)
{
	int result = ::recv(impl->sock, (char *) data, size, 0);
	if (result < 0) throw CL_Error("CL_Socket::recv failed");

	if (impl->read) impl->read->start_listen();
	return result;
}

void CL_Socket::push(const std::string &string)
{
	impl->push_stack.push(string);
}

int CL_Socket::recv(void *data, int size, CL_IPAddress &from)
{
	char *_data = (char *) data;
	int total = 0;

	// Start filling data with pushed data (if any):
	while (!impl->push_stack.empty())
	{
		std::string &str = impl->push_stack.top();
		if (str.size() < (unsigned int)size)
		{
			memcpy(_data, str.data(), str.size());
			_data += str.size();
			total += str.size();
			size -= str.size();
			impl->push_stack.pop();
		}
		else
		{
			memcpy(_data, str.data(), size);
			str = str.substr(size);
			total += size;
			return total;
		}
	}

	// Fill with data from network socket:
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	socklen_t addr_size = sizeof(addr_in);

	int result = ::recvfrom(
		impl->sock,
		_data,
		size,
		0,
		(sockaddr *) &addr_in,
		&addr_size);
	if (result < 0) throw CL_Error("CL_Socket::recv failed");
	total += result;

	if (impl->read) impl->read->start_listen();
	from = CL_Socket_Generic::create_ip_address(addr_in);
	return total;
}

void CL_Socket::connect(const CL_IPAddress &address)
{
	sockaddr addr;
	int len;

	address.get_addrinfo(proto, addr, len, impl->domain);

	int result = ::connect(impl->sock, &addr, len);
	if (result < 0) 
	{
		throw CL_Error("CL_Socket::connect failed: " + CL_Socket_Generic::get_error_message());
	}
	
	if (impl->read) impl->read->start_listen();
	if (impl->exception) impl->exception->start_listen();
}

void CL_Socket::shutdown(ShutdownHow how)
{
	int in_how = 0;
	switch (how)
	{
	case shutdown_receive: in_how = 0; break;
	case shutdown_send:    in_how = 1; break;
	}

	int result = ::shutdown(impl->sock, in_how);
	if (result < 0) throw CL_Error("CL_Socket::shutdown failed");
}

void CL_Socket::bind(const CL_IPAddress &address)
{
	sockaddr addr;
	int len;

	memset(&addr, 0, sizeof(addr));
	address.get_addrinfo(proto, addr, len, impl->domain);

// Unixes are bit more picky about when a port can be rebound. Override this behavior to match Windows.
// (not to mention spare us of every linux user asking why they cannot bind after they closed previous socket.)
// -- mbn 22. june 2003
#ifndef WIN32
	int value = 1;
	setsockopt(impl->sock, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
#endif

	int result = ::bind(impl->sock, &addr, len);
	if (result < 0) throw CL_Error("CL_Socket::bind failed: " + CL_Socket_Generic::get_error_message());
}

void CL_Socket::listen(int backlog)
{
	int result = ::listen(impl->sock, backlog);
	if (result < 0) throw CL_Error("CL_Socket::listen failed");
}

CL_Socket CL_Socket::accept()
{
	int new_sock = ::accept(impl->sock, NULL, NULL);
	if (new_sock == INVALID_SOCKET) throw CL_Error("CL_Socket::accept failed");
	if (impl->read) impl->read->start_listen();
	return CL_Socket(new_sock);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket signals:

CL_Signal_v0 &CL_Socket::sig_read_triggered()
{
	if (impl->keep_alive_helper == 0) impl->keep_alive_helper = new CL_Socket_KeepAliveHelper(impl);
	return impl->keep_alive_helper->sig_read_triggered;
}

CL_Signal_v0 &CL_Socket::sig_write_triggered()
{
	if (impl->keep_alive_helper == 0) impl->keep_alive_helper = new CL_Socket_KeepAliveHelper(impl);
	return impl->keep_alive_helper->sig_write_triggered;
}

CL_Signal_v0 &CL_Socket::sig_exception_triggered()
{
	if (impl->keep_alive_helper == 0) impl->keep_alive_helper = new CL_Socket_KeepAliveHelper(impl);
	return impl->keep_alive_helper->sig_exception_triggered;
}

CL_Signal_v0 &CL_Socket::sig_disconnected()
{
	if (impl->keep_alive_helper == 0) impl->keep_alive_helper = new CL_Socket_KeepAliveHelper(impl);
	return impl->keep_alive_helper->sig_disconnected;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket implementation:

CL_Socket::CL_Socket(CL_Socket_Generic *impl)
: impl(impl)
{
	impl->add_ref();
	input.socket = impl;
	output.socket = impl;
}
