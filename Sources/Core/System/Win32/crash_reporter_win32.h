/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_crash_reporter_win32
#define header_crash_reporter_win32

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Generic/crash_reporter_generic.h"

/*
	HAS_PDB_SUPPORT enables support for the crash reporter.
	If its not defined, then the crash reporter will do nothing.

	HAS_SE_TRANSLATOR enables support for using Visual C++ exception handling
	to catch the WIN32 structured exception. This allows us to catch it at a
	much earlier stage than otherwise.

	As of this date, both will be defined if compiled with Visual C++ 7 or better,
	and otherwise disabled. People porting to another compile may try define first
	pdb support, and then maybe afterwards se translator if the compiler support
	this msvc keyword.

	For users with Visual C++ 6, it is possible to enable this as well, but it
	requires that you have installed a recent version of the Microsoft Platform SDK.
*/

#if _MSC_VER >= 1300
#define HAS_SE_TRANSLATOR
#define HAS_PDB_SUPPORT
#endif

#ifdef HAS_PDB_SUPPORT

// imagehlp.h must be compiled with packing to eight-byte-boundaries,
// but does nothing to enforce that. I am grateful to Jeff Shanholtz
// <JShanholtz@premia.com> for finding this problem.
#pragma pack( push, before_imagehlp, 8 )
#include <imagehlp.h>
#pragma pack( pop, before_imagehlp )
#endif

class CL_CrashReporter_Win32 : public CL_CrashReporter_Generic
{
//! Construction:
public:
	CL_CrashReporter_Win32();

	~CL_CrashReporter_Win32();

//! Implementation:
public:
#ifdef HAS_PDB_SUPPORT
	typedef BOOL (__stdcall *TypeSymCleanup)( IN HANDLE hProcess );

	typedef PVOID (__stdcall *TypeSymFunctionTableAccess)( HANDLE hProcess, DWORD AddrBase );

	typedef BOOL (__stdcall *TypeSymGetLineFromAddr)(
		IN HANDLE hProcess,
		IN DWORD dwAddr,
		OUT PDWORD pdwDisplacement,
		OUT PIMAGEHLP_LINE Line );

	typedef DWORD (__stdcall *TypeSymGetModuleBase)( IN HANDLE hProcess, IN DWORD dwAddr );

	typedef BOOL (__stdcall *TypeSymGetModuleInfo)(
		IN HANDLE hProcess,
		IN DWORD dwAddr,
		OUT PIMAGEHLP_MODULE ModuleInfo );

	typedef DWORD (__stdcall *TypeSymGetOptions)( VOID );

	typedef BOOL (__stdcall *TypeSymGetSymFromAddr)(
		IN HANDLE hProcess,
		IN DWORD dwAddr,
		OUT PDWORD pdwDisplacement,
		OUT PIMAGEHLP_SYMBOL Symbol );

	typedef BOOL (__stdcall *TypeSymInitialize)(
		IN HANDLE hProcess,
		IN PSTR UserSearchPath,
		IN BOOL fInvadeProcess );

	typedef DWORD (__stdcall *TypeSymLoadModule)(
		IN HANDLE hProcess,
		IN HANDLE hFile,
		IN PSTR ImageName,
		IN PSTR ModuleName,
		IN DWORD BaseOfDll,
		IN DWORD SizeOfDll );

	typedef DWORD (__stdcall *TypeSymSetOptions)(
		IN DWORD SymOptions );

	typedef BOOL (__stdcall *TypeStackWalk)(
		DWORD MachineType,
		HANDLE hProcess,
		HANDLE hThread,
		LPSTACKFRAME StackFrame,
		PVOID ContextRecord,
		PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine,
		PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine,
		PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine,
		PTRANSLATE_ADDRESS_ROUTINE TranslateAddress );

	typedef DWORD (__stdcall WINAPI *TypeUnDecorateSymbolName)(
		PCSTR DecoratedName,
		PSTR UnDecoratedName,
		DWORD UndecoratedLength,
		DWORD Flags );

	typedef BOOL (WINAPI *TypeMiniDumpWriteDump)(
		HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
		CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
		CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
		CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

	static HINSTANCE moduleImagehlp;

	static HINSTANCE moduleDbghlp;

	static TypeSymCleanup ptrSymCleanup;

	static TypeSymFunctionTableAccess ptrSymFunctionTableAccess;

	static TypeSymGetLineFromAddr ptrSymGetLineFromAddr;

	static TypeSymGetModuleBase ptrSymGetModuleBase;

	static TypeSymGetModuleInfo ptrSymGetModuleInfo;

	static TypeSymGetOptions ptrSymGetOptions;

	static TypeSymGetSymFromAddr ptrSymGetSymFromAddr;

	static TypeSymInitialize ptrSymInitialize;

	static TypeSymLoadModule ptrSymLoadModule;

	static TypeSymSetOptions ptrSymSetOptions;

	static TypeStackWalk ptrStackWalk;

	static TypeUnDecorateSymbolName ptrUnDecorateSymbolName;

	static TypeMiniDumpWriteDump ptrMiniDumpWriteDump;

#ifdef HAS_SE_TRANSLATOR
	static _se_translator_function ptrOldFilter;
#else
	static LPTOP_LEVEL_EXCEPTION_FILTER ptrOldFilter;
#endif

	static std::string userSearchPath;

#ifdef HAS_SE_TRANSLATOR
	static void unhandledExceptionFilter(unsigned int exceptionCode, PEXCEPTION_POINTERS exceptionInfo);
#else
	static LONG WINAPI unhandledExceptionFilter(PEXCEPTION_POINTERS exceptionInfo);
#endif

	static DWORD WINAPI dumpStack(LPVOID lpThreadParameter);

	static void enumAndLoadModuleSymbols( HANDLE hProcess, DWORD pid );

	struct ModuleEntry
	{
		std::string imageName;
		std::string moduleName;
		DWORD baseAddress;
		DWORD size;
	};

	typedef std::vector< ModuleEntry > ModuleList;

	typedef ModuleList::iterator ModuleListIter;

	static bool fillModuleList( ModuleList& modules, DWORD pid, HANDLE hProcess );

	static bool fillModuleListTH32( ModuleList& modules, DWORD pid );

	static bool fillModuleListPSAPI( ModuleList& modules, DWORD pid, HANDLE hProcess );
#endif
};

#endif
