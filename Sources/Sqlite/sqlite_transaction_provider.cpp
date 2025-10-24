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
#include "sqlite_transaction_provider.h"
#include "sqlite_connection_provider.h"
#include "API/Database/db_command_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"
#include "sqlite3.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteTransactionProvider Construction:

CL_SqliteTransactionProvider::CL_SqliteTransactionProvider(CL_SqliteConnectionProvider *connection, const CL_DBTransaction::Type type)
: connection(connection), type(type)
{
	CL_String text;
	switch (type)
	{
	case CL_DBTransaction::deferred:
		text = "BEGIN DEFERRED TRANSACTION";
		break;
	case CL_DBTransaction::immediate:
		text = "BEGIN IMMEDIATE TRANSACTION";
		break;
	case CL_DBTransaction::exclusive:
		text = "BEGIN EXCLUSIVE TRANSACTION";
		break;
	default:
		throw CL_Exception("Unknown transaction type");
	}
	std::auto_ptr<CL_DBCommandProvider> command(connection->create_command(text, CL_DBCommand::sql_statement));
	connection->execute_non_query(command.get());
	command.reset();
	connection->active_transaction = this;
}

CL_SqliteTransactionProvider::~CL_SqliteTransactionProvider()
{
	rollback();
	if (connection)
		connection->active_transaction = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteTransactionProvider Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SqliteTransactionProvider Operations:

void CL_SqliteTransactionProvider::commit()
{
	if (connection && connection->active_transaction)
	{
		std::auto_ptr<CL_DBCommandProvider> command(connection->create_command("COMMIT TRANSACTION", CL_DBCommand::sql_statement));
		connection->execute_non_query(command.get());
		command.reset();
		connection->active_transaction = 0;
	}
}

void CL_SqliteTransactionProvider::rollback()
{
	if (connection && connection->active_transaction)
	{
		std::auto_ptr<CL_DBCommandProvider> command(connection->create_command("ROLLBACK TRANSACTION", CL_DBCommand::sql_statement));
		try
		{
			connection->execute_non_query(command.get());
		}
		catch (CL_Exception e)
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
// CL_SqliteTransactionProvider Implementation:
