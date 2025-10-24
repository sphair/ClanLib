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

#ifndef header_log
#define header_log

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
#include "clanstring.h"

//: Logging interface.
//- !group=Core/System!
//- !header=core.h!
class CL_API_CORE CL_Log
{
//! Construction:
public:

//! Attributes:
public:
	//: Returns the signal that gets emitted every time a log is posted.
	static CL_Signal_v3<const std::string &, int, const std::string &> &sig_log();

//! Operations:
public:
	//: Posts a log message with severity 0.
	static void log(const std::string &group, const std::string &message);

	//: Posts a log message.
	static void log(const std::string &group, int severity, const std::string &message);

	template<class Param1>
	static void log(const std::string &group, const std::string &format, const Param1 &p1)
	{
		log(group, CL_String::format(format, p1));
	}

	template<class Param1, class Param2>
	static void log(const std::string &group, const std::string &format, const Param1 &p1, const Param2 &p2)
	{
		log(group, CL_String::format(format, p1, p2));
	}

	template<class Param1, class Param2, class Param3>
	static void log(const std::string &group, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3)
	{
		log(group, CL_String::format(format, p1, p2, p3));
	}

	template<class Param1, class Param2, class Param3, class Param4>
	static void log(const std::string &group, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4)
	{
		log(group, CL_String::format(format, p1, p2, p3, p4));
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5>
	static void log(const std::string &group, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5)
	{
		log(group, CL_String::format(format, p1, p2, p3, p4, p5));
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>
	static void log(const std::string &group, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5, const Param6 &p6)
	{
		log(group, CL_String::format(format, p1, p2, p3, p4, p5, p6));
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7>
	static void log(const std::string &group, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5, const Param6 &p6, const Param7 &p7)
	{
		log(group, CL_String::format(format, p1, p2, p3, p4, p5, p6, p7));
	}

	template<class Param1>
	static void log(const std::string &group, int severity, const std::string &format, const Param1 &p1)
	{
		log(group, severity, CL_String::format(format, p1));
	}

	template<class Param1, class Param2>
	static void log(const std::string &group, int severity, const std::string &format, const Param1 &p1, const Param2 &p2)
	{
		log(group, severity, CL_String::format(format, p1, p2));
	}

	template<class Param1, class Param2, class Param3>
	static void log(const std::string &group, int severity, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3)
	{
		log(group, severity, CL_String::format(format, p1, p2, p3));
	}

	template<class Param1, class Param2, class Param3, class Param4>
	static void log(const std::string &group, int severity, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4)
	{
		log(group, severity, CL_String::format(format, p1, p2, p3, p4));
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5>
	static void log(const std::string &group, int severity, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5)
	{
		log(group, severity, CL_String::format(format, p1, p2, p3, p4, p5));
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>
	static void log(const std::string &group, int severity, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5, const Param6 &p6)
	{
		log(group, severity, CL_String::format(format, p1, p2, p3, p4, p5, p6));
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7>
	static void log(const std::string &group, int severity, const std::string &format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5, const Param6 &p6, const Param7 &p7)
	{
		log(group, severity, CL_String::format(format, p1, p2, p3, p4, p5, p6, p7));
	}

//! Implementation:
private:
};

#endif
