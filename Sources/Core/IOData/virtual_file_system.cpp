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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_source.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_directory_listing.h"
#include "API/Core/Text/string_format.h"
#include "virtual_file_source_file.h"
#include "virtual_file_source_zip.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// VirtualFileSystem_Impl Class:

class VirtualFileSystem_Impl
{
//! Construction:
public:
	VirtualFileSystem_Impl() : provider(0)
	{
	}

	~VirtualFileSystem_Impl()
	{
		delete provider;
	}

//! Attributes:
public:
	VirtualFileSource *provider;

	std::vector< std::pair<std::string, VirtualFileSystem> > mounts;
};

/////////////////////////////////////////////////////////////////////////////
// VirtualFileSystem Construction:

VirtualFileSystem::VirtualFileSystem()
: impl(new VirtualFileSystem_Impl)
{
}

VirtualFileSystem::VirtualFileSystem(VirtualFileSystem::NullVFS null_fs)
{
}

VirtualFileSystem::VirtualFileSystem(VirtualFileSource *provider)
: impl(new VirtualFileSystem_Impl)
{
	impl->provider = provider;
}

VirtualFileSystem::VirtualFileSystem(const std::string &path, bool is_zip_file)
: impl(new VirtualFileSystem_Impl)
{
	if (is_zip_file)
		impl->provider = new VirtualFileSource_Zip(ZipArchive(path));
	else
		impl->provider = new VirtualFileSource_File(path);
}

VirtualFileSystem::~VirtualFileSystem()
{
}

/////////////////////////////////////////////////////////////////////////////
// VirtualFileSystem Attributes:

VirtualDirectory VirtualFileSystem::get_root_directory()
{
	return open_directory(std::string());
}

bool VirtualFileSystem::is_mount(const std::string &mount_point)
{
	std::string mount_point_slash = PathHelp::add_trailing_slash(mount_point, PathHelp::path_type_virtual);
	int index, size;
	size = (int) impl->mounts.size();
	for (index = 0; index < size; index++)
	{
		if (impl->mounts[index].first == mount_point_slash)
		{
			return true;
		}
	}
	return false;
}

VirtualDirectoryListing VirtualFileSystem::get_directory_listing(const std::string &path_rel)
{
	std::string path = PathHelp::make_absolute(
		"/",
		path_rel,
		PathHelp::path_type_virtual);

	// First see if its a mount point:
	int index, size;
	size = (int) impl->mounts.size();
	for (index = 0; index < size; index++)
	{
		if (impl->mounts[index].first == path.substr(0, impl->mounts[index].first.length()))
		{
			return impl->mounts[index].second.get_directory_listing( path.substr(impl->mounts[index].first.length(), path.length()));
		}
	}

	// Try open locally, if we got a file provider attached
	if (impl->provider)
	{
		return VirtualDirectoryListing(
			impl->provider,
			PathHelp::make_relative(
				"/",
				path,
				PathHelp::path_type_virtual));		
	}
	else
		throw Exception(string_format("Unable to list directory: %1", path));

}

VirtualFileSource *VirtualFileSystem::get_provider()
{
	return impl->provider;
}

std::string VirtualFileSystem::get_path() const
{
	if (impl->provider)
		return impl->provider->get_path();
	return "";
}

std::string VirtualFileSystem::get_identifier() const
{
	std::string internal_name = "/";

	// Add on the mount point names
	int index, size;
	size = (int) impl->mounts.size();
	for (index = 0; index < size; index++)
	{
		internal_name += impl->mounts[index].first;
		internal_name += impl->mounts[index].second.get_identifier();
	}

	if (impl->provider)
		internal_name = internal_name + impl->provider->get_identifier();

	return internal_name;
}

/////////////////////////////////////////////////////////////////////////////
// VirtualFileSystem Operations:

VirtualDirectory VirtualFileSystem::open_directory(const std::string &path)
{
	return VirtualDirectory(
		*this,
		PathHelp::make_absolute(
			"/",
			path,
			PathHelp::path_type_virtual));
}

IODevice VirtualFileSystem::open_file(const std::string &filename_rel,
	File::OpenMode mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags) const
{
	std::string filename = PathHelp::make_absolute(
		"/",
		filename_rel,
		PathHelp::path_type_virtual);

	// First see if its a file for one of our mount points:
	int index, size;
	size = (int) impl->mounts.size();
	for (index = 0; index < size; index++)
	{
		if (impl->mounts[index].first == filename.substr(0, impl->mounts[index].first.length()))
		{
			return impl->mounts[index].second.open_file( filename.substr(impl->mounts[index].first.length(), filename.length()));
		}
	}

	// Try open locally, if we got a file provider attached
	if (impl->provider)
	{
		return impl->provider->open_file(
			PathHelp::make_relative(
				"/",
				filename,
				PathHelp::path_type_virtual), mode, access, share, flags);
	}
	else
	{
		throw Exception(string_format("Unable to open file: %1", filename));
	}
}

void VirtualFileSystem::mount(const std::string &mount_point, VirtualFileSystem fs)
{
	std::string mount_point_slash = PathHelp::add_trailing_slash(
		PathHelp::make_absolute(
			"/",
			mount_point,
			PathHelp::path_type_virtual),
		PathHelp::path_type_virtual);
	impl->mounts.push_back(std::pair<std::string, VirtualFileSystem>(mount_point_slash, fs));
}

void VirtualFileSystem::mount(const std::string &mount_point, const std::string &path, bool is_zip_file)
{
	if (is_zip_file)
		mount(mount_point, VirtualFileSystem(new VirtualFileSource_Zip(ZipArchive(path))));
	else
		mount(mount_point, VirtualFileSystem(new VirtualFileSource_File(path)));
}

void VirtualFileSystem::unmount(const std::string &mount_point)
{
	std::string mount_point_slash = PathHelp::add_trailing_slash(
		PathHelp::make_absolute(
			"/",
			mount_point,
			PathHelp::path_type_virtual),
		PathHelp::path_type_virtual);
	int index, size;
	size = (int) impl->mounts.size();
	for (index = 0; index < size; index++)
	{
		if (impl->mounts[index].first == mount_point_slash)
		{
			impl->mounts.erase(impl->mounts.begin() + index);
			size--;
			index--;
		}
	}
}

bool VirtualFileSystem::has_directory(const std::string &directory)
{
	VirtualDirectoryListing list = get_directory_listing(PathHelp::get_basepath(directory, PathHelp::path_type_virtual));
	std::string dir_name = PathHelp::get_filename(PathHelp::remove_trailing_slash(directory));
	while (list.next())
	{
		if (dir_name == list.get_filename() && list.is_directory())
			return true;
	}

	return false;
}

bool VirtualFileSystem::has_file(const std::string &filename)
{
	VirtualDirectoryListing list = get_directory_listing(PathHelp::get_basepath(filename, PathHelp::path_type_virtual));
	std::string fil_name = PathHelp::get_filename(filename);
	while (list.next())
	{
		if (fil_name == list.get_filename() && !list.is_directory())
			return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// VirtualFileSystem Implementation:

}
