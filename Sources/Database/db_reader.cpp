/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Database/db_reader.h"
#include "API/Database/db_connection.h"
#include "API/Core/System/datetime.h"
#include "API/Core/System/databuffer.h"
#include "API/Database/db_value.h"
#include "db_connection_impl.h"
#include "db_reader_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DBReader Construction:

DBReader::DBReader()
{
}

DBReader::DBReader(const std::shared_ptr<DBReader_Impl> &impl)
: impl(impl)
{
}

DBReader::~DBReader()
{
}

/////////////////////////////////////////////////////////////////////////////	
// DBReader Attributes:

int DBReader::get_column_count() const
{
	return impl->provider->get_column_count();
}

std::string DBReader::get_column_name(int index) const
{
	return impl->provider->get_column_name(index);
}

int DBReader::get_name_index(const std::string &name) const
{
	return impl->provider->get_name_index(name);
}

DBValue DBReader::get_column_value(int index) const
{
	return DBValue(*this, index);
}

DBValue DBReader::get_column_value(const std::string &name) const
{
	return DBValue(*this, name);
}

std::string DBReader::get_column_string(int index) const
{
	return impl->provider->get_column_string(index);
}

bool DBReader::get_column_bool(int index) const
{
	return impl->provider->get_column_bool(index);
}

char DBReader::get_column_char(int index) const
{
	return impl->provider->get_column_char(index);
}

unsigned char DBReader::get_column_uchar(int index) const
{
	return impl->provider->get_column_uchar(index);
}

int DBReader::get_column_int(int index) const
{
	return impl->provider->get_column_int(index);
}

unsigned int DBReader::get_column_uint(int index) const
{
	return impl->provider->get_column_uint(index);
}

double DBReader::get_column_double(int index) const
{
	return impl->provider->get_column_double(index);
}

DateTime DBReader::get_column_datetime(int index) const
{
	return impl->provider->get_column_datetime(index);
}

DataBuffer DBReader::get_column_binary(int index) const
{
	return impl->provider->get_column_binary(index);
}

std::string DBReader::get_column_string(const std::string &column_name) const
{
	return impl->provider->get_column_string(get_name_index(column_name));
}

bool DBReader::get_column_bool(const std::string &column_name) const
{
	return impl->provider->get_column_bool(get_name_index(column_name));
}

char DBReader::get_column_char(const std::string &column_name) const
{
	return impl->provider->get_column_char(get_name_index(column_name));
}

unsigned char DBReader::get_column_uchar(const std::string &column_name) const
{
	return impl->provider->get_column_uchar(get_name_index(column_name));
}

int DBReader::get_column_int(const std::string &column_name) const
{
	return impl->provider->get_column_int(get_name_index(column_name));
}

unsigned int DBReader::get_column_uint(const std::string &column_name) const
{
	return impl->provider->get_column_uint(get_name_index(column_name));
}

double DBReader::get_column_double(const std::string &column_name) const
{
	return impl->provider->get_column_double(get_name_index(column_name));
}

DateTime DBReader::get_column_datetime(const std::string &column_name) const
{
	return impl->provider->get_column_datetime(get_name_index(column_name));
}

DataBuffer DBReader::get_column_binary(const std::string &column_name) const
{
	return impl->provider->get_column_binary(get_name_index(column_name));
}

DBReaderProvider *DBReader::get_provider()
{
	return impl->provider;
}


/////////////////////////////////////////////////////////////////////////////
// DBReader Operations:

bool DBReader::retrieve_row()
{
	return impl->provider->retrieve_row();
}

void DBReader::close()
{
	impl->provider->close();
}

/////////////////////////////////////////////////////////////////////////////
// DBReader Implementation:

}
