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
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_source.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_directory_listing.h"
#include "API/Core/Text/string_format.h"
#include "virtual_file_source_file.h"
#include "virtual_file_source_zip.h"

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSystem_Impl Class:

class CL_VirtualFileSystem_Impl
{
//! Construction:
public:
	CL_VirtualFileSystem_Impl() : provider(0)
	{
	}

	~CL_VirtualFileSystem_Impl()
	{
		delete provider;
	}

//! Attributes:
public:
	CL_VirtualFileSource *provider;

	std::vector< std::pair<CL_String, CL_VirtualFileSystem> > mounts;
};

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSystem Construction:

CL_VirtualFileSystem::CL_VirtualFileSystem()
: impl(new CL_VirtualFileSystem_Impl)
{
}

CL_VirtualFileSystem::CL_VirtualFileSystem(CL_VirtualFileSystem::CL_NullVFS null_fs)
{
}

CL_VirtualFileSystem::CL_VirtualFileSystem(CL_VirtualFileSource *provider)
: impl(new CL_VirtualFileSystem_Impl)
{
	impl->provider = provider;
}

CL_VirtualFileSystem::CL_VirtualFileSystem(const CL_String &path, bool is_zip_file)
: impl(new CL_VirtualFileSystem_Impl)
{
	if (is_zip_file)
		impl->provider = new CL_VirtualFileSource_Zip(CL_ZipArchive(path));
	else
		impl->provider = new CL_VirtualFileSource_File(path);
}

CL_VirtualFileSystem::~CL_VirtualFileSystem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSystem Attributes:

CL_VirtualDirectory CL_VirtualFileSystem::get_root_directory()
{
	return open_directory(CL_String());
}

bool CL_VirtualFileSystem::is_mount(const CL_String &mount_point)
{
	CL_String mount_point_slash = CL_PathHelp::add_trailing_slash(mount_point, CL_PathHelp::path_type_virtual);
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

CL_VirtualDirectoryListing CL_VirtualFileSystem::get_directory_listing(const CL_String &path_rel)
{
	CL_String path = CL_PathHelp::make_absolute(
		"/",
		path_rel,
		CL_PathHelp::path_type_virtual);

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
		return CL_VirtualDirectoryListing(
			impl->provider,
			CL_PathHelp::make_relative(
				"/",
				path,
				CL_PathHelp::path_type_virtual));		
	}
	else
		throw CL_Exception(cl_format("Unable to list directory: %1", path));

}

CL_VirtualFileSource *CL_VirtualFileSystem::get_provider()
{
	return impl->provider;
}

CL_String CL_VirtualFileSystem::get_identifier() const
{
	CL_String internal_name = "/";

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
// CL_VirtualFileSystem Operations:

CL_VirtualDirectory CL_VirtualFileSystem::open_directory(const CL_String &path)
{
	return CL_VirtualDirectory(
		*this,
		CL_PathHelp::make_absolute(
			"/",
			path,
			CL_PathHelp::path_type_virtual));
}

CL_IODevice CL_VirtualFileSystem::open_file(const CL_String &filename_rel,
	CL_File::OpenMode mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags) const
{
	CL_String filename = CL_PathHelp::make_absolute(
		"/",
		filename_rel,
		CL_PathHelp::path_type_virtual);

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
			CL_PathHelp::make_relative(
				"/",
				filename,
				CL_PathHelp::path_type_virtual), mode, access, share, flags);
	}
	else
	{
		throw CL_Exception(cl_format("Unable to open file: %1", filename));
	}
}

void CL_VirtualFileSystem::mount(const CL_String &mount_point, CL_VirtualFileSystem fs)
{
	CL_String mount_point_slash = CL_PathHelp::add_trailing_slash(
		CL_PathHelp::make_absolute(
			"/",
			mount_point,
			CL_PathHelp::path_type_virtual),
		CL_PathHelp::path_type_virtual);
	impl->mounts.push_back(std::pair<CL_String, CL_VirtualFileSystem>(mount_point_slash, fs));
}

void CL_VirtualFileSystem::mount(const CL_String &mount_point, const CL_String &path, bool is_zip_file)
{
	if (is_zip_file)
		mount(mount_point, CL_VirtualFileSystem(new CL_VirtualFileSource_Zip(CL_ZipArchive(path))));
	else
		mount(mount_point, CL_VirtualFileSystem(new CL_VirtualFileSource_File(path)));
}

void CL_VirtualFileSystem::unmount(const CL_String &mount_point)
{
	CL_String mount_point_slash = CL_PathHelp::add_trailing_slash(
		CL_PathHelp::make_absolute(
			"/",
			mount_point,
			CL_PathHelp::path_type_virtual),
		CL_PathHelp::path_type_virtual);
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

bool CL_VirtualFileSystem::has_directory(const CL_String &directory)
{
	CL_VirtualDirectoryListing list = get_directory_listing(CL_PathHelp::get_basepath(directory, CL_PathHelp::path_type_virtual));
	while (list.next())
	{
		if (directory == list.get_filename() && list.is_directory())
			return true;
	}

	return false;
}

bool CL_VirtualFileSystem::has_file(const CL_String &filename)
{
	CL_VirtualDirectoryListing list = get_directory_listing(CL_PathHelp::get_basepath(filename, CL_PathHelp::path_type_virtual));
	while (list.next())
	{
		if (filename == list.get_filename() && !list.is_directory())
			return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualFileSystem Implementation:
