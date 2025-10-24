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

#include "Core/precomp.h"
#include "API/Core/System/crash_reporter.h"
#include "API/Core/System/log.h"
#include "crash_reporter_win32.h"

#ifdef _MSC_VER
#pragma warning (disable:4535)
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_CrashReporter_Win32 Construction:

CL_CrashReporter_Win32::CL_CrashReporter_Win32()
{
#ifdef HAS_PDB_SUPPORT
	if (moduleImagehlp == 0)
	{
		moduleImagehlp = LoadLibrary( "imagehlp.dll" );
		if ( moduleImagehlp == 0 )
		{
			CL_Log::log("debug", "Unable to load imagehlp.dll. Crash reporter disabled.");
			return;
		}

		ptrSymCleanup = (TypeSymCleanup) GetProcAddress( moduleImagehlp, "SymCleanup" );
		ptrSymFunctionTableAccess = (TypeSymFunctionTableAccess) GetProcAddress( moduleImagehlp, "SymFunctionTableAccess" );
		ptrSymGetLineFromAddr = (TypeSymGetLineFromAddr) GetProcAddress( moduleImagehlp, "SymGetLineFromAddr" );
		ptrSymGetModuleBase = (TypeSymGetModuleBase) GetProcAddress( moduleImagehlp, "SymGetModuleBase" );
		ptrSymGetModuleInfo = (TypeSymGetModuleInfo) GetProcAddress( moduleImagehlp, "SymGetModuleInfo" );
		ptrSymGetOptions = (TypeSymGetOptions) GetProcAddress( moduleImagehlp, "SymGetOptions" );
		ptrSymGetSymFromAddr = (TypeSymGetSymFromAddr) GetProcAddress( moduleImagehlp, "SymGetSymFromAddr" );
		ptrSymInitialize = (TypeSymInitialize) GetProcAddress( moduleImagehlp, "SymInitialize" );
		ptrSymSetOptions = (TypeSymSetOptions) GetProcAddress( moduleImagehlp, "SymSetOptions" );
		ptrStackWalk = (TypeStackWalk) GetProcAddress( moduleImagehlp, "StackWalk" );
		ptrUnDecorateSymbolName = (TypeUnDecorateSymbolName) GetProcAddress( moduleImagehlp, "UnDecorateSymbolName" );
		ptrSymLoadModule = (TypeSymLoadModule) GetProcAddress( moduleImagehlp, "SymLoadModule" );

		if (
			ptrSymCleanup == 0 ||
			ptrSymFunctionTableAccess == 0 ||
			ptrSymGetLineFromAddr == 0 ||
			ptrSymGetModuleBase == 0 ||
			ptrSymGetModuleInfo == 0 ||
			ptrSymGetOptions == 0 ||
			ptrSymGetSymFromAddr == 0 ||
			ptrSymInitialize == 0 ||
			ptrSymSetOptions == 0 ||
			ptrStackWalk == 0 ||
			ptrUnDecorateSymbolName == 0 ||
			ptrSymLoadModule == 0)
		{
			CL_Log::log("debug", "Unable to find all symbols in imagehlp.dll. Crash reporter disabled.");
			FreeLibrary( moduleImagehlp );
			moduleImagehlp = 0;
			return;
		}

		// Setup search path for pdb (symbol information) files:

		#define TTBUFLEN 65536 // for a temp buffer
		char tt[TTBUFLEN];

		if ( GetCurrentDirectoryA( TTBUFLEN, tt ) )
			userSearchPath += tt + std::string( ";" );
		// dir with executable
		if ( GetModuleFileNameA( 0, tt, TTBUFLEN ) )
		{
			char *p;
			for ( p = tt + strlen( tt ) - 1; p >= tt; -- p )
			{
				// locate the rightmost path separator
				if ( *p == '\\' || *p == '/' || *p == ':' )
					break;
			}
			// if we found one, p is pointing at it; if not, tt only contains
			// an exe name (no path), and p points before its first byte
			if ( p != tt ) // path sep found?
			{
				if ( *p == ':' ) // we leave colons in place
					++ p;
				*p = '\0'; // eliminate the exe name and last path sep
				userSearchPath += tt + std::string( ";" );
			}
		}
		// environment variable _NT_SYMBOL_PATH
		if ( GetEnvironmentVariableA( "_NT_SYMBOL_PATH", tt, TTBUFLEN ) )
			userSearchPath += tt + std::string( ";" );
		// environment variable _NT_ALTERNATE_SYMBOL_PATH
		if ( GetEnvironmentVariableA( "_NT_ALTERNATE_SYMBOL_PATH", tt, TTBUFLEN ) )
			userSearchPath += tt + std::string( ";" );
		// environment variable SYSTEMROOT
		if ( GetEnvironmentVariableA( "SYSTEMROOT", tt, TTBUFLEN ) )
			userSearchPath += tt + std::string( ";" );

		if ( userSearchPath.size() > 0 ) // if we added anything, we have a trailing semicolon
			userSearchPath = userSearchPath.substr( 0, userSearchPath.size() - 1 );
	}
	
	if (moduleDbghlp == 0)
	{
		moduleDbghlp = LoadLibrary( "dbghelp.dll" );
		if ( moduleDbghlp == 0 )
		{
			CL_Log::log("debug", "Unable to load dbghlp.dll. Crash reporter disabled.");
			return;
		}

		ptrMiniDumpWriteDump = (TypeMiniDumpWriteDump) GetProcAddress( moduleDbghlp, "MiniDumpWriteDump" );

		if (ptrMiniDumpWriteDump == 0)
		{
			CL_Log::log("debug", "Unable to find all symbols in dbghlp.dll. Crash reporter disabled.");
			FreeLibrary( moduleDbghlp );
			moduleDbghlp = 0;
			return;
		}
	}

#ifdef HAS_SE_TRANSLATOR
	ptrOldFilter = _set_se_translator(&CL_CrashReporter_Win32::unhandledExceptionFilter);
#else
	ptrOldFilter = SetUnhandledExceptionFilter(&CL_CrashReporter_Win32::unhandledExceptionFilter);
#endif

#endif
}

CL_CrashReporter_Win32::~CL_CrashReporter_Win32()
{
#ifdef HAS_PDB_SUPPORT
#ifdef HAS_SE_TRANSLATOR
	_set_se_translator(ptrOldFilter);
#else
	// Dont do this since exception filter seem to be on process level and not thread level:
	// ptrOldFilter = SetUnhandledExceptionFilter(ptrOldFilter);
#endif
#endif

//	if (moduleImagehlp == 0) return;
//	FreeLibrary( moduleImagehlp );
//	moduleImagehlp = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CrashReporter Implementation:

#ifdef HAS_PDB_SUPPORT

struct DumpParams
{
	HANDLE hProcess;
	HANDLE hThread;
	int threadId;
	PEXCEPTION_POINTERS exceptionInfo;
	unsigned int exceptionCode;
};

#ifdef HAS_SE_TRANSLATOR
void CL_CrashReporter_Win32::unhandledExceptionFilter(unsigned int exceptionCode, PEXCEPTION_POINTERS exceptionInfo)
#else
LONG CL_CrashReporter_Win32::unhandledExceptionFilter(PEXCEPTION_POINTERS exceptionInfo)
#endif
{
#ifndef HAS_SE_TRANSLATOR
	unsigned int exceptionCode = GetExceptionCode();
#endif

	// Ignore those bloody breakpoints!
	if (exceptionCode == EXCEPTION_BREAKPOINT) return;

	DumpParams dumpParams;
	dumpParams.hProcess = GetCurrentProcess();
	dumpParams.hThread = GetCurrentThread();
	dumpParams.threadId = GetCurrentThreadId();
	dumpParams.exceptionInfo = exceptionInfo;
	dumpParams.exceptionCode = exceptionCode;

	DWORD threadId;
	HANDLE hThread = CreateThread(0, 0, &CL_CrashReporter_Win32::dumpStack, &dumpParams, 0, &threadId);
	WaitForSingleObject(hThread, INFINITE);

//	if (ptrOldFilter) ptrOldFilter(exceptionCode, exceptionInfo);
	abort();
#ifndef HAS_SE_TRANSLATOR
	return EXCEPTION_CONTINUE_SEARCH;
#endif
}

DWORD WINAPI CL_CrashReporter_Win32::dumpStack(LPVOID lpThreadParameter)
{
	DumpParams *dumpParams = (DumpParams *) lpThreadParameter;
	HANDLE hProcess = dumpParams->hProcess;
	HANDLE hThread = dumpParams->hThread;
	PEXCEPTION_POINTERS exceptionInfo = dumpParams->exceptionInfo;

	// Get path to executable:
	TCHAR szDllName[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFilename[256];
	TCHAR szExt[256];

	GetModuleFileName(0, szDllName, _MAX_PATH);
	_splitpath(szDllName, szDrive, szDir, szFilename, szExt);

	TCHAR fileName[1024];

	// Produce minidump file:
	_snprintf(fileName, 1024, "%s%scoredump-%d.dmp", szDrive, szDir, dumpParams->threadId);
	HANDLE hFile = CreateFile(
		fileName,
		GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	MINIDUMP_EXCEPTION_INFORMATION exInfo;

	exInfo.ThreadId = dumpParams->threadId;
	exInfo.ExceptionPointers = exceptionInfo;
	exInfo.ClientPointers = 0;

	ptrMiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &exInfo, 0, 0);

	CloseHandle(hFile);

	// why oh why does SymInitialize() want a writeable string?
	char tt[TTBUFLEN];
	strncpy( tt, userSearchPath.c_str(), TTBUFLEN );
	tt[TTBUFLEN - 1] = '\0'; // if strncpy() overruns, it doesn't add the null terminator

	ptrSymInitialize(hProcess, tt, FALSE);

	DWORD symOptions;
	symOptions = ptrSymGetOptions();
	symOptions |= SYMOPT_LOAD_LINES;
	symOptions &= ~SYMOPT_UNDNAME;
	ptrSymSetOptions( symOptions );

	#define MAXNAMELEN 1024 // max name length for found symbols
	#define IMGSYMLEN ( sizeof IMAGEHLP_SYMBOL )

	IMAGEHLP_SYMBOL *pSym = (IMAGEHLP_SYMBOL *) malloc( IMGSYMLEN + MAXNAMELEN );
	char undName[MAXNAMELEN]; // undecorated name
	char undFullName[MAXNAMELEN]; // undecorated name with all shenanigans
	IMAGEHLP_MODULE Module;
	IMAGEHLP_LINE Line;

	memset( pSym, '\0', IMGSYMLEN + MAXNAMELEN );
	pSym->SizeOfStruct = IMGSYMLEN;
	pSym->MaxNameLength = MAXNAMELEN;

	memset( &Line, '\0', sizeof Line );
	Line.SizeOfStruct = sizeof Line;

	memset( &Module, '\0', sizeof Module );
	Module.SizeOfStruct = sizeof Module;

	DWORD offsetFromSymbol; // tells us how far from the symbol we were
	offsetFromSymbol = 0;

	// Enumerate modules and tell imagehlp.dll about them.
	enumAndLoadModuleSymbols( hProcess, GetCurrentProcessId() );

	STACKFRAME stackframe; // in/out stackframe
	memset( &stackframe, 0, sizeof(stackframe) );
	stackframe.AddrPC.Offset = exceptionInfo->ContextRecord->Eip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = exceptionInfo->ContextRecord->Ebp;
	stackframe.AddrFrame.Mode = AddrModeFlat;

	// normally, call ImageNtHeader() and use machine info from PE header
	DWORD imageType = IMAGE_FILE_MACHINE_I386;

	_snprintf(fileName, 1024, "%s%scoredump-%d.txt", szDrive, szDir, dumpParams->threadId);

	hFile = CreateFile(
		fileName,
		GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	char stringBuffer[1024];
	DWORD bytesWritten;

	char *exceptionStr = "Unknown exception.";
	switch (dumpParams->exceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		exceptionStr = "EXCEPTION_ACCESS_VIOLATION, The thread attempted to read from or write to a virtual address for which it does not have the appropriate access.";
		break;

	case EXCEPTION_BREAKPOINT:
		exceptionStr = "EXCEPTION_BREAKPOINT, A breakpoint was encountered.";
		break;

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		exceptionStr = "EXCEPTION_DATATYPE_MISALIGNMENT, The thread attempted to read or write data that is misaligned on hardware that does not provide alignment.";
		break;

	case EXCEPTION_SINGLE_STEP:
		exceptionStr = "EXCEPTION_SINGLE_STEP, A trace trap or other single-instruction mechanism signaled that one instruction has been executed.";
		break;

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		exceptionStr = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED, The thread attempted to access an array element that is out of bounds, and the underlying hardware supports bounds checking.";
		break;

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		exceptionStr = "EXCEPTION_FLT_DENORMAL_OPERAND, One of the operands in a floating-point operation is denormal.";
		break;

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		exceptionStr = "EXCEPTION_FLT_DIVIDE_BY_ZERO, The thread attempted to divide a floating-point value by a floating-point divisor of zero.";
		break;

	case EXCEPTION_FLT_INEXACT_RESULT:
		exceptionStr = "EXCEPTION_FLT_INEXACT_RESULT, The result of a floating-point operation cannot be represented exactly as a decimal fraction.";
		break;

	case EXCEPTION_FLT_INVALID_OPERATION:
		exceptionStr = "EXCEPTION_FLT_INVALID_OPERATION, This exception represents any floating-point exception not included in this list.";
		break;

	case EXCEPTION_FLT_OVERFLOW:
		exceptionStr = "EXCEPTION_FLT_OVERFLOW, The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.";
		break;

	case EXCEPTION_FLT_STACK_CHECK:
		exceptionStr = "EXCEPTION_FLT_STACK_CHECK, The stack overflowed or underflowed as the result of a floating-point operation.";
		break;

	case EXCEPTION_FLT_UNDERFLOW:
		exceptionStr = "EXCEPTION_FLT_UNDERFLOW, The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.";
		break;

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		exceptionStr = "EXCEPTION_INT_DIVIDE_BY_ZERO, The thread attempted to divide an integer value by an integer divisor of zero.";
		break;

	case EXCEPTION_INT_OVERFLOW:
		exceptionStr = "EXCEPTION_INT_OVERFLOW, The result of an integer operation caused a carry out of the most significant bit of the result.";
		break;

	case EXCEPTION_PRIV_INSTRUCTION:
		exceptionStr = "EXCEPTION_PRIV_INSTRUCTION, The thread attempted to execute an instruction whose operation is not allowed in the current machine mode.";
		break;

	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		exceptionStr = "EXCEPTION_NONCONTINUABLE_EXCEPTION, The thread attempted to continue execution after a noncontinuable exception occurred.";
		break;
	}

	_snprintf(stringBuffer, 1024, "Exception in thread %d: %s\r\n", dumpParams->threadId, exceptionStr);
	WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);

	_snprintf(stringBuffer, 1024, "-- Begin stack trace --\r\n");
	WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);

	_snprintf(stringBuffer, 1024, " Nr Flags PC       Return   Frame    Stack\r\n");
	WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);

	int frameNum = 0;
	while (true)
	{
		// if this returns ERROR_INVALID_ADDRESS (487) or ERROR_NOACCESS (998), you can
		// assume that either you are done, or that the stack is so hosed that the next
		// deeper frame could not be found.

		if (!ptrStackWalk(
				imageType,
				hProcess,
				hThread,
				&stackframe,
				exceptionInfo->ContextRecord,
				0,
				ptrSymFunctionTableAccess,
				ptrSymGetModuleBase,
				0 ) )
			break;

		_snprintf(
			stringBuffer,
			1024,
			"%3d %c%c    %08lx %08lx %08lx %08lx ",
			frameNum,
			stackframe.Far? 'F': '.',
			stackframe.Virtual? 'V': '.',
			stackframe.AddrPC.Offset,
			stackframe.AddrReturn.Offset,
			stackframe.AddrFrame.Offset,
			stackframe.AddrStack.Offset );

		WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);

		if ( stackframe.AddrPC.Offset == 0 )
		{
			_snprintf(stringBuffer, 1024, "(-nosymbols- PC == 0)\r\n" );
			WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
		}
		else
		{
			// we seem to have a valid PC

			// show procedure info (SymGetSymFromAddr())
			if ( ! ptrSymGetSymFromAddr( hProcess, stackframe.AddrPC.Offset, &offsetFromSymbol, pSym ) )
			{
				if ( GetLastError() != 487 )
				{
					_snprintf(stringBuffer, 1024, "SymGetSymFromAddr(): gle = %lu\r\n", GetLastError() );
					WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
				}
			}
			else
			{
				// UnDecorateSymbolName()
				ptrUnDecorateSymbolName( pSym->Name, undName, MAXNAMELEN, UNDNAME_NAME_ONLY );
				ptrUnDecorateSymbolName( pSym->Name, undFullName, MAXNAMELEN, UNDNAME_COMPLETE );
				_snprintf(stringBuffer, 1024, "%s", undName );
				WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
				if ( offsetFromSymbol != 0 )
				{
					_snprintf(stringBuffer, 1024, " %+ld bytes", (long) offsetFromSymbol );
					WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
				}
				WriteFile(hFile, "\r\n", 2, &bytesWritten, 0 );
				_snprintf(stringBuffer, 1024, "    Sig:  %s\r\n", pSym->Name );
				WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
				_snprintf(stringBuffer, 1024, "    Decl: %s\r\n", undFullName );
				WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
			}

			// show line number info, NT5.0-method (SymGetLineFromAddr())
			if ( ptrSymGetLineFromAddr != 0 )
			{ // yes, we have SymGetLineFromAddr()
				if ( ! ptrSymGetLineFromAddr( hProcess, stackframe.AddrPC.Offset, &offsetFromSymbol, &Line ) )
				{
					if ( GetLastError() != 487 )
					{
						_snprintf(stringBuffer, 1024, "SymGetLineFromAddr(): gle = %lu\r\n", GetLastError() );
						WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
					}
				}
				else
				{
					_snprintf(stringBuffer, 1024, "    Line: %s(%lu) %+ld bytes\r\n",
						Line.FileName, Line.LineNumber, offsetFromSymbol );
					WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
				}
			}

			// show module info (SymGetModuleInfo())
			if ( ! ptrSymGetModuleInfo( hProcess, stackframe.AddrPC.Offset, &Module ) )
			{
				_snprintf(stringBuffer, 1024, "SymGetModuleInfo): gle = %lu\r\n", GetLastError() );
				WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
			}
			else
			{ // got module info OK
				char ty[80];
				switch ( Module.SymType )
				{
				case SymNone:
					strcpy( ty, "-nosymbols-" );
					break;
				case SymCoff:
					strcpy( ty, "COFF" );
					break;
				case SymCv:
					strcpy( ty, "CV" );
					break;
				case SymPdb:
					strcpy( ty, "PDB" );
					break;
				case SymExport:
					strcpy( ty, "-exported-" );
					break;
				case SymDeferred:
					strcpy( ty, "-deferred-" );
					break;
				case SymSym:
					strcpy( ty, "SYM" );
					break;
				default:
					_snprintf( ty, sizeof ty, "symtype=%ld", (long) Module.SymType );
					break;
				}

				_snprintf(stringBuffer, 1024, "    Mod:  %s[%s], base: %08lxh\r\n",
					Module.ModuleName, Module.ImageName, Module.BaseOfImage );
				WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
				_snprintf(stringBuffer, 1024, "    Sym:  type: %s, file: %s\r\n",
					ty, Module.LoadedImageName );
				WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);
			}
		}

		// no return address means no deeper stackframe
		if ( stackframe.AddrReturn.Offset == 0 ) break;

		frameNum++;
	}

	_snprintf(stringBuffer, 1024, "-- End stack trace --\r\n");
	WriteFile(hFile, stringBuffer, strlen(stringBuffer), &bytesWritten, 0);

	CloseHandle(hFile);

	ptrSymCleanup(hProcess);

	// todo: signal CL_CrashReporter::sig_crash here
/*
	hFile = CreateFile(
		fileName,
		GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	int pos = 0;
	DWORD bytesRead;
	ReadFile(hFile, stringBuffer, 1024, &bytesRead, 0);
	stringBuffer[bytesRead] = 0;
	while (bytesRead > 0)
	{
		char *ptrNewline = strstr(stringBuffer, "\r\n");
		if (ptrNewline)
		{
			ptrNewline[0] = 0;
			CL_Log::log("debug", stringBuffer);
			pos += ptrNewline-stringBuffer+2;
			SetFilePointer(hFile, pos, 0, FILE_BEGIN);
		}

		ReadFile(hFile, stringBuffer, 1024, &bytesRead, 0);
		stringBuffer[bytesRead] = 0;
	}

	CloseHandle(hFile);
*/

	return 0;
}

void CL_CrashReporter_Win32::enumAndLoadModuleSymbols( HANDLE hProcess, DWORD pid )
{
	ModuleList modules;
	ModuleListIter it;
	char *img, *mod;

	// fill in module list
	fillModuleList( modules, pid, hProcess );

	for ( it = modules.begin(); it != modules.end(); ++ it )
	{
		// unfortunately, SymLoadModule() wants writeable strings
		img = new char[(*it).imageName.size() + 1];
		strcpy( img, (*it).imageName.c_str() );
		mod = new char[(*it).moduleName.size() + 1];
		strcpy( mod, (*it).moduleName.c_str() );

		ptrSymLoadModule( hProcess, 0, img, mod, (*it).baseAddress, (*it).size );
/*		if ( ptrSymLoadModule( hProcess, 0, img, mod, (*it).baseAddress, (*it).size ) == 0 )
			printf( "Error %lu loading symbols for \"%s\"\n",
				GetLastError(), (*it).moduleName.c_str() );
		else
			printf( "Symbols loaded: \"%s\"\n", (*it).moduleName.c_str() );
*/
		delete [] img;
		delete [] mod;
	}
}



bool CL_CrashReporter_Win32::fillModuleList( ModuleList& modules, DWORD pid, HANDLE hProcess )
{
	// try toolhelp32 first
	if ( fillModuleListTH32( modules, pid ) )
		return true;
	// nope? try psapi, then
	return fillModuleListPSAPI( modules, pid, hProcess );
}



// miscellaneous toolhelp32 declarations; we cannot #include the header
// because not all systems may have it
#define MAX_MODULE_NAME32 255
#define TH32CS_SNAPMODULE   0x00000008
#pragma pack( push, 8 )
typedef struct tagMODULEENTRY32
{
    DWORD   dwSize;
    DWORD   th32ModuleID;       // This module
    DWORD   th32ProcessID;      // owning process
    DWORD   GlblcntUsage;       // Global usage count on the module
    DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
    BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
    DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
    HMODULE hModule;            // The hModule of this module in th32ProcessID's context
    char    szModule[MAX_MODULE_NAME32 + 1];
    char    szExePath[MAX_PATH];
} MODULEENTRY32;
typedef MODULEENTRY32 *  PMODULEENTRY32;
typedef MODULEENTRY32 *  LPMODULEENTRY32;
#pragma pack( pop )



bool CL_CrashReporter_Win32::fillModuleListTH32( ModuleList& modules, DWORD pid )
{
	// CreateToolhelp32Snapshot()
	typedef HANDLE (__stdcall *tCT32S)( DWORD dwFlags, DWORD th32ProcessID );
	// Module32First()
	typedef BOOL (__stdcall *tM32F)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );
	// Module32Next()
	typedef BOOL (__stdcall *tM32N)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );

	// I think the DLL is called tlhelp32.dll on Win9X, so we try both
	const char *dllname[] = { "kernel32.dll", "tlhelp32.dll" };
	HINSTANCE hToolhelp;
	tCT32S pCT32S;
	tM32F pM32F;
	tM32N pM32N;

	HANDLE hSnap;
	MODULEENTRY32 me = { sizeof me };
	bool keepGoing;
	ModuleEntry e;
	int i;

	#define lenof(a) (sizeof(a) / sizeof((a)[0]))
	for ( i = 0; i < lenof( dllname ); ++ i )
	{
		hToolhelp = LoadLibraryA( dllname[i] );
		if ( hToolhelp == 0 )
			continue;
		pCT32S = (tCT32S) GetProcAddress( hToolhelp, "CreateToolhelp32Snapshot" );
		pM32F = (tM32F) GetProcAddress( hToolhelp, "Module32First" );
		pM32N = (tM32N) GetProcAddress( hToolhelp, "Module32Next" );
		if ( pCT32S != 0 && pM32F != 0 && pM32N != 0 )
			break; // found the functions!
		FreeLibrary( hToolhelp );
		hToolhelp = 0;
	}

	if ( hToolhelp == 0 ) // nothing found?
		return false;

	hSnap = pCT32S( TH32CS_SNAPMODULE, pid );
	if ( hSnap == (HANDLE) -1 )
		return false;

	keepGoing = !!pM32F( hSnap, &me );
	while ( keepGoing )
	{
		// here, we have a filled-in MODULEENTRY32
//		printf( "%08lXh %6lu %-15.15s %s\n", me.modBaseAddr, me.modBaseSize, me.szModule, me.szExePath );
		e.imageName = me.szExePath;
		e.moduleName = me.szModule;
		e.baseAddress = (DWORD) me.modBaseAddr;
		e.size = me.modBaseSize;
		modules.push_back( e );
		keepGoing = !!pM32N( hSnap, &me );
	}

	CloseHandle( hSnap );

	FreeLibrary( hToolhelp );

	return modules.size() != 0;
}



// miscellaneous psapi declarations; we cannot #include the header
// because not all systems may have it
typedef struct _MODULEINFO {
    LPVOID lpBaseOfDll;
    DWORD SizeOfImage;
    LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;



bool CL_CrashReporter_Win32::fillModuleListPSAPI( ModuleList& modules, DWORD pid, HANDLE hProcess )
{
	// EnumProcessModules()
	typedef BOOL (__stdcall *tEPM)( HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded );
	// GetModuleFileNameEx()
	typedef DWORD (__stdcall *tGMFNE)( HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
	// GetModuleBaseName() -- redundant, as GMFNE() has the same prototype, but who cares?
	typedef DWORD (__stdcall *tGMBN)( HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
	// GetModuleInformation()
	typedef BOOL (__stdcall *tGMI)( HANDLE hProcess, HMODULE hModule, LPMODULEINFO pmi, DWORD nSize );

	HINSTANCE hPsapi;
	tEPM pEPM;
	tGMFNE pGMFNE;
	tGMBN pGMBN;
	tGMI pGMI;

	unsigned int i;
	ModuleEntry e;
	DWORD cbNeeded;
	MODULEINFO mi;
	HMODULE *hMods = 0;
	char *tt = 0;

	hPsapi = LoadLibrary( "psapi.dll" );
	if ( hPsapi == 0 )
		return false;

	modules.clear();

	pEPM = (tEPM) GetProcAddress( hPsapi, "EnumProcessModules" );
	pGMFNE = (tGMFNE) GetProcAddress( hPsapi, "GetModuleFileNameExA" );
	pGMBN = (tGMFNE) GetProcAddress( hPsapi, "GetModuleBaseNameA" );
	pGMI = (tGMI) GetProcAddress( hPsapi, "GetModuleInformation" );
	if ( pEPM == 0 || pGMFNE == 0 || pGMBN == 0 || pGMI == 0 )
	{
		// yuck. Some API is missing.
		FreeLibrary( hPsapi );
		return false;
	}

	hMods = new HMODULE[TTBUFLEN / sizeof HMODULE];
	tt = new char[TTBUFLEN];
	// not that this is a sample. Which means I can get away with
	// not checking for errors, but you cannot. :)

	if ( ! pEPM( hProcess, hMods, TTBUFLEN, &cbNeeded ) )
	{
//		printf( "EPM failed, gle = %lu\n", GetLastError() );
		goto cleanup;
	}

	if ( cbNeeded > TTBUFLEN )
	{
//		printf( "More than %lu module handles. Huh?\n", lenof( hMods ) );
		goto cleanup;
	}

	for ( i = 0; i < cbNeeded / sizeof hMods[0]; ++ i )
	{
		// for each module, get:
		// base address, size
		pGMI( hProcess, hMods[i], &mi, sizeof mi );
		e.baseAddress = (DWORD) mi.lpBaseOfDll;
		e.size = mi.SizeOfImage;
		// image file name
		tt[0] = '\0';
		pGMFNE( hProcess, hMods[i], tt, TTBUFLEN );
		e.imageName = tt;
		// module name
		tt[0] = '\0';
		pGMBN( hProcess, hMods[i], tt, TTBUFLEN );
		e.moduleName = tt;
//		printf( "%08lXh %6lu %-15.15s %s\n", e.baseAddress,
//			e.size, e.moduleName.c_str(), e.imageName.c_str() );

		modules.push_back( e );
	}

cleanup:
	if ( hPsapi )
		FreeLibrary( hPsapi );
	delete [] tt;
	delete [] hMods;

	return modules.size() != 0;
}

HINSTANCE CL_CrashReporter_Win32::moduleImagehlp = 0;

HINSTANCE CL_CrashReporter_Win32::moduleDbghlp = 0;

CL_CrashReporter_Win32::TypeSymCleanup CL_CrashReporter_Win32::ptrSymCleanup = 0;

CL_CrashReporter_Win32::TypeSymFunctionTableAccess CL_CrashReporter_Win32::ptrSymFunctionTableAccess = 0;

CL_CrashReporter_Win32::TypeSymGetLineFromAddr CL_CrashReporter_Win32::ptrSymGetLineFromAddr = 0;

CL_CrashReporter_Win32::TypeSymGetModuleBase CL_CrashReporter_Win32::ptrSymGetModuleBase = 0;

CL_CrashReporter_Win32::TypeSymGetModuleInfo CL_CrashReporter_Win32::ptrSymGetModuleInfo = 0;

CL_CrashReporter_Win32::TypeSymGetOptions CL_CrashReporter_Win32::ptrSymGetOptions = 0;

CL_CrashReporter_Win32::TypeSymGetSymFromAddr CL_CrashReporter_Win32::ptrSymGetSymFromAddr = 0;

CL_CrashReporter_Win32::TypeSymInitialize CL_CrashReporter_Win32::ptrSymInitialize = 0;

CL_CrashReporter_Win32::TypeSymLoadModule CL_CrashReporter_Win32::ptrSymLoadModule = 0;

CL_CrashReporter_Win32::TypeSymSetOptions CL_CrashReporter_Win32::ptrSymSetOptions = 0;

CL_CrashReporter_Win32::TypeStackWalk CL_CrashReporter_Win32::ptrStackWalk = 0;

CL_CrashReporter_Win32::TypeUnDecorateSymbolName CL_CrashReporter_Win32::ptrUnDecorateSymbolName = 0;

CL_CrashReporter_Win32::TypeMiniDumpWriteDump CL_CrashReporter_Win32::ptrMiniDumpWriteDump = 0;

_se_translator_function CL_CrashReporter_Win32::ptrOldFilter = 0;

std::string CL_CrashReporter_Win32::userSearchPath;

#endif
