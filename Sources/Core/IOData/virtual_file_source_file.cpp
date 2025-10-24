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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "virtual_file_source_file.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_directory_listing_entry.h"

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSource_File Construction:

CL_VirtualFileSource_File::CL_VirtualFileSource_File(const CL_String &path)
: path(CL_PathHelp::add_trailing_slash(path, CL_PathHelp::path_type_file))
{
}

CL_VirtualFileSource_File::~CL_VirtualFileSource_File()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSource_File Attributes:

CL_String CL_VirtualFileSource_File::get_path() const
{
	return path;
}

CL_String CL_VirtualFileSource_File::get_identifier() const
{
	return path;
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSource_File Operations:

CL_IODevice CL_VirtualFileSource_File::open_file(const CL_String &filename,
	CL_File::OpenMode mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
{
	return CL_File(path + filename, mode, access, share, flags);
}

bool CL_VirtualFileSource_File::initialize_directory_listing(const CL_String &path)
{
	return dir_scanner.scan(path);
}

bool CL_VirtualFileSource_File::next_file(CL_VirtualDirectoryListingEntry &entry)
{
	bool next = dir_scanner.next();

	if( next )
	{
		entry.set_directory(dir_scanner.is_directory());
		entry.set_filename(dir_scanner.get_name());
		entry.set_hidden(dir_scanner.is_hidden());
		entry.set_readable(dir_scanner.is_readable());
		entry.set_writable(dir_scanner.is_writable());
	}

	return next;
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSource_File Implementation:
