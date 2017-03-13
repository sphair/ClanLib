/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#include "Network/precomp.h"
#include "API/Network/Socket/network_condition_variable.h"
#include "tcp_socket.h"

#ifndef WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

namespace clan
{
#if defined(WIN32)
	class NetworkConditionVariableImpl
	{
	public:
		NetworkConditionVariableImpl()
		{
			notify_handle = CreateEvent(0, TRUE, FALSE, 0);
			if (notify_handle == INVALID_HANDLE_VALUE)
				throw Exception("Unable to create event object");
		}

		~NetworkConditionVariableImpl()
		{
			CloseHandle(notify_handle);
		}

		HANDLE notify_handle;
	};

	NetworkConditionVariable::NetworkConditionVariable() : impl(std::make_shared<NetworkConditionVariableImpl>())
	{
	}

	bool NetworkConditionVariable::wait_impl(int count, NetworkEvent **events, int timeout)
	{
		std::vector<HANDLE> handles;
		handles.reserve(count + 1);
		for (int i = 0; i < count; i++)
		{
			handles.push_back(events[i]->get_socket_handle()->wait_handle);
		}
		handles.push_back(impl->notify_handle);

		DWORD result = WaitForMultipleObjects(handles.size(), &handles[0], FALSE, timeout > 0 ? timeout : INFINITE);
		if (result == WAIT_TIMEOUT)
			return false;
		else if (result < WAIT_OBJECT_0 || result > WAIT_OBJECT_0 + count)
			throw Exception("WaitForMultipleObjects failed");

		int event_index = result - WAIT_OBJECT_0;
		if (event_index < count)
			events[event_index]->get_socket_handle()->reset_wait_handle();
		else
			ResetEvent(impl->notify_handle);

		return true;
	}

	void NetworkConditionVariable::notify()
	{
		SetEvent(impl->notify_handle);
	}

#else

	class NetworkConditionVariableImpl
	{
	public:
		NetworkConditionVariableImpl()
		{
			int result = pipe(notify_handle);
			if (result < 0)
				throw Exception("Unable to create pipe handle");

			result = fcntl(notify_handle[0], F_SETFL, O_NONBLOCK);
			if (result < 0)
			{
				::close(notify_handle[0]);
				::close(notify_handle[1]);
				throw Exception("Unable to set pipe non-blocking mode");
			}
		}

		~NetworkConditionVariableImpl()
		{
			::close(notify_handle[0]);
			::close(notify_handle[1]);
		}

		void reset_notify()
		{
			unsigned char buf;
			while (read(notify_handle[0], &buf, 1) == 1);
		}

		void set_notify()
		{
			::write(notify_handle[1], "x", 1);
		}

		int notify_handle[2];
	};

	NetworkConditionVariable::NetworkConditionVariable() : impl(std::make_shared<NetworkConditionVariableImpl>())
	{
	}

	bool NetworkConditionVariable::wait_impl(int count, NetworkEvent **events, int timeout_ms)
	{
		int max_fd = impl->notify_handle[0];

		fd_set rfds, wfds;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_SET(impl->notify_handle[0], &rfds);

		for (int i = 0; i < count; i++)
		{
			events[i]->get_socket_handle()->begin_wait(rfds, wfds, max_fd);
		}

		timeval tv;
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = (timeout_ms % 1000) * 1000;

		//FIXME - Should this have: Use "!impl->can_write ? &wfds : 0" ??
		int result = select(max_fd + 1, &rfds, 0, 0, timeout_ms >= 0 ? &tv : 0);
		if (result == -1)
			throw Exception("select failed");

		for (int i = 0; i < count; i++)
		{
			events[i]->get_socket_handle()->end_wait(rfds, wfds);
		}

		impl->reset_notify();

		return result > 0;
	}

	void NetworkConditionVariable::notify()
	{
		impl->set_notify();
	}

#endif
}
