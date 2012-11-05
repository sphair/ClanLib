/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "Sqlite/precomp.h"
#include "sqlite_reader_provider.h"
#include "sqlite_connection_provider.h"
#include "sqlite_command_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/datetime.h"
#include "API/Core/System/system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SqliteReaderProvider Construction:

SqliteReaderProvider::SqliteReaderProvider(SqliteConnectionProvider *connection, SqliteCommandProvider *command)
: connection(connection), command(command), vm(command->vm), finished(false), closed(false), destroy_command(false)
{
	connection->active_reader = this;
}

SqliteReaderProvider::~SqliteReaderProvider()
{
	close();
}

/////////////////////////////////////////////////////////////////////////////
// SqliteReaderProvider Attributes:

int SqliteReaderProvider::get_column_count() const
{
	return sqlite3_column_count(vm);
}

std::string SqliteReaderProvider::get_column_name(int index) const
{
	return (std::string::value_type *) sqlite3_column_name(vm, index);
}

int SqliteReaderProvider::get_name_index(const std::string &name) const
{
	int count = get_column_count();
	for (int index = 0; index < count; index++)
	{
		const char *col_name = sqlite3_column_name(vm, index);
		if (col_name != 0 && col_name == name)
		{
			return index;
		}
	}

	throw Exception(string_format("No such column name %1", name));
}

std::string SqliteReaderProvider::get_column_string(int index) const
{
	std::string::value_type *str = (std::string::value_type *) sqlite3_column_text(vm, index);
	if (str != 0)
		return str;
	else
		return std::string();
}

bool SqliteReaderProvider::get_column_bool(int index) const
{
	return sqlite3_column_int(vm, index) != 0;
}

int SqliteReaderProvider::get_column_int(int index) const
{
	return sqlite3_column_int(vm, index);
}

double SqliteReaderProvider::get_column_double(int index) const
{
	return sqlite3_column_double(vm, index);
}

DateTime SqliteReaderProvider::get_column_datetime(int index) const
{
	return SqliteConnectionProvider::from_sql_datetime(get_column_string(index));
}

DataBuffer SqliteReaderProvider::get_column_binary(int index) const
{
	const void *blob = sqlite3_column_blob(vm, index);
	int size = sqlite3_column_bytes(vm, index);
	return DataBuffer(blob, size);
}

/////////////////////////////////////////////////////////////////////////////
// SqliteReaderProvider Operations:

bool SqliteReaderProvider::retrieve_row()
{
	if (!finished)
	{
		for (int i=0; i<1000; i++)
		{
			int result = sqlite3_step(vm);
			switch (result)
			{
			case SQLITE_BUSY:
				System::sleep(1);
				break;
			case SQLITE_ROW:
				return true;
			case SQLITE_DONE:
				if (command)
					command->last_insert_rowid = sqlite3_last_insert_rowid(connection->db);
				finished = true;
				return false;
			case SQLITE_ERROR:
				{
					finished = true;
					sqlite3_reset(vm);
					const char *err = sqlite3_errmsg(connection->db);
					std::string error = (err == 0) ? "Unknown database error!" : err;
					throw Exception(StringHelp::local8_to_text(error));
				}
			case SQLITE_MISUSE:
				finished = true;
				throw Exception("Database Misuse!");
			default:
				finished = true;
				throw Exception(string_format("Unknown database result code: %1", result));
			}
		}
		throw Exception("Database Busy!");
	}
	else
	{
		return false;
	}
}

void SqliteReaderProvider::close()
{
	if (!closed)
	{
		closed = true;
		if (!finished)
			retrieve_row();
		sqlite3_reset(vm);

		if (connection)
			connection->active_reader = 0;

		if (destroy_command)
		{
			int result = sqlite3_finalize(vm);
			if (result != SQLITE_OK)
			{
				const char *err = sqlite3_errmsg(connection->db);
				std::string error = (err == 0) ? "Unknown error" : err;
				throw Exception(StringHelp::local8_to_text(error));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// SqliteReaderProvider Implementation:

}
