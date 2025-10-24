/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Core/System/mutex.h"
#include "API/Core/System/memory_pool.h"
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

#ifndef __APPLE__
#include <execinfo.h>
#endif
#include <cxxabi.h>
#include <cstring>
#include <cstdlib>
#endif

#ifdef WIN32
#ifdef DEBUG
#include <crtdbg.h>
#endif
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_System Operations:

void *CL_System::aligned_alloc(size_t size, size_t alignment)
{
	void *ptr;
#ifdef _MSC_VER
	ptr = _aligned_malloc(size, alignment);
	if (!ptr)
		throw CL_Exception("Out of memory");
#else
	// posix_memalign required alignment to be a min of sizeof(void *)
	if (alignment < sizeof(void *))
		alignment = sizeof(void *);

	if (posix_memalign( (void **) &ptr, alignment, size))
	{
		throw CL_Exception("Out of memory");
	}
#endif
	return ptr;
}

void CL_System::aligned_free(void *ptr)
{
	if (ptr)
	{
#ifdef _MSC_VER
		_aligned_free(ptr);
#else
		free(ptr);
#endif
	}
}

int CL_System::capture_stack_trace(int frames_to_skip, int max_frames, void **out_frames, unsigned int *out_hash)
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
		FuncRtlCaptureStackBackTrace *ptrRtlCaptureStackBackTrace = (FuncRtlCaptureStackBackTrace *) GetProcAddress(module, "RtlCaptureStackBackTrace");
		if (ptrRtlCaptureStackBackTrace)
			capturedFrames = ptrRtlCaptureStackBackTrace(frames_to_skip+1, max_frames, out_frames, (DWORD *) out_hash);
		FreeLibrary(module);
	}

	if (capturedFrames == 0 && out_hash)
		*out_hash = 0;
	return capturedFrames;

#elif !defined(__APPLE__)
	// Ensure the output is cleared
	memset(out_frames, 0, (sizeof(void *)) * max_frames);

	if (out_hash)
		*out_hash = 0;

	return (backtrace(out_frames, max_frames));
#endif
}

std::vector<CL_String> CL_System::get_stack_frames_text(void **frames, int num_frames)
{
#ifdef WIN32
	static CL_Mutex mutex;
	CL_MutexSection mutex_lock(&mutex);

	BOOL result = SymInitialize(GetCurrentProcess(), NULL, TRUE);
	if (!result)
		return std::vector<CL_String>();

	std::vector<CL_String> backtrace_text;
	for (unsigned short i = 0; i < num_frames; i++)
	{
		unsigned char buffer[sizeof(IMAGEHLP_SYMBOL64) + 128];
		IMAGEHLP_SYMBOL64 *symbol64 = reinterpret_cast<IMAGEHLP_SYMBOL64*>(buffer);
		memset(symbol64, 0, sizeof(IMAGEHLP_SYMBOL64) + 128);
		symbol64->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		symbol64->MaxNameLength = 128;
		
		DWORD64 displacement = 0;
		BOOL result = SymGetSymFromAddr64(GetCurrentProcess(), (DWORD64) frames[i], &displacement, symbol64);
		if (result)
		{
			IMAGEHLP_LINE64 line64;
			DWORD displacement = 0;
			memset(&line64, 0, sizeof(IMAGEHLP_LINE64));
			line64.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			result = SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64) frames[i], &displacement, &line64);
			if (result)
			{
				backtrace_text.push_back(
					cl_format(
						"%1 (%2, line %3)",
						CL_StringHelp::local8_to_text(symbol64->Name),
						CL_StringHelp::local8_to_text(line64.FileName),
						(int) line64.LineNumber));
			}
			else
			{
				backtrace_text.push_back(symbol64->Name);
			}
		}
	}

	SymCleanup(GetCurrentProcess());
	return backtrace_text;

#elif !defined(__APPLE__)

	char **strings;
 	strings = backtrace_symbols(frames, num_frames);
	if (!strings)
	{
		return std::vector<CL_String>();
	}

	std::vector<CL_String> backtrace_text;
     
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
		char *new_function = abi::__cxa_demangle(function, 0, 0, &status);
		if (new_function)	// Was correctly decoded
		{
			function = new_function;
		}

		backtrace_text.push_back( cl_format("%1 (%2)", function, filename));

		if (new_function)
		{
			free(new_function);
		}
	}

	free (strings);
	return backtrace_text;
#endif
}

void CL_System::sleep(int msecs)
{
#ifdef WIN32
	Sleep(msecs);
#else
	timeval tv;
	tv.tv_sec = msecs / 1000;
	tv.tv_usec = (msecs % 1000) * 1000;
	select(0, 0, 0, 0, &tv);
#endif
}
CL_Mutex *CL_System::get_sharedptr_mutex()
{
	static CL_Mutex *sharedptr_mutex = new CL_Mutex;
	return sharedptr_mutex;
}

void *operator new(size_t size, CL_PreallocatedMemory *memory)
{
	return memory;
}

int CL_System::get_num_cores()
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

/////////////////////////////////////////////////////////////////////////////
// Temporary memory pool management:

class CL_ThreadTempStackPool : public CL_MemoryPool
{
public:
	CL_ThreadTempStackPool()
	: ref_count(0)
	{
		last_block = (AllocLink *) data;
		last_block->prev = 0;
		last_block->next = 0;
		last_block->flags = alloc_flag_free;
	}

	~CL_ThreadTempStackPool()
	{
	}

	void *alloc(size_t size)
	{
		size = (size + 3) / 4;
		size *= 4;
		if (size > 16*1024)
			return new char[size];

		// to do: insert 0xfd (guard bytes) before and after allocated memory.

		char *p_begin = ((char *) last_block) + sizeof(AllocLink);
		char *p_end = p_begin + size;
		if (p_end + sizeof(AllocLink) > data + capacity)
		{
#if defined(WIN32) && defined(_DEBUG)
			DebugBreak();
#endif
			throw CL_Exception("Thread temporary memory pool exhausted");
		}

#ifdef _DEBUG
		memset(p_begin, 0xcd, size);
#endif

		AllocLink *link = last_block;
#ifdef _DEBUG
		link->guard1 = 0xdeadbeef;
		link->guard2 = 0xdeadbeef;
#endif
		last_block = (AllocLink *) p_end;
		link->next = last_block;
		link->flags = alloc_flag_used;
		last_block->prev = link;
		last_block->next = 0;
		last_block->flags = alloc_flag_free;
		return p_begin;
	}

	void free(void *ptr)
	{
		if (ptr < data || ptr >= (void *) (data+capacity))
		{
			delete[] ((char *) ptr);
			return;
		}

		char *p_begin = (char *) ptr;
		AllocLink *link = (AllocLink *) (p_begin - sizeof(AllocLink));
		link->flags = alloc_flag_free;

#ifdef _DEBUG
		_ASSERT(link->guard1 == 0xdeadbeef);
		_ASSERT(link->guard2 == 0xdeadbeef);

		char *p_end = (char *) link->next;
		memset(p_begin, 0xdd, p_end - p_begin);
#endif

		if (link->next == last_block)
		{
			while (link->prev && link->prev->flags == alloc_flag_free)
			{
				link = link->prev;
			}
			link->next = 0;
			last_block = link;
		}
	}

	int add_reference()
	{
		return ++ref_count;
	}

	int release_reference()
	{
		--ref_count;
		if (ref_count == 0)
		{
			delete this;
			return 0;
		}
		else
		{
			return ref_count;
		}
	}

	enum
	{
		alloc_flag_used = 1,
		alloc_flag_free = 2
	};

	struct AllocLink
	{
#ifdef _DEBUG
		unsigned int guard1;
#endif
		AllocLink *prev;
		AllocLink *next;
		unsigned int flags;
#ifdef _DEBUG
		unsigned int guard2;
#endif
	};

	int ref_count;
	enum { capacity = 1024*1024 };
	char data[capacity];
	AllocLink *last_block;
};

#ifdef WIN32
	static DWORD cl_temp_pool_tls_index = TLS_OUT_OF_INDEXES;
	CL_Mutex cl_temp_pool_tls_mutex;
	class CL_TempPoolTLSCleanUpHandler
	{
	public:
		~CL_TempPoolTLSCleanUpHandler()
		{
			if (cl_temp_pool_tls_index != TLS_OUT_OF_INDEXES)
			{
				TlsFree(cl_temp_pool_tls_index);
				cl_temp_pool_tls_index = TLS_OUT_OF_INDEXES;
			}
		}
	} cl_temp_pool_tls_cleanup;
#elif defined(__APPLE__)
	static bool cl_temp_pool_tls_index_created = false;
	static pthread_key_t cl_temp_pool_tls_index;
	CL_Mutex cl_temp_pool_tls_mutex;
#else
	__thread CL_ThreadTempStackPool *cl_tls_temp_pool = 0;
#endif

void CL_System::alloc_thread_temp_pool()
{
#ifdef WIN32
	if (cl_temp_pool_tls_index == TLS_OUT_OF_INDEXES)
	{
		CL_MutexSection mutex_lock(&cl_temp_pool_tls_mutex);
		cl_temp_pool_tls_index = TlsAlloc();
	}

	CL_ThreadTempStackPool *cl_temp_pool = (CL_ThreadTempStackPool *) TlsGetValue(cl_temp_pool_tls_index);
	if (cl_temp_pool == 0)
	{
		cl_temp_pool = new CL_ThreadTempStackPool;
		TlsSetValue(cl_temp_pool_tls_index, cl_temp_pool);
	}
	cl_temp_pool->add_reference();
#elif defined(__APPLE__)
	if (!cl_temp_pool_tls_index_created)
	{
		CL_MutexSection mutex_lock(&cl_temp_pool_tls_mutex);
		pthread_key_create(&cl_temp_pool_tls_index, 0);
		cl_temp_pool_tls_index_created = true;
	}
	
	CL_ThreadTempStackPool *cl_temp_pool = (CL_ThreadTempStackPool *) pthread_getspecific(cl_temp_pool_tls_index);
	if (cl_temp_pool == 0)
	{
		cl_temp_pool = new CL_ThreadTempStackPool;
		pthread_setspecific(cl_temp_pool_tls_index, cl_temp_pool);
	}
	cl_temp_pool->add_reference();
#else
	if (cl_tls_temp_pool == 0)
		cl_tls_temp_pool = new CL_ThreadTempStackPool;
	cl_tls_temp_pool->add_reference();
#endif
}

void CL_System::free_thread_temp_pool()
{
#ifdef WIN32
	CL_ThreadTempStackPool *cl_temp_pool = (CL_ThreadTempStackPool *) TlsGetValue(cl_temp_pool_tls_index);
	if (cl_temp_pool && cl_temp_pool->release_reference() == 0)
		TlsSetValue(cl_temp_pool_tls_index, 0);
#elif defined(__APPLE__)
	CL_ThreadTempStackPool *cl_temp_pool = (CL_ThreadTempStackPool *) pthread_getspecific(cl_temp_pool_tls_index);
	if (cl_temp_pool && cl_temp_pool->release_reference() == 0)
		pthread_setspecific(cl_temp_pool_tls_index, 0);
#else
	if (cl_tls_temp_pool && cl_tls_temp_pool->release_reference() == 0)
		cl_tls_temp_pool = 0;
#endif
}

CL_MemoryPool *CL_MemoryPool::get_temp_pool()
{
#ifdef WIN32
	CL_ThreadTempStackPool *cl_temp_pool = (CL_ThreadTempStackPool *) TlsGetValue(cl_temp_pool_tls_index);
	return cl_temp_pool;
#elif defined(__APPLE__)
	CL_ThreadTempStackPool *cl_temp_pool = (CL_ThreadTempStackPool *) pthread_getspecific(cl_temp_pool_tls_index);
	return cl_temp_pool;
#else
	return cl_tls_temp_pool;
#endif
}
