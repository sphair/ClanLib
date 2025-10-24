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
#include "API/Database/db_reader.h"
#include "API/Database/db_connection.h"
#include "API/Core/System/datetime.h"
#include "API/Core/System/databuffer.h"
#include "db_connection_impl.h"
#include "db_reader_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DBReader Construction:

CL_DBReader::CL_DBReader()
{
}

CL_DBReader::CL_DBReader(const CL_SharedPtr<CL_DBReader_Impl> &impl)
: impl(impl)
{
}

CL_DBReader::~CL_DBReader()
{
}

/////////////////////////////////////////////////////////////////////////////	
// CL_DBReader Attributes:

int CL_DBReader::get_column_count() const
{
	return impl->provider->get_column_count();
}

CL_String CL_DBReader::get_column_name(int index) const
{
	return impl->provider->get_column_name(index);
}

int CL_DBReader::get_name_index(const CL_StringRef &name) const
{
	return impl->provider->get_name_index(name);
}

CL_String CL_DBReader::get_column_string(int index) const
{
	return impl->provider->get_column_string(index);
}

bool CL_DBReader::get_column_bool(int index) const
{
	return impl->provider->get_column_bool(index);
}

int CL_DBReader::get_column_int(int index) const
{
	return impl->provider->get_column_int(index);
}

double CL_DBReader::get_column_double(int index) const
{
	return impl->provider->get_column_double(index);
}

CL_DateTime CL_DBReader::get_column_datetime(int index) const
{
	return impl->provider->get_column_datetime(index);
}

CL_DataBuffer CL_DBReader::get_column_binary(int index) const
{
	return impl->provider->get_column_binary(index);
}

CL_DBReaderProvider *CL_DBReader::get_provider()
{
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DBReader Operations:

bool CL_DBReader::retrieve_row()
{
	return impl->provider->retrieve_row();
}

void CL_DBReader::close()
{
	impl->provider->close();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DBReader Implementation:
