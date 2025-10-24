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
*/

#include "Sqlite/precomp.h"
#include "sqlite_connection_provider.h"
#include "sqlite_command_provider.h"
#include "sqlite_reader_provider.h"
#include "sqlite_transaction_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/datetime.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteConnectionProvider Construction:

CL_SqliteConnectionProvider::CL_SqliteConnectionProvider(const CL_StringRef &db_filename)
: active_transaction(0), active_reader(0), db(0)
{
	int result = sqlite3_open(CL_StringHelp::text_to_utf8(db_filename).c_str(), &db);
	if (result != SQLITE_OK)
	{
		if (db)
			sqlite3_close(db);
		throw CL_Exception("Unable to open database");
	}
}

CL_SqliteConnectionProvider::~CL_SqliteConnectionProvider()
{
	if (active_reader)
		active_reader->connection = 0;
	if (active_transaction)
		active_transaction->connection = 0;

	sqlite3_close(db);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteConnectionProvider Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SqliteConnectionProvider Operations:

CL_DBCommandProvider *CL_SqliteConnectionProvider::create_command(const CL_StringRef &text, CL_DBCommand::Type type)
{
	if (type != CL_DBCommand::sql_statement)
		throw CL_Exception("Sqlite database connections only support SQL statement commands");
	else
		return new CL_SqliteCommandProvider(this, text);
}

CL_DBTransactionProvider *CL_SqliteConnectionProvider::begin_transaction(CL_DBTransaction::Type type)
{
	if (active_transaction)
		throw CL_Exception("Only one database transaction may be active for a connection");
	else
		return new CL_SqliteTransactionProvider(this, type);
}

CL_DBReaderProvider *CL_SqliteConnectionProvider::execute_reader(CL_DBCommandProvider *command)
{
	if (active_reader)
		throw CL_Exception("Only one database reader may be active for a connection");
	else
		return new CL_SqliteReaderProvider(this, dynamic_cast<CL_SqliteCommandProvider*>(command));
}

CL_String CL_SqliteConnectionProvider::execute_scalar_string(CL_DBCommandProvider *command)
{
	std::auto_ptr<CL_DBReaderProvider> reader(execute_reader(command));
	if (!reader->retrieve_row())
		throw CL_Exception("Database command statement returned no value");
	CL_String value = reader->get_column_string(0);
	reader->close();
	return value;
}

int CL_SqliteConnectionProvider::execute_scalar_int(CL_DBCommandProvider *command)
{
	std::auto_ptr<CL_DBReaderProvider> reader(execute_reader(command));
	if (!reader->retrieve_row())
		throw CL_Exception("Database command statement returned no value");
	int value = reader->get_column_int(0);
	reader->close();
	return value;
}

void CL_SqliteConnectionProvider::execute_non_query(CL_DBCommandProvider *command)
{
	std::auto_ptr<CL_DBReaderProvider> reader(execute_reader(command));
	reader->retrieve_row();
	reader->close();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteConnectionProvider Implementation:

CL_String CL_SqliteConnectionProvider::to_sql_datetime(const CL_DateTime &value)
{
	return value.to_short_datetime_string();
}

CL_DateTime CL_SqliteConnectionProvider::from_sql_datetime(const CL_String &value)
{
	return CL_DateTime::from_short_date_string(value);
}

CL_String CL_SqliteConnectionProvider::int_to_string(int value, int length)
{
	CL_String str = CL_StringHelp::int_to_text(value);
	return CL_String(length-str.length(), L'0') + str;
}

int CL_SqliteConnectionProvider::string_to_int(const CL_String &str, int offset, int length)
{
	return CL_StringHelp::text_to_int(str.substr(offset, length));
}
