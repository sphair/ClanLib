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


namespace clan
{
/// \addtogroup clanDatabase_System clanDatabase System
/// \{

class DataBuffer;
class DateTime;
class DBConnection;
class DBCommandProvider;
class DBCommand_Impl;

/// \brief Database command.
class DBCommand
{
/// \name Construction
/// \{
public:
	enum Type
	{
		stored_procedure,
		sql_statement
	};

	/// \brief Constructs a database command
	DBCommand();

	/// \brief Constructs a DBCommand
	///
	/// \param impl = Shared Ptr
	DBCommand(const std::shared_ptr<DBCommand_Impl> &impl);

	~DBCommand();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the index for a given input parameter name
	int get_input_parameter_column(const std::string &name) const;

	/// \brief Returns the output parameter containing the row id of the last inserted row
	int get_output_last_insert_rowid() const;

	/// \brief Returns the provider interface for this command
	DBCommandProvider *get_provider();
/// \}

/// \name Operations
/// \{
public:
	/// \brief Sets the specified input parameter index from a string value
	void set_input_parameter_string(int index, const std::string &value);

	/// \brief Sets the specified input parameter index from a boolean value
	void set_input_parameter_bool(int index, bool value);

	/// \brief Sets the specified input parameter index from an integer value
	void set_input_parameter_int(int index, int value);

	/// \brief Sets the specified input parameter index from a double value
	void set_input_parameter_double(int index, double value);

	/// \brief Sets the specified input parameter index from a DateTime value
	void set_input_parameter_datetime(int index, const DateTime &value);

	/// \brief Sets the specified input parameter index from a DataBuffer value
	void set_input_parameter_binary(int index, const DataBuffer &value);

	/// \brief Sets the specified input parameter index from a value
	template<class ValueType>
	void set_input_parameter(int index, ValueType value);

	/// \brief Sets the specified input parameter name from a value
	template<class ValueType>
	void set_input_parameter(const std::string &name, ValueType value);

/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<DBCommand_Impl> impl;

/// \}
};

}

/// \}
