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

//! clanCore="System"
//! header=core.h

#ifndef header_crash_reporter
#define header_crash_reporter

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../signals.h"
#include "call_stack.h"
#include "sharedptr.h"

class CL_CrashReporter_Generic;

//: Crash reporting tool.
//- !group=Core/System!
//- !header=core.h!
//- <p>Constructing an instance of this class in a thread will cause any fatal errors
//- (ie. segmentation faults) to be caught and handled by the crash reporter. It
//- will then dump the callstack to file and then afterwards emit sig_crash.</p>
class CL_API_CORE CL_CrashReporter
{
//! Construction:
public:
	CL_CrashReporter();

	~CL_CrashReporter();

//! Attributes:
public:
	//: Returns the flag indicating if it will dump the callstack to file.
	static bool get_dump_to_file();

	//: If enabled, will dump the callstack to file before emitting sig_crash.
	static void set_dump_to_file(bool enable);

//! Operations:
public:
	//: Signal emitted when fatal errors are caught.
	static CL_Signal_v2<std::string, CL_CallStack> &sig_crash();

//! Implementation:
private:
	CL_SharedPtr<CL_CrashReporter_Generic> impl;
};

#endif
