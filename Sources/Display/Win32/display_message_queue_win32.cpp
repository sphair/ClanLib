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
*/

#include "Display/precomp.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/event_provider.h"
#include "API/Core/System/event.h"
#include "API/Core/System/thread_local_storage.h"
#include "API/Display/Window/display_window_message.h"
#include "display_message_queue_win32.h"
#include "win32_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_Win32 construction:

CL_DisplayMessageQueue_Win32::CL_DisplayMessageQueue_Win32()
{
}

CL_DisplayMessageQueue_Win32::~CL_DisplayMessageQueue_Win32()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_Win32 attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayMessageQueue_Win32 operations:

int CL_DisplayMessageQueue_Win32::wait(int count, CL_Event const * const * events, int timeout, bool wait_all)
{
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
		return -2;


	DWORD timeout_win32 = (timeout == -1) ? INFINITE : timeout;

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
		DWORD result = MsgWaitForMultipleObjectsEx(0, 0, timeout_win32, QS_ALLEVENTS|QS_SENDMESSAGE|QS_RAWINPUT, wait_all ? MWMO_WAITALL : 0);
		if (result == WAIT_OBJECT_0)
		{
			return -2;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		std::vector<HANDLE> handles;
		for (index_events = 0; index_events < count; index_events++)
		{
			int num_handles = events[index_events]->get_event_provider()->get_num_event_handles();
			for (int i=0; i<num_handles; i++)
				handles.push_back(events[index_events]->get_event_provider()->get_event_handle(i));
		}
		while (true)
		{
			DWORD result = MsgWaitForMultipleObjectsEx(handles.size(), &handles[0], timeout_win32, QS_ALLEVENTS|QS_SENDMESSAGE|QS_RAWINPUT, wait_all ? MWMO_WAITALL : 0);
			DWORD index = 0;
			if (result == WAIT_TIMEOUT)
				break;
			else if (result == WAIT_FAILED)
				throw CL_Exception(cl_text("WaitForMultipleObjects failed"));
			else if (result >= WAIT_OBJECT_0 && result < WAIT_OBJECT_0 + num_events+1)
				index = result - WAIT_OBJECT_0;
			else if (result >= WAIT_ABANDONED_0 && result < WAIT_ABANDONED_0 + num_events)
				index = result - WAIT_ABANDONED_0;
			else
				continue;

			if (index == num_events)
			{
				return -2;
			}

			for (index_events = 0; index_events < count; index_events++)
			{
				int num_handles = events[index_events]->get_event_provider()->get_num_event_handles();
				if (index < (DWORD) num_handles)
				{
					bool flagged = events[index_events]->get_event_provider()->check_after_wait((int) index);
					if (flagged)
						return index_events;
					break;
				}
				index -= (DWORD) num_handles;
			}
		}

		return -1;
	}
}

CL_DisplayWindowMessage CL_DisplayMessageQueue_Win32::get_message()
{
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	BOOL result = GetMessage(&msg, 0, 0, 0);
	if (result == 0)
	{
		return CL_DisplayWindowMessage::null();
	}
	else
	{
		CL_DisplayWindowMessage clmsg;
		CL_DataBuffer buffer(&msg, sizeof(msg));
		clmsg.set_msg(cl_text("MSG"), buffer);
		return clmsg;
	}
}

CL_DisplayWindowMessage CL_DisplayMessageQueue_Win32::peek_message(bool block)
{
	if (block)
		wait(0, 0, -1, false);
	MSG msg;
	memset(&msg, 0, sizeof(MSG));
	BOOL result = PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
	if (result == 0)
	{
		return CL_DisplayWindowMessage::null();
	}
	else
	{
		CL_DisplayWindowMessage clmsg;
		CL_DataBuffer buffer(&msg, sizeof(msg));
		clmsg.set_msg(cl_text("MSG"), buffer);
		return clmsg;
	}
}

void CL_DisplayMessageQueue_Win32::dispatch_message(const CL_DisplayWindowMessage &message)
{
	const CL_DataBuffer buffer = message.get_msg(cl_text("MSG"));
	if (buffer.get_size() == sizeof(MSG) )
	{
		MSG msg;
		memcpy(&msg, buffer.get_data(), sizeof(msg));
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CL_SharedPtr<ThreadData> data = get_thread_data();
	for (std::vector<CL_Win32Window *>::size_type i = 0; i < data->windows.size(); i++)
	{
		data->windows[i]->get_ic().process_messages();
	}
}

void CL_DisplayMessageQueue_Win32::add_client(CL_Win32Window *window)
{
	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	thread_data->windows.push_back(window);
}

void CL_DisplayMessageQueue_Win32::remove_client(CL_Win32Window *window)
{
	CL_SharedPtr<ThreadData> thread_data = get_thread_data();
	std::vector<CL_Win32Window *>::size_type index, size;
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
// CL_DisplayMessageQueue_Win32 implementation:

CL_SharedPtr<CL_DisplayMessageQueue_Win32::ThreadData> CL_DisplayMessageQueue_Win32::get_thread_data()
{
	CL_SharedPtr<ThreadData> data(CL_ThreadLocalStorage::get_variable(cl_text("CL_DisplayMessageQueue_Win32::thread_data")));
	if (data.is_null())
	{
		data = CL_SharedPtr<ThreadData>(new ThreadData);
		CL_ThreadLocalStorage::set_variable(cl_text("CL_DisplayMessageQueue_Win32::thread_data"), data);
	}
	return data;
}
