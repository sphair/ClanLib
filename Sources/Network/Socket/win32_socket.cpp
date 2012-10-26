/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "win32_socket.h"
#include "API/Core/Text/string_format.h"
#include "API/Network/Socket/socket_name.h"
#include <Mstcpip.h>

namespace clan
{

Win32Socket::Win32Socket()
: handle(INVALID_SOCKET), close_handle(true), event_handle(0), receive_handle(0), send_handle(0), except_handle(0)
{
}

Win32Socket::Win32Socket(SOCKET handle)
: handle(handle), close_handle(true), event_handle(0), receive_handle(0), send_handle(0), except_handle(0)
{
	try
	{
		create_event_handles();
		select_events();
	}
	catch (...)
	{
		close_handles();
		throw;
	}
}

Win32Socket::~Win32Socket()
{
	disconnect_abortive();
}

void Win32Socket::set_handle(SOCKET new_handle, bool new_close_handle)
{
	close_handles();
	handle = new_handle;
	close_handle = new_close_handle;
	create_event_handles();
	select_events();
}

void Win32Socket::create_tcp()
{
	create_socket_handle(SOCK_STREAM);
	create_event_handles();
	select_events();
}

void Win32Socket::create_udp()
{
	create_socket_handle(SOCK_DGRAM);
	create_event_handles();
	select_events();
}

void Win32Socket::close_handles()
{
	if (handle != INVALID_SOCKET)
	{
		WSAEventSelect(handle, 0, 0);
		if (close_handle)
			closesocket(handle);
	}
	if (event_handle)
		CloseHandle(event_handle);
	if (receive_handle)
		CloseHandle(receive_handle);
	if (send_handle)
		CloseHandle(send_handle);
	if (except_handle)
		CloseHandle(except_handle);
	handle = INVALID_SOCKET;
	event_handle = 0;
	receive_handle = 0;
	send_handle = 0;
	except_handle = 0;
}

void Win32Socket::disconnect_graceful(int timeout)
{
	if (handle != INVALID_SOCKET)
	{
		linger linger_value;
		linger_value.l_onoff = 1;
		linger_value.l_linger = (timeout + 500) / 1000;
		setsockopt(handle, SOL_SOCKET, SO_LINGER, (const char *) &linger_value, sizeof(linger));
	}
	close_handles();
}

void Win32Socket::disconnect_abortive()
{
	if (handle != INVALID_SOCKET)
	{
		// This should cause a hard closure according to the winsock documentation,
		// so since that's what Microsoft officially says, it is probably not what
		// is going to happen..
		linger linger_value;
		linger_value.l_onoff = 1;
		linger_value.l_linger = 0;
		setsockopt(handle, SOL_SOCKET, SO_LINGER, (const char *) &linger_value, sizeof(linger));
	}
	close_handles();
}

void Win32Socket::create_socket_handle(int type)
{
	handle = socket(AF_INET, type, 0);
	if (handle == INVALID_SOCKET)
		throw Exception("Unable to create socket handle");
}

void Win32Socket::create_event_handles()
{
	event_handle = CreateEvent(0, TRUE, FALSE, 0);
	receive_handle = CreateEvent(0, TRUE, FALSE, 0);
	send_handle = CreateEvent(0, TRUE, FALSE, 0);
	except_handle = CreateEvent(0, TRUE, FALSE, 0);
	if (handle == 0 || receive_handle == 0 || send_handle == 0 || except_handle == 0)
		throw Exception("CreateEvent failed");
}

void Win32Socket::set_nodelay(bool enable)
{
	int value = enable ? 1 : 0;
	int result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(int));
	throw_if_failed(result);
}

void Win32Socket::set_keep_alive(bool enable, int timeout, int interval)
{
	int value = enable ? 1 : 0;
	int result = setsockopt(handle, SOL_SOCKET, SO_KEEPALIVE, (const char *) &value, sizeof(int));
	throw_if_failed(result);

	if (enable && timeout != 0 && interval != 0)
	{
		tcp_keepalive keepalive = { 0 };
		keepalive.onoff = 1;
		keepalive.keepalivetime = timeout;
		keepalive.keepaliveinterval = interval;
		DWORD bytes_returned = 0;
		result = WSAIoctl(handle, SIO_KEEPALIVE_VALS, &keepalive, sizeof(tcp_keepalive), 0, 0, &bytes_returned, 0, 0);
		throw_if_failed(result);
	}
}

void Win32Socket::bind(const SocketName &socketname, bool reuse_address)
{
	if (reuse_address)
	{
		int value = 1;
		int result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
		throw_if_failed(result);
	}

	sockaddr_in addr;
	socketname.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = ::bind(handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
	throw_if_failed(result);
}

void Win32Socket::listen(int backlog)
{
	int result = ::listen(handle, backlog);
	throw_if_failed(result);
}

SOCKET Win32Socket::accept(SocketName &out_socketname)
{
	sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(sockaddr_in));
	int size = sizeof(sockaddr_in);
	SOCKET accepted_socket = ::accept(handle, (sockaddr *) &new_addr, &size);
	throw_if_invalid(accepted_socket);
	out_socketname.from_sockaddr(AF_INET, (sockaddr *) &new_addr, size);
	reset_receive();
	return accepted_socket;
}

void Win32Socket::connect(const SocketName &socketname)
{
	sockaddr_in addr;
	socketname.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = ::connect(handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
	if (result == SOCKET_ERROR)
	{
		int errorcode = WSAGetLastError();
		if ( ! ( (errorcode ==  WSAEWOULDBLOCK) || (errorcode ==  WSAEINPROGRESS) ) )
			throw Exception(error_to_string(errorcode));
	}
}

SocketName Win32Socket::get_local_name() const
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	int size = sizeof(sockaddr_in);
	int result = getsockname(handle, (sockaddr *) &addr, &size);
	throw_if_failed(result);

	SocketName name;
	name.from_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	return name;
}

SocketName Win32Socket::get_remote_name() const
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	int size = sizeof(sockaddr_in);
	int result = getpeername(handle, (sockaddr *) &addr, &size);
	throw_if_failed(result);

	SocketName name;
	name.from_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	return name;
}

int Win32Socket::receive(void *data, int size)
{
	int result = ::recv(handle, (char *) data, size, 0);
	throw_if_failed(result);
	reset_receive();
	return result;
}

int Win32Socket::receive_from(void *data, int size, SocketName &out_socketname)
{
	sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(sockaddr_in));
	int addr_size = sizeof(sockaddr_in);
	int result = ::recvfrom(handle, (char *) data, size, 0, (sockaddr *) &new_addr, &addr_size);
	throw_if_failed(result);
	out_socketname.from_sockaddr(AF_INET, (sockaddr *) &new_addr, size);
	reset_receive();
	return result;
}

int Win32Socket::peek(void *data, int size)
{
	int result = ::recv(handle, (char *) data, size, MSG_PEEK);
	throw_if_failed(result);
	reset_receive();
	return result;
}

int Win32Socket::peek_from(void *data, int size, SocketName &out_socketname)
{
	sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(sockaddr_in));
	int addr_size = sizeof(sockaddr_in);
	int result = ::recvfrom(handle, (char *) data, size, MSG_PEEK, (sockaddr *) &new_addr, &addr_size);
	throw_if_failed(result);
	out_socketname.from_sockaddr(AF_INET, (sockaddr *) &new_addr, size);
	reset_receive();
	return result;
}

int Win32Socket::send(const void *data, int size)
{
	int result = ::send(handle, (const char *) data, size, 0);
	if (result == SOCKET_ERROR)
	{
		int errorcode = WSAGetLastError();
		if (errorcode == WSAEWOULDBLOCK)
		{
			reset_send();
			return 0;
		}
		else
		{
			throw Exception(error_to_string(errorcode));
		}
	}
	else
	{
		return result;
	}
}

int Win32Socket::send_to(const void *data, int size, const SocketName &socketname)
{
	sockaddr_in addr;
	socketname.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = ::sendto(handle, (const char *) data, size, 0, (const sockaddr *) &addr, sizeof(sockaddr_in));
	if (result == SOCKET_ERROR)
	{
		int errorcode = WSAGetLastError();
		if (errorcode == WSAEWOULDBLOCK)
		{
			reset_send();
			return 0;
		}
		else
		{
			throw Exception(error_to_string(errorcode));
		}
	}
	else
	{
		return result;
	}
}

void Win32Socket::close_send()
{
	shutdown(handle, SD_SEND);
}

void Win32Socket::select_events()
{
	int result = WSAEventSelect(handle, event_handle, FD_READ|FD_WRITE|FD_OOB|FD_ACCEPT|FD_CONNECT|FD_CLOSE);
	throw_if_failed(result);
}

void Win32Socket::process_events()
{
	WSANETWORKEVENTS events = { 0 };
	int result = WSAEnumNetworkEvents(handle, event_handle, &events);
	throw_if_failed(result);

	bool receive_flag = false;
	bool send_flag = false;
	bool except_flag = false;
	if (events.lNetworkEvents & (FD_READ|FD_ACCEPT|FD_CLOSE))
		receive_flag = true;
	if (events.lNetworkEvents & FD_WRITE)
		send_flag = true;
	if (events.lNetworkEvents & FD_OOB)
		except_flag = true;

	if (events.lNetworkEvents & FD_CONNECT)
	{
		if (events.iErrorCode[FD_CONNECT_BIT] == 0)
			send_flag = true;
		else
			except_flag = true;
	}

	if (receive_flag)
		SetEvent(receive_handle);
	if (send_flag)
		SetEvent(send_handle);
	if (except_flag)
		SetEvent(except_handle);
}

void Win32Socket::reset_receive()
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(handle, &fds);
	timeval tv = { 0, 0 };
	int result = select(handle+1, &fds, 0, 0, &tv);
	throw_if_failed(result);
	if (FD_ISSET(handle, &fds) == 0)
		ResetEvent(receive_handle);
}

void Win32Socket::reset_send()
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(handle, &fds);
	timeval tv = { 0, 0 };
	int result = select(handle+1, 0, &fds, 0, &tv);
	throw_if_failed(result);
	if (FD_ISSET(handle, &fds) == 0)
		ResetEvent(send_handle);
}

void Win32Socket::reset_except()
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(handle, &fds);
	timeval tv = { 0, 0 };
	int result = select(handle+1, 0, 0, &fds, &tv);
	throw_if_failed(result);
	if (FD_ISSET(handle, &fds) == 0)
		ResetEvent(except_handle);
}

void Win32Socket::throw_if_invalid(SOCKET result) const
{
	if (result == INVALID_SOCKET)
		throw Exception(error_to_string(WSAGetLastError()));
}

void Win32Socket::throw_if_failed(int result) const
{
	if (result == SOCKET_ERROR)
		throw Exception(error_to_string(WSAGetLastError()));
}

void Win32Socket::throw_if_socket_failed(int result) const
{
	if (result == SOCKET_ERROR)
	{
		int error = 0;
		int length = sizeof(int);
		getsockopt(handle, SOL_SOCKET, SO_ERROR, (char *) &error, &length);
		throw Exception(error_to_string(error));
	}
}

std::string Win32Socket::error_to_string(int err)
{
	switch (err)
	{
	case WSAEACCES:
		return "Permission denied";
	case WSAEADDRINUSE:
		return "Address already in use";
	case WSAEADDRNOTAVAIL:
		return "Cannot assign requested address";
	case WSAEAFNOSUPPORT:
		return "Address family not supported by protocol family";
	case WSAEALREADY:
		return "Operation already in progress";
	case WSAECONNABORTED:
		return "Software caused connection abort";
	case WSAECONNREFUSED:
		return "Connection refused";
	case WSAECONNRESET:
		return "Connection reset by peer";
	case WSAEDESTADDRREQ:
		return "Destination address required";
	case WSAEFAULT:
		return "Bad address";
	case WSAEHOSTDOWN:
		return "Host is down";
	case WSAEINPROGRESS:
		return "Operation now in progress";
	case WSAEINTR:
		return "Interrupted function call";
	case WSAEINVAL:
		return "Invalid argument";
	case WSAEISCONN:
		return "Socket is already connected";
	case WSAEMFILE:
		return "Too many open files";
	case WSAEMSGSIZE:
		return "Message too long";
	case WSAENETDOWN:
		return "Network is down";
	case WSAENETRESET:
		return "Network dropped connection on reset";
	case WSAENETUNREACH:
		return "Network is unreachable";
	case WSAENOBUFS:
		return "No buffer space available";
	case WSAENOPROTOOPT:
		return "Bad protocol option";
	case WSAENOTCONN:
		return "Socket is not connected";
	case WSAENOTSOCK:
		return "Socket operation on nonsocket";
	case WSAEOPNOTSUPP:
		return "Operation not supported";
	case WSAEPFNOSUPPORT:
		return "Protocol family not supported";
	case WSAEPROCLIM:
		return "Too many processes";
	case WSAEPROTONOSUPPORT:
		return "Protocol not supported";
	case WSAEPROTOTYPE:
		return "Protocol wrong type for socket";
	case WSAESHUTDOWN:
		return "Cannot send after socket shutdown";
	case WSAESOCKTNOSUPPORT:
		return "Socket type not supported";
	case WSAETIMEDOUT:
		return "Connection timed out";
//	case WSATYPE_NOT_FOUND:
//		return "Class type not found";
	case WSAEWOULDBLOCK:
		return "Resource temporarily unavailable";
	case WSAHOST_NOT_FOUND:
		return "Host not found";
//	case WSA_INVALID_HANDLE:
//		return "Specified event object handle is invalid";
//	case WSA_INVALID_PARAMETER:
//		return "One or more parameters are invalid";
//	case WSAINVALIDPROCTABLE:
//		return "Invalid procedure table from service provider";
//	case WSAINVALIDPROVIDER:
//		return "Invalid service provider version number";
//	case WSA_IO_INCOMPLETE:
//		return "Overlapped I/O event object not in signaled state";
//	case WSA_IO_PENDING:
//		return "Overlapped operations will complete later";
//	case WSA_NOT_ENOUGH_MEMORY:
//		return "Insufficient memory available";
	case WSANOTINITIALISED:
		return "Successful WSAStartup not yet performed";
	case WSANO_DATA:
		return "Valid name, no data record of requested type";
	case WSANO_RECOVERY:
		return "This is a nonrecoverable error";
//	case WSAPROVIDERFAILEDINIT:
//		return "Unable to initialize a service provider";
//	case WSASYSCALLFAILURE:
//		return "System call failure";
	case WSASYSNOTREADY:
		return "Network subsystem is unavailable";
	case WSATRY_AGAIN:
		return "Nonauthoritative host not found";
	case WSAVERNOTSUPPORTED:
		return "Winsock.dll version out of range";
	case WSAEDISCON:
		return "Graceful shutdown in progress";
//	case WSA_OPERATION_ABORTED:
//		return "Overlapped operation aborted";
	default:
		return string_format("Unknown socket error %1", err);
	}
}

}
