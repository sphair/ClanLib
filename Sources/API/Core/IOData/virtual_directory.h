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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"
#include <memory>
#include "file.h"

namespace clan
{

class IODevice;
class VirtualFileSystem;
class VirtualDirectory_Impl;
class VirtualDirectoryListing;

/// \brief Virtual File System (VFS) directory.
class CL_API_CORE VirtualDirectory
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a virtual directory.
	/** File system is treated as File
	    The base path is set the current working directory*/
	VirtualDirectory();

	/// \brief Constructs a virtual directory.
	/** param: file_system = File system to use
	    param: base_path = Base path of the directory*/
	VirtualDirectory(const VirtualFileSystem &file_system, const std::string &base_path);

	~VirtualDirectory();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the file system the directory belongs to.
	/** \return The Virtual File System. (use is_null() to check if it contains a file system)*/
	VirtualFileSystem get_file_system();

	/// \brief Returns the absolute path of the virtual directory.
	/** If VirtualFileSystem was not set, it is is a full path.
	    Else, it is the path specified to the constructor
	    \return The path*/
	const std::string &get_path() const;

	/// \brief List contents of directory.
	/** This is only available if VirtualFileSystem was set. If not, use DirectoryScanner() instead.*/
	VirtualDirectoryListing get_directory_listing();

	/// \brief Get the identifier of this directory
	///
	/// The exact format of this identifier depends on the implementation of the underlying filesystem and mounts
	/// \return the identifier
	std::string get_identifier() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Open a directory relatively to this one.
	/** This function uses make_path_absolute()
	    path: Relative path to use*/
	VirtualDirectory open_directory(const std::string &path);

	/// \brief Open a file relatively to this directory.
	/** This function uses make_path_absolute()
	    A Exception is thrown if the file could not be opened
	    param: filename = File to open (may be a path)
	    param: mode = File::OpenMode modes
	    param: access = File::AccessFlags flags
	    param: share = File::ShareFlags flags
	    param: flags = File::Flags flags
	    \return The file*/
	IODevice open_file(
		const std::string &filename,
		File::OpenMode mode = File::open_existing,
		unsigned int access = File::access_read | File::access_write,
		unsigned int share = File::share_all,
		unsigned int flags = 0);

	/// \brief Open a file read only, avoiding potential "const" problems
	IODevice open_file_read(
		const std::string &filename) const;

	/// \brief Convert a relative path to an absolute one.
	/** If VirtualFileSystem was not set, this function uses PathHelp::path_type_file
	    If VirtualFileSystem is set, this function uses PathHelp::path_type_virtual
	    param: relative_path = The relative path
	    \return The absolute string*/
	std::string make_path_absolute(const std::string &relative_path) const;

	/// \brief Convert an absolute path to a relative one.
	/** If VirtualFileSystem was not set, this function uses PathHelp::path_type_file
	    If VirtualFileSystem is set, this function uses PathHelp::path_type_virtual
	    param: absolute_path = The absolute path
	    \return The relative string*/
	std::string make_path_relative(const std::string &absolute_path) const;

	/// \brief Mount a file system.
	/** This is only available if VirtualFileSystem was set
	    Filenames starting with "mount_point" at the start will be replaced by the filesystem specified by "fs"
	    (ie the the base_path is ignored)
	    For example:
	     VirtualFileSystem new_vfs(new MyFileSource("Hello"));
	     dir.mount("ABC", new_vfs);
	    param: mount_point = Mount alias name to use
	    param: fs = Filesystem to use*/
	void mount(const std::string &mount_point, VirtualFileSystem &fs);

	/// \brief Mount a file system.
	/** This is only available if VirtualFileSystem was set
	    Filenames starting with "mount_point" at the start will be replaced by the path specified by "path"
	    (ie the the base_path is ignored)
	    param: mount_point = Mount alias name to use
	    param: path = Path which "mount_point" should point to*/
	void mount(const std::string &mount_point, const std::string &path);

	/// \brief Unmount a file system.
	/** This is only available if VirtualFileSystem was set
	    param: mount_point = The mount point to unmount*/
	void unmount(const std::string &mount_point);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<VirtualDirectory_Impl> impl;
/// \}
};

}

/// \}
