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
*/

#include "Display/precomp.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/event_provider.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/thread_local_storage.h"
#include "API/Core/System/keep_alive.h"
#include "display_message_queue_win32.h"
#ifdef __MINGW32__
#include "API/Display/Window/display_window.h"
#endif
#include "win32_window.h"

namespace clan
{

DisplayMessageQueue_Win32 DisplayMessageQueue_Win32::message_queue;

/////////////////////////////////////////////////////////////////////////////
// DisplayMessageQueue_Win32 construction:

DisplayMessageQueue_Win32::DisplayMessageQueue_Win32()
{
}

DisplayMessageQueue_Win32::~DisplayMessageQueue_Win32()
{
}

/////////////////////////////////////////////////////////////////////////////
// DisplayMessageQueue_Win32 attributes:

/////////////////////////////////////////////////////////////////////////////
// DisplayMessageQueue_Win32 operations:

int DisplayMessageQueue_Win32::wait(const std::vector<Event> &events, int timeout)
{
	bool wait_all = false;	// Only wait for first object to signal

	// We have to start out by asking PeekMessage if there's a message available, because
	// MsgWaitForMultipleObjectsEx was coded by spaghetti coders at Microsoft
	// that couldn't be bothered to document their mess. It seems that there is an unspecified
	// amount of synchronization objects that get reset by calling MsgWaitForMultipleObjectsEx,
	// making it almost impossible in an object oriented manner to be sure that its safe to call
	// MsgWaitForMultipleObjectsEx again.
	// Even though they added the MWMO_INPUTAVAILABLE flag, it does not protect us against all the
	// synchronization objects that risk being reset and is therefore useless to us.
	// By using PeekMessage we can determine whether anything was pending, including pending SendMessage
	// calls and other 'goodies' of the Win32 message queue nightmare.
	MSG msg;
	BOOL message_available = PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
	if (message_available)
	{
		process_message();
		return events.size();
	}


	DWORD timeout_win32 = (timeout == -1) ? INFINITE : timeout;

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
		DWORD result = MsgWaitForMultipleObjectsEx(0, 0, timeout_win32, QS_ALLEVENTS|QS_SENDMESSAGE|QS_RAWINPUT, wait_all ? MWMO_WAITALL : 0);
		if (result == WAIT_OBJECT_0)
		{
			process_message();
			return events.size();
		}
	}
	else
	{
		std::vector<HANDLE> handles;
		for (std::vector<Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
		{
			int num_handles = events[index_events].get_event_provider()->get_num_event_handles();
			for (int i=0; i<num_handles; i++)
				handles.push_back(events[index_events].get_event_provider()->get_event_handle(i));
		}
		while (true)
		{
			DWORD result = MsgWaitForMultipleObjectsEx(handles.size(), &handles[0], timeout_win32, QS_ALLEVENTS|QS_SENDMESSAGE|QS_RAWINPUT, wait_all ? MWMO_WAITALL : 0);
			DWORD index = 0;
			if (result == WAIT_TIMEOUT)
				break;
			else if (result == WAIT_FAILED)
			{
				DWORD error_code = GetLastError();
				throw Exception(string_format("WaitForMultipleObjects failed : %1", (int) error_code));
			}
			else if (result >= WAIT_OBJECT_0 && result < WAIT_OBJECT_0 + num_events+1)
				index = result - WAIT_OBJECT_0;
			else if (result >= WAIT_ABANDONED_0 && result < WAIT_ABANDONED_0 + num_events)
				index = result - WAIT_ABANDONED_0;
			else
				continue;

			if (index == num_events)
			{
				process_message();
				return events.size();
			}

			for (std::vector<Event>::size_type index_events = 0; index_events < events.size(); ++index_events)
			{
				int num_handles = events[index_events].get_event_provider()->get_num_event_handles();
				if (index < (DWORD) num_handles)
				{
					bool flagged = events[index_events].get_event_provider()->check_after_wait((int) index);
					if (flagged)
						return index_events;
					break;
				}
				index -= (DWORD) num_handles;
			}
		}

	}
	return -1;

}

void DisplayMessageQueue_Win32::add_client(Win32Window *window)
{
	// (Always set the message queue, because the display target may have changed)
	KeepAlive::func_event_wait() = bind_member(&message_queue, &DisplayMessageQueue_Win32::wait);

	std::shared_ptr<ThreadData> thread_data = get_thread_data();

	thread_data->windows.push_back(window);
}

void DisplayMessageQueue_Win32::remove_client(Win32Window *window)
{
	std::shared_ptr<ThreadData> thread_data = get_thread_data();
	std::vector<Win32Window *>::size_type index, size;
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
// DisplayMessageQueue_Win32 implementation:

std::shared_ptr<DisplayMessageQueue_Win32::ThreadData> DisplayMessageQueue_Win32::get_thread_data()
{
	std::shared_ptr<ThreadData> data = std::dynamic_pointer_cast<ThreadData>(ThreadLocalStorage::get_variable("DisplayMessageQueue_Win32::thread_data"));
	if (!data)
	{
		data = std::shared_ptr<ThreadData>(new ThreadData);
		ThreadLocalStorage::set_variable("DisplayMessageQueue_Win32::thread_data", data);
	}
	return data;
}

void DisplayMessageQueue_Win32::process_message()
{
	// We must use PeekMessage instead of GetMessage because GetMessage can
	// block even when MsgWaitForMultipleObjects reported messages to be
	// available.  This can happen because SendMessage between threads and
	// internal system events are processed when GetMessage and PeekMessage
	// are called and such messages are processed directly and not returned
	// by PeekMessage/GetMessage.  A call to GetMessage may therefore block
	// until a message intended for us arrives.
	//
	// PeekMessage+PM_REMOVE equals to a non-blocking GetMessage call.

	if (moduleKernel32 == 0)
	{
		// See http://support.microsoft.com/kb/976038
		// Required for exceptions and access violations not to be caught by DispatchMessage.
		moduleKernel32 = LoadLibrary(L"kernel32.dll");
		ptrSetProcessUserModeExceptionPolicy = (FuncSetProcessUserModeExceptionPolicy *)GetProcAddress(moduleKernel32, "SetProcessUserModeExceptionPolicy");
		ptrGetProcessUserModeExceptionPolicy = (FuncGetProcessUserModeExceptionPolicy *)GetProcAddress(moduleKernel32, "GetProcessUserModeExceptionPolicy");
		if (ptrSetProcessUserModeExceptionPolicy && ptrGetProcessUserModeExceptionPolicy)
		{
			DWORD flags = 0;
			if (ptrGetProcessUserModeExceptionPolicy(&flags))
				ptrSetProcessUserModeExceptionPolicy(flags & ~WIN32_PROCESS_CALLBACK_FILTER_ENABLED);
		}
	}

	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	BOOL result = PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
	if (result)
		DispatchMessage(&msg);
	
	std::shared_ptr<ThreadData> data = get_thread_data();
	for (std::vector<Win32Window *>::size_type i = 0; i < data->windows.size(); i++)
	{
		InputContext context = data->windows[i]->get_ic();
		context.process_messages();
	}
}

HMODULE DisplayMessageQueue_Win32::moduleKernel32 = 0;
DisplayMessageQueue_Win32::FuncSetProcessUserModeExceptionPolicy *DisplayMessageQueue_Win32::ptrSetProcessUserModeExceptionPolicy = 0;
DisplayMessageQueue_Win32::FuncGetProcessUserModeExceptionPolicy *DisplayMessageQueue_Win32::ptrGetProcessUserModeExceptionPolicy = 0;

}
