
#include "Network/precomp.h"
#include "API/Network/Socket/tcp_listen.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/exception.h"
#include "tcp_socket.h"

namespace clan
{

#if defined(WIN32)

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

	TCPListen::TCPListen()
	{
	}

	TCPListen::TCPListen(const SocketName &endpoint, int backlog, bool reuse_address)
		: impl(new TCPSocket())
	{
		if (reuse_address)
		{
			int value = 1;
			int result = setsockopt(impl->handle, SOL_SOCKET, SO_REUSEADDR, (const char *)&value, sizeof(int));
			if (result == SOCKET_ERROR)
				throw Exception("Could not set reuse address socket option");
		}

		int receive_buffer_size = 600 * 1024;
		int send_buffer_size = 600 * 1024;

		//int result = setsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		//if (result == SOCKET_ERROR)
		//	throw Exception("Could not set socket receive buffer size");

		int result = setsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (const char *)&send_buffer_size, sizeof(int));
		if (result == SOCKET_ERROR)
			throw Exception("Could not set socket send buffer size");

		int value = 1;
		result = setsockopt(impl->handle, IPPROTO_TCP, TCP_NODELAY, (const char *)&value, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		result = ::bind(impl->handle, (const sockaddr *)&addr, sizeof(sockaddr_in));
		if (result == SOCKET_ERROR)
			throw Exception("Could not bind socket to end point");

		result = ::listen(impl->handle, backlog);
		if (result == SOCKET_ERROR)
			throw Exception("Unable to set socket into listen state");

		result = WSAEventSelect(impl->handle, impl->wait_handle, FD_ACCEPT);
		if (result == SOCKET_ERROR)
			throw Exception("WSAEventSelect failed");
	}

	TCPListen::~TCPListen()
	{
	}

	void TCPListen::close()
	{
		if (impl)
			impl->close();
	}

	TCPConnection TCPListen::accept(SocketName &out_end_point)
	{
		sockaddr_in peer_address = { 0 };
		int peer_address_length = sizeof(sockaddr_in);

		SOCKET result = ::accept(impl->handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_address_length);
		if (result == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return TCPConnection();
			else
				throw Exception("Error accepting from socket");
		}

		out_end_point = SocketName();
		out_end_point.from_sockaddr(AF_INET, reinterpret_cast<sockaddr*>(&peer_address), peer_address_length);
		return TCPConnection(std::make_shared<TCPSocket>(result));
	}

	SocketHandle *TCPListen::get_socket_handle()
	{
		return impl.get();
	}

#else

	TCPListen::TCPListen()
	{
	}

	TCPListen::TCPListen(const SocketName &endpoint, int backlog, bool reuse_address)
		: impl(new TCPSocket())
	{
		if (reuse_address)
		{
			int value = 1;
			int result = setsockopt(impl->handle, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
			if (result == -1)
				throw Exception("Could not set reuse address socket option");
		}

		int receive_buffer_size = 600*1024;
		int send_buffer_size = 600*1024;

		// int result = setsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		// if (result == -1)
		//	throw Exception("Could not set socket receive buffer size");

		int result = setsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (const char *) &send_buffer_size, sizeof(int));
		if (result == -1)
			throw Exception("Could not set socket send buffer size");

		int value = 1;
		result = setsockopt(impl->handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
		result = ::bind(impl->handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
		if (result == -1)
			throw Exception("Could not bind socket to end point");

		result = ::listen(impl->handle, backlog);
		if (result == -1)
			throw Exception("Unable to set socket into listen state");

		int nonblocking = 1;
		ioctl(impl->handle, FIONBIO, &nonblocking);
	}

	TCPListen::~TCPListen()
	{
	}

	void TCPListen::close()
	{
		if (impl)
			impl->close();
	}

	TCPConnection TCPListen::accept(SocketName &out_end_point)
	{
		sockaddr_in peer_address = { 0 };
		socklen_t peer_address_length = sizeof(sockaddr_in);

		int result = ::accept(impl->handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_address_length);
		if (result == -1)
		{
			if (errno == EWOULDBLOCK)
				return TCPConnection();
			else
				throw Exception("Error accepting from socket");
		}

		out_end_point = SocketName::from_sockaddr(AF_INET, reinterpret_cast<sockaddr*>(&peer_address), peer_address_length);
		return TCPConnection(std::make_shared<TCPSocket>(result));
	}

	SocketHandle *TCPListen::get_socket_handle()
	{
		return impl.get();
	}

#endif

}
