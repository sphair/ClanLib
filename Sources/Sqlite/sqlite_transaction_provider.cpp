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

#include "Sqlite/precomp.h"
#include "sqlite_transaction_provider.h"
#include "sqlite_connection_provider.h"
#include "API/Database/db_command_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"
#include <memory>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SqliteTransactionProvider Construction:

SqliteTransactionProvider::SqliteTransactionProvider(SqliteConnectionProvider *connection, const DBTransaction::Type type)
: connection(connection), type(type)
{
	std::string text;
	switch (type)
	{
	case DBTransaction::deferred:
		text = "BEGIN DEFERRED TRANSACTION";
		break;
	case DBTransaction::immediate:
		text = "BEGIN IMMEDIATE TRANSACTION";
		break;
	case DBTransaction::exclusive:
		text = "BEGIN EXCLUSIVE TRANSACTION";
		break;
	default:
		throw Exception("Unknown transaction type");
	}
	std::unique_ptr<DBCommandProvider> command(connection->create_command(text, DBCommand::sql_statement));
	connection->execute_non_query(command.get());
	command.reset();
	connection->active_transaction = this;
}

SqliteTransactionProvider::~SqliteTransactionProvider()
{
	rollback();
	if (connection)
		connection->active_transaction = 0;
}

/////////////////////////////////////////////////////////////////////////////
// SqliteTransactionProvider Attributes:


/////////////////////////////////////////////////////////////////////////////
// SqliteTransactionProvider Operations:

void SqliteTransactionProvider::commit()
{
	if (connection && connection->active_transaction)
	{
		std::unique_ptr<DBCommandProvider> command(connection->create_command("COMMIT TRANSACTION", DBCommand::sql_statement));
		connection->execute_non_query(command.get());
		command.reset();
		connection->active_transaction = 0;
	}
}

void SqliteTransactionProvider::rollback()
{
	if (connection && connection->active_transaction)
	{
		std::unique_ptr<DBCommandProvider> command(connection->create_command("ROLLBACK TRANSACTION", DBCommand::sql_statement));
		try
		{
			connection->execute_non_query(command.get());
		}
		catch (const Exception &e)
		{
			// In some situations a rollback will fail. From the sqlite documentation:
			//
			// "It is recommended that applications respond to the errors listed above
			// by explicitly issuing a ROLLBACK command. If the transaction has already
			// been rolled back automatically by the error response, then the ROLLBACK
			// command will fail with an error, but no harm is caused by this."
		}
		command.reset();
		connection->active_transaction = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// SqliteTransactionProvider Implementation:

}
