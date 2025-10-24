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
#include "crash_reporter_generic.h"

#ifdef WIN32
	#define CL_CrashReporter_Platform CL_CrashReporter_Win32
	#include "../Win32/crash_reporter_win32.h"
#else
	#define CL_CrashReporter_Platform CL_CrashReporter_Unix
	#include "../Unix/crash_reporter_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CrashReporter Construction:

CL_CrashReporter::CL_CrashReporter() : impl(new CL_CrashReporter_Platform)
{
}

CL_CrashReporter::~CL_CrashReporter()
{
}

/////////////////////////////////////////////////////////////////////////////
// CrashReporter Attributes:

static bool dump_to_file = true;

bool CL_CrashReporter::get_dump_to_file()
{
	return dump_to_file;
}

void CL_CrashReporter::set_dump_to_file(bool enable)
{
	dump_to_file = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CrashReporter Operations:

CL_Signal_v2<std::string, CL_CallStack> &CL_CrashReporter::sig_crash()
{
	static CL_Signal_v2<std::string, CL_CallStack> signal;
	return signal;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CrashReporter Implementation:
