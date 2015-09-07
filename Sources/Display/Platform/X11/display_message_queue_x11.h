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

#pragma once

#include "Display/System/run_loop_impl.h"
#include <vector>
#include <X11/Xlib.h>
#include "API/Core/System/thread_local_storage.h"
#include <unistd.h>
#include <fcntl.h>

namespace clan
{
	class X11Window;

	class DisplayMessageQueue_X11 : public RunLoopImpl
	{
	public:
		DisplayMessageQueue_X11();
		~DisplayMessageQueue_X11();

		::Display *get_display();
		void add_client(X11Window *window);
		void remove_client(X11Window *window);

		class ThreadData : public ThreadLocalStorageData
		{
		public:
			ThreadData() {}
			std::vector<X11Window *> windows;
		};

		std::shared_ptr<ThreadData> get_thread_data();

		void set_mouse_capture(X11Window *window, bool state);

		// The library will be opened / closed by this class
		// Returns 0 if the library could not be found
		// Currently, only supports a single library
		void *dlopen_opengl(const char *filename, int flag);

		void run() override;
		void exit() override;
		bool process(int timeout_ms) override;
		void post_async_work_needed() override;

	private:
		void process_message();

		X11Window *current_mouse_capture_window = nullptr;
		::Display *display = nullptr;
		void *dlopen_lib_handle = nullptr;

		class NotifyEvent
		{
		public:
			NotifyEvent()
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

			~NotifyEvent()
			{
				::close(notify_handle[0]);
				::close(notify_handle[1]);
			}

			int read_fd() const { return notify_handle[0]; }

			void reset()
			{
				unsigned char buf;
				while (read(notify_handle[0], &buf, 1) == 1);
			}

			void set()
			{
				::write(notify_handle[1], "x", 1);
			}

		private:
			int notify_handle[2];
		};

		NotifyEvent async_work_event;
		NotifyEvent exit_event;
	};
}

/*
namespace clan
{

class Event;

class SocketMessage_X11
{
public:
	SocketMessage_X11() {}

	EventProvider::EventType type;
	int handle;
};

class X11Window;

class DisplayMessageQueue_X11
{
/// \name Construction
/// \{

public:
	DisplayMessageQueue_X11();

	~DisplayMessageQueue_X11();

	static DisplayMessageQueue_X11 message_queue;

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	int wait(const std::vector<Event> &events, int timeout);

	::Display *get_display();
	void add_client(X11Window *window);
	void remove_client(X11Window *window);

	void set_mouse_capture(X11Window *window, bool state);

	// The library will be opened / closed by this class
	// Returns 0 if the library could not be found
	// Currently, only supports a single library
	void *dlopen_opengl(const char *filename, int flag);
/// \}
/// \name Implementation
/// \{

private:
	void process_message();

	class ThreadData : public ThreadLocalStorageData
	{
	public:
		ThreadData() {}
		std::vector<X11Window *> windows;
	};

	 std::shared_ptr<ThreadData> get_thread_data();
	bool has_internal_messages();
	int msg_wait_for_multiple_objects(std::vector<SocketMessage_X11> &event_handles, int timeout);
	void process_queued_events();
	X11Window *current_mouse_capture_window;

	::Display *display;
	void *dlopen_lib_handle;
/// \}
};

}
*/
