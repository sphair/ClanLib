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
**    Animehunter
*/


#pragma once

namespace clan
{

class DBValue 
{
public:
	DBValue();
	DBValue(const DBReader &db_reader, int column_index);
	DBValue(const DBReader &db_reader, const std::string &column_name);

	int to_integer() const;
	std::string to_string() const;
	bool to_boolean() const;
	double to_double() const;
	DataBuffer to_binary() const;
	DateTime to_datetime() const;

	inline operator int() const { return to_integer(); }
	inline operator std::string() const { return to_string(); }
	inline operator bool() const { return to_boolean(); }
	inline operator double() const { return to_double(); }
	inline operator DataBuffer() const { return to_binary(); }
	inline operator DateTime() const { return to_datetime(); }

private:

	enum ParamType 
	{
		cl_index, cl_name
	};

	ParamType param_type;

	int column_index;
	std::string column_name;

	DBReader db_reader;
};

}
