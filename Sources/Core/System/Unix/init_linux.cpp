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

#include <libgen.h>
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include "implementation.h"
#include "init_linux.h"
// note: this cannot be replaced by <ctime>! (timeval needs to be defined)
#include <sys/time.h>
#include "API/Core/System/setupcore.h"
#include "API/Core/System/system.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

static int init_ref_count = 0;
long _begin_time;

void init_system()
{
	init_ref_count++;
	if (init_ref_count > 1) return;

	timeval tv;
	gettimeofday(&tv, NULL);
	_begin_time = (long) tv.tv_sec*(long) 1000+(long) tv.tv_usec/(long) 1000;

	#ifdef NDEBUG
	signal(SIGSEGV, deinit);
	#endif
}

void deinit_system()
{
	init_ref_count--;
	if (init_ref_count > 0) return;

	#ifdef NDEBUG
	signal(SIGSEGV,SIG_DFL); //restore default behavior
	#endif
}

unsigned int CL_System::sys_time()
{
	timeval tv;
	gettimeofday(&tv, NULL);

	return (long) tv.tv_sec*(long) 1000 + (long) tv.tv_usec/(long) 1000 - _begin_time;
}

void CL_System::sleep(int millis)
{
	timeval tv;
	tv.tv_sec = millis/1000;
	tv.tv_usec = (millis%1000)*1000;
	select(0, 0, 0, 0, &tv);
}

std::string CL_System::get_exe_path()
{
	char exe_file[PATH_MAX + 1];
#ifdef __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (mainBundle) 
	{
		CFURLRef mainURL = CFBundleCopyBundleURL(mainBundle);
		
		if (mainURL) 
		{
			int ok = CFURLGetFileSystemRepresentation (
				mainURL, (Boolean) true, (UInt8*)exe_file, PATH_MAX 
			);
			
			if (ok)
			{
				return std::string(exe_file) + "/";
			}
		}
	}
	
	throw CL_Error("get_exe_path failed");	

#else
	int size;
	size = readlink("/proc/self/exe", exe_file, PATH_MAX);
	if (size < 0)
	{
		throw CL_Error(strerror(errno));
	}
	else
	{
		exe_file[size] = '\0';
		return std::string(dirname(exe_file)) + "/";
	}
#endif
	
}

/* EOF */
