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

class CL_DateTime;
class CL_DataBuffer;

/// \brief Database reader provider.
///
/// \xmlonly !group=Database/System! !header=database.h! \endxmlonly
class CL_API_DATABASE CL_DBReaderProvider
{
/// \name Construction
/// \{
public:
	virtual ~CL_DBReaderProvider() { return; }
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the number of columns in the result set
	virtual int get_column_count() const = 0;
	
	/// \brief Returns the name of the specified column index
	virtual CL_String get_column_name(int index) const = 0;
	
	/// \brief Returns the index of the specified column name
	virtual int get_name_index(const CL_StringRef &name) const = 0;

	/// \brief Retrieves the value of the specified column as a string
	virtual CL_String get_column_string(int index) const = 0;
	
	/// \brief Retrieves the value of the specified column as a boolean
	virtual bool get_column_bool(int index) const = 0;
	
	/// \brief Retrieves the value of the specified column as an integer
	virtual int get_column_int(int index) const = 0;
	
	/// \brief Retrieves the value of the specified column as a double
	virtual double get_column_double(int index) const = 0;
	
	/// \brief Retrieves the value of the specified column as a CL_DateTime
	virtual CL_DateTime get_column_datetime(int index) const = 0;
	
	/// \brief Retrieves the value of the specified column as a CL_DataBuffer
	virtual CL_DataBuffer get_column_binary(int index) const = 0;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Retrieves a row from the command execution result
	/// \return True if a row was retrieved, false if there are no more rows
	virtual bool retrieve_row() = 0;
	
	/// \brief Closes the database reader
	virtual void close() = 0;
/// \}

/// \name Implementation
/// \{
private:
/// \}
};

/// \}
