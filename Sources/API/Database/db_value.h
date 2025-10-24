/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_DBValue 
{

public:

	CL_DBValue();
	CL_DBValue(const CL_DBReader &db_reader, int column_index);
	CL_DBValue(const CL_DBReader &db_reader, const CL_StringRef &column_name);

	int to_integer() const;
	CL_String to_string() const;
	bool to_boolean() const;
	double to_double() const;
	CL_DataBuffer to_binary() const;
	CL_DateTime to_datetime() const;

	inline operator int() const { return to_integer(); }
	inline operator CL_String() const { return to_string(); }
	inline operator bool() const { return to_boolean(); }
	inline operator double() const { return to_double(); }
	inline operator CL_DataBuffer() const { return to_binary(); }
	inline operator CL_DateTime() const { return to_datetime(); }

private:

	enum ParamType 
	{
		cl_index, cl_name
	};

	ParamType param_type;

	int column_index;
	CL_String column_name;

	CL_DBReader db_reader;
};

