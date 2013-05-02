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
#include <dlfcn.h>

namespace clan
{

DisplayMessageQueue_X11 DisplayMessageQueue_X11::message_queue;

/////////////////////////////////////////////////////////////////////////////
// DisplayMessageQueue_X11 construction:

DisplayMessageQueue_X11::DisplayMessageQueue_X11() : current_mouse_capture_window(NULL), display(0), dlopen_lib_handle(NULL)
{
}

DisplayMessageQueue_X11::~DisplayMessageQueue_X11()
{
	if (display)
	{
		XCloseDisplay(display);
	}

	// This MUST be called after XCloseDisplay - It is used for http://www.xfree86.org/4.8.0/DRI11.html
	if (dlopen_lib_handle)
	{
		dlclose(dlopen_lib_handle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// DisplayMessageQueue_X11 attributes:


/////////////////////////////////////////////////////////////////////////////
// DisplayMessageQueue_X11 operations:

void *DisplayMessageQueue_X11::dlopen_opengl(const char *filename, int flag)
{
	if (!dlopen_lib_handle)		// This is a shared resource. We assume that filename and flags will never change, which makes sense in this case
	{
		dlopen_lib_handle = ::dlopen(filename, flag);
	}
	return dlopen_lib_handle;
}

::Display *DisplayMessageQueue_X11::get_display()
{
	if (!display)
	{
		display = XOpenDisplay(NULL);
		if (!display)
			throw Exception("Could not open X11 display!");
	}
	return display;
}

int DisplayMessageQueue_X11::wait(const std::vector<Event> &events, int timeout)
{
	process_queued_events();

	int num_events = 0;
	for (std::vector<Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
	{
		bool flagged = events[index_events].get_event_provider()->check_before_wait();
		if (flagged)
			return index_events;
		num_events += events[index_events].get_event_provider()->get_num_event_handles();
	}

	if (num_events == 0)
	{
		std::vector<SocketMessage_X11> empty_handles;
		int result = msg_wait_for_multiple_objects(empty_handles, timeout);
		if (result == 0)	// X11 message found
		{
			process_message();
			return events.size();

		}
	}
	else
	{

		std::vector<SocketMessage_X11> handles;

		for (std::vector<Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
		{
			int num_handles = events[index_events].get_event_provider()->get_num_event_handles();
			for (int i=0; i<num_handles; i++)
			{
				EventProvider *provider = events[index_events].get_event_provider();
				if (provider == 0)
					throw Exception("EventProvider is a null pointer!");

				SocketMessage_X11 msg;
				
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

			for (std::vector<Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
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

void DisplayMessageQueue_X11::add_client(X11Window *window)
{
	// (Always set the message queue, because the display target may have changed)
	KeepAlive::func_event_wait().set(&message_queue, &DisplayMessageQueue_X11::wait);

	 std::shared_ptr<ThreadData> thread_data = get_thread_data();
	thread_data->windows.push_back(window);
}

void DisplayMessageQueue_X11::remove_client(X11Window *window)
{
	 std::shared_ptr<ThreadData> thread_data = get_thread_data();
	std::vector<X11Window *>::size_type index, size;
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

void DisplayMessageQueue_X11::set_mouse_capture(X11Window *window, bool state)
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
// DisplayMessageQueue_X11 implementation:

 std::shared_ptr<DisplayMessageQueue_X11::ThreadData> DisplayMessageQueue_X11::get_thread_data()
{
	std::shared_ptr<ThreadData> data = std::dynamic_pointer_cast<ThreadData>(ThreadLocalStorage::get_variable("DisplayMessageQueue_X11::thread_data"));
	if (!data)
	{
		data =  std::shared_ptr<ThreadData>(new ThreadData);
		ThreadLocalStorage::set_variable("DisplayMessageQueue_X11::thread_data", data);
	}
	return data;
}

// event_handles: The handles
// timeout: timeout in milliseconds. <0 = No timeout. 0 = Return straight away
// Returns: 0 to event_handles.size()-1 --> An event triggered
// Returns: event_handles.size() -> A new message is available, but not the one that we are waiting for
// Returns: -1 -->  timeout
int DisplayMessageQueue_X11::msg_wait_for_multiple_objects(std::vector<SocketMessage_X11> &event_handles, int timeout)
{
	bool internal_message_found = has_internal_messages();
	if (internal_message_found)
	{
		timeout = 0;		// Do not wait, if an internal event is available
	}

	std::vector<SocketMessage_X11> all_events = event_handles;

	if ( (internal_message_found == false) && (timeout != 0) ) // We do not need to check for window file descriptors, when a message was already found, or when we exit straight away
	{

		SocketMessage_X11 screen_connection;
		screen_connection.type = EventProvider::type_fd_read;
		screen_connection.handle = ConnectionNumber(display);
		all_events.push_back(screen_connection);

		// Get the window file descriptors
		 std::shared_ptr<ThreadData> thread_data = get_thread_data();
		std::vector<X11Window *>::size_type index, size;
		size = thread_data->windows.size();
		for (index = 0; index < size; index++)
		{
			std::vector<SocketMessage_X11> window_events = thread_data->windows[index]->get_window_socket_messages();
			std::vector<SocketMessage_X11>::size_type index, window_events_size, current_size;
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

	std::vector<SocketMessage_X11>::size_type message_index, num_messages;
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
			throw Exception("Invalid file descriptor handle - maybe use poll() instead of select()?");
		}

		switch (all_events[message_index].type)
		{
			case EventProvider::type_fd_read:
				FD_SET(handle, &rfds);
				if (handle > highest_fd)
					highest_fd = handle;
				reads = true;
				break;
			case EventProvider::type_fd_write:
				FD_SET(handle, &wfds);
				if (handle > highest_fd)
					highest_fd = handle;
				writes = true;
				break;
			case EventProvider::type_fd_exception:
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
				case EventProvider::type_fd_read:
					if (FD_ISSET(handle, &rfds))
						object_id = message_index;
					break;
				case EventProvider::type_fd_write:
					if (FD_ISSET(handle, &wfds))
						object_id = message_index;
					break;
				case EventProvider::type_fd_exception:
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

bool DisplayMessageQueue_X11::has_internal_messages()
{
	bool message_flag = false;

	if (display)
	{
		if (XPending(display) > 0)
		{
			message_flag = true;
		}
	}

	 std::shared_ptr<ThreadData> thread_data = get_thread_data();
	std::vector<X11Window *>::size_type index, size;
	size = thread_data->windows.size();
	for (index = 0; index < size; index++)
	{
		if (thread_data->windows[index]->process_window_sockets(true))
			message_flag = true;
	}
	return message_flag;
}

void DisplayMessageQueue_X11::process_message()
{
	 std::shared_ptr<ThreadData> data = get_thread_data();

	XEvent event;
	while(XPending(display) > 0)
	{
		XNextEvent(display, &event);

		for (std::vector<X11Window *>::size_type i = 0; i < data->windows.size(); i++)
		{
			X11Window *window = data->windows[i];
			if (window->get_window() == event.xany.window)
			{
				X11Window *mouse_capture_window = current_mouse_capture_window;
				if (mouse_capture_window == NULL)
					mouse_capture_window = window;

				window->process_message(event, mouse_capture_window);
			}
		}
	}

	for (std::vector<X11Window *>::size_type i = 0; i < data->windows.size(); i++)
	{
		data->windows[i]->process_message_complete();
	}

	// Process all input context messages (done seperately, because of the mouse_capture hack)
	for (std::vector<X11Window *>::size_type i = 0; i < data->windows.size(); i++)
	{
		InputContext context = data->windows[i]->get_ic();
		context.process_messages();
	}
}

void DisplayMessageQueue_X11::process_queued_events()
{
	std::shared_ptr<ThreadData> thread_data = get_thread_data();
	std::vector<X11Window *>::size_type index, size;
	size = thread_data->windows.size();
	for (index = 0; index < size; index++)
	{
		thread_data->windows[index]->process_queued_events();
	}
}

}
