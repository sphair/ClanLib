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
#include "sqlite_connection_provider.h"
#include "sqlite_command_provider.h"
#include "sqlite_reader_provider.h"
#include "sqlite_transaction_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/datetime.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include <memory>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SqliteConnectionProvider Construction:

SqliteConnectionProvider::SqliteConnectionProvider(const std::string &db_filename)
: active_transaction(0), active_reader(0), db(0)
{
	int result = sqlite3_open(StringHelp::text_to_utf8(db_filename).c_str(), &db);
	if (result != SQLITE_OK)
	{
		if (db)
			sqlite3_close(db);
		throw Exception("Unable to open database");
	}
}

SqliteConnectionProvider::~SqliteConnectionProvider()
{
	if (active_reader)
		active_reader->connection = 0;
	if (active_transaction)
		active_transaction->connection = 0;

	sqlite3_close(db);
}

/////////////////////////////////////////////////////////////////////////////
// SqliteConnectionProvider Attributes:


/////////////////////////////////////////////////////////////////////////////
// SqliteConnectionProvider Operations:

DBCommandProvider *SqliteConnectionProvider::create_command(const std::string &text, DBCommand::Type type)
{
	if (type != DBCommand::sql_statement)
		throw Exception("Sqlite database connections only support SQL statement commands");
	else
		return new SqliteCommandProvider(this, text);
}

DBTransactionProvider *SqliteConnectionProvider::begin_transaction(DBTransaction::Type type)
{
	if (active_transaction)
		throw Exception("Only one database transaction may be active for a connection");
	else
		return new SqliteTransactionProvider(this, type);
}

DBReaderProvider *SqliteConnectionProvider::execute_reader(DBCommandProvider *command)
{
	if (active_reader)
		throw Exception("Only one database reader may be active for a connection");
	else
		return new SqliteReaderProvider(this, dynamic_cast<SqliteCommandProvider*>(command));
}

std::string SqliteConnectionProvider::execute_scalar_string(DBCommandProvider *command)
{
	std::unique_ptr<DBReaderProvider> reader(execute_reader(command));
	if (!reader->retrieve_row())
		throw Exception("Database command statement returned no value");
	std::string value = reader->get_column_string(0);
	reader->close();
	return value;
}

int SqliteConnectionProvider::execute_scalar_int(DBCommandProvider *command)
{
	std::unique_ptr<DBReaderProvider> reader(execute_reader(command));
	if (!reader->retrieve_row())
		throw Exception("Database command statement returned no value");
	int value = reader->get_column_int(0);
	reader->close();
	return value;
}

void SqliteConnectionProvider::execute_non_query(DBCommandProvider *command)
{
	std::unique_ptr<DBReaderProvider> reader(execute_reader(command));
	reader->retrieve_row();
	reader->close();
}

/////////////////////////////////////////////////////////////////////////////
// SqliteConnectionProvider Implementation:

std::string SqliteConnectionProvider::to_sql_datetime(const DateTime &value)
{
	return value.to_short_datetime_string();
}

DateTime SqliteConnectionProvider::from_sql_datetime(const std::string &value)
{
	return DateTime::from_short_date_string(value);
}

std::string SqliteConnectionProvider::int_to_string(int value, int length)
{
	std::string str = StringHelp::int_to_text(value);
	return std::string(length-str.length(), L'0') + str;
}

int SqliteConnectionProvider::string_to_int(const std::string &str, int offset, int length)
{
	return StringHelp::text_to_int(str.substr(offset, length));
}

}
