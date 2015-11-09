
#include "Network/precomp.h"
#include "API/Network/Socket/udp_socket.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/exception.h"
#include "tcp_socket.h"
#include "../setupnetwork.h"

#if !defined(WIN32)
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#endif
#if defined(__APPLE__)
#define SOL_TCP IPPROTO_TCP
#endif

namespace clan
{

#if defined(WIN32)

	class UDPSocketImpl : public SocketHandle
	{
	public:
		UDPSocketImpl()
		{
			handle = socket(AF_INET, SOCK_DGRAM, 0);
			if (handle == INVALID_SOCKET)
				throw Exception("Unable to create socket handle");
		}

		UDPSocketImpl(SOCKET init_handle)
		{
			handle = init_handle;
			if (handle == INVALID_SOCKET)
				throw Exception("Invalid socket handle");
		}
	};

	UDPSocket::UDPSocket() : impl(new UDPSocketImpl())
	{
		int result = WSAEventSelect(impl->handle, impl->wait_handle, FD_READ);
		if (result == SOCKET_ERROR)
			throw Exception("WSAEventSelect failed");
	}

	UDPSocket::~UDPSocket()
	{
	}

	SocketHandle *UDPSocket::get_socket_handle()
	{
		return impl.get();
	}

	void UDPSocket::bind(const SocketName &endpoint)
	{
		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		int result = ::bind(impl->handle, (const sockaddr *)&addr, sizeof(sockaddr_in));
		if (result == SOCKET_ERROR)
			throw Exception("Could not bind socket to end point");
	}

	void UDPSocket::send(const void *data, int size, const SocketName &endpoint)
	{
		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));

		int result = sendto(impl->handle, static_cast<const char*>(data), size, 0, (const sockaddr *)&addr, sizeof(sockaddr_in));
		if (result == SOCKET_ERROR)
		{
			int last_error = WSAGetLastError();
			if (last_error != WSAENOBUFS)
			{
				throw Exception("Error writing to udp socket");
			}
		}
	}

	int UDPSocket::read(void *data, int size, SocketName &endpoint)
	{
		sockaddr_in addr;
		int addr_len = sizeof(sockaddr_in);

		int result = recvfrom(impl->handle, static_cast<char*>(data), size, 0, (sockaddr *)&addr, &addr_len);
		if (result == SOCKET_ERROR)
		{
			int last_error = WSAGetLastError();
			// To do: maybe return different return values for sending errors
			if (last_error == WSAEWOULDBLOCK || last_error == WSAEMSGSIZE || last_error == WSAECONNRESET || last_error == WSAENETRESET)
			{
				endpoint = SocketName();
				return -1;
			}
			else
			{
				throw Exception("Error reading from udp socket");
			}
		}

		endpoint = SocketName();
		endpoint.from_sockaddr(AF_INET, (sockaddr *)&addr, addr_len);
		return result;
	}

	void UDPSocket::close()
	{
		impl->close();
	}

#else

	class UDPSocketImpl : public SocketHandle
	{
	public:
		UDPSocketImpl()
			: handle(-1)
		{
			SetupNetwork::start();

			handle = socket(AF_INET, SOCK_DGRAM, 0);
			if (handle == -1)
				throw Exception("Unable to create socket handle");
		}

		UDPSocketImpl(int handle)
			: handle(handle)
		{
		}

		~UDPSocketImpl()
		{
			close();
		}

		void close()
		{
			if (handle != -1)
			{
				::close(handle);
				handle = -1;
			}
		}

		void begin_wait(fd_set &rfds, fd_set &wfds, int &max_fd) override
		{
			FD_SET(handle, &rfds);
			max_fd = std::max(max_fd, handle);
		}

		void end_wait(fd_set &rfds, fd_set &wfds) override
		{
		}

		int handle;
	};

	UDPSocket::UDPSocket() : impl(new UDPSocketImpl())
	{
		int nonblocking = 1;
		ioctl(impl->handle, FIONBIO, &nonblocking);
	}

	UDPSocket::~UDPSocket()
	{
	}

	SocketHandle *UDPSocket::get_socket_handle()
	{
		return impl.get();
	}

	void UDPSocket::bind(const SocketName &endpoint)
	{
		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		int result = ::bind(impl->handle, (const sockaddr *)&addr, sizeof(sockaddr_in));
		if (result == -1)
			throw Exception("Could not bind socket to end point");
	}

	void UDPSocket::send(const void *data, int size, const SocketName &endpoint)
	{
		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));

		sendto(impl->handle, static_cast<const char*>(data), size, 0, (const sockaddr *)&addr, sizeof(sockaddr_in));
	}

	int UDPSocket::read(void *data, int size, SocketName &endpoint)
	{
		sockaddr_in addr;
		socklen_t addr_len = sizeof(sockaddr_in);

		int result = recvfrom(impl->handle, static_cast<char*>(data), size, 0, (sockaddr *)&addr, &addr_len);
		if (result == -1)
		{
			if (errno == EWOULDBLOCK || errno == EMSGSIZE || errno == ECONNRESET || errno == ENETRESET)
			{
				endpoint = SocketName();
				return -1;
			}
			else
			{
				throw Exception("Error reading from udp socket");
			}
		}

		endpoint.from_sockaddr(AF_INET, (sockaddr *)&addr, addr_len);
		return result;
	}

#endif
}
