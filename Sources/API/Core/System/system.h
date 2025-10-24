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

#ifndef header_system
#define header_system

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

#include <string>

class CL_EventListener;
class CL_Signal_v0;

//: Generic functions that didn't fit in other places.
//- !group=Core/System!
//- !header=core.h!
class CL_API_CORE CL_System
{
//! Attributes:
public:
	//: Get the current time (since system boot), in milliseconds, including effects of suspend_time()
	static unsigned int get_time();
	
	//: Returns true if MMX technology is available on this CPU.
	static bool detect_mmx();
	
	//: Returns true if 3DNow! technology is available on this CPU.
	static bool detect_3dnow();

	//: Returns true if extended 3DNow! is present on this CPU.
	static bool detect_ext_3dnow();

	//: Returns the amount of cpus and -1 if it wasn't possible to detect
	static int get_num_cores();

//! Operations:
public:
	//: Checks for system events, updates input and other stuff.
	static void keep_alive();

	//: Checks for system events (pump the CL_KeepAlive objects).
	//- <p>Return when 'millis' has elapsed.</p>
	static void keep_alive(int millis);

	//: Checks for system events (pump the CL_KeepAlive objects).
	//- <p>Return when one of the events trigger, or when timeout has elapsed.</p>
	static bool keep_alive(CL_EventListener &events, int timeout = -1);

	//: Sleep for 'millis' milliseconds.
	static void sleep(int millis);

	//: Suspend the time.
	//- <p>get_time() will continue to return the same value until resume_time() is called.</p>
	static void suspend_time();

	//: Resumes the time.
	//- <p>get_time() will return the current time again.</p>
	static void resume_time();
	
	//: Returns the full dirname of the executable that started this
	//: process (aka argv[0])
	//- <p>This is necessary since when programms are started from
	//- the PATH there is no clean and simple way to figure out
	//- the location of the data files, thus information is read
	//- from <tt>/proc/self/exe</tt> under GNU/Linux and from
	//- GetModuleFileName() on Win32.</p>
        //return: full dirname of the executable, trailing slash is included
	static std::string get_exe_path();

//! Implementation:
private:
	//Starting sys_time() of the least recently started but still active suspend
	static unsigned int susp_start;

	//Number of active suspends
	static unsigned int susp_cnt;

	//Time taken up by suspends that occurred before
	static unsigned int susp_accum;
	
	//Done by platform-specific stuff, returns the cur time without dealing with any of this suspend/resume stuff
	static unsigned int sys_time();
};

#endif
