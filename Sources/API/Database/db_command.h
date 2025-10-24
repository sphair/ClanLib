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

class CL_DataBuffer;
class CL_DateTime;
class CL_DBConnection;
class CL_DBCommandProvider;
class CL_DBCommand_Impl;

/// \brief Database command.
///
/// \xmlonly !group=Database/System! !header=database.h! \endxmlonly
class CL_API_DATABASE CL_DBCommand
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
	CL_DBCommand();

	/// \brief Constructs a DBCommand
	///
	/// \param impl = Shared Ptr
	CL_DBCommand(const CL_SharedPtr<CL_DBCommand_Impl> &impl);

	~CL_DBCommand();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the index for a given input parameter name
	int get_input_parameter_column(const CL_StringRef &name) const;

	/// \brief Returns the output parameter containing the row id of the last inserted row
	int get_output_last_insert_rowid() const;

	/// \brief Returns the provider interface for this command
	CL_DBCommandProvider *get_provider();
/// \}

/// \name Operations
/// \{
public:
	/// \brief Sets the specified input parameter index from a string value
	void set_input_parameter_string(int index, const CL_StringRef &value);

	/// \brief Sets the specified input parameter index from a boolean value
	void set_input_parameter_bool(int index, bool value);

	/// \brief Sets the specified input parameter index from an integer value
	void set_input_parameter_int(int index, int value);

	/// \brief Sets the specified input parameter index from a double value
	void set_input_parameter_double(int index, double value);

	/// \brief Sets the specified input parameter index from a CL_DateTime value
	void set_input_parameter_datetime(int index, const CL_DateTime &value);

	/// \brief Sets the specified input parameter index from a CL_DataBuffer value
	void set_input_parameter_binary(int index, const CL_DataBuffer &value);
/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_DBCommand_Impl> impl;

/// \}
};

/// \}
