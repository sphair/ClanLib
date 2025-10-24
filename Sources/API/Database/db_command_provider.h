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

class CL_DataBuffer;
class CL_DateTime;

/// \brief Database command provider.
///
/// \xmlonly !group=Database/System! !header=database.h! \endxmlonly
class CL_API_DATABASE CL_DBCommandProvider
{
/// \name Construction
/// \{
public:
	virtual ~CL_DBCommandProvider() { return; }
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the index for a given input parameter name
	virtual int get_input_parameter_column(const CL_StringRef &name) const = 0;

	/// \brief Returns the output parameter containing the row id of the last inserted row
	virtual int get_output_last_insert_rowid() const = 0;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Sets the specified input parameter index from a string value
	virtual void set_input_parameter_string(int index, const CL_StringRef &value) = 0;

	/// \brief Sets the specified input parameter index from a boolean value
	virtual void set_input_parameter_bool(int index, bool value) = 0;

	/// \brief Sets the specified input parameter index from an integer value
	virtual void set_input_parameter_int(int index, int value) = 0;

	/// \brief Sets the specified input parameter index from a double value
	virtual void set_input_parameter_double(int index, double value) = 0;

	/// \brief Sets the specified input parameter index from a CL_DateTime value
	virtual void set_input_parameter_datetime(int index, const CL_DateTime &value) = 0;

	/// \brief Sets the specified input parameter index from a CL_DataBuffer value
	virtual void set_input_parameter_binary(int index, const CL_DataBuffer &value) = 0;
/// \}

/// \name Implementation
/// \{
private:
/// \}
};

/// \}
