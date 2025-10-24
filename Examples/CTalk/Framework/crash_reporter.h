
#pragma once

#ifdef _MSC_VER

#include <windows.h>
#include <dbghelp.h>
#include <signal.h>

class CrashReporter
{
public:
	CrashReporter(const CL_String &reports_directory, const CL_String &uploader_executable = CL_String());
	~CrashReporter();

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
	static CL_String reports_directory;
	static CL_String uploader_exe;

	static CL_Mutex mutex;

	static DWORD WINAPI create_dump_main(LPVOID thread_parameter);

	static int generate_report_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
	static void on_terminate();
	static void on_sigabort(int);
	static void on_se_unhandled_exception(unsigned int exception_code, PEXCEPTION_POINTERS exception_pointers);
	static LONG WINAPI on_win32_unhandled_exception(PEXCEPTION_POINTERS exception_pointers);

	typedef BOOL (WINAPI *MiniDumpWriteDumpPointer)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION);
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

class CrashReporter
{
public:
	CrashReporter(const CL_String &reports_directory, const CL_String &uploader_executable = CL_String());

	static void invoke() { }
	static void generate_report() { }
};

#endif
