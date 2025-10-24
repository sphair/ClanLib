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

/////////////////////////////////////////////////////////////////////////////
// CL_DBConnection Construction:

CL_DBConnection::CL_DBConnection()
{
}

CL_DBConnection::CL_DBConnection(CL_DBConnectionProvider *provider)
: impl(new CL_DBConnection_Impl(provider))
{
}

CL_DBConnection::~CL_DBConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DBConnection Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_DBConnection Operations:

CL_DBCommand CL_DBConnection::create_command(const CL_StringRef &text, CL_DBCommand::Type type)
{
	CL_SharedPtr<CL_DBCommand_Impl> cmd_impl(new CL_DBCommand_Impl(impl->provider->create_command(text, type)));
	return CL_DBCommand(cmd_impl);
}

CL_DBTransaction CL_DBConnection::begin_transaction(CL_DBTransaction::Type type)
{
	CL_SharedPtr<CL_DBTransaction_Impl> transaction_impl(new CL_DBTransaction_Impl(impl->provider->begin_transaction(type)));
	return CL_DBTransaction(transaction_impl);
}

CL_DBReader CL_DBConnection::execute_reader(CL_DBCommand &command)
{
	CL_SharedPtr<CL_DBReader_Impl> reader_impl(new CL_DBReader_Impl(impl->provider->execute_reader(command.get_provider())));
	return CL_DBReader(reader_impl);
}

CL_String CL_DBConnection::execute_scalar_string(CL_DBCommand &command)
{
	return impl->provider->execute_scalar_string(command.get_provider());
}

int CL_DBConnection::execute_scalar_int(CL_DBCommand &command)
{
	return impl->provider->execute_scalar_int(command.get_provider());
}

void CL_DBConnection::execute_non_query(CL_DBCommand &command)
{
	impl->provider->execute_non_query(command.get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// CL_DBConnection Implementation:
