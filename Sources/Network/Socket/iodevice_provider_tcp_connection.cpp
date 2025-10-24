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

#include "Network/precomp.h"
#include "iodevice_provider_tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/logger.h"
#ifdef WIN32
typedef int socklen_t;
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Construction:

CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection()
: handle(-1), close_socket(false), eof_received(false)
{
}
	
CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection(
	const CL_SocketName &remote)
: handle(-1), close_socket(false), eof_received(false)
{
	handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1)
		throw CL_Exception(cl_format(cl_text("Unable to create socket: %1"), get_error_message()));
	close_socket = true;
#ifdef WIN32
	wsa_event_handler.start_select(handle);
	read_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_write));
#else
	int nonblocking = 1;
	ioctl(handle, FIONBIO, &nonblocking);
	read_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));
	write_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_write));

#ifdef SO_NOSIGPIPE
	int value = 1;
	setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
#endif
	sockaddr_in addr;
	remote.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = connect_blocked(handle, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result < 0)
	{
		CL_String msg = get_error_message();
		disconnect_abortive();
		throw CL_Exception(cl_format(cl_text("Unable to connect to %1 port %2: %3"),
			remote.get_address(), remote.get_port(), msg));
	}
}
	
CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection(
	const CL_SocketName &remote,
	const CL_SocketName &local)
: handle(-1), close_socket(false)
{
	handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1)
		throw CL_Exception(cl_format(cl_text("Unable to create socket: %1"), get_error_message()));
	close_socket = true;

#ifdef WIN32
	wsa_event_handler.start_select(handle);
	read_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_write));
#else
	int nonblocking = 1;
	ioctl(handle, FIONBIO, &nonblocking);
	read_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));
	write_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_write));

#ifdef SO_NOSIGPIPE
	int value = 1;
	setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
#endif
		
	sockaddr_in local_addr;
	local.to_sockaddr(AF_INET, (sockaddr *) &local_addr, sizeof(sockaddr_in));
	int result = bind(handle, (sockaddr *) &local_addr, sizeof(sockaddr_in));
	if (result < 0)
	{
		CL_String msg = get_error_message();
		disconnect_abortive();
		if (!local.get_address().empty())
			throw CL_Exception(cl_format(cl_text("Unable to bind to local address %1 port %2: %3"),
				local.get_address(), local.get_port(), msg));
		else
			throw CL_Exception(cl_format(cl_text("Unable to bind to local port %1: %2"),
				local.get_port(), msg));
	}
	
	sockaddr_in addr;
	remote.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	result = connect_blocked(handle, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result < 0)
	{
		CL_String msg = get_error_message();
		disconnect_abortive();
		throw CL_Exception(cl_format(cl_text("Unable to connect to %1 port %2: %3"),
			remote.get_address(), remote.get_port(), msg));
	}
}
	
CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection(
	int handle,
	bool close_socket)
: handle(handle), close_socket(close_socket)
{
#ifdef WIN32
	wsa_event_handler.start_select(handle);
	read_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_write));
#else
	read_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));
	write_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_write));

#ifdef SO_NOSIGPIPE
	int value = 1;
	setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
#endif
}

CL_IODeviceProvider_TCPConnection::~CL_IODeviceProvider_TCPConnection()
{
	disconnect_abortive();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Attributes:

CL_SocketName CL_IODeviceProvider_TCPConnection::get_local_name() const
{
	if (handle == -1)
		throw CL_Exception(cl_text("Socket not connected"));
	
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	socklen_t size = sizeof(sockaddr_in);
	int result = getsockname(handle, (sockaddr *) &addr, &size);
	if (result == -1)
		throw CL_Exception(cl_format(cl_text("getsockname failed: %1"), get_error_message()));

	CL_SocketName name;
	name.from_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	return name;
}

CL_SocketName CL_IODeviceProvider_TCPConnection::get_remote_name() const
{
	if (handle == -1)
		throw CL_Exception(cl_text("Socket not connected"));
	
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	socklen_t size = sizeof(sockaddr_in);
	int result = getpeername(handle, (sockaddr *) &addr, &size);
	if (result == -1)
		throw CL_Exception(cl_format(cl_text("getsockname failed: %1"), get_error_message()));
	
	CL_SocketName name;
	name.from_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	return name;
}

CL_Event CL_IODeviceProvider_TCPConnection::get_read_event()
{
	return read_event;
}

CL_Event CL_IODeviceProvider_TCPConnection::get_write_event()
{
	return write_event;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Operations:

bool CL_IODeviceProvider_TCPConnection::connect(const CL_SocketName &remote)
{
	disconnect_abortive();

	handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1)
		return false;
	close_socket = true;

#ifdef WIN32
	wsa_event_handler.start_select(handle);
	read_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_write));
#else
	int nonblocking = 1;
	ioctl(handle, FIONBIO, &nonblocking);
	read_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));
	write_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_write));

#ifdef SO_NOSIGPIPE
	int value = 1;
	setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
#endif
	
	sockaddr_in addr;
	remote.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = connect_blocked(handle, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result < 0)
	{
		cl_log_event("debug", "Socket connect failed; %1", get_error_message());
		disconnect_abortive();
		return false;
	}

	return true;
}

bool CL_IODeviceProvider_TCPConnection::connect(
	const CL_SocketName &remote,
	const CL_SocketName &local)
{
	disconnect_abortive();

	handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1)
		return false;
	close_socket = true;

#ifdef WIN32
	wsa_event_handler.start_select(handle);
	read_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_write));
#else
	int nonblocking = 1;
	ioctl(handle, FIONBIO, &nonblocking);
	read_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));
	write_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_write));

#ifdef SO_NOSIGPIPE
	int value = 1;
	setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
#endif
		
	sockaddr_in local_addr;
	local.to_sockaddr(AF_INET, (sockaddr *) &local_addr, sizeof(sockaddr_in));
	int result = bind(handle, (sockaddr *) &local_addr, sizeof(sockaddr_in));
	if (result < 0)
	{
		cl_log_event("debug", "Socket bind failed; %1", get_error_message());
		disconnect_abortive();
		return false;
	}
	
	sockaddr_in addr;
	remote.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	result = connect_blocked(handle, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result < 0)
	{
		cl_log_event("debug", "Socket connect failed; %1", get_error_message());
		disconnect_abortive();
		return false;
	}
	return true;
}
	
void CL_IODeviceProvider_TCPConnection::set_handle(int new_handle, bool new_close_socket)
{
	disconnect_abortive();
	handle = new_handle;
	close_socket = new_close_socket;
}

void CL_IODeviceProvider_TCPConnection::disconnect_abortive()
{
	if (close_socket)
	{
#ifdef WIN32
		shutdown(handle, SD_RECEIVE);
#else
		shutdown(handle, SHUT_RD);
#endif
		// This should cause a hard closure according to the winsock documentation,
		// so since that's what Microsoft offically says, it is probably not what
		// is going to happen..
		linger linger_value;
		linger_value.l_onoff = 1;
		linger_value.l_linger = 0;
		setsockopt(handle, SOL_SOCKET, SO_LINGER, (const char *) &linger_value, sizeof(linger));
#ifdef WIN32
		closesocket(handle);
#else
		close(handle);
#endif
	}
	handle = -1;
	close_socket = false;
}

void CL_IODeviceProvider_TCPConnection::disconnect_graceful()
{
	if (close_socket)
	{
		// Linger around until we sent all data:
		// (We cannot use SO_LINGER because it is not blocking, and linger
		// functionality vary a lot between TCP/IP stack implementations)
#ifdef WIN32
		shutdown(handle, SD_SEND);
#else
		shutdown(handle, SHUT_WR);
#endif
		if (!eof_received)
		{
			// Wait for the FIN acknowledgement:
			read_event.wait(10*1000);
			// Causes a 'connection reset' shutdown, if there is still data on the socket:
#ifdef WIN32
			shutdown(handle, SD_RECEIVE);
#else
			shutdown(handle, SHUT_RD);
#endif
		}
	}
#ifdef WIN32
	wsa_event_handler.stop_select();
	if (close_socket)
		closesocket(handle);
#else
	if (close_socket)
		close(handle);
#endif
	handle = -1;
	close_socket = false;
}

void CL_IODeviceProvider_TCPConnection::set_nodelay(bool enable)
{
	int value = enable ? 1 : 0;
	int result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(int));
	if (result < 0)
		throw CL_Exception(cl_format(cl_text("Unable to set TCP_NODELAY option on socket: %1"), get_error_message()));
}

void CL_IODeviceProvider_TCPConnection::flush()
{
	set_nodelay(true);
	send_blocked(handle, 0, 0, 0);
	set_nodelay(false);
}

int CL_IODeviceProvider_TCPConnection::send(const void *data, int len, bool send_all)
{
	const char *d = (const char *) data;
	int pos = 0;
	while (pos < len)
	{
#ifdef MSG_NOSIGNAL
		int result = send_blocked(handle, d+pos, len-pos, MSG_NOSIGNAL);
#else
		int result = send_blocked(handle, d+pos, len-pos, 0);
#endif
		if (result < 0)
			throw CL_Exception(cl_format(cl_text("Socket send failed: %1"), get_error_message()));
		pos += result;
		if (result == 0)
			break;
		if (!send_all)
			break;
	}
	return pos;
}

int CL_IODeviceProvider_TCPConnection::receive(void *data, int len, bool receive_all)
{
	char *d = (char *) data;
	int pos = 0;
	while (pos < len)
	{
		int result = recv_blocked(handle, d+pos, len-pos, 0);
		if (result < 0)
		{
			throw CL_Exception(cl_format(cl_text("Socket receive failed; %1"), get_error_message()));
		}
		pos += result;
		if (result == 0)
		{
			eof_received = true;
			break;
		}
		if (!receive_all)
			break;
	}
	return pos;
}

int CL_IODeviceProvider_TCPConnection::peek(void *data, int len)
{
	int result = recv_blocked(handle, (char *) data, len, MSG_PEEK);
	if (result < 0)
		throw CL_Exception(cl_format(cl_text("Socket receive (peek) failed; %1"), get_error_message()));
	return result;
}

CL_IODeviceProvider *CL_IODeviceProvider_TCPConnection::duplicate()
{
	throw CL_Exception(cl_text("CL_IODeviceProvider_TCPConnection::duplicate() - duplicate not supported for TCP connections."));
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Implementation:

int CL_IODeviceProvider_TCPConnection::connect_blocked(int handle, const void *name, int namelen)
{
	int result = ::connect(handle, (const sockaddr *) name, namelen);
	if (is_blocked(result))
	{
		fd_set wfds, efds;
		FD_ZERO(&wfds);
		FD_ZERO(&efds);
		FD_SET(handle, &wfds);
		FD_SET(handle, &efds);
		timeval tv;
		tv.tv_sec = 15;
		tv.tv_usec = 0;
		result = select(handle+1, 0, &wfds, &efds, &tv);
		if (result < 0)
			return result;
		if (result == 0)
			throw CL_Exception(cl_text("Socket connect timed out"));

		if (FD_ISSET(handle, &wfds))
			return 0;
		else if (FD_ISSET(handle, &efds))
			return -1;
//			return connect(handle, name, namelen);
	}
	return result;
}

int CL_IODeviceProvider_TCPConnection::recv_blocked(int handle, char *buf, int len, int flags)
{
	int result = ::recv(handle, buf, len, flags);
	if (is_blocked(result))
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(handle, &rfds);
		timeval tv;
		tv.tv_sec = 15;
		tv.tv_usec = 0;
		result = select(handle+1, &rfds, 0, 0, &tv);
		if (result < 0)
			return result;
		if (result == 0)
			throw CL_Exception(cl_text("Socket receive timed out"));
		result = ::recv(handle, buf, len, flags);
	}
	return result;
}

int CL_IODeviceProvider_TCPConnection::send_blocked(int handle, const char *buf, int len, int flags)
{
	int result = ::send(handle, buf, len, flags);
	if (is_blocked(result))
	{
		fd_set wfds;
		FD_ZERO(&wfds);
		FD_SET(handle, &wfds);
		timeval tv;
		tv.tv_sec = 15;
		tv.tv_usec = 0;
		result = select(handle+1, 0, &wfds, 0, &tv);
		if (result < 0)
			return result;
		if (result == 0)
			throw CL_Exception(cl_text("Socket send timed out"));
		result = ::send(handle, buf, len, flags);
	}
	return result;
}

bool CL_IODeviceProvider_TCPConnection::is_blocked(int result)
{
	if (result >= 0)
		return false;
#ifdef WIN32
	return WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINPROGRESS;
#else
	return errno == EWOULDBLOCK || errno == EINPROGRESS;
#endif
	return false;
}

CL_String CL_IODeviceProvider_TCPConnection::get_error_message()
{
#ifdef WIN32
	int err = WSAGetLastError();
	switch (err)
	{
	case WSAEACCES:
		return cl_text("Permission denied");
	case WSAEADDRINUSE:
		return cl_text("Address already in use");
	case WSAEADDRNOTAVAIL:
		return cl_text("Cannot assign requested address");
	case WSAEAFNOSUPPORT:
		return cl_text("Address family not supported by protocol family");
	case WSAEALREADY:
		return cl_text("Operation already in progress");
	case WSAECONNABORTED:
		return cl_text("Software caused connection abort");
	case WSAECONNREFUSED:
		return cl_text("Connection refused");
	case WSAECONNRESET:
		return cl_text("Connection reset by peer");
	case WSAEDESTADDRREQ:
		return cl_text("Destination address required");
	case WSAEFAULT:
		return cl_text("Bad address");
	case WSAEHOSTDOWN:
		return cl_text("Host is down");
	case WSAEINPROGRESS:
		return cl_text("Operation now in progress");
	case WSAEINTR:
		return cl_text("Interrupted function call");
	case WSAEINVAL:
		return cl_text("Invalid argument");
	case WSAEISCONN:
		return cl_text("Socket is already connected");
	case WSAEMFILE:
		return cl_text("Too many open files");
	case WSAEMSGSIZE:
		return cl_text("Message too long");
	case WSAENETDOWN:
		return cl_text("Network is down");
	case WSAENETRESET:
		return cl_text("Network dropped connection on reset");
	case WSAENETUNREACH:
		return cl_text("Network is unreachable");
	case WSAENOBUFS:
		return cl_text("No buffer space available");
	case WSAENOPROTOOPT:
		return cl_text("Bad protocol option");
	case WSAENOTCONN:
		return cl_text("Socket is not connected");
	case WSAENOTSOCK:
		return cl_text("Socket operation on nonsocket");
	case WSAEOPNOTSUPP:
		return cl_text("Operation not supported");
	case WSAEPFNOSUPPORT:
		return cl_text("Protocol family not supported");
	case WSAEPROCLIM:
		return cl_text("Too many processes");
	case WSAEPROTONOSUPPORT:
		return cl_text("Protocol not supported");
	case WSAEPROTOTYPE:
		return cl_text("Protocol wrong type for socket");
	case WSAESHUTDOWN:
		return cl_text("Cannot send after socket shutdown");
	case WSAESOCKTNOSUPPORT:
		return cl_text("Socket type not supported");
	case WSAETIMEDOUT:
		return cl_text("Connection timed out");
//	case WSATYPE_NOT_FOUND:
//		return cl_text("Class type not found");
	case WSAEWOULDBLOCK:
		return cl_text("Resource temporarily unavailable");
	case WSAHOST_NOT_FOUND:
		return cl_text("Host not found");
//	case WSA_INVALID_HANDLE:
//		return cl_text("Specified event object handle is invalid");
//	case WSA_INVALID_PARAMETER:
//		return cl_text("One or more parameters are invalid");
//	case WSAINVALIDPROCTABLE:
//		return cl_text("Invalid procedure table from service provider");
//	case WSAINVALIDPROVIDER:
//		return cl_text("Invalid service provider version number");
//	case WSA_IO_INCOMPLETE:
//		return cl_text("Overlapped I/O event object not in signaled state");
//	case WSA_IO_PENDING:
//		return cl_text("Overlapped operations will complete later");
//	case WSA_NOT_ENOUGH_MEMORY:
//		return cl_text("Insufficient memory available");
	case WSANOTINITIALISED:
		return cl_text("Successful WSAStartup not yet performed");
	case WSANO_DATA:
		return cl_text("Valid name, no data record of requested type");
	case WSANO_RECOVERY:
		return cl_text("This is a nonrecoverable error");
//	case WSAPROVIDERFAILEDINIT:
//		return cl_text("Unable to initialize a service provider");
//	case WSASYSCALLFAILURE:
//		return cl_text("System call failure");
	case WSASYSNOTREADY:
		return cl_text("Network subsystem is unavailable");
	case WSATRY_AGAIN:
		return cl_text("Nonauthoritative host not found");
	case WSAVERNOTSUPPORTED:
		return cl_text("Winsock.dll version out of range");
	case WSAEDISCON:
		return cl_text("Graceful shutdown in progress");
//	case WSA_OPERATION_ABORTED:
//		return cl_text("Overlapped operation aborted");
	default:
		return cl_format(cl_text("Unknown socket error %1"), err);
	}
#else
	char buffer[256];
	memset(buffer, 0, 256);
	strerror_r(errno, buffer, 256);
	return CL_String( buffer );
#endif
}
