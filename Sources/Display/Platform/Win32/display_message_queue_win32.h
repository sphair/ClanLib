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

#pragma once

#include "API/Core/System/thread_local_storage.h"
#include "Display/System/run_loop_impl.h"
#include <vector>
#include <WinNT.h>

namespace clan
{
	class Win32Window;

	class DisplayMessageQueue_Win32 : public RunLoopImpl
	{
	public:
		DisplayMessageQueue_Win32();
		~DisplayMessageQueue_Win32();

		void run() override;
		void exit() override;
		bool process(int timeout_ms) override;
		void post_async_work_needed() override;

		static bool should_apply_vista_x64_workaround()
		{
#ifdef _M_X86
			return ptrSetProcessUserModeExceptionPolicy == 0 || ptrGetProcessUserModeExceptionPolicy == 0;
#else
			return false;
#endif
		}

	private:
		static LRESULT WINAPI async_message_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static void allow_exceptions();
		bool process_message(MSG &msg);

		bool exit_loop = false;
		HWND async_message_window_handle;

#define WIN32_PROCESS_CALLBACK_FILTER_ENABLED 0x1
		typedef BOOL(WINAPI FuncSetProcessUserModeExceptionPolicy)(DWORD dwFlags);
		typedef BOOL(WINAPI FuncGetProcessUserModeExceptionPolicy)(LPDWORD lpFlags);
		static HMODULE moduleKernel32;
		static FuncSetProcessUserModeExceptionPolicy *ptrSetProcessUserModeExceptionPolicy;
		static FuncGetProcessUserModeExceptionPolicy *ptrGetProcessUserModeExceptionPolicy;
	};

	struct WIN32_EXCEPTION_REGISTRATION_RECORD
	{
		WIN32_EXCEPTION_REGISTRATION_RECORD *Next;
		void *Handler;
	};

	// Workaround for the KB976038 bug on unpatched systems.
	class SEHCatchAllWorkaround
	{
	public:
		SEHCatchAllWorkaround()
			: was_patched(DisplayMessageQueue_Win32::should_apply_vista_x64_workaround()), old_exception_handler(0)
		{
			if (was_patched)
			{
				// remove all exception handler with exception of the default handler
				NT_TIB *tib = get_tib();
				old_exception_handler = (WIN32_EXCEPTION_REGISTRATION_RECORD*)tib->ExceptionList;
				while (((WIN32_EXCEPTION_REGISTRATION_RECORD*)tib->ExceptionList)->Next != (WIN32_EXCEPTION_REGISTRATION_RECORD*)-1)
					tib->ExceptionList = (_EXCEPTION_REGISTRATION_RECORD*)((WIN32_EXCEPTION_REGISTRATION_RECORD*)tib->ExceptionList)->Next;
			}
		}

		void unpatch()
		{
			if (was_patched)
			{
				// restore old exception handler
				NT_TIB *tib = get_tib();
				tib->ExceptionList = (_EXCEPTION_REGISTRATION_RECORD*)old_exception_handler;
			}
		}

	private:
		// get thread information block
		NT_TIB *get_tib()
		{
			NT_TIB * pTib = (NT_TIB *)NtCurrentTeb();
			return pTib;
			//#ifdef _MSC_VER
			//return (NT_TIB *)__readfsdword(0x18);
			//#else
			//NT_TIB *tib = 0;
			//__asm
			//{
			//	mov EAX, FS:[18h]
			//	mov [tib], EAX
			//}
			//return tib;
			//#endif
		}

		bool was_patched;
		WIN32_EXCEPTION_REGISTRATION_RECORD* old_exception_handler;
	};

}
