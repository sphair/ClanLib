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

/// \addtogroup clanDatabase_System clanDatabase System
/// \{

#pragma once

#include "api_database.h"
#include "db_command.h"
#include "db_transaction.h"

class CL_DBCommandProvider;
class CL_DBTransactionProvider;
class CL_DBReaderProvider;

/// \brief Database connection provider.
///
/// \xmlonly !group=Database/System! !header=database.h! \endxmlonly
class CL_API_DATABASE CL_DBConnectionProvider
{
/// \name Construction
/// \{
public:
	virtual ~CL_DBConnectionProvider() { return; }
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	/// \brief Create database command.
	virtual CL_DBCommandProvider *create_command(const CL_StringRef &text, CL_DBCommand::Type type) = 0;

	/// \brief Begin a transaction.
	virtual CL_DBTransactionProvider *begin_transaction(CL_DBTransaction::Type type) = 0;

	/// \brief Begin execution of database command.
	virtual CL_DBReaderProvider *execute_reader(CL_DBCommandProvider *command) = 0;

	/// \brief Execute database command and returns the first column of the first row.
	virtual CL_String execute_scalar_string(CL_DBCommandProvider *command) = 0;

	/// \brief Execute database command and returns the first column of the first row.
	virtual int execute_scalar_int(CL_DBCommandProvider *command) = 0;

	/// \brief Execute database command.
	virtual void execute_non_query(CL_DBCommandProvider *command) = 0;
/// \}

/// \name Implementation
/// \{
private:
/// \}
};

/// \}
