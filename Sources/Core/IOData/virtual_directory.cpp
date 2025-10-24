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
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_file_source.h"
#include "API/Core/IOData/virtual_directory_listing.h"
#include "API/Core/System/weakptr.h"

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectory_Impl Class:

class CL_VirtualDirectory_Impl
{
//! Construction:
public:
	CL_VirtualDirectory_Impl() : file_system(CL_VirtualFileSystem::CL_NullVFS()) { return; }

//! Attributes:
public:
	CL_VirtualFileSystem file_system;

	CL_String base_path;
};

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectory Construction:

CL_VirtualDirectory::CL_VirtualDirectory()
: impl(new CL_VirtualDirectory_Impl)
{
#ifdef WIN32
	TCHAR buffer[MAX_PATH];
	buffer[0] = 0;
	if (GetCurrentDirectory(MAX_PATH, buffer) > 0)
		impl->base_path = buffer;
#else
	char buffer[1024];
	buffer[0] = 0;
	char *result = getcwd(buffer, 1024);
	if (result)
		impl->base_path = buffer;
#endif
}

CL_VirtualDirectory::CL_VirtualDirectory(
	const CL_VirtualFileSystem &file_system,
	const CL_String &base_path)
: impl(new CL_VirtualDirectory_Impl)
{
	impl->file_system = file_system;
	if (impl->file_system.is_null())
		impl->base_path = CL_PathHelp::add_trailing_slash(base_path, CL_PathHelp::path_type_file);
	else
		impl->base_path = CL_PathHelp::add_trailing_slash(base_path, CL_PathHelp::path_type_virtual);
}

CL_VirtualDirectory::~CL_VirtualDirectory()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectory Attributes:

CL_VirtualFileSystem CL_VirtualDirectory::get_file_system()
{
	return impl->file_system;
}

const CL_String &CL_VirtualDirectory::get_path() const
{
	return impl->base_path;
}

CL_VirtualDirectoryListing CL_VirtualDirectory::get_directory_listing()
{
	if (impl->file_system.is_null())
		throw CL_Exception("CL_VirtualDirectory::get_directory_listing only supported for virtual file systems");

	return CL_VirtualDirectoryListing(get_file_system().get_provider(), get_file_system().get_provider()->get_path() + get_path());
}

CL_String CL_VirtualDirectory::get_identifier() const
{
	CL_String internal_name;

	if (!impl->file_system.is_null())
		internal_name = impl->file_system.get_identifier();

	return internal_name + impl->base_path;
}


/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectory Operations:

CL_VirtualDirectory CL_VirtualDirectory::open_directory(const CL_String &path)
{
	return CL_VirtualDirectory(impl->file_system, make_path_absolute(path));
}

CL_IODevice CL_VirtualDirectory::open_file(
	const CL_String &filename,
	CL_File::OpenMode mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
{
	if (impl->file_system.is_null())
		return CL_File(make_path_absolute(filename), mode, access, share, flags);
	else
		return impl->file_system.open_file(make_path_absolute(filename), mode, access, share, flags);
}

CL_IODevice CL_VirtualDirectory::open_file_read(
	const CL_String &filename) const
{
	if (impl->file_system.is_null())
		return CL_File(make_path_absolute(filename), CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
	else
		return impl->file_system.open_file(make_path_absolute(filename), CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
}

CL_String CL_VirtualDirectory::make_path_absolute(const CL_String &relative_path) const
{
	if (impl->file_system.is_null())
		return CL_PathHelp::make_absolute(impl->base_path, relative_path, CL_PathHelp::path_type_file);
	else
		return CL_PathHelp::make_absolute(impl->base_path, relative_path, CL_PathHelp::path_type_virtual);
}

CL_String CL_VirtualDirectory::make_path_relative(const CL_String &absolute_path) const
{
	if (impl->file_system.is_null())
		return CL_PathHelp::make_relative(impl->base_path, absolute_path, CL_PathHelp::path_type_file);
	else
		return CL_PathHelp::make_relative(impl->base_path, absolute_path, CL_PathHelp::path_type_virtual);
}

void CL_VirtualDirectory::mount(const CL_String &mount_point, CL_VirtualFileSystem &fs)
{
	if (impl->file_system.is_null())
		throw CL_Exception("CL_VirtualDirectory::mount only supported for virtual file systems");
	else
		impl->file_system.mount(make_path_absolute(mount_point), fs);
}

void CL_VirtualDirectory::mount(const CL_String &mount_point, const CL_String &path)
{
	if (impl->file_system.is_null())
		throw CL_Exception("CL_VirtualDirectory::mount only supported for virtual file systems");
	else
		impl->file_system.mount(make_path_absolute(mount_point), path);
}

void CL_VirtualDirectory::unmount(const CL_String &mount_point)
{
	if (impl->file_system.is_null())
		throw CL_Exception("CL_VirtualDirectory::unmount only supported for virtual file systems");
	else
		impl->file_system.unmount(make_path_absolute(mount_point));
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectory Implementation:
