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

#include "Database/precomp.h"
#include "API/Database/db_command.h"
#include "API/Database/db_connection.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/logger.h"
#include "db_connection_impl.h"
#include "db_command_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DBCommand Construction:

CL_DBCommand::CL_DBCommand()
{
}

CL_DBCommand::CL_DBCommand(const CL_SharedPtr<CL_DBCommand_Impl> &impl)
: impl(impl)
{
}

CL_DBCommand::~CL_DBCommand()
{
}

/////////////////////////////////////////////////////////////////////////////	
// CL_DBCommand Attributes:

int CL_DBCommand::get_input_parameter_column(const CL_StringRef &name) const
{
	return impl->provider->get_input_parameter_column(name);
}

int CL_DBCommand::get_output_last_insert_rowid() const
{
	return impl->provider->get_output_last_insert_rowid();
}

CL_DBCommandProvider *CL_DBCommand::get_provider()
{
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DBCommand Operations:

void CL_DBCommand::set_input_parameter_string(int index, const CL_StringRef &value)
{
	impl->provider->set_input_parameter_string(index, value);
}

void CL_DBCommand::set_input_parameter_bool(int index, bool value)
{
	impl->provider->set_input_parameter_bool(index, value);
}

void CL_DBCommand::set_input_parameter_int(int index, int value)
{
	impl->provider->set_input_parameter_int(index, value);
}

void CL_DBCommand::set_input_parameter_double(int index, double value)
{
	impl->provider->set_input_parameter_double(index, value);
}

void CL_DBCommand::set_input_parameter_datetime(int index, const CL_DateTime &value)
{
	impl->provider->set_input_parameter_datetime(index, value);
}

void CL_DBCommand::set_input_parameter_binary(int index, const CL_DataBuffer &value)
{
	impl->provider->set_input_parameter_binary(index, value);
}

template<>
void CL_DBCommand::set_input_parameter<int>(int index, int value)
{
	set_input_parameter_int(index, value);
}

template<>
void CL_DBCommand::set_input_parameter<const CL_String &>(int index, const CL_String &value)
{
	set_input_parameter_string(index, value);
}

template<>
void CL_DBCommand::set_input_parameter<bool>(int index, bool value)
{
	set_input_parameter_bool(index, value);
}

template<>
void CL_DBCommand::set_input_parameter<double>(int index, double value)
{
	set_input_parameter_double(index, value);
}

template<>
void CL_DBCommand::set_input_parameter<const CL_DateTime &>(int index, const CL_DateTime &value)
{
	set_input_parameter_datetime(index, value);
}

template<>
void CL_DBCommand::set_input_parameter<const CL_DataBuffer &>(int index,  const CL_DataBuffer &value)
{
	set_input_parameter_binary(index, value);
}

template<>
void CL_DBCommand::set_input_parameter<int>(const CL_StringRef &name, int value)
{
	set_input_parameter_int(get_input_parameter_column(name), value);
}

template<>
void CL_DBCommand::set_input_parameter<const CL_String &>(const CL_StringRef &name, const CL_String &value)
{
	set_input_parameter_string(get_input_parameter_column(name), value);
}

template<>
void CL_DBCommand::set_input_parameter<bool>(const CL_StringRef &name, bool value)
{
	set_input_parameter_bool(get_input_parameter_column(name), value);
}

template<>
void CL_DBCommand::set_input_parameter<double>(const CL_StringRef &name, double value)
{
	set_input_parameter_double(get_input_parameter_column(name), value);
}

template<>
void CL_DBCommand::set_input_parameter<const CL_DateTime &>(const CL_StringRef &name, const CL_DateTime &value)
{
	set_input_parameter_datetime(get_input_parameter_column(name), value);
}

template<>
void CL_DBCommand::set_input_parameter<const CL_DataBuffer &>(const CL_StringRef &name,  const CL_DataBuffer &value)
{
	set_input_parameter_binary(get_input_parameter_column(name), value);
}


/////////////////////////////////////////////////////////////////////////////
// CL_DBCommand Implementation:
