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

#include "Database/precomp.h"
#include "API/Database/db_reader.h"
#include "API/Database/db_connection.h"
#include "API/Core/System/datetime.h"
#include "API/Core/System/databuffer.h"
#include "API/Database/db_value.h"
#include "db_connection_impl.h"
#include "db_reader_impl.h"

namespace clan
{

DBValue::DBValue()
{

}

DBValue::DBValue(const DBReader &db_reader, int column_index)
	: db_reader(db_reader), column_index(column_index), param_type(cl_index)
{
	
}

DBValue::DBValue(const DBReader &db_reader, const std::string &column_name)
	: db_reader(db_reader), column_name(column_name), param_type(cl_name)
{

}


int DBValue::to_integer() const
{
	if (param_type == cl_index)
	{
		return db_reader.get_column_int(column_index);
	}
	else if (param_type == cl_name)
	{
		return db_reader.get_column_int(column_name);
	}
	else
		throw Exception("Invalid column type!");
}

std::string DBValue::to_string() const
{
	if (param_type == cl_index)
	{
		return db_reader.get_column_string(column_index);
	}
	else if (param_type == cl_name)
	{
		return db_reader.get_column_string(column_name);
	}
	else
		throw Exception("Invalid column type!");
}

bool DBValue::to_boolean() const
{
	if (param_type == cl_index)
	{
		return db_reader.get_column_bool(column_index);
	}
	else if (param_type == cl_name)
	{
		return db_reader.get_column_bool(column_name);
	}
	else
		throw Exception("Invalid column type!");
}

double DBValue::to_double() const
{
	if (param_type == cl_index)
	{
		return db_reader.get_column_double(column_index);
	}
	else if (param_type == cl_name)
	{
		return db_reader.get_column_double(column_name);
	}
	else
		throw Exception("Invalid column type!");
}

DataBuffer DBValue::to_binary() const
{
	if (param_type == cl_index)
	{
		return db_reader.get_column_binary(column_index);
	}
	else if (param_type == cl_name)
	{
		return db_reader.get_column_binary(column_name);
	}
	else
		throw Exception("Invalid column type!");
}

DateTime DBValue::to_datetime() const
{
	if (param_type == cl_index)
	{
		return db_reader.get_column_datetime(column_index);
	}
	else if (param_type == cl_name)
	{
		return db_reader.get_column_datetime(column_name);
	}
	else
		throw Exception("Invalid column type!");
}

}
