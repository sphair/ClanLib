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

#include "API/Database/db_connection_provider.h"

namespace clan
{
/// \addtogroup clanSqlite_System clanSqlite System
/// \{

class SqliteTransactionProvider;
class SqliteReaderProvider;

/// \brief Sqlite database connection provider
class SqliteConnectionProvider : public DBConnectionProvider
{
/// \name Construction
/// \{
public:
	SqliteConnectionProvider(const std::string &db_filename);
	~SqliteConnectionProvider();
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	DBCommandProvider *create_command(const std::string &text, DBCommand::Type type);
	DBTransactionProvider *begin_transaction(DBTransaction::Type type);
	DBReaderProvider *execute_reader(DBCommandProvider *command);
	std::string execute_scalar_string(DBCommandProvider *command);
	int execute_scalar_int(DBCommandProvider *command);
	void execute_non_query(DBCommandProvider *command);
/// \}

/// \name Implementation
/// \{
private:
	static std::string to_sql_datetime(const DateTime &value);
	static DateTime from_sql_datetime(const std::string &value);
	static std::string int_to_string(int value, int length);
	static int string_to_int(const std::string &str, int offset, int length);

	SqliteTransactionProvider *active_transaction;
	SqliteReaderProvider *active_reader;
	sqlite3 *db;

	friend class SqliteReaderProvider;
	friend class SqliteTransactionProvider;
	friend class SqliteCommandProvider;
/// \}
};

}

/// \}
