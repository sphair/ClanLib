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

#include "Database/precomp.h"
#include "API/Core/System/databuffer.h"
#include "API/Database/db_connection.h"
#include "API/Database/db_connection_provider.h"
#include "API/Database/db_command.h"
#include "API/Database/db_reader.h"
#include "db_connection_impl.h"
#include "db_command_impl.h"
#include "db_transaction_impl.h"
#include "db_reader_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DBConnection Construction:

DBConnection::DBConnection()
{
}

DBConnection::DBConnection(DBConnectionProvider *provider)
: impl(new DBConnection_Impl(provider))
{
}

DBConnection::~DBConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// DBConnection Attributes:


/////////////////////////////////////////////////////////////////////////////
// DBConnection Operations:

DBCommand DBConnection::create_command(const std::string &text, DBCommand::Type type)
{
	std::shared_ptr<DBCommand_Impl> cmd_impl(new DBCommand_Impl(impl->provider->create_command(text, type)));
	return DBCommand(cmd_impl);
}

DBTransaction DBConnection::begin_transaction(DBTransaction::Type type)
{
	std::shared_ptr<DBTransaction_Impl> transaction_impl(new DBTransaction_Impl(impl->provider->begin_transaction(type)));
	return DBTransaction(transaction_impl);
}

DBReader DBConnection::execute_reader(DBCommand &command)
{
	std::shared_ptr<DBReader_Impl> reader_impl(new DBReader_Impl(impl->provider->execute_reader(command.get_provider())));
	return DBReader(reader_impl);
}

std::string DBConnection::execute_scalar_string(DBCommand &command)
{
	return impl->provider->execute_scalar_string(command.get_provider());
}

int DBConnection::execute_scalar_int(DBCommand &command)
{
	return impl->provider->execute_scalar_int(command.get_provider());
}

void DBConnection::execute_non_query(DBCommand &command)
{
	impl->provider->execute_non_query(command.get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// DBConnection Implementation:

}
