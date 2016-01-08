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

#pragma once

#ifdef _MSC_VER
#include <windows.h>
#include <dbghelp.h>
#include <signal.h>
#include <mutex>
#endif

namespace clan
{

#ifdef _MSC_VER

	class CrashReporter_Impl
	{
	public:
		CrashReporter_Impl(const std::string &reports_directory, const std::string &uploader_executable = std::string());
		~CrashReporter_Impl();

		static void hook_thread();

		static void invoke();
		static void generate_report();

	private:
		struct DumpParams
		{
			HANDLE hprocess;
			HANDLE hthread;
			int thread_id;
			PEXCEPTION_POINTERS exception_pointers;
			unsigned int exception_code;
		};

		void load_dbg_help();
		static void create_dump(DumpParams *params, bool launch_uploader);
		static std::wstring reports_directory;
		static std::wstring uploader_exe;

		static std::recursive_mutex mutex;

		static DWORD WINAPI create_dump_main(LPVOID thread_parameter);

		static int generate_report_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
		static void on_terminate();
		static void on_sigabort(int);
		static void on_se_unhandled_exception(unsigned int exception_code, PEXCEPTION_POINTERS exception_pointers);
		static LONG WINAPI on_win32_unhandled_exception(PEXCEPTION_POINTERS exception_pointers);

		typedef BOOL(WINAPI *MiniDumpWriteDumpPointer)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION);
		HMODULE module_dbghlp;
		static MiniDumpWriteDumpPointer func_MiniDumpWriteDump;

		bool enforce_filter(bool enforce);
		bool write_memory(BYTE* target, const BYTE* source, DWORD size);
#ifdef _M_X64
		static const BYTE patch_bytes[6];
		BYTE original_bytes[6];
#else
		static const BYTE patch_bytes[5];
		BYTE original_bytes[5];
#endif
	};

#else

	class CrashReporter_Impl
	{
	public:
		CrashReporter_Impl(const std::string &reports_directory, const std::string &uploader_executable = std::string());

		static void hook_thread() {}
		static void invoke() { }
		static void generate_report() { }
	};

#endif

}
