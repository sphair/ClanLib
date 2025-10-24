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

/// \addtogroup clanDatabase_System clanDatabase System
/// \{

#pragma once

#include "api_database.h"
#include "db_command.h"
#include "db_transaction.h"

class CL_DBTransaction;
class CL_DBReader;
class CL_DBConnectionProvider;
class CL_DBConnection_Impl;

/// \brief Database connection.
///
/// \xmlonly !group=Database/System! !header=database.h! \endxmlonly
class CL_API_DATABASE CL_DBConnection
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a database connection.
	CL_DBConnection();

	/// \brief Constructs a DBConnection
	///
	/// \param provider = DBConnection Provider
	CL_DBConnection(CL_DBConnectionProvider *provider);

	~CL_DBConnection();
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	/// \brief Create database command.
	CL_DBCommand create_command(const CL_StringRef &text, CL_DBCommand::Type type = CL_DBCommand::sql_statement);

	/// \brief Begin a transaction.
	CL_DBTransaction begin_transaction(CL_DBTransaction::Type type = CL_DBTransaction::deferred);

	/// \brief Begin execution of database command.
	CL_DBReader execute_reader(CL_DBCommand &command);

	/// \brief Execute database command and returns the first column of the first row.
	CL_String execute_scalar_string(CL_DBCommand &command);

	/// \brief Execute database command and returns the first column of the first row.
	int execute_scalar_int(CL_DBCommand &command);

	/// \brief Execute database command.
	void execute_non_query(CL_DBCommand &command);
/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_DBConnection_Impl> impl;

/// \}
};

/// \}
