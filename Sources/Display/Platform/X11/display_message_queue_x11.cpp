/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "display_message_queue_x11.h"
#include "x11_window.h"
#include <dlfcn.h>
#include "../../setup_display.h"
#include "API/Core/System/system.h"

namespace clan
{
	DisplayMessageQueue_X11::DisplayMessageQueue_X11()
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
			display = XOpenDisplay(nullptr);
			if (!display)
				throw Exception("Could not open X11 display!");
		}
		return display;
	}

	void DisplayMessageQueue_X11::add_client(X11Window *window)
	{
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

	std::shared_ptr<DisplayMessageQueue_X11::ThreadData> DisplayMessageQueue_X11::get_thread_data()
	{
		std::shared_ptr<ThreadData> data = std::dynamic_pointer_cast<ThreadData>(ThreadLocalStorage::get_variable("DisplayMessageQueue_X11::thread_data"));
		if (!data)
		{
			data = std::shared_ptr<ThreadData>(new ThreadData);
			ThreadLocalStorage::set_variable("DisplayMessageQueue_X11::thread_data", data);
		}
		return data;
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
				current_mouse_capture_window = nullptr;
		}

	}

	void DisplayMessageQueue_X11::run()
	{
		process(-1);
	}

	void DisplayMessageQueue_X11::exit()
	{
		exit_event.set();
	}

	bool DisplayMessageQueue_X11::process(int timeout_ms)
	{
		auto time_start = System::get_time();
		int x11_handle = ConnectionNumber(display);



		while (true)
		{
			process_message();

			auto time_now = System::get_time();
			int time_remaining_ms = timeout_ms - (time_now - time_start);

			struct timeval tv;
			if (time_remaining_ms > 0)
			{
				tv.tv_sec = time_remaining_ms / 1000;
				tv.tv_usec = (time_remaining_ms % 1000) * 1000;
			}
			else
			{
				tv.tv_sec = 0;
				tv.tv_usec = 0;
			}

			fd_set rfds;
			FD_ZERO(&rfds);

			FD_SET(x11_handle, &rfds);
			FD_SET(async_work_event.read_fd(), &rfds);
			FD_SET(exit_event.read_fd(), &rfds);

			int result = select(std::max(std::max(async_work_event.read_fd(), x11_handle), exit_event.read_fd()) + 1, &rfds, nullptr, nullptr, &tv);
			if (result > 0)
			{
				if (FD_ISSET(async_work_event.read_fd(), &rfds))
				{
					async_work_event.reset();
					process_async_work();
				}
				if (FD_ISSET(exit_event.read_fd(), &rfds))
				{
					exit_event.reset();
					return false;
				}
			}
			else
			{
				break;
			}
		}
		return true;
	}

	void DisplayMessageQueue_X11::post_async_work_needed()
	{
		async_work_event.set();
	}

	void DisplayMessageQueue_X11::process_message()
	{
		std::shared_ptr<ThreadData> data = get_thread_data();

		::Display *display = get_display();
		XEvent event;
		while (XPending(display) > 0)
		{
			XNextEvent(display, &event);

			for (auto & elem : data->windows)
			{
				X11Window *window = elem;
				if (window->get_window() == event.xany.window)
				{
					X11Window *mouse_capture_window = current_mouse_capture_window;
					if (mouse_capture_window == nullptr)
						mouse_capture_window = window;

					window->process_message(event, mouse_capture_window);
				}
			}
		}

		data = get_thread_data();	// We must update the thread data, since "window->process_message" above might have deleted a window
		for (auto & elem : data->windows)
		{
			InputContext context = elem->get_ic();
			elem->process_window();
			if (!context.is_disposed())	// Call if window was not destroyed
				context.process_messages();
		}
	}
}

