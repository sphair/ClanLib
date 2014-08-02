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

#include "db_command.h"
#include "db_transaction.h"

namespace clan
{
/// \addtogroup clanDatabase_System clanDatabase System
/// \{

class DBTransaction;
class DBReader;
class DBConnectionProvider;
class DBConnection_Impl;

/// \brief Database connection.
class DBConnection
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a database connection.
	DBConnection();

	/// \brief Constructs a DBConnection
	///
	/// \param provider = DBConnection Provider
	DBConnection(DBConnectionProvider *provider);

	~DBConnection();
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	/// \brief Create database command.
	DBCommand create_command(const std::string &text, DBCommand::Type type = DBCommand::sql_statement);

	/// \brief Create database command with 1 input argument.
	template <class Arg1>
	DBCommand create_command(const std::string &format, Arg1 arg1, DBCommand::Type type = DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).get_result(); }

	/// \brief Create database command with 2 input arguments.
	template <class Arg1, class Arg2>
	DBCommand create_command(const std::string &format, Arg1 arg1, Arg2 arg2, DBCommand::Type type = DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).get_result(); }

	/// \brief Create database command with 3 input arguments.
	template <class Arg1, class Arg2, class Arg3>
	DBCommand create_command(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, DBCommand::Type type = DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).get_result(); }

	/// \brief Create database command with 4 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4>
	DBCommand create_command(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, DBCommand::Type type = DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).get_result(); }

	/// \brief Create database command with 5 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	DBCommand create_command(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, DBCommand::Type type = DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).set_arg(arg5).get_result(); }

	/// \brief Create database command with 6 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	DBCommand create_command(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, DBCommand::Type type = DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).set_arg(arg5).set_arg(arg6).get_result(); }

	/// \brief Create database command with 7 input arguments.
	template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
	DBCommand create_command(const std::string &format, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, DBCommand::Type type = DBCommand::sql_statement)
	{ return begin_arg(format, type).set_arg(arg1).set_arg(arg2).set_arg(arg3).set_arg(arg4).set_arg(arg5).set_arg(arg6).set_arg(arg7).get_result(); }

	/// \brief Begin a transaction.
	DBTransaction begin_transaction(DBTransaction::Type type = DBTransaction::deferred);

	/// \brief Begin execution of database command.
	DBReader execute_reader(DBCommand &command);

	/// \brief Execute database command and returns the first column of the first row.
	std::string execute_scalar_string(DBCommand &command);

	/// \brief Execute database command and returns the first column of the first row.
	int execute_scalar_int(DBCommand &command);

	/// \brief Execute database command.
	void execute_non_query(DBCommand &command);
/// \}

/// \name Implementation
/// \{
private:
	class DBArg
	{
	public:
		DBArg(DBConnection &db, const std::string &format, DBCommand::Type type) : cmd(db.create_command(format, type)), i(1){}

		DBArg &set_arg(const std::string &arg)
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

		DBArg &set_arg(const DateTime &arg)
		{
			cmd.set_input_parameter_datetime(i, arg);
			i++;
			return *this;
		}

		DBArg &set_arg(const DataBuffer &arg)
		{
			cmd.set_input_parameter_binary(i, arg);
			i++;
			return *this;
		}

		DBCommand get_result() const
		{
			return cmd;
		}

	private:
		DBCommand cmd;
		int i;
	};

	DBArg begin_arg(const std::string &format, DBCommand::Type type)
	{
		return DBArg(*this, format, type);
	}

	std::shared_ptr<DBConnection_Impl> impl;

/// \}
};

}

/// \}
