/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/thread_local_storage.h"
#include "API/Core/System/event_provider.h"
#include "API/Core/System/event.h"
#include "API/Core/System/keep_alive.h"
#include "display_message_queue_x11.h"
#include "x11_window.h"

CL_DisplayMessageQueue_X11 CL_DisplayMessageQueue_X11::message_queue;

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_X11 construction:

CL_DisplayMessageQueue_X11::CL_DisplayMessageQueue_X11() : current_mouse_capture_window(NULL)
{
}

CL_DisplayMessageQueue_X11::~CL_DisplayMessageQueue_X11()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_X11 attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_X11 operations:

int CL_DisplayMessageQueue_X11::wait(const std::vector<CL_Event> &events, int timeout)
{
	process_queued_events();

	int num_events = 0;
	for (std::vector<CL_Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
	{
		bool flagged = events[index_events].get_event_provider()->check_before_wait();
		if (flagged)
			return index_events;
		num_events += events[index_events].get_event_provider()->get_num_event_handles();
	}

	if (num_events == 0)
	{
		std::vector<CL_SocketMessage_X11> empty_handles;
		int result = msg_wait_for_multiple_objects(empty_handles, timeout);
		if (result == 0)	// X11 message found
		{
			process_message();
			return events.size();

		}
	}
	else
	{

		std::vector<CL_SocketMessage_X11> handles;

		for (std::vector<CL_Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
		{
			int num_handles = events[index_events].get_event_provider()->get_num_event_handles();
			for (int i=0; i<num_handles; i++)
			{
				CL_EventProvider *provider = events[index_events].get_event_provider();
				if (provider == 0)
					throw CL_Exception("CL_EventProvider is a null pointer!");

				CL_SocketMessage_X11 msg;
				
				msg.handle = provider->get_event_handle(i);
				msg.type = provider->get_event_type(i);
				handles.push_back(msg);
			}
		}

		while (true)
		{
			int index = msg_wait_for_multiple_objects(handles, timeout);
			if (index < 0)
				break;

			if (index == num_events)
			{
				process_message();
				return events.size();
			}

			for (std::vector<CL_Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
			{
				int num_handles = events[index_events].get_event_provider()->get_num_event_handles();
				if (index < num_handles)
				{
					bool flagged = events[index_events].get_event_provider()->check_after_wait(index);
					if (flagged)
						return index_events;
					break;
				}
				index -= num_handles;
			}
		}

	}
	return -1;
}

void CL_DisplayMessageQueue_X11::add_client(CL_X11Window *window)
{
	// (Always set the message queue, because the display target may have changed)
	CL_KeepAlive::func_event_wait().set(&message_queue, &CL_DisplayMessageQueue_X11::wait);

	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	thread_data->windows.push_back(window);
}

void CL_DisplayMessageQueue_X11::remove_client(CL_X11Window *window)
{
	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_X11Window *>::size_type index, size;
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

void CL_DisplayMessageQueue_X11::set_mouse_capture(CL_X11Window *window, bool state)
{
	if (state)
	{
		current_mouse_capture_window = window;
	}
	else
	{
		if (current_mouse_capture_window == window)
			current_mouse_capture_window = NULL;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_X11 implementation:

CL_SharedPtr<CL_DisplayMessageQueue_X11::ThreadData> CL_DisplayMessageQueue_X11::get_thread_data()
{
	CL_SharedPtr<ThreadData> data = cl_dynamic_pointer_cast<ThreadData>(CL_ThreadLocalStorage::get_variable("CL_DisplayMessageQueue_X11::thread_data"));
	if (!data)
	{
		data = CL_SharedPtr<ThreadData>(new ThreadData);
		CL_ThreadLocalStorage::set_variable("CL_DisplayMessageQueue_X11::thread_data", data);
	}
	return data;
}

// event_handles: The handles
// timeout: timeout in milliseconds. <0 = No timeout. 0 = Return straight away
// Returns: 0 to event_handles.size()-1 --> An event triggered
// Returns: event_handles.size() -> A new message is available, but not the one that we are waiting for
// Returns: -1 -->  timeout
int CL_DisplayMessageQueue_X11::msg_wait_for_multiple_objects(std::vector<CL_SocketMessage_X11> &event_handles, int timeout)
{
	bool internal_message_found = has_internal_messages();
	if (internal_message_found)
	{
		timeout = 0;		// Do not wait, if an internal event is available
	}

	std::vector<CL_SocketMessage_X11> all_events = event_handles;

	if ( (internal_message_found == false) && (timeout != 0) ) // We do not need to check for window file descriptors, when a message was already found, or when we exit straight away
	{
		// Get the window file descriptors
		CL_SharedPtr<ThreadData> thread_data = get_thread_data();
		std::vector<CL_X11Window *>::size_type index, size;
		size = thread_data->windows.size();
		for (index = 0; index < size; index++)
		{
			std::vector<CL_SocketMessage_X11> window_events = thread_data->windows[index]->get_window_socket_messages();
			std::vector<CL_SocketMessage_X11>::size_type index, window_events_size, current_size;
			window_events_size = window_events.size();

			if (window_events_size)
			{
				// Concatenate the events to all_events
				current_size = all_events.size();
				all_events.resize( current_size + window_events_size );
				for(index = 0; index < window_events_size; ++ index)
				{
					all_events[current_size + index] = window_events[index];
				}
			}
		}
	}

	std::vector<CL_SocketMessage_X11>::size_type message_index, num_messages;
	num_messages = all_events.size();

	if (num_messages == 0)	// Nothing to wait for - Exit now
	{
		if (internal_message_found)
			return 0;
		return -1;
	}

	struct timeval tv;
	if (timeout >0)
	{
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
	}
	else if (timeout == 0)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 0;
	}
	else
	{
		tv.tv_sec = 0x7FFFFFFF;
		tv.tv_usec = 0;
	}

	bool reads = false;
	bool writes = false;
	bool exceptions = false;
	int highest_fd = -1;
	fd_set rfds, wfds, efds;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);


	for (message_index=0; message_index < num_messages; ++message_index)
	{
		int handle = all_events[message_index].handle;

		if (handle > FD_SETSIZE)
		{
			throw CL_Exception("Invalid file descriptor handle - maybe use poll() instead of select()?");
		}

		switch (all_events[message_index].type)
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
	&tv);

	int object_id = -1;

	if (result > 0)
	{
		// find the flagged sockets
		for (message_index=0; message_index < num_messages; ++message_index)
		{
			int handle = all_events[message_index].handle;
			switch (all_events[message_index].type)
			{
				case CL_EventProvider::type_fd_read:
					if (FD_ISSET(handle, &rfds))
						object_id = message_index;
					break;
				case CL_EventProvider::type_fd_write:
					if (FD_ISSET(handle, &wfds))
						object_id = message_index;
					break;
				case CL_EventProvider::type_fd_exception:
					if (FD_ISSET(handle, &efds))
						object_id = message_index;
					break;
			}
			if (object_id >=0)
				break;
		}
	}

	if (timeout != 0)	// Check for messages after the select (to improve responsiveness)
	{
		if (has_internal_messages())
		{
			if (object_id < 0)	// Not set yet
			{
				object_id = event_handles.size();	// A new (unknown) message available
			}		
		} 
	}

	if (object_id < 0)
	{
		if (internal_message_found)
			object_id = event_handles.size();	// A new (unknown) message available
	}

	return object_id;
}

bool CL_DisplayMessageQueue_X11::has_internal_messages()
{
	bool message_flag = false;

	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_X11Window *>::size_type index, size;
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

void CL_DisplayMessageQueue_X11::process_message()
{
	CL_SharedPtr<ThreadData> data = get_thread_data();
	for (std::vector<CL_X11Window *>::size_type i = 0; i < data->windows.size(); i++)
	{
		CL_X11Window *window = data->windows[i];

		CL_X11Window *mouse_capture_window = current_mouse_capture_window;
		if (mouse_capture_window == NULL)
			mouse_capture_window = window;

		window->get_message(mouse_capture_window);
	}

	// Process all input context messages (done seperately, because of the mouse_capture hack)
	for (std::vector<CL_X11Window *>::size_type i = 0; i < data->windows.size(); i++)
	{
		CL_X11Window *window = data->windows[i];
		window->get_ic().process_messages();
	}
}

void CL_DisplayMessageQueue_X11::process_queued_events()
{
	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_X11Window *>::size_type index, size;
	size = thread_data->windows.size();
	for (index = 0; index < size; index++)
	{
		thread_data->windows[index]->process_queued_events();
	}
}


