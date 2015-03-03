
#include "Network/precomp.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "tcp_socket.h"
#include "API/Core/System/exception.h"

namespace clan
{

#if defined(WIN32)

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

	TCPConnection::TCPConnection()
	{
	}

	TCPConnection::TCPConnection(const SocketName &endpoint)
		: impl(new TCPSocket())
	{
		int receive_buffer_size = 600 * 1024;
		int send_buffer_size = 600 * 1024;
		//int result = setsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		int result = setsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (const char *)&send_buffer_size, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		result = ::connect(impl->handle, (const sockaddr *)&addr, sizeof(sockaddr_in));
		if (result == SOCKET_ERROR)
			throw Exception("Connect to server failed");

		int value = 1;
		result = setsockopt(impl->handle, IPPROTO_TCP, TCP_NODELAY, (const char *)&value, sizeof(int));

		result = WSAEventSelect(impl->handle, impl->wait_handle, FD_READ | FD_WRITE | FD_CLOSE);
		if (result == SOCKET_ERROR)
			throw Exception("WSAEventSelect failed");

		//int receive_buffer_size = 0;
		//int send_buffer_size = 0;
		//int len = sizeof(int);
		//getsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (char *) &receive_buffer_size, &len);
		//getsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (char *) &send_buffer_size, &len);
	}

	TCPConnection::TCPConnection(const std::shared_ptr<TCPSocket> &impl)
		: impl(impl)
	{
		int receive_buffer_size = 600 * 1024;
		int send_buffer_size = 600 * 1024;
		//int result = setsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		int result = setsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (const char *)&send_buffer_size, sizeof(int));

		result = WSAEventSelect(impl->handle, impl->wait_handle, FD_READ | FD_WRITE | FD_CLOSE);
		if (result == SOCKET_ERROR)
			throw Exception("WSAEventSelect failed");
	}

	TCPConnection::~TCPConnection()
	{
	}

	int TCPConnection::write(const void *data, int size)
	{
		int result = ::send(impl->handle, static_cast<const char *>(data), size, 0);
		if (result == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return -1;
			else
				throw Exception("Error writing to server");
		}
		return result;
	}

	int TCPConnection::read(void *data, int size)
	{
		int result = ::recv(impl->handle, static_cast<char *>(data), size, 0);
		if (result == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return -1;
			else
				throw Exception("Error reading from server");
		}
		return result;
	}

	void TCPConnection::close()
	{
		if (impl)
			impl->close();
	}

	SocketName TCPConnection::get_local_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		int size = sizeof(sockaddr_in);
		int result = getsockname(impl->handle, (sockaddr *)&addr, &size);
		if (result == SOCKET_ERROR)
			throw Exception("Error retrieving local socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

	SocketName TCPConnection::get_remote_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		int size = sizeof(sockaddr_in);
		int result = getpeername(impl->handle, (sockaddr *)&addr, &size);
		if (result == SOCKET_ERROR)
			throw Exception("Error retrieving remote socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

	SocketHandle *TCPConnection::get_socket_handle()
	{
		return impl.get();
	}

#else

	void TCPConnection::init_sockets()
	{
	}

	TCPConnection::TCPConnection()
	{
	}

	TCPConnection::TCPConnection(const SocketName &endpoint)
		: impl(new TCPSocket())
	{
		//int receive_buffer_size = 600*1024;
		int send_buffer_size = 600*1024;
		//int result = setsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		int result = setsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (const char *) &send_buffer_size, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
		result = ::connect(impl->handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
		if (result == -1)
			throw Exception("Connect to server failed");

		int value = 1;
		result = setsockopt(impl->handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(int));

		//int receive_buffer_size = 0;
		//int send_buffer_size = 0;
		//int len = sizeof(int);
		//getsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (char *) &receive_buffer_size, &len);
		//getsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (char *) &send_buffer_size, &len);

		int nonblocking = 1;
		ioctl(impl->handle, FIONBIO, &nonblocking);
	}

	TCPConnection::TCPConnection(const std::shared_ptr<TCPSocket> &impl)
		: impl(impl)
	{
		//int receive_buffer_size = 600*1024;
		int send_buffer_size = 600*1024;
		//int result = setsockopt(impl->handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		setsockopt(impl->handle, SOL_SOCKET, SO_SNDBUF, (const char *) &send_buffer_size, sizeof(int));

		int nonblocking = 1;
		ioctl(impl->handle, FIONBIO, &nonblocking);
	}

	TCPConnection::~TCPConnection()
	{
	}

	int TCPConnection::write(const void *data, int size)
	{
		int result = ::send(impl->handle, static_cast<const char *>(data), size, 0);
		if (result == -1)
		{
			if (errno == EWOULDBLOCK)
			{
				impl->can_write = false;
				return -1;
			}
			else
			{
				throw Exception("Error writing to server");
			}
		}
		return result;
	}

	int TCPConnection::read(void *data, int size)
	{
		int result = ::recv(impl->handle, static_cast<char *>(data), size, 0);
		if (result == -1)
		{
			if (errno == EWOULDBLOCK)
				return -1;
			else
				throw Exception("Error reading from server");
		}
		return result;
	}

	void TCPConnection::close()
	{
		if (impl)
			impl->close();
	}

	SocketName TCPConnection::get_local_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		socklen_t size = sizeof(sockaddr_in);
		int result = getsockname(impl->handle, (sockaddr *)&addr, &size);
		if (result == -1)
			throw Exception("Error retrieving local socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

	SocketName TCPConnection::get_remote_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		socklen_t size = sizeof(sockaddr_in);
		int result = getpeername(impl->handle, (sockaddr *)&addr, &size);
		if (result == -1)
			throw Exception("Error retrieving remote socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

	SocketHandle *TCPConnection::get_socket_handle()
	{
		return impl.get();
	}

#endif

}
