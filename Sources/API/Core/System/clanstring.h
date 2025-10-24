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

#ifndef header_clanstring
#define header_clanstring

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

#include <sstream>
#include <string>
#include <vector>

class CL_ResourceManager;

class CL_API_CORE CL_String
//: ClanLib string class.
//- !group=Core/System!
//- !header=core.h!
//- <p>This is a general purpose string class with formatting capabilities.</p>
//- <p>Example: str::string s = CL_String::format("You rock my %1, %2 (%3)", "world", name, 47);</p>
{
//! Operations:
public:
	//: Loads a string from a resource.
	//param res_id: Resource to load string from.
	//param resources: Resource manager used to load string.
	//param def_value: Default value if resource is not found.
	static std::string load(const std::string &res_id, CL_ResourceManager *resources);

	static std::string load(const std::string &res_id, CL_ResourceManager *resources, const std::string &def_value);
	
	//: Returns the first n characters of the string
	static std::string left(std::string string, int n);
	
	//: Returns the last n characters in the string
	static std::string right(std::string string, int n);

	//: String formatting.
	//- <p>The format specified is of the form "Some text %1, some more text %2".
	//- The %n markers can then be search/replaced using the arg() functions.</p>
	//- <pre>
	//- Example 1:
	//-   CL_String str;
	//-   str.format("%1 world! Meaning of life is: %2");
	//-   str.arg("Hello");
	//-   str.arg(42);
	//- Example 2:
	//-   CL_String str;
	//-   str.format("%1 is better than %2", "ClanLib 0.7", "ClanLib 0.6");
	//- </pre>
	//static std::string format(const std::string &format);

	template<class Param1>
	static std::string format(const std::string &str_format, const Param1 &p1)
	{
		std::string ret = str_format;
		arg(ret, p1, 1);
		
		return ret;
	}

	template<class Param1, class Param2>
	static std::string format(const std::string &str_format, const Param1 &p1, const Param2 &p2)
	{
		std::string ret = str_format;
		arg(ret, p1, 1);
		arg(ret, p2, 2);

		return ret;
	}

	template<class Param1, class Param2, class Param3>
	static std::string format(const std::string &str_format, const Param1 &p1, const Param2 &p2, const Param3 &p3)
	{
		std::string ret = str_format;
		arg(ret, p1, 1);
		arg(ret, p2, 2);
		arg(ret, p3, 3);
		return ret;
	}

	template<class Param1, class Param2, class Param3, class Param4>
	static std::string format(const std::string &str_format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4)
	{
		std::string ret = str_format;
		arg(ret, p1, 1);
		arg(ret, p2, 2);
		arg(ret, p3, 3);
		arg(ret, p4, 4);
		return ret;
	}
	
	template<class Param1, class Param2, class Param3, class Param4, class Param5>
	static std::string format(const std::string &str_format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5)
	{
		std::string ret = str_format;
		arg(ret, p1, 1);
		arg(ret, p2, 2);
		arg(ret, p3, 3);
		arg(ret, p4, 4);
		arg(ret, p5, 5);
		return ret;
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>
	static std::string format(const std::string &str_format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5, const Param6 &p6)
	{
		std::string ret = str_format;
		arg(ret, p1, 1);
		arg(ret, p2, 2);
		arg(ret, p3, 3);
		arg(ret, p4, 4);
		arg(ret, p5, 5);
		arg(ret, p6, 6);
		return ret;
	}

	template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7>
	static std::string format(const std::string &str_format, const Param1 &p1, const Param2 &p2, const Param3 &p3, const Param4 &p4, const Param5 &p5, const Param6 &p6, const Param7 &p7)
	{
		std::string ret = str_format;
		arg(ret, p1, 1);
		arg(ret, p2, 2);
		arg(ret, p3, 3);
		arg(ret, p4, 4);
		arg(ret, p5, 5);
		arg(ret, p6, 6);
		arg(ret, p7, 7);
		return ret;
	}

	//: Converts from an integer to string.
	static std::string from_int(int value);

	//: Converts from an float to string.
	static std::string from_float(float value);

	//: Converts from an double to string.
	static std::string from_double(double value);

	//: Returns "true" or "false" based on a boolean.
	static std::string from_bool(bool value);

	//: Converts a string to a boolean.
	//- <p>Accepted values: "1", "0", "yes", "no", "true", "false". All other values return false.
	//- The function is not case sensitive.</p>
	static bool to_bool(const std::string &value);

	//: Converts string to an integer.
	static int to_int(const std::string&);

	//: Converts string to a floating point number.
	static float to_float(const std::string&);
	
	//: Converts string to a double precision floating point number.
	static double to_double(const std::string&);

	//: Returns the string with all characters converted to lowercase.
	static std::string to_lower(const std::string&);
	
	//: Returns the string with all characters converted to uppercase.
	static std::string to_upper(const std::string&);

	//: Breaks up the string into tokens.
	static std::vector<std::string> tokenize(const std::string &str, const std::string &delimiter, bool skip_empty = true);

	//: Does a case insensitive compare of string to other string.
	static bool compare_nocase(const std::string&, const std::string&);

	//: Returns the path part of a pathname (path+filename). If no path part is found, "." is returned.
	static std::string get_path(const std::string &pathname);

	//: Returns the filename part of a pathname (path+filename). If no path part is found, the entire string is returned.
	static std::string get_filename(const std::string &pathname);

	//: Returns the extension path of a filename (text after the last dot). If no extension part is found, an empty string is returned.
	static std::string get_extension(const std::string &filename);

	//: Returns a string trimmed for space characters in both ends of string.
	static std::string trim_spaces(const std::string &str);

	//: Returns a string trimmed for whitespace characters in both ends of string.
	//- <p>Whitespace are space, tabs, newlines and carrage return characters.</p>
	static std::string trim_whitespace(const std::string &str);

	//: Return a string with C style escapes ({'\','n'}, {'\', 't'} etc.) replaced with the real escapes ('\n', '\t').
	//- <p>At the moment \n, \t and \\ are converted.<br>Single '\' will appear as such unless followed by one of the supported escape character.</p>
	static std::string convert_c_escapes(const std::string &str);

	//: Convert any type to a string
	//- <p>Convert any type into a string, it is required that a operator<<(std::ostream, T) is defined for that type
	template <class T>
	static std::string to(const T& any)
	{
		std::ostringstream oss;
		oss << any;
		return oss.str();
	}

	//: Convert a string into any type
	//- <p>Convert a string into the given type, it is required that a operator>>(std::ostream, T) is defined for that type	and that the string is a valid representation of that type. On error false is returned, on success true. The input string is not modified on error</p>
	template <class T>
	static bool from(const std::string& rep, T& x)
	{
		// this is necessary so that if "x" is not modified if the conversion fails
		T temp;
		std::istringstream iss(rep);
		
		iss >> temp;
  
		if (iss.fail()) {
			return false;
		} else {
			x = temp;
			return true;
		}
	}

//! Implementation:
private:
	// Private Constructor to prevent people from creating instances of CL_String
	CL_String() {;}

	// Replaces argument %(num) in format with arg
	static void arg(std::string &format, const std::string &arg, int num);

	static void arg(std::string &format, const char *arg, int num);

	static void arg(std::string &format, int number, int num);

	static void arg(std::string &format, float number, int num);

	static void arg(std::string &format, double number, int num);
};

#endif
