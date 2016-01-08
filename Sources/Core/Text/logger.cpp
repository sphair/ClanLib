/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Core/System/datetime.h"
#include "API/Core/Text/logger.h"
#include "API/Core/Text/string_format.h"
#include <algorithm>
#include <mutex>

namespace clan
{
	Logger::Logger()
	{
		enable();
	}

	Logger::~Logger()
	{
		disable();
	}

	std::vector<Logger*> Logger::instances;
	std::recursive_mutex Logger::mutex;

	void Logger::enable()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(Logger::mutex);
		if (std::find(instances.begin(), instances.end(), this) == instances.end())
			instances.push_back(this);
	}

	void Logger::disable()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(Logger::mutex);
		auto il = std::find(instances.begin(), instances.end(), this);
		if (il != instances.end())
			instances.erase(il);
	}

	StringFormat Logger::get_log_string(const std::string &type, const std::string &text)
	{
		std::string months[] =
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

		std::string days[] =
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
		DateTime cur_time = DateTime::get_current_utc_time();

#ifdef WIN32
		StringFormat format("%1 %2 %3 %4:%5:%6 %7 UTC [%8] %9\r\n");
#else
		StringFormat format("%1 %2 %3 %4:%5:%6 %7 UTC [%8] %9\n");
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

		return format;
	}

	void log_event(const std::string &type, const std::string &text)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(Logger::mutex);
		if (Logger::instances.empty())
			return;
		for (auto & instance : Logger::instances)
			(instance)->log(type, text);
	}
}
