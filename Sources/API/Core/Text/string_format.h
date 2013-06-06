/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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


#pragma once

#include "../api_core.h"
#include <vector>

namespace clan
{
/// \addtogroup clanCore_Text clanCore Text
/// \{

/// \brief String formatting class.
class CL_API_CORE StringFormat
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a TempStringFormat
	///
	/// \param format_string = String Ref
	StringFormat(const std::string &format_string);

	~StringFormat();

/// \}
/// \name Attributes
/// \{

public:
	const std::string &get_result() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set arg
	///
	/// \param index = value
	/// \param text = String Ref
	void set_arg(int index, const std::string &text);

	/// \brief Set arg
	///
	/// \param index = value
	/// \param value = value
	/// \param min_length = value
	void set_arg(int index, int value, int min_length = 0);

	/// \brief Set arg
	///
	/// \param index = value
	/// \param value = value
	/// \param min_length = value
	void set_arg(int index, unsigned int value, int min_length = 0);

	/// \brief Set arg
	///
	/// \param index = value
	/// \param value = value
	/// \param min_length = value
	void set_arg(int index, long unsigned int value, int min_length = 0);

	/// \brief Set arg
	///
	/// \param index = value
	/// \param value = value
	/// \param min_length = value
	void set_arg(int index, long long value, int min_length = 0);

	/// \brief Set arg
	///
	/// \param index = value
	/// \param value = value
	/// \param min_length = value
	void set_arg(int index, unsigned long long value, int min_length = 0);

	/// \brief Set arg
	///
	/// \param index = value
	/// \param value = value
	void set_arg(int index, float value);

	/// \brief Set arg
	///
	/// \param index = value
	/// \param value = value
	void set_arg(int index, double value);

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Create arg
	///
	/// \param index = value
	/// \param start = value
	/// \param length = value
	void create_arg(int index, int start, int length);

	std::string string;

	struct ArgPosition
	{
		ArgPosition() : start(0), length(-1) { }
		ArgPosition(int s, int l) : start(s), length(l) {}
		int start;
		int length;
	};

	std::vector<ArgPosition> args;
/// \}
};

inline std::string string_format(const std::string &format)
{ return format; }

template <class Arg1>
std::string string_format(const std::string &format, Arg1 arg1)
{ StringFormat f(format); f.set_arg(1, arg1); return f.get_result(); }

template <class Arg1, class Arg2>
std::string string_format(const std::string &format, Arg1 arg1, Arg2 arg2)
{ StringFormat f(format); f.set_arg(1, arg1); f.set_arg(2, arg2); return f.get_result(); }

template <class Arg1, class Arg2, class Arg3>
std::string string_format(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3)
{ StringFormat f(format); f.set_arg(1, arg1); f.set_arg(2, arg2); f.set_arg(3, arg3); return f.get_result(); }

template <class Arg1, class Arg2, class Arg3, class Arg4>
std::string string_format(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{ StringFormat f(format); f.set_arg(1, arg1); f.set_arg(2, arg2); f.set_arg(3, arg3); f.set_arg(4, arg4); return f.get_result(); }

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
std::string string_format(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
{ StringFormat f(format); f.set_arg(1, arg1); f.set_arg(2, arg2); f.set_arg(3, arg3); f.set_arg(4, arg4); f.set_arg(5, arg5); return f.get_result(); }

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
std::string string_format(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
{ StringFormat f(format); f.set_arg(1, arg1); f.set_arg(2, arg2); f.set_arg(3, arg3); f.set_arg(4, arg4); f.set_arg(5, arg5); f.set_arg(6, arg6); return f.get_result(); }

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
std::string string_format(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7)
{ StringFormat f(format); f.set_arg(1, arg1); f.set_arg(2, arg2); f.set_arg(3, arg3); f.set_arg(4, arg4); f.set_arg(5, arg5); f.set_arg(6, arg6); f.set_arg(7, arg7); return f.get_result(); }

}

/// \}
