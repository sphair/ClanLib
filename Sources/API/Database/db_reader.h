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

class DBValue;
class DateTime;
class DataBuffer;
class DBConnection;
class DBReaderProvider;
class DBReader_Impl;

/// \brief Database reader.
class DBReader
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a database reader
	DBReader();

	/// \brief Constructs a DBReader
	///
	/// \param impl = Shared Ptr
	DBReader(const std::shared_ptr<DBReader_Impl> &impl);

	~DBReader();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the number of columns in the result set
	int get_column_count() const;
	
	/// \brief Returns the name of the specified column index
	std::string get_column_name(int index) const;
	
	/// \brief Returns the index of the specified column name
	int get_name_index(const std::string &name) const;

	/// \brief Retrieves the value of the specified column
	DBValue get_column_value(int index) const;

	/// \brief Retrieves the value of the specified column name
	DBValue get_column_value(const std::string &name) const;

	/// \brief Retrieves the value of the specified column as a string
	std::string get_column_string(int index) const;
	
	/// \brief Retrieves the value of the specified column as a boolean
	bool get_column_bool(int index) const;
	
	/// \brief Retrieves the value of the specified column as a boolean
	char get_column_char(int index) const;
	
	/// \brief Retrieves the value of the specified column as a boolean
	unsigned char get_column_uchar(int index) const;
	
	/// \brief Retrieves the value of the specified column as an integer
	int get_column_int(int index) const;
	
	/// \brief Retrieves the value of the specified column as an integer
	unsigned int get_column_uint(int index) const;
	
	/// \brief Retrieves the value of the specified column as a double
	double get_column_double(int index) const;
	
	/// \brief Retrieves the value of the specified column as a DateTime
	DateTime get_column_datetime(int index) const;
	
	/// \brief Retrieves the value of the specified column as a DataBuffer
	DataBuffer get_column_binary(int index) const;

	/// \brief Retrieves the value of the specified column as a string
	std::string get_column_string(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as a boolean
	bool get_column_bool(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as a boolean
	char get_column_char(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as a boolean
	unsigned char get_column_uchar(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as an integer
	int get_column_int(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as an integer
	unsigned int get_column_uint(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as a double
	double get_column_double(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as a DateTime
	DateTime get_column_datetime(const std::string &column_name) const;

	/// \brief Retrieves the value of the specified column as a DataBuffer
	DataBuffer get_column_binary(const std::string &column_name) const;

	/// \brief Returns the provider interface for this reader
	DBReaderProvider *get_provider();
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
	std::shared_ptr<DBReader_Impl> impl;

/// \}
};

}

/// \}
