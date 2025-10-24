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
#include "API/Core/System/clanstring.h"
#include "API/Core/Resources/resourcetype_string.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Display/color.h"

#include <ctype.h>
#include <stdio.h>
#include <cstring>

#ifdef WIN32
#define snprintf _snprintf
#define CLANSTRING_PATH_DIVIDER "/\\"
#else
#define CLANSTRING_PATH_DIVIDER '/'
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_String operations:

std::string CL_String::load(const std::string &res_id, CL_ResourceManager *resources)
{
	CL_Resource &res = resources->get_resource(res_id);
	res.load();
	CL_ResourceData_String *data = (CL_ResourceData_String *) res.get_data("string");
	res.unload();
	return data->value;
}

std::string CL_String::load(const std::string &res_id, CL_ResourceManager *resources, const std::string &def_value)
{
	try
	{
		CL_Resource &res = resources->get_resource(res_id);
		res.load();
		CL_ResourceData_String *data = (CL_ResourceData_String *) res.get_data("string");
		res.unload();
		return data->value;
	}
	catch (CL_Error err)
	{
		return def_value;
	}
}

std::string CL_String::left(std::string string, int n)
{
	return string.substr(0, n);
}

std::string CL_String::right(std::string string, int n)
{
	return string.substr(string.length() - n, string.length());
}

void CL_String::arg(std::string &format, const std::string &arg, int num)
{
	//Covert num into a string
	char number[10];
	std::string num_string = "%";
	
	memset(number, 0, 10);
	snprintf(number, 10, "%d", num);
	
	num_string += number;
	
	std::string::size_type offset = format.find(num_string);
	
	if(offset == std::string::npos)
	{
		//Ah shit!  The value doesn't exist!
		throw CL_Error("Unable to find value in controlled string: " + format);
	}
	
	format.replace(format.begin() + offset, format.begin() + offset + num_string.length(), arg);
}

void CL_String::arg(std::string &format, const char *arg, int num)
{
	CL_String::arg(format, std::string(arg), num);
}

void CL_String::arg(std::string &format, int number, int num)
{
	char arg[10];
	
	memset(arg, 0, 10);
	snprintf(arg, 10, "%d", number);
	
	CL_String::arg(format, arg, num);
}

void CL_String::arg(std::string &format, float number, int num)
{
	char arg[32];

	memset(arg, 0, 32);
	snprintf(arg, 32, "%f", number);
	
	CL_String::arg(format, arg, num);
}

void CL_String::arg(std::string &format, double number, int num)
{
	char arg[32];
	
	memset(arg, 0, 32);
	snprintf(arg, 32, "%#f", number);
	
	CL_String::arg(format, arg, num);
}

std::string CL_String::from_int(int value)
{
	char str[32];
	memset(str, 0, 32);
	snprintf(str, 32, "%d", value);
	return std::string(str);
}

std::string CL_String::from_float(float value)
{
	char str[32];
	memset(str, 0, 32);
	snprintf(str, 32, "%f", value);
	return std::string(str);
}

std::string CL_String::from_double(double value)
{
	char str[32];
	memset(str, 0, 32);
	snprintf(str, 32, "%#f", value);
	return std::string(str);
}

std::string CL_String::from_bool(bool value)
{
	if (value) return "true";
	return "false";
}

	//- <p>Accepted values: "0", "1", "yes", "no", "true", "false". The function is not case sensitive.</p>
bool CL_String::to_bool(const std::string &value)
{
	std::string str = CL_String::to_lower(value);
	if (str == "1") return true;
	if (str == "0") return false;
	if (str == "yes") return true;
	if (str == "no") return false;
	if (str == "true") return true;
	if (str == "false") return false;
	return false;
}

int CL_String::to_int(const std::string &str)
{
	return atoi(str.c_str());
}

float CL_String::to_float(const std::string &str) 
{
	return (float) atof(str.c_str());
}

double CL_String::to_double(const std::string &str) 
{
	return atof(str.c_str());
}

std::string CL_String::to_lower(const std::string &str) 
{
	std::string lower_impl = str;

	for( std::string::iterator i = lower_impl.begin();
		i != lower_impl.end();
		++i )
	{
		*i = tolower(*i);
	}

	return lower_impl;
}

std::string CL_String::to_upper(const std::string &str) 
{
	std::string upper_impl = str;

	for( std::string::iterator i = upper_impl.begin();
		i != upper_impl.end();
		++i )
	{
		*i = toupper(*i);
	}

	return upper_impl;
}

std::vector<std::string> CL_String::tokenize(const std::string &str, const std::string &delimit, bool skip_empty)
{
	std::vector<std::string> ret;
	std::string::size_type last_pos = 0;
	std::string::size_type pos = std::string::npos;
	if (skip_empty)
	{
		last_pos = str.find_first_not_of(delimit, 0);	
		if (std::string::npos != last_pos)
			pos = str.find_first_of(delimit, last_pos);
		while (std::string::npos != pos || std::string::npos != last_pos)
		{
			ret.push_back(str.substr(last_pos, pos - last_pos));
			last_pos = str.find_first_not_of(delimit, pos);
			pos = str.find_first_of(delimit, last_pos);
		}
	}
	else
	{
		pos = str.find_first_of(delimit, last_pos);
		while (std::string::npos != pos)
		{
			ret.push_back(str.substr(last_pos, pos - last_pos));
			last_pos = pos + 1;
			pos = str.find_first_of(delimit, last_pos);
		}
		ret.push_back(str.substr(last_pos));
	}
	return ret;
}

bool CL_String::compare_nocase(const std::string &a, const std::string &b)
{
	std::string::const_iterator i = a.begin();
	std::string::const_iterator j = b.begin();
	std::string::const_iterator i_end = a.end();
	std::string::const_iterator j_end = b.end();

	for(; (i != i_end) && (j != j_end); ++i, ++j)
	{
		if( tolower(*i) != tolower(*j) )
		{
			return false;
		}
	}

	return i == i_end && j == j_end;
}

std::string CL_String::get_path(const std::string &pathname)
{
	std::string::size_type last_slash = pathname.find_last_of(CLANSTRING_PATH_DIVIDER);

	if (last_slash == std::string::npos) return std::string(".");
	return pathname.substr(0, last_slash);
}

std::string CL_String::get_filename(const std::string &pathname)
{
	std::string::size_type last_slash = pathname.find_last_of(CLANSTRING_PATH_DIVIDER);

	if (last_slash == std::string::npos) return pathname;
	return pathname.substr(last_slash+1);
}

std::string CL_String::get_extension(const std::string &filename)
{
	std::string::size_type pos = filename.find_last_of('.');
	if (pos == filename.npos) return std::string();
	return filename.substr(pos+1);
}

std::string CL_String::trim_spaces(const std::string &str)
{
	std::string::size_type firstchar = str.find_first_not_of(' ');
	std::string::size_type lastchar = str.find_last_not_of(' ');

	if (firstchar == std::string::npos) return std::string();
	if (lastchar == std::string::npos) return std::string();

	return str.substr(firstchar, lastchar-firstchar+1);
}

std::string CL_String::trim_whitespace(const std::string &str)
{
	std::string::size_type length = str.length();
	if (length == 0) return str;

	std::string::size_type firstchar;
	std::string::size_type lastchar;
	
	for (firstchar = 0; firstchar < length; firstchar++)
	{
		if (
			str[firstchar] != ' ' &&
			str[firstchar] != '\t' &&
			str[firstchar] != '\r' &&
			str[firstchar] != '\n')
		{
			break;
		}

		if (firstchar == length-1) return std::string();
	}

	for (lastchar = length-1; lastchar >= 0; lastchar--)
	{
		if (
			str[lastchar] != ' ' &&
			str[lastchar] != '\t' &&
			str[lastchar] != '\r' &&
			str[lastchar] != '\n')
		{
			break;
		}

		if (lastchar == 0) return std::string();
	}

	return str.substr(firstchar, lastchar-firstchar+1);
}

std::string CL_String::convert_c_escapes(const std::string &str)
{
	std::string copy = str;

	std::string::size_type pos = copy.find("\\n");
	while( pos != std::string::npos )
	{
		copy = copy.replace( pos, 2, "\n" ); // replace 2 chars at pos with '\n'.
		pos = copy.find("\\n", pos);
	}

	pos = copy.find("\\t");
	while( pos != std::string::npos )
	{
		copy = copy.replace( pos, 2, "\t" );
		pos = copy.find("\\t", pos);
	}

	pos = copy.find("\\\\");
	while( pos != std::string::npos )
	{
		copy = copy.replace( pos, 2, "\\" );
		pos = copy.find("\\", pos);
	}

	return copy;
}

/////////////////////////////////////////////////////////////////////////////
// CL_String implementation:
