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
*/

#include "Sqlite/precomp.h"
#include "sqlite_reader_provider.h"
#include "sqlite_connection_provider.h"
#include "sqlite_command_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/datetime.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "sqlite3.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteReaderProvider Construction:

CL_SqliteReaderProvider::CL_SqliteReaderProvider(CL_SqliteConnectionProvider *connection, CL_SqliteCommandProvider *command)
: connection(connection), command(command), vm(command->vm), finished(false), closed(false), destroy_command(false)
{
	connection->active_reader = this;
}

CL_SqliteReaderProvider::~CL_SqliteReaderProvider()
{
	close();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteReaderProvider Attributes:

int CL_SqliteReaderProvider::get_column_count() const
{
	return sqlite3_column_count(vm);
}

CL_String CL_SqliteReaderProvider::get_column_name(int index) const
{
	return (CL_String8::char_type *) sqlite3_column_name(vm, index);
}

int CL_SqliteReaderProvider::get_name_index(const CL_StringRef &name) const
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

	throw CL_Exception(cl_format("No such column name %1", name));
}

CL_String CL_SqliteReaderProvider::get_column_string(int index) const
{
	CL_String8::char_type *str = (CL_String8::char_type *) sqlite3_column_text(vm, index);
	if (str != 0)
		return str;
	else
		return CL_String();
}

bool CL_SqliteReaderProvider::get_column_bool(int index) const
{
	return sqlite3_column_int(vm, index) != 0;
}

int CL_SqliteReaderProvider::get_column_int(int index) const
{
	return sqlite3_column_int(vm, index);
}

double CL_SqliteReaderProvider::get_column_double(int index) const
{
	return sqlite3_column_double(vm, index);
}

CL_DateTime CL_SqliteReaderProvider::get_column_datetime(int index) const
{
	return CL_SqliteConnectionProvider::from_sql_datetime(get_column_string(index));
}

CL_DataBuffer CL_SqliteReaderProvider::get_column_binary(int index) const
{
	const void *blob = sqlite3_column_blob(vm, index);
	int size = sqlite3_column_bytes(vm, index);
	return CL_DataBuffer(blob, size);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteReaderProvider Operations:

bool CL_SqliteReaderProvider::retrieve_row()
{
	if (!finished)
	{
		for (int i=0; i<1000; i++)
		{
			int result = sqlite3_step(vm);
			switch (result)
			{
			case SQLITE_BUSY:
				CL_System::sleep(1);
				break;
			case SQLITE_ROW:
				return true;
			case SQLITE_DONE:
				if (command)
					command->last_insert_rowid = sqlite3_last_insert_rowid(connection->db);
				finished = true;
				return false;
			case SQLITE_ERROR:
				finished = true;
				throw CL_Exception("Database Error!");
			case SQLITE_MISUSE:
				finished = true;
				throw CL_Exception("Database Misuse!");
			default:
				finished = true;
				throw CL_Exception(cl_format("Unknown database result code: %1", result));
			}
		}
		throw CL_Exception("Database Busy!");
	}
	else
	{
		return false;
	}
}

void CL_SqliteReaderProvider::close()
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
				CL_StringRef8 error = (err == 0) ? "Unknown error" : err;
				throw CL_Exception(CL_StringHelp::local8_to_text(error));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteReaderProvider Implementation:
