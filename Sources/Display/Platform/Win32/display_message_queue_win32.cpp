/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/Core/Text/string_format.h"
#include "API/Core/System/thread_local_storage.h"
#include "API/Display/Window/display_window.h"
#include "display_message_queue_win32.h"
#include "win32_window.h"
#include "../../setup_display.h"
#include "API/Core/System/system.h"

namespace clan
{
	#define WM_ASYNC_WORK (WM_USER + 500)
	#define WM_EXIT_LOOP (WM_USER + 501)

	DisplayMessageQueue_Win32::DisplayMessageQueue_Win32()
	{
		WNDCLASSEX class_desc = { 0 };
		class_desc.cbSize = sizeof(WNDCLASSEX);
		class_desc.lpszClassName = L"AsyncMessageWindow";
		class_desc.lpfnWndProc = DisplayMessageQueue_Win32::async_message_window_proc;
		RegisterClassEx(&class_desc);

		async_message_window_handle = CreateWindowEx(0, L"AsyncMessageWindow", L"Message Window", WS_POPUP, 0, 0, 100, 100, 0, 0, GetModuleHandle(0), 0);
	}

	DisplayMessageQueue_Win32::~DisplayMessageQueue_Win32()
	{
		DestroyWindow(async_message_window_handle);
	}

	void DisplayMessageQueue_Win32::run()
	{
		while (true)
		{
			MSG msg;
			BOOL result = GetMessage(&msg, 0, 0, 0);
			if (result < 0)
				throw Exception("GetMessage failed!");

			if (!process_message(msg))
				break;
		}
	}

	void DisplayMessageQueue_Win32::exit()
	{
		PostMessage(async_message_window_handle, WM_EXIT_LOOP, 0, 0);
	}

	bool DisplayMessageQueue_Win32::process(int timeout_ms)
	{
		auto time_start = System::get_time();
		while (true)
		{
			while (true)
			{
				MSG msg;
				BOOL result = PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
				if (result)
				{
					if (!process_message(msg))
						return false;
				}
				else
				{
					break;
				}
			}

			auto time_now = System::get_time();
			int64_t time_remaining_ms = timeout_ms - (time_now - time_start);

			if (time_remaining_ms <= 0)
				break;

			if (MsgWaitForMultipleObjects(0, 0, FALSE, time_remaining_ms, QS_ALLEVENTS | QS_SENDMESSAGE | QS_RAWINPUT) == WAIT_TIMEOUT)
				break;
		}

		return true;
	}

	void DisplayMessageQueue_Win32::post_async_work_needed()
	{
		PostMessage(async_message_window_handle, WM_ASYNC_WORK, 0, 0);
	}

	LRESULT DisplayMessageQueue_Win32::async_message_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_EXIT_LOOP)
		{
			SetupDisplay::get_message_queue()->exit_loop = true;
		}
		else if (msg == WM_ASYNC_WORK)
		{
			SetupDisplay::get_message_queue()->process_async_work();
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool DisplayMessageQueue_Win32::process_message(MSG &msg)
	{
		if (msg.message == WM_QUIT)
			return false;

		allow_exceptions();

		DispatchMessage(&msg);

		if (exit_loop)
		{
			exit_loop = false;
			return false;
		}

		return true;
	}

	void DisplayMessageQueue_Win32::allow_exceptions()
	{
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
	}

	HMODULE DisplayMessageQueue_Win32::moduleKernel32 = 0;
	DisplayMessageQueue_Win32::FuncSetProcessUserModeExceptionPolicy *DisplayMessageQueue_Win32::ptrSetProcessUserModeExceptionPolicy = 0;
	DisplayMessageQueue_Win32::FuncGetProcessUserModeExceptionPolicy *DisplayMessageQueue_Win32::ptrGetProcessUserModeExceptionPolicy = 0;
}
