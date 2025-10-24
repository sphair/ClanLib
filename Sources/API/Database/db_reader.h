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

class CL_DateTime;
class CL_DataBuffer;
class CL_DBConnection;
class CL_DBReaderProvider;
class CL_DBReader_Impl;

/// \brief Database reader.
///
/// \xmlonly !group=Database/System! !header=database.h! \endxmlonly
class CL_API_DATABASE CL_DBReader
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a database reader
	CL_DBReader();

	/// \brief Constructs a DBReader
	///
	/// \param impl = Shared Ptr
	CL_DBReader(const CL_SharedPtr<CL_DBReader_Impl> &impl);

	~CL_DBReader();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the number of columns in the result set
	int get_column_count() const;
	
	/// \brief Returns the name of the specified column index
	CL_String get_column_name(int index) const;
	
	/// \brief Returns the index of the specified column name
	int get_name_index(const CL_StringRef &name) const;

	/// \brief Retrieves the value of the specified column as a string
	CL_String get_column_string(int index) const;
	
	/// \brief Retrieves the value of the specified column as a boolean
	bool get_column_bool(int index) const;
	
	/// \brief Retrieves the value of the specified column as an integer
	int get_column_int(int index) const;
	
	/// \brief Retrieves the value of the specified column as a double
	double get_column_double(int index) const;
	
	/// \brief Retrieves the value of the specified column as a CL_DateTime
	CL_DateTime get_column_datetime(int index) const;
	
	/// \brief Retrieves the value of the specified column as a CL_DataBuffer
	CL_DataBuffer get_column_binary(int index) const;

	/// \brief Returns the provider interface for this reader
	CL_DBReaderProvider *get_provider();
/// \}

/// \name Operations
/// \{
public:
	/// \brief Retrieves a row from the command execution result
	/// \return True if a row was retrieved, false if there are no more rows
	bool retrieve_row();
	
	/// \brief Closes the database reader
	void close();
/// \}

/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_DBReader_Impl> impl;

/// \}
};

/// \}
