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

#include "socket_select.h"
#include "socket_generic.h"

#include "API/Core/System/mutex.h"
#include "API/Core/System/cl_assert.h"

#include <vector>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_SocketSelect construction:

CL_SocketSelect::CL_SocketSelect()
: listen_thread(NULL), mutex(NULL), stop_thread(false)
{
#ifdef WIN32
	signal_event = WSACreateEvent();
#else
	pipe(signal_pipes);
#endif

	mutex = CL_Mutex::create();
	listen_thread = new CL_Thread(this);

	listen_thread->start();
}

CL_SocketSelect::~CL_SocketSelect()
{
	stop_thread = true;
	signal_listen_thread();
	listen_thread->wait();
	delete listen_thread;
	delete mutex;

#ifdef WIN32
	WSACloseEvent(signal_event);
#else
	close(signal_pipes[0]);
	close(signal_pipes[1]);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_SocketSelect attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_SocketSelect operations:

void CL_SocketSelect::listen_read(CL_EventTrigger_Socket *socket)
{
	{
		CL_MutexSection mutex_section(mutex);
		if (std::find(read_sockets.begin(), read_sockets.end(), socket) == read_sockets.end())
			read_sockets.push_back(socket);
	}
	
	signal_listen_thread();
}

void CL_SocketSelect::listen_write(CL_EventTrigger_Socket *socket)
{
	{
		CL_MutexSection mutex_section(mutex);
		if (std::find(write_sockets.begin(), write_sockets.end(), socket) == write_sockets.end())
			write_sockets.push_back(socket);
	}
	
	signal_listen_thread();
}

void CL_SocketSelect::listen_exception(CL_EventTrigger_Socket *socket)
{
	{
		CL_MutexSection mutex_section(mutex);
		if (std::find(exception_sockets.begin(), exception_sockets.end(), socket) == exception_sockets.end())
			exception_sockets.push_back(socket);
	}
	
	signal_listen_thread();
}

void CL_SocketSelect::remove_read(CL_EventTrigger_Socket *socket)
{
	{
		CL_MutexSection mutex_section(mutex);
		read_sockets.remove(socket);
	}
	
	signal_listen_thread();
}

void CL_SocketSelect::remove_write(CL_EventTrigger_Socket *socket)
{
	{
		CL_MutexSection mutex_section(mutex);
		write_sockets.remove(socket);
	}
	
	signal_listen_thread();
}

void CL_SocketSelect::remove_exception(CL_EventTrigger_Socket *socket)
{
	{
		CL_MutexSection mutex_section(mutex);
		exception_sockets.remove(socket);
	}

	signal_listen_thread();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SocketSelect implementation:

void CL_SocketSelect::run()
{
	while (!stop_thread)
	{
#ifdef WIN32
		std::vector<WSAEVENT> events;
		std::vector<int> sockets;
		std::list<CL_EventTrigger_Socket*>::iterator it;

		{
			CL_MutexSection mutex_section(mutex);

			WSAResetEvent(signal_event);
			events.push_back(signal_event);

			std::map<int, unsigned int> flags;

			for (it = read_sockets.begin(); it != read_sockets.end(); it++)
			{
				int sock = (*it)->get_socket()->sock;
				if (flags.find(sock) == flags.end()) flags[sock] = FD_READ|FD_ACCEPT|FD_CLOSE;
				else flags[sock] |= FD_READ|FD_ACCEPT|FD_CLOSE;
			}
			for (it = write_sockets.begin(); it != write_sockets.end(); it++)
			{
				int sock = (*it)->get_socket()->sock;
				if (flags.find(sock) == flags.end()) flags[sock] = FD_WRITE|FD_CONNECT;
				else flags[sock] |= FD_WRITE|FD_CONNECT;
			}
			for (it = exception_sockets.begin(); it != exception_sockets.end(); it++)
			{
				int sock = (*it)->get_socket()->sock;
				if (flags.find(sock) == flags.end()) flags[sock] = FD_OOB;
				else flags[sock] |= FD_OOB;
			}

			std::map<int, unsigned int>::iterator it_flags;
			for (it_flags = flags.begin(); it_flags != flags.end(); it_flags++)
			{
				WSAEVENT event = WSACreateEvent();
				WSAEventSelect(it_flags->first, event, it_flags->second);
				events.push_back(event);
				sockets.push_back(it_flags->first);
			}
		}

		int num_events = events.size();

		DWORD result = WSAWaitForMultipleEvents(
			num_events, &events[0], FALSE, WSA_INFINITE, FALSE);

		cl_assert(
			result != WSA_WAIT_FAILED &&
			result != WAIT_IO_COMPLETION &&
			result != WSA_WAIT_TIMEOUT &&
			result >= WSA_WAIT_EVENT_0);

		for (int i=1; i<num_events; i++)
		{
			WSAEventSelect(sockets[i-1], events[i], 0);
			WSACloseEvent(events[i]);
		}

		if (result == WSA_WAIT_EVENT_0) continue; // signal event

		int sock = sockets[result - WSA_WAIT_EVENT_0-1];

		// ok, we found a trigger. Now thanks to winsock lameness, we could only listen for ALL events for a socket,
		// so now we do a old-skool select() on this socket. As usual Microsoft managed to fuck up.

		{
			fd_set rfds, wfds, efds;
			FD_ZERO(&rfds);
			FD_ZERO(&wfds);
			FD_ZERO(&efds);
			FD_SET(sock, &rfds);
			FD_SET(sock, &wfds);
			FD_SET(sock, &efds);

			timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			
			int result = select(sock+1, &rfds, &wfds, &efds, &tv);

			// verify trigger is still valid. if it is, raise its flag.
			CL_MutexSection mutex_section(mutex);
			if (FD_ISSET(sock, &rfds))
			{
				for (it = read_sockets.begin(); it != read_sockets.end(); it++)
				{
					CL_EventTrigger_Socket *trigger = *it;
					if (trigger->get_socket()->sock == sock)
					{
						trigger->set_flag();
						read_sockets.erase(it);
						break;
					}
				}
			}
			if (FD_ISSET(sock, &wfds))
			{
				for (it = write_sockets.begin(); it != write_sockets.end(); it++)
				{
					CL_EventTrigger_Socket *trigger = *it;
					if (trigger->get_socket()->sock == sock)
					{
						trigger->set_flag();
						write_sockets.erase(it);
						break;
					}
				}
			}
			if (FD_ISSET(sock, &efds))
			{
				for (it = exception_sockets.begin(); it != exception_sockets.end(); it++)
				{
					CL_EventTrigger_Socket *trigger = *it;
					if (trigger->get_socket()->sock == sock)
					{
						trigger->set_flag();
						exception_sockets.erase(it);
						break;
					}
				}
			}
		}
#else
		bool write_empty, exception_empty;
		fd_set rfds, wfds, efds;
		std::list<CL_EventTrigger_Socket*>::iterator it;
		int highest_fd = signal_pipes[0];
		FD_ZERO(&rfds);
		FD_SET(signal_pipes[0], &rfds);

		// lock mutex and generate listen file descriptor tables.
		{
			CL_MutexSection mutex_section(mutex);

			write_empty = write_sockets.empty();
			exception_empty = exception_sockets.empty();

			for (it = read_sockets.begin(); it != read_sockets.end(); it++)
			{
				int sock = (*it)->get_socket()->sock;

				if (sock > highest_fd) highest_fd = sock;
				FD_SET(sock, &rfds);
			}

			if (write_empty)
			{
				FD_ZERO(&wfds);
				for (it = write_sockets.begin(); it != write_sockets.end(); it++)
				{
					int sock = (*it)->get_socket()->sock;

					if (sock > highest_fd) highest_fd = sock;
					FD_SET(sock, &wfds);
				}
			}

			if (exception_empty)
			{
				FD_ZERO(&efds);
				for (it = exception_sockets.begin(); it != exception_sockets.end(); it++)
				{
					int sock = (*it)->get_socket()->sock;

					if (sock > highest_fd) highest_fd = sock;
					FD_SET(sock, &efds);
				}
			}
		}

		int result = select(
			highest_fd+1,
			&rfds,
			write_empty ? NULL : &wfds,
			exception_empty ? NULL : &efds,
			0);

		if (FD_ISSET(signal_pipes[0], &rfds)) // signal event
		{
			// Empty pipe for signals:
			while (true)
			{
				fd_set rfds;
				FD_ZERO(&rfds);
				FD_SET(signal_pipes[0], &rfds);
				timeval tv;
				tv.tv_sec = 0;
				tv.tv_usec = 0;
				if (select(signal_pipes[0]+1, &rfds, 0, 0, &tv) <= 0) break;
				
				char v = 0;
				read(signal_pipes[0], &v, 1);
			}
			continue;
		}

		// lock mutex, check results and notify triggers.
		{
			CL_MutexSection mutex_section(mutex);

			if (result > 0)
			{
				for (it = read_sockets.begin(); it != read_sockets.end();)
				{
					int sock = (*it)->get_socket()->sock;
					if (FD_ISSET(sock, &rfds))
					{
						(*it)->set_flag();
						it = read_sockets.erase(it);
						continue;
					}
					it++;
				}

				if (!write_empty)
				{
					for (it = write_sockets.begin(); it != write_sockets.end();)
					{
						int sock = (*it)->get_socket()->sock;
						if (FD_ISSET(sock, &wfds))
						{
							(*it)->set_flag();
							it = write_sockets.erase(it);
							continue;
						}
						it++;
					}
				}

				if (!exception_empty)
				{
					for (it = exception_sockets.begin(); it != exception_sockets.end();)
					{
						int sock = (*it)->get_socket()->sock;
						if (FD_ISSET(sock, &efds))
						{
							(*it)->set_flag();
							it = exception_sockets.erase(it);
							continue;
						}
						it++;
					}
				}
			}
		}
#endif
	}
}

void CL_SocketSelect::signal_listen_thread()
{
	#ifdef WIN32
	WSASetEvent(signal_event);
	#else
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(signal_pipes[0], &rfds);
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if (select(signal_pipes[0]+1, &rfds, 0, 0, &tv) == 0)
	{
		char byte = 1;
		write(signal_pipes[1], &byte, 1);
	}
	#endif
}
