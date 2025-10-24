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
**    Harry Storbacka
**    Mark Page
*/

#include "SDL/precomp.h"
#include "API/Core/System/event_provider.h"
#include "API/Core/System/event.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/thread_local_storage.h"
#include "API/Display/Window/display_window_message.h"
#include "display_message_queue_sdl.h"
#include "sdl_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_SDL construction:

CL_DisplayMessageQueue_SDL::CL_DisplayMessageQueue_SDL() : has_clan_event_peeked(false)
{
}

CL_DisplayMessageQueue_SDL::~CL_DisplayMessageQueue_SDL()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_SDL attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_SDL operations:

int CL_DisplayMessageQueue_SDL::wait(int count, CL_Event const * const * events, int timeout, bool wait_all)
{
	int index_events;
	int num_events = 0;
	for (index_events = 0; index_events < count; index_events++)
	{
		bool flagged = events[index_events]->get_event_provider()->check_before_wait();
		if (flagged)
			return index_events;
		num_events += events[index_events]->get_event_provider()->get_num_event_handles();
	}

	if (num_events == 0)
	{
		int result = msg_wait_for_multiple_objects(0, 0, timeout);
		if (result == 0)
			return -2;
		else
			return -1;
	}
	else
	{
		std::vector<CL_SOCKET_MESSAGE> handles;
		for (index_events = 0; index_events < count; index_events++)
		{
			int num_handles = events[index_events]->get_event_provider()->get_num_event_handles();
			for (int i=0; i<num_handles; i++)
			{
				CL_EventProvider *provider = events[index_events]->get_event_provider();
				if (provider == 0)
					throw CL_Exception(cl_text("CL_EventProvider is a null pointer!"));

				CL_SOCKET_MESSAGE msg;
				
				msg.handle = provider->get_event_handle(i);
				msg.type = provider->get_event_type(i);
				handles.push_back(msg);
			}
		}

		while (true)
		{
			int result = msg_wait_for_multiple_objects(handles.size(), &handles[0], timeout);
			int index = 0;
			if (result >= 0 && result < num_events+1)
				index = result;
			else
				break;

			if (index == num_events)
				return -2;

			for (index_events = 0; index_events < count; index_events++)
			{
				int num_handles = events[index_events]->get_event_provider()->get_num_event_handles();
				if (index < (int) num_handles)
				{
					bool flagged = events[index_events]->get_event_provider()->check_after_wait((int) index);
					if (flagged)
						return index_events;
					break;
				}
				index -= (int) num_handles;
			}
		}

		return -1;
	}
}

CL_DisplayWindowMessage CL_DisplayMessageQueue_SDL::get_message()
{
	if (has_clan_event_peeked)
	{
		has_clan_event_peeked = false;
		CL_DisplayWindowMessage clmsg;
		CL_DataBuffer buffer(&clan_event_peeked, sizeof(clan_event_peeked));
		clmsg.set_msg(cl_text("SDL_Event"), buffer);
		return clmsg;			
	}

	SDL_Event clan_event;

	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_SDLWindow *>::size_type index, size;
	size = thread_data->windows.size();
	for (index = 0; index < size; index++)
	{
		if (thread_data->windows[index]->get_message(clan_event))
		{
			CL_DisplayWindowMessage clmsg;
			CL_DataBuffer buffer(&clan_event, sizeof(clan_event));
			clmsg.set_msg(cl_text("SDL_Event"), buffer);
			return clmsg;			
		}
	}
	return CL_DisplayWindowMessage::null();
}

CL_DisplayWindowMessage CL_DisplayMessageQueue_SDL::peek_message(bool block)
{
	if (has_clan_event_peeked)
	{
		CL_DisplayWindowMessage clmsg;
		CL_DataBuffer buffer(&clan_event_peeked, sizeof(clan_event_peeked));
		clmsg.set_msg(cl_text("SDL_Event"), buffer);
		return clmsg;			
	}

	if (block)
		wait(0, 0, -1, false);


	SDL_Event clan_event;

	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_SDLWindow *>::size_type index, size;
	size = thread_data->windows.size();
	for (index = 0; index < size; index++)
	{
		if (thread_data->windows[index]->get_message(clan_event))
		{
			clan_event_peeked = clan_event;	
			has_clan_event_peeked = true;

			CL_DisplayWindowMessage clmsg;
			CL_DataBuffer buffer(&clan_event, sizeof(clan_event));
			clmsg.set_msg(cl_text("SDL_Event"), buffer);
			return clmsg;			
		}
	}
	return CL_DisplayWindowMessage::null();
}

void CL_DisplayMessageQueue_SDL::dispatch_message(const CL_DisplayWindowMessage &message)
{
	// TODO: Fix me for linux (use XSendEvent( disp, window, False, 0, &event ); somehow
	//MSG msg = message.get_msg();
	//TranslateMessage(&msg);
	//DispatchMessage(&msg);

	CL_SharedPtr<ThreadData> data = get_thread_data();
	for (std::vector<CL_SDLWindow *>::size_type i = 0; i < data->windows.size(); i++)
	{
		data->windows[i]->get_ic().process_messages();
	}

}

void CL_DisplayMessageQueue_SDL::add_client(CL_SDLWindow *window)
{
	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	thread_data->windows.push_back(window);
}

void CL_DisplayMessageQueue_SDL::remove_client(CL_SDLWindow *window)
{
	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_SDLWindow *>::size_type index, size;
	size = thread_data->windows.size();
	for (index = 0; index < size; index++)
	{
		if (thread_data->windows[index] == window)
		{
			thread_data->windows.erase(thread_data->windows.begin() + index);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_SDL implementation:

CL_SharedPtr<CL_DisplayMessageQueue_SDL::ThreadData> CL_DisplayMessageQueue_SDL::get_thread_data()
{
	CL_SharedPtr<ThreadData> data(CL_ThreadLocalStorage::get_variable(cl_text("CL_DisplayMessageQueue_SDL::thread_data")));
	if (data.is_null())
	{
		data = CL_SharedPtr<ThreadData>(new ThreadData);
		CL_ThreadLocalStorage::set_variable(cl_text("CL_DisplayMessageQueue_SDL::thread_data"), data);
	}
	return data;
}

// count: number of objects (Maybe 0)
// handle_ptr: The handles (count in length). (Maybe NULL)
// timeout: timeout in milliseconds. <0 = No timeout. 0 = Return straight away
// Returns: 0 to count(-1) --> An event triggered
// Returns: count -> A new message is available, but not the one that we are waiting for
// Returns: -1 --> A message available (or timeout)
int CL_DisplayMessageQueue_SDL::msg_wait_for_multiple_objects(int count, CL_SOCKET_MESSAGE *handle_ptr, int timeout)
{
	unsigned int start_time = 0;
	if (timeout > 0)
	{
		start_time = CL_System::get_time();
	}
#ifndef WIN32
	//TODO: Fixme for win32
	struct timeval empty_timeout;
	empty_timeout.tv_sec = 0;
	empty_timeout.tv_usec = 0;
#endif
	while (true)
	{
#ifndef WIN32
		if (count > 0)
		{
			bool reads = false;
			bool writes = false;
			bool exceptions = false;
			int highest_fd = -1;
			fd_set rfds, wfds, efds;
			FD_ZERO(&rfds);
			FD_ZERO(&wfds);
			FD_ZERO(&efds);

			for (int i=0; i<count; i++)
			{
				int handle = handle_ptr[i].handle;
				switch (handle_ptr[i].type)
				{
					case CL_EventProvider::type_fd_read:
						FD_SET(handle, &rfds);
						if (handle > highest_fd)
							highest_fd = handle;
						reads = true;
						break;
					case CL_EventProvider::type_fd_write:
							FD_SET(handle, &wfds);
					if (handle > highest_fd)
							highest_fd = handle;
						writes = true;
							break;
					case CL_EventProvider::type_fd_exception:
					FD_SET(handle, &efds);
						if (handle > highest_fd)
							highest_fd = handle;
						exceptions = true;
						break;
				}
			}

			int result = select(
				highest_fd+1,
				reads ? &rfds : 0,
				writes ? &wfds : 0,
				exceptions ? &efds : 0,
				&empty_timeout);

			if (result > 0)
			{
				// find the flagged sockets
				for (int i=0; i<count; i++)
				{
					int handle = handle_ptr[i].handle;
					switch (handle_ptr[i].type)
					{
						case CL_EventProvider::type_fd_read:
							if (FD_ISSET(handle, &rfds))
								return i;
							break;
						case CL_EventProvider::type_fd_write:
							if (FD_ISSET(handle, &wfds))
								return i;
							break;
						case CL_EventProvider::type_fd_exception:
							if (FD_ISSET(handle, &efds))
								return i;
							break;
					}
				}
			}
		}
#endif

		if (has_internal_messages())
		{
			break;
		}

		if (timeout == 0)	// No timeout specified
		{
			return -1;
		}

		if (timeout > 0)
		{
			unsigned int time_now = CL_System::get_time();
			if (time_now < start_time)	// Integer wraparound
			{
				start_time = 0;	// TODO: Fix integer wraparound correctly
			}

			if ((time_now - start_time) >= timeout)
			{
				return -1;
			}
#ifdef WIN32
			::Sleep(0);	// Let other processes have some cpu cycles
#else
			sleep(0);	// Let other processes have some cpu cycles
#endif
		}
	}
	return count;
}

bool CL_DisplayMessageQueue_SDL::has_internal_messages()
{
	if (has_clan_event_peeked)
	{
		return true;
	}

	bool message_flag = false;

	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_SDLWindow *>::size_type index, size;
	size = thread_data->windows.size();
	for (index = 0; index < size; index++)
	{
		if (thread_data->windows[index]->has_messages())
		{
			message_flag = true;
		}
	}
	return message_flag;
}
