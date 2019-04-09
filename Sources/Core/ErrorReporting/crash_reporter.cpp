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
*/

#include "Core/precomp.h"
#include "API/Core/ErrorReporting/crash_reporter.h"
#include "API/Core/Text/string_help.h"
#include "crash_reporter_impl.h"

namespace clan
{
	CrashReporter::CrashReporter(const std::string &reports_directory, const std::string &uploader_executable)
		: impl(std::make_shared<CrashReporter_Impl>(reports_directory, uploader_executable))
	{
	}

	CrashReporter::~CrashReporter()
	{
	}

	void CrashReporter::hook_thread()
	{
		CrashReporter_Impl::hook_thread();
	}

	void CrashReporter::invoke()
	{
		CrashReporter_Impl::invoke();
	}

	void CrashReporter::generate_report()
	{
		CrashReporter_Impl::generate_report();
	}

	/////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER

#pragma warning(disable: 4535) // warning C4535: calling _set_se_translator() requires /EHa

	std::wstring CrashReporter_Impl::reports_directory;
	std::wstring CrashReporter_Impl::uploader_exe;
	CrashReporter_Impl::MiniDumpWriteDumpPointer CrashReporter_Impl::func_MiniDumpWriteDump = 0;
	std::recursive_mutex CrashReporter_Impl::mutex;

	CrashReporter_Impl::CrashReporter_Impl(const std::string &new_reports_directory, const std::string &new_uploader_executable)
	{
		reports_directory = StringHelp::utf8_to_ucs2(new_reports_directory);
		if (!reports_directory.empty() && (reports_directory.back() != L'/' && reports_directory.back() != L'\\'))
			reports_directory.push_back(L'\\');
		uploader_exe = StringHelp::utf8_to_ucs2(new_uploader_executable);
		load_dbg_help();
		hook_thread();
		enforce_filter(true);
	}

	CrashReporter_Impl::~CrashReporter_Impl()
	{
	}

	void CrashReporter_Impl::invoke()
	{
#ifdef _DEBUG
		DebugBreak(); // Bring up the debugger if it is running
#endif
		Sleep(1000); // Give any possibly logging functionality a little time before we die
		RaiseException(EXCEPTION_NONCONTINUABLE_EXCEPTION, EXCEPTION_NONCONTINUABLE_EXCEPTION, 0, 0);
	}

	void CrashReporter_Impl::generate_report()
	{
		__try
		{
			RaiseException(EXCEPTION_BREAKPOINT, EXCEPTION_BREAKPOINT, 0, 0);
		}
		__except (generate_report_filter(GetExceptionCode(), GetExceptionInformation()))
		{
		}
	}

	void CrashReporter_Impl::create_dump(DumpParams *dump_params, bool launch_uploader)
	{
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		HANDLE file = INVALID_HANDLE_VALUE;
		TCHAR minidump_filename[MAX_PATH];
		for (int counter = 1; counter < 1000 && file == INVALID_HANDLE_VALUE; counter++)
		{
			swprintf_s(minidump_filename, L"%s%04d-%02d-%02d %02d.%02d.%02d (%d).dmp", reports_directory.c_str(), systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, counter);
			file = CreateFile(minidump_filename, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
		}

		MINIDUMP_EXCEPTION_INFORMATION info;
		info.ThreadId = dump_params->thread_id;
		info.ExceptionPointers = dump_params->exception_pointers;
		info.ClientPointers = 0;
		MINIDUMP_TYPE type = static_cast<MINIDUMP_TYPE>(
			MiniDumpWithHandleData |
			MiniDumpWithProcessThreadData |
			MiniDumpWithFullMemoryInfo |
			MiniDumpWithThreadInfo);
		func_MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, type, &info, 0, 0);
		CloseHandle(file);

		if (launch_uploader && !uploader_exe.empty())
		{
			STARTUPINFO startup_info;
			PROCESS_INFORMATION process_info;
			memset(&startup_info, 0, sizeof(STARTUPINFO));
			memset(&process_info, 0, sizeof(PROCESS_INFORMATION));
			startup_info.cb = sizeof(STARTUPINFO);
			if (CreateProcess(uploader_exe.c_str(), minidump_filename, 0, 0, FALSE, 0, 0, 0, &startup_info, &process_info))
			{
				CloseHandle(process_info.hThread);
				CloseHandle(process_info.hProcess);
			}
		}
	}

	int CrashReporter_Impl::generate_report_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
	{
		DumpParams dump_params;
		dump_params.hprocess = GetCurrentProcess();
		dump_params.hthread = GetCurrentThread();
		dump_params.thread_id = GetCurrentThreadId();
		dump_params.exception_pointers = ep;
		dump_params.exception_code = code;
		create_dump(&dump_params, false);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void CrashReporter_Impl::load_dbg_help()
	{
		module_dbghlp = LoadLibrary(L"dbghelp.dll");
		if (module_dbghlp)
			func_MiniDumpWriteDump = reinterpret_cast<MiniDumpWriteDumpPointer>(GetProcAddress(module_dbghlp, "MiniDumpWriteDump"));
	}

	void CrashReporter_Impl::hook_thread()
	{
		set_terminate(&CrashReporter_Impl::on_terminate);
		_set_abort_behavior(0, _CALL_REPORTFAULT | _WRITE_ABORT_MSG);
		signal(SIGABRT, &CrashReporter_Impl::on_sigabort);
		_set_se_translator(&CrashReporter_Impl::on_se_unhandled_exception);
		SetUnhandledExceptionFilter(&CrashReporter_Impl::on_win32_unhandled_exception);
	}

	void CrashReporter_Impl::on_terminate()
	{
		invoke();
	}

	void CrashReporter_Impl::on_sigabort(int)
	{
		invoke();
	}

	void CrashReporter_Impl::on_se_unhandled_exception(unsigned int exception_code, PEXCEPTION_POINTERS exception_pointers)
	{
		// Ignore those bloody breakpoints!
		if (exception_code == EXCEPTION_BREAKPOINT) return;

		DumpParams dump_params;
		dump_params.hprocess = GetCurrentProcess();
		dump_params.hthread = GetCurrentThread();
		dump_params.thread_id = GetCurrentThreadId();
		dump_params.exception_pointers = exception_pointers;
		dump_params.exception_code = exception_code;

		// Ensure we only get a dump of the first thread crashing - let other threads block here.
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);

		// Create dump in separate thread:
		DWORD threadId;
		HANDLE hThread = CreateThread(0, 0, &CrashReporter_Impl::create_dump_main, &dump_params, 0, &threadId);
		if (hThread)
			WaitForSingleObject(hThread, INFINITE);
		TerminateProcess(GetCurrentProcess(), 255);
	}

	LONG CrashReporter_Impl::on_win32_unhandled_exception(PEXCEPTION_POINTERS exception_pointers)
	{
		DumpParams dump_params;
		dump_params.hprocess = GetCurrentProcess();
		dump_params.hthread = GetCurrentThread();
		dump_params.thread_id = GetCurrentThreadId();
		dump_params.exception_pointers = exception_pointers;
		dump_params.exception_code = 0;

		// Ensure we only get a dump of the first thread crashing - let other threads block here.
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);

		// Create minidump in seperate thread:
		DWORD threadId;
		HANDLE hThread = CreateThread(0, 0, &CrashReporter_Impl::create_dump_main, &dump_params, 0, &threadId);
		if (hThread)
			WaitForSingleObject(hThread, INFINITE);
		TerminateProcess(GetCurrentProcess(), 255);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	DWORD WINAPI CrashReporter_Impl::create_dump_main(LPVOID thread_parameter)
	{
		create_dump(reinterpret_cast<DumpParams *>(thread_parameter), true);
		TerminateProcess(GetCurrentProcess(), 255);
		return 0;
	}

	bool CrashReporter_Impl::enforce_filter(bool bEnforce)
	{
		DWORD ErrCode = 0;

		// Obtain the address of SetUnhandledExceptionFilter 

		HMODULE hLib = GetModuleHandle(L"kernel32.dll");
		if (hLib == NULL)
		{
			ErrCode = GetLastError();
			return false;
		}

		BYTE* pTarget = (BYTE*)GetProcAddress(hLib, "SetUnhandledExceptionFilter");
		if (pTarget == 0)
		{
			ErrCode = GetLastError();
			return false;
		}

		if (IsBadReadPtr(pTarget, sizeof(original_bytes)))
		{
			return false;
		}

		if (bEnforce)
		{
			// Save the original contents of SetUnhandledExceptionFilter 
			memcpy(original_bytes, pTarget, sizeof(original_bytes));

			// Patch SetUnhandledExceptionFilter 
			if (!write_memory(pTarget, patch_bytes, sizeof(patch_bytes)))
				return false;
		}
		else
		{
			// Restore the original behavior of SetUnhandledExceptionFilter 
			if (!write_memory(pTarget, original_bytes, sizeof(original_bytes)))
				return false;
		}

		// Success 
		return true;
	}


	bool CrashReporter_Impl::write_memory(BYTE* pTarget, const BYTE* pSource, DWORD Size)
	{
		DWORD ErrCode = 0;

		// Check parameters 

		if (pTarget == 0)
		{
			return false;
		}

		if (pSource == 0)
		{
			return false;
		}

		if (Size == 0)
		{
			return false;
		}

		if (IsBadReadPtr(pSource, Size))
		{
			return false;
		}

		// Modify protection attributes of the target memory page 

		DWORD OldProtect = 0;

		if (!VirtualProtect(pTarget, Size, PAGE_EXECUTE_READWRITE, &OldProtect))
		{
			ErrCode = GetLastError();
			return false;
		}

		// Write memory 

		memcpy(pTarget, pSource, Size);

		// Restore memory protection attributes of the target memory page 

		DWORD Temp = 0;

		if (!VirtualProtect(pTarget, Size, OldProtect, &Temp))
		{
			ErrCode = GetLastError();
			return false;
		}

		// Success 
		return true;
	}

#if defined(_M_X64)
	const BYTE CrashReporter_Impl::patch_bytes[6] = { 0x48, 0x31, 0xC0, 0xC2, 0x00, 0x00 };
#elif defined(_M_IX86)
	const BYTE CrashReporter_Impl::patch_bytes[5] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#endif

#else

	CrashReporter_Impl::CrashReporter_Impl(const std::string &reports_directory, const std::string &uploader_executable)
	{
	}

#endif

}
