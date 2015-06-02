
#pragma once

#include "API/Core/System/exception.h"
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

	class SocketHandle
	{
	public:
		SocketHandle() : handle(INVALID_SOCKET)
		{
			SetupNetwork::start();
			wait_handle = CreateEvent(0, TRUE, FALSE, 0);
			if (wait_handle == INVALID_HANDLE_VALUE)
				throw Exception("Unable to create event object for socket");
		}

		~SocketHandle()
		{
			close();
		}

		void close()
		{
			if (handle != INVALID_SOCKET)
			{
				closesocket(handle);
				handle = INVALID_SOCKET;
			}

			if (wait_handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(wait_handle);
				wait_handle = INVALID_HANDLE_VALUE;
			}
		}

		void reset_wait_handle()
		{
			WSANETWORKEVENTS network_events = { 0 };
			int iresult = WSAEnumNetworkEvents(handle, wait_handle, &network_events);
			if (iresult == SOCKET_ERROR)
				throw Exception("WSAEnumNetworkEvents failed");
		}

		HANDLE wait_handle;
		SOCKET handle;
	};

	class TCPSocket : public SocketHandle
	{
	public:
		TCPSocket()
		{
			handle = socket(AF_INET, SOCK_STREAM, 0);
			if (handle == INVALID_SOCKET)
				throw Exception("Unable to create socket handle");
		}

		TCPSocket(SOCKET init_handle)
		{
			handle = init_handle;
			if (handle == INVALID_SOCKET)
				throw Exception("Invalid socket handle");
		}
	};

#else

	class SocketHandle
	{
	public:
		virtual void begin_wait(fd_set &rfds, fd_set &wfds, int &max_fd) = 0;
		virtual void end_wait(fd_set &rfds, fd_set &wfds) = 0;
	};

	class TCPSocket : public SocketHandle
	{
	public:
		TCPSocket()
			: handle(-1), can_write(false)
		{
			handle = socket(AF_INET, SOCK_STREAM, 0);
			if (handle == -1)
				throw Exception("Unable to create socket handle");

#if defined(SO_NOSIGPIPE)
			int value = 1;
			setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
		}

		TCPSocket(int handle)
			: handle(handle)
		{
		}

		~TCPSocket()
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
			if (!can_write)
				FD_SET(handle, &wfds);
			max_fd = std::max(max_fd, handle);
		}

		void end_wait(fd_set &rfds, fd_set &wfds) override
		{
			if (FD_ISSET(handle, &wfds))
			{
				can_write = true;
			}
		}

		int handle;
		bool can_write;
	};

#endif

}
