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

	/// \brief Create database command with 1 input argument.
	template <class Arg1>
	CL_DBCommand create_command(const CL_StringRef &format, Arg1 arg1, CL_DBCommand::Type type = CL_DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).get_result(); }

	/// \brief Create database command with 2 input arguments.
	template <class Arg1, class Arg2>
	CL_DBCommand create_command(const CL_StringRef &format, Arg1 arg1, Arg2 arg2, CL_DBCommand::Type type = CL_DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).get_result(); }

	/// \brief Create database command with 3 input arguments.
	template <class Arg1, class Arg2, class Arg3>
	CL_DBCommand create_command(const CL_StringRef &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, CL_DBCommand::Type type = CL_DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).get_result(); }

	/// \brief Create database command with 4 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4>
	CL_DBCommand create_command(const CL_StringRef &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, CL_DBCommand::Type type = CL_DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).get_result(); }

	/// \brief Create database command with 5 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	CL_DBCommand create_command(const CL_StringRef &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, CL_DBCommand::Type type = CL_DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).set_arg(arg5).get_result(); }

	/// \brief Create database command with 6 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	CL_DBCommand create_command(const CL_StringRef &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, CL_DBCommand::Type type = CL_DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).set_arg(arg5).set_arg(arg6).get_result(); }

	/// \brief Create database command with 7 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
	CL_DBCommand create_command(const CL_StringRef &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, CL_DBCommand::Type type = CL_DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).set_arg(arg5).set_arg(arg6).set_arg(arg7).get_result(); }

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
	class DBArg
	{
	public:
		DBArg(CL_DBConnection &db, const CL_StringRef &format, CL_DBCommand::Type type) : cmd(db.create_command(format, type)), i(1){}

		DBArg &set_arg(const CL_StringRef &arg)
		{
			cmd.set_input_parameter_string(i, arg);
			i++;
			return *this;
		}

		DBArg &set_arg(const char *arg)
		{
			cmd.set_input_parameter_string(i, arg);
			i++;
			return *this;
		}

		DBArg &set_arg(bool arg)
		{
			cmd.set_input_parameter_bool(i, arg);
			i++;
			return *this;
		}

		DBArg &set_arg(int arg)
		{
			cmd.set_input_parameter_int(i, arg);
			i++;
			return *this;
		}

		DBArg &set_arg(double arg)
		{
			cmd.set_input_parameter_double(i, arg);
			i++;
			return *this;
		}

		DBArg &set_arg(const CL_DateTime &arg)
		{
			cmd.set_input_parameter_datetime(i, arg);
			i++;
			return *this;
		}

		DBArg &set_arg(const CL_DataBuffer &arg)
		{
			cmd.set_input_parameter_binary(i, arg);
			i++;
			return *this;
		}

		CL_DBCommand get_result() const
		{
			return cmd;
		}

	private:
		CL_DBCommand cmd;
		int i;
	};

	DBArg begin_arg(const CL_StringRef &format, CL_DBCommand::Type type)
	{
		return DBArg(*this, format, type);
	}

	CL_SharedPtr<CL_DBConnection_Impl> impl;

/// \}
};

/// \}
