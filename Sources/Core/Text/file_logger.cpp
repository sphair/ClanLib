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
*/

#include "Core/precomp.h"
#include "API/Core/Text/file_logger.h"
#include "API/Core/IOData/file.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/datetime.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FileLogger Construction:

CL_FileLogger::CL_FileLogger(const CL_StringRef &filename) : file(0)
{
	file = new CL_File(filename, CL_File::open_always, CL_File::access_read_write);
}

CL_FileLogger::~CL_FileLogger()
{
	delete file;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FileLogger Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_FileLogger Operations:

void CL_FileLogger::log(const CL_StringRef &type, const CL_StringRef &text)
{
	CL_StringRef months[] =
	{
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec"
	};
	
	CL_StringRef days[] =
	{
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat"
	};

	// Tue Nov 16 11:34:15 CET 2004
	CL_DateTime cur_time = CL_DateTime::get_current_utc_time();

#ifdef WIN32
	CL_StringFormat format("%1 %2 %3 %4:%5:%6 %7 UTC [%8] %9\r\n");
#else
	CL_StringFormat format("%1 %2 %3 %4:%5:%6 %7 UTC [%8] %9\n");
#endif
	format.set_arg(1, days[cur_time.get_day_of_week()]);
	format.set_arg(2, months[cur_time.get_month() - 1]);
	format.set_arg(3, cur_time.get_day());
	format.set_arg(4, cur_time.get_hour(), 2);
	format.set_arg(5, cur_time.get_minutes(), 2);
	format.set_arg(6, cur_time.get_seconds(), 2);
	format.set_arg(7, cur_time.get_year());
	format.set_arg(8, type);
	format.set_arg(9, text);
	CL_String8 log_line = CL_StringHelp::text_to_local8(format.get_result());

	file->seek(0, CL_File::seek_end);
	file->write(log_line.data(), (int) log_line.length());
}

/////////////////////////////////////////////////////////////////////////////
// CL_FileLogger Implementation:
