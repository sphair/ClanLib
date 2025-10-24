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
**    Kenneth Gangstoe
*/

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once

#include "../api_core.h"
#include "../IOData/datatypes.h"

/// \brief Date/Time class.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_DateTime
{
/// \name Construction
/// \{
public:
	enum TimeZone
	{
		local_timezone,
		utc_timezone
	};

	/// \brief Constructs a date/time object.
	CL_DateTime();
	CL_DateTime(int year, int month, int day, int hour = 0, int minute = 0, int seconds = 0, int nanoseconds = 0, TimeZone timezone=utc_timezone);
	~CL_DateTime();

	/// \brief Get current system time in local time zone.
	static CL_DateTime get_current_local_time();

	/// \brief Get current system time in UTC.
	static CL_DateTime get_current_utc_time();

	/// \brief Converts a time tick value (number of 100-nanosecond intervals since January 1, 1601 UTC) to a date time in the local time zone.
	static CL_DateTime get_local_time_from_ticks(cl_int64 ticks);

	/// \brief Converts a time tick value (number of 100-nanosecond intervals since January 1, 1601 UTC) to a date time in UTC.
	static CL_DateTime get_utc_time_from_ticks(cl_int64 ticks);

	static CL_DateTime from_short_date_string(const CL_String &value);

/// \}

/// \name Attributes
/// \{
public:
	bool is_null() const;
	unsigned short get_year() const;

	/// \brief Returns the month number in range 1-12.
	///
	/// \return 1 = Jan ... 12 = December
	unsigned char get_month() const;
	unsigned char get_day() const;
	unsigned char get_hour() const;
	unsigned char get_minutes() const;
	unsigned char get_seconds() const;
	unsigned int get_nanoseconds() const;
	TimeZone get_timezone() const;

	/// \brief Get the day of the week
	///
	/// \return 0 = Sunday ... 6 = Saturday
	unsigned int get_day_of_week() const;
/// \}

/// \name Operations
/// \{
public:
	void set_null();
	void set_date(int year, int month, int day, int hour = 0, int minute = 0, int seconds = 0, int nanoseconds = 0, TimeZone timezone = utc_timezone);
	void set_year(int year);
	void set_month(int month);
	void set_day(int day);
	void set_hour(int hour);
	void set_minutes(int minutes);
	void set_seconds(int seconds);
	void set_nanoseconds(int nanoseconds);
	void set_timezone(TimeZone timezone);
/*
	void add_years(int years);
	void add_days(int days);
	void add_months(int months);
	void add_hours(int hours);
	void add_minutes(int minutes);
	void add_seconds(int seconds);
	void add_nanoseconds(int nanoseconds);
*/
	CL_DateTime to_utc() const;
	CL_DateTime to_local() const;

	/// \brief Converts the date to the number of 100-nanosecond intervals since January 1, 1601 UTC.
	cl_int64 to_ticks() const;

	/// \brief Mon Mar 3 2007
	CL_String to_long_date_string() const;

	/// \brief yyyy-mm-dd
	CL_String to_short_date_string() const;

	/// \brief yyyy-mm-dd hh:mm:ss
	CL_String to_short_datetime_string() const;

	/// \brief hh:mm:ss
	CL_String to_long_time_string() const;

	/// \brief hh:mm
	CL_String to_short_time_string() const;

	/// \brief Mon Feb 3 12:32:54 2008
	CL_String to_string() const;

	bool operator <(const CL_DateTime &other) const;
	bool operator <=(const CL_DateTime &other) const;
	bool operator >(const CL_DateTime &other) const;
	bool operator >=(const CL_DateTime &other) const;
	bool operator ==(const CL_DateTime &other) const;
	bool operator !=(const CL_DateTime &other) const;
/// \}

/// \name Implementation
/// \{
private:
	void throw_if_invalid_date(int year, int month, int day, int hour, int minute, int seconds, int nanoseconds) const;
	void throw_if_null() const;

	unsigned short year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char seconds;
	unsigned int nanoseconds;

	TimeZone timezone;
	static const cl_int64 ticks_from_1601_to_1900;
/// \}
};
/// \}
