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
#include "API/Core/Text/string_help.h"
#include <memory>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectory_Impl Class:

class VirtualDirectory_Impl
{
//! Construction:
public:
	VirtualDirectory_Impl() : file_system(VirtualFileSystem::NullVFS()) { return; }

//! Attributes:
public:
	VirtualFileSystem file_system;

	std::string base_path;
};

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectory Construction:

VirtualDirectory::VirtualDirectory()
: impl(new VirtualDirectory_Impl)
{
#ifdef WIN32
	WCHAR buffer[MAX_PATH];
	buffer[0] = 0;
	if (GetCurrentDirectory(MAX_PATH, buffer) > 0)
		impl->base_path = StringHelp::ucs2_to_utf8(buffer);
#else
	char buffer[1024];
	buffer[0] = 0;
	char *result = getcwd(buffer, 1024);
	if (result)
		impl->base_path = buffer;
#endif
}

VirtualDirectory::VirtualDirectory(
	const VirtualFileSystem &file_system,
	const std::string &base_path)
: impl(new VirtualDirectory_Impl)
{
	impl->file_system = file_system;
	if (impl->file_system.is_null())
		impl->base_path = PathHelp::add_trailing_slash(base_path, PathHelp::path_type_file);
	else
		impl->base_path = PathHelp::add_trailing_slash(base_path, PathHelp::path_type_virtual);
}

VirtualDirectory::~VirtualDirectory()
{
}

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectory Attributes:

VirtualFileSystem VirtualDirectory::get_file_system()
{
	return impl->file_system;
}

const std::string &VirtualDirectory::get_path() const
{
	return impl->base_path;
}

VirtualDirectoryListing VirtualDirectory::get_directory_listing()
{
	if (impl->file_system.is_null())
		throw Exception("VirtualDirectory::get_directory_listing only supported for virtual file systems");

	return VirtualDirectoryListing(get_file_system().get_provider(), get_path());
}

std::string VirtualDirectory::get_identifier() const
{
	std::string internal_name;

	if (!impl->file_system.is_null())
		internal_name = impl->file_system.get_identifier();

	return internal_name + impl->base_path;
}


/////////////////////////////////////////////////////////////////////////////
// VirtualDirectory Operations:

VirtualDirectory VirtualDirectory::open_directory(const std::string &path)
{
	return VirtualDirectory(impl->file_system, make_path_absolute(path));
}

IODevice VirtualDirectory::open_file(
	const std::string &filename,
	File::OpenMode mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
{
	if (impl->file_system.is_null())
		return File(make_path_absolute(filename), mode, access, share, flags);
	else
		return impl->file_system.open_file(make_path_absolute(filename), mode, access, share, flags);
}

IODevice VirtualDirectory::open_file_read(
	const std::string &filename) const
{
	if (impl->file_system.is_null())
		return File(make_path_absolute(filename), File::open_existing, File::access_read, File::share_all, 0);
	else
		return impl->file_system.open_file(make_path_absolute(filename), File::open_existing, File::access_read, File::share_all, 0);
}

std::string VirtualDirectory::make_path_absolute(const std::string &relative_path) const
{
	if (impl->file_system.is_null())
		return PathHelp::make_absolute(impl->base_path, relative_path, PathHelp::path_type_file);
	else
		return PathHelp::make_absolute(impl->base_path, relative_path, PathHelp::path_type_virtual);
}

std::string VirtualDirectory::make_path_relative(const std::string &absolute_path) const
{
	if (impl->file_system.is_null())
		return PathHelp::make_relative(impl->base_path, absolute_path, PathHelp::path_type_file);
	else
		return PathHelp::make_relative(impl->base_path, absolute_path, PathHelp::path_type_virtual);
}

void VirtualDirectory::mount(const std::string &mount_point, VirtualFileSystem &fs)
{
	if (impl->file_system.is_null())
		throw Exception("VirtualDirectory::mount only supported for virtual file systems");
	else
		impl->file_system.mount(make_path_absolute(mount_point), fs);
}

void VirtualDirectory::mount(const std::string &mount_point, const std::string &path)
{
	if (impl->file_system.is_null())
		throw Exception("VirtualDirectory::mount only supported for virtual file systems");
	else
		impl->file_system.mount(make_path_absolute(mount_point), path);
}

void VirtualDirectory::unmount(const std::string &mount_point)
{
	if (impl->file_system.is_null())
		throw Exception("VirtualDirectory::unmount only supported for virtual file systems");
	else
		impl->file_system.unmount(make_path_absolute(mount_point));
}

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectory Implementation:

}
