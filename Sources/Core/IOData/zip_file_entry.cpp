/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/IOData/zip_file_entry.h"
#include "zip_file_entry_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileEntry construction:

CL_Zip_FileEntry::CL_Zip_FileEntry() : impl(new CL_Zip_FileEntry_Generic)
{
	impl->type = CL_Zip_FileEntry_Generic::type_file;
}
	
CL_Zip_FileEntry::CL_Zip_FileEntry(const CL_Zip_FileEntry &copy) : impl(new CL_Zip_FileEntry_Generic)
{
	*impl = *copy.impl;
}
	
CL_Zip_FileEntry::~CL_Zip_FileEntry()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileEntry attributes:

const std::string &CL_Zip_FileEntry::get_filename() const
{
	return impl->record.filename;
}

const std::string &CL_Zip_FileEntry::get_local_filename() const
{
	return impl->record.local_filename;
}
	
cl_int64 CL_Zip_FileEntry::get_uncompressed_size()
{
	return impl->record.uncompressed_size;
}
	
cl_int64 CL_Zip_FileEntry::get_compressed_size()
{
	return impl->record.compressed_size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileEntry operations:

CL_Zip_FileEntry &CL_Zip_FileEntry::operator =(const CL_Zip_FileEntry &copy)
{
	*impl = *copy.impl;
	return *this;
}
	 
void CL_Zip_FileEntry::set_filename(const std::string &filename)
{
	impl->record.file_name_length = filename.length();
	impl->record.filename = filename;
}

void CL_Zip_FileEntry::set_local_filename( const std::string &local_filename )
{
	impl->record.local_filename = local_filename;
}


/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileEntry implementation:
