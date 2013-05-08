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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"
#include <memory>
#include "file.h"

namespace clan
{

class IODevice;
class VirtualDirectory;
class VirtualFileSystem_Impl;
class VirtualFileSource;
class VirtualDirectoryListing;

/// \brief Virtual File System (VFS).
class CL_API_CORE VirtualFileSystem
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a file system.
	VirtualFileSystem();

	/// \brief Constructs a VirtualFileSystem
	///
	/// \param provider = Virtual File Source
	VirtualFileSystem(VirtualFileSource *provider);

	/// \brief Constructs a VirtualFileSystem
	///
	/// \param path = String
	/// \param is_zip_file = bool
	VirtualFileSystem(const std::string &path, bool is_zip_file = false);

	~VirtualFileSystem();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the file system is null.
	bool is_null() const { return !impl; }

	/// \brief Returns the root directory for the file system.
	VirtualDirectory get_root_directory();

	/// \brief Returns true if a path is a mount point.
	bool is_mount(const std::string &mount_point);

	/// \brief Return directory listing for path.
	VirtualDirectoryListing get_directory_listing(const std::string &path_rel);

	/// \brief Return true if the root of the filesystem contains the specified file.
	bool has_file(const std::string &filename);

	/// \brief Return true if the root of the filesystem contains the specified directory.
	bool has_directory(const std::string &directory);

	/// \brief Returns the file source for this file system.
	VirtualFileSource *get_provider();

	/// \brief Returns a path to the file source for this file system.
	std::string get_path() const;

	/// \brief Get the identifier of this file source
	///
	/// The exact format of this identifier depends on the implementation of the underlying filesystem and mounts
	/// \return the identifier
	std::string get_identifier() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Opens a virtual directory.
	VirtualDirectory open_directory(const std::string &path);

	/// \brief Opens a file.
	/** param: mode = File::OpenMode modes
	    param: access = File::AccessFlags flags
	    param: share = File::ShareFlags flags
	    param: flags = File::Flags flags
	    \return The IODevice*/
	IODevice open_file(const std::string &filename,
		File::OpenMode mode = File::open_existing,
		unsigned int access = File::access_read | File::access_write,
		unsigned int share = File::share_all,
		unsigned int flags = 0) const;

	/// \brief Mounts a file system at mount point.
	/** This is only available if VirtualFileSystem was set
	    Filenames starting with "mount_point" at the start will be replaced by the filesystem specified by "fs"
	    (ie the the base_path is ignored)
	    For example:
	     VirtualFileSystem new_vfs(new MyFileSource("Hello"));
	     vfs.mount("ABC", new_vfs);
	    param: mount_point = Mount alias name to use
	    param: fs = Filesystem to use*/
	void mount(const std::string &mount_point, VirtualFileSystem fs);

	/// \brief Mounts a file system at mount point.
	/** Filenames starting with "mount_point" at the start will be replaced by the path specified by "path"
	    (ie the the base_path is ignored)
	    param: mount_point = Mount alias name to use
	    param: path = Path which "mount_point" should point to
	    param: is_zip_file = false, create as a VirtualFileSource_File, else create as a VirtualFileSource_Zip*/
	void mount(const std::string &mount_point, const std::string &path, bool is_zip_file);

	/// \brief Unmount a file system.
	/** param: mount_point = The mount point to unmount*/
	void unmount(const std::string &mount_point);

/// \}
/// \name Implementation
/// \{

private:
	/** !hide!*/
	class NullVFS { };
	explicit VirtualFileSystem(class NullVFS null_fs);

	std::shared_ptr<VirtualFileSystem_Impl> impl;

	friend class VirtualDirectory_Impl;
/// \}
};

}

/// \}
