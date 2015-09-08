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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/System/system.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#if defined(HAVE_SYS_SYSCTL_H) && \
    !defined(_SC_NPROCESSORS_ONLN) && !defined(_SC_NPROC_ONLN)
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

#if !defined __APPLE__ && !defined __ANDROID__
#include <execinfo.h>
#endif
#include <cxxabi.h>
#include <cstring>
#include <cstdlib>
#endif

#ifdef _MSC_VER
#ifdef DEBUG
#include <crtdbg.h>
#endif
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#endif

#ifdef __MINGW32__
#include <dbghelp.h>
#include <malloc.h>
#endif

#ifdef __ANDROID__
// Remove this, when it works without
extern "C" {
	// Note, this still does not work. humm
extern int posix_memalign(void **memptr, size_t alignment, size_t size);
}  // extern "C"
#endif

namespace clan
{
	void *System::aligned_alloc(size_t size, size_t alignment)
	{
		void *ptr;
#if defined _MSC_VER || (defined __MINGW32__ && __MSVCRT_VERSION__ >= 0x0700)
		ptr = _aligned_malloc(size, alignment);
		if (!ptr)
			throw Exception("Out of memory");
#elif defined __MINGW32__
		ptr = __mingw_aligned_malloc(size, alignment);
		if (!ptr)
			throw Exception("Out of memory");	
#else
		// posix_memalign required alignment to be a min of sizeof(void *)
		if (alignment < sizeof(void *))
			alignment = sizeof(void *);

		if (posix_memalign( (void **) &ptr, alignment, size))
		{
			throw Exception("Out of memory");
		}
#endif
		return ptr;
	}

	void System::aligned_free(void *ptr)
	{
		if (ptr)
		{
#if defined _MSC_VER || (defined __MINGW32__ && __MSVCRT_VERSION__ >= 0x0700)
			_aligned_free(ptr);
#elif defined __MINGW32__
			__mingw_aligned_free (ptr);
#else
			free(ptr);
#endif
		}
	}

	int System::capture_stack_trace(int frames_to_skip, int max_frames, void **out_frames, unsigned int *out_hash)
	{
#ifdef WIN32
		if (max_frames > 32)
			max_frames = 32;

		unsigned short capturedFrames = 0;

		// RtlCaptureStackBackTrace is only available on Windows XP or newer versions of Windows
		typedef WORD(NTAPI FuncRtlCaptureStackBackTrace)(DWORD, DWORD, PVOID *, PDWORD);
		HMODULE module = LoadLibrary(TEXT("kernel32.dll"));
		if (module)
		{
			FuncRtlCaptureStackBackTrace *ptrRtlCaptureStackBackTrace = (FuncRtlCaptureStackBackTrace *)GetProcAddress(module, "RtlCaptureStackBackTrace");
			if (ptrRtlCaptureStackBackTrace)
				capturedFrames = ptrRtlCaptureStackBackTrace(frames_to_skip + 1, max_frames, out_frames, (DWORD *)out_hash);
			FreeLibrary(module);
		}

		if (capturedFrames == 0 && out_hash)
			*out_hash = 0;
		return capturedFrames;

#elif !defined __APPLE__ && !defined __ANDROID__
		// Ensure the output is cleared
		memset(out_frames, 0, (sizeof(void *)) * max_frames);

		if (out_hash)
			*out_hash = 0;

		return (backtrace(out_frames, max_frames));
#else
		return 0;
#endif
	}

	std::vector<std::string> System::get_stack_frames_text(void **frames, int num_frames)
	{
#ifdef WIN32
		static std::recursive_mutex mutex;
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);

		BOOL result = SymInitialize(GetCurrentProcess(), NULL, TRUE);
		if (!result)
			return std::vector<std::string>();

		std::vector<std::string> backtrace_text;
		for (unsigned short i = 0; i < num_frames; i++)
		{
			unsigned char buffer[sizeof(IMAGEHLP_SYMBOL64) + 128];
			IMAGEHLP_SYMBOL64 *symbol64 = reinterpret_cast<IMAGEHLP_SYMBOL64*>(buffer);
			memset(symbol64, 0, sizeof(IMAGEHLP_SYMBOL64) + 128);
			symbol64->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
			symbol64->MaxNameLength = 128;

			DWORD64 displacement = 0;
			BOOL result = SymGetSymFromAddr64(GetCurrentProcess(), (DWORD64)frames[i], &displacement, symbol64);
			if (result)
			{
				IMAGEHLP_LINE64 line64;
				DWORD displacement = 0;
				memset(&line64, 0, sizeof(IMAGEHLP_LINE64));
				line64.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				result = SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)frames[i], &displacement, &line64);
				if (result)
				{
					backtrace_text.push_back(
						string_format(
						"%1 (%2, line %3)",
						StringHelp::local8_to_text(symbol64->Name),
						StringHelp::local8_to_text(line64.FileName),
						(int)line64.LineNumber));
				}
				else
				{
					backtrace_text.push_back(symbol64->Name);
				}
			}
		}

		SymCleanup(GetCurrentProcess());
		return backtrace_text;

#elif !defined __APPLE__ && !defined __ANDROID__

		char **strings;
		strings = backtrace_symbols(frames, num_frames);
		if (!strings)
		{
			return std::vector<std::string>();
		}

		std::vector<std::string> backtrace_text;

		for (int cnt = 0; cnt < num_frames; cnt++)
		{
			// Decode the strings
			char *ptr = strings[cnt];
			char *filename = ptr;
			const char *function = "";

			// Find function name
			while(*ptr)
			{
				if (*ptr=='(')	// Found function name
				{
					*(ptr++) = 0;
					function = ptr;
					break;
				}
				ptr++;
			}

			// Find offset
			if (function[0])	// Only if function was found
			{
				while(*ptr)	
				{
					if (*ptr=='+')	// Found function offset
					{
						*(ptr++) = 0;
						break;
					}
					if (*ptr==')')	// Not found function offset, but found, end of function
					{
						*(ptr++) = 0;
						break;
					}
					ptr++;
				}
			}

			int status;
			char *new_function = abi::__cxa_demangle(function, nullptr, nullptr, &status);
			if (new_function)	// Was correctly decoded
			{
				function = new_function;
			}

			backtrace_text.push_back( string_format("%1 (%2)", function, filename));

			if (new_function)
			{
				free(new_function);
			}
		}

		free (strings);
		return backtrace_text;
#else
		return std::vector<std::string>();
#endif
	}

	void System::sleep(int msecs)
	{
#ifdef WIN32
		Sleep(msecs);
#else
		timeval tv;
		tv.tv_sec = msecs / 1000;
		tv.tv_usec = (msecs % 1000) * 1000;
		select(0, nullptr, nullptr, nullptr, &tv);
#endif
	}

	void System::pause(int msecs)
	{
#ifdef WIN32

		// For sleep less than 30ms (except 0), we perform a spinlock to increase the accuracy of sleep() to avoid the win32 scheduler misunderstanding the sleep hint
		if ((msecs < 30) && (msecs >0))
		{
			uint64_t time_start = get_time();
			do
			{
				Sleep(0);
			} while ((get_time() - time_start) < msecs);
		}
		else
		{
			Sleep(msecs);
		}

#else
		timeval tv;
		tv.tv_sec = msecs / 1000;
		tv.tv_usec = (msecs % 1000) * 1000;
		select(0, nullptr, nullptr, nullptr, &tv);
#endif
	}

	int System::get_num_cores()
	{
#ifdef WIN32
		SYSTEM_INFO system_info;
		memset(&system_info, 0, sizeof(SYSTEM_INFO));
		GetSystemInfo(&system_info);
		return system_info.dwNumberOfProcessors;
#else
		long cpus =  -1;
# if defined(_SC_NPROCESSORS_ONLN)
		cpus = sysconf(_SC_NPROCESSORS_ONLN);
# elif defined(_SC_NPROC_ONLN)
		cpus = sysconf(_SC_NPROC_ONLN);
# elif defined(HAVE_SYS_SYSCTL_H)
		int mib[2];
		size_t len;

		mib[0] = CTL_HW;
		mib[1] = HW_NCPU;

		len = sizeof(cpus);
		(void)sysctl(mib, 2, &cpus, &len, NULL, 0);
# endif

		return (cpus < 1)?-1 : static_cast<int>(cpus);

#endif	// WIN32
	}
}
