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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"
#include "file.h"
class CL_IODevice;
class CL_VirtualDirectory;
class CL_VirtualFileSystem_Impl;
class CL_VirtualFileSource;
class CL_VirtualDirectoryListing;

/// \brief Virtual File System (VFS).
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_VirtualFileSystem
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a file system.
	CL_VirtualFileSystem();

	/// \brief Constructs a VirtualFileSystem
	///
	/// \param provider = Virtual File Source
	CL_VirtualFileSystem(CL_VirtualFileSource *provider);

	/// \brief Constructs a VirtualFileSystem
	///
	/// \param path = String
	/// \param is_zip_file = bool
	CL_VirtualFileSystem(const CL_String &path, bool is_zip_file = false);

	~CL_VirtualFileSystem();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the file system is null.
	bool is_null() const { return impl.is_null(); }

	/// \brief Returns the root directory for the file system.
	CL_VirtualDirectory get_root_directory();

	/// \brief Returns true if a path is a mount point.
	bool is_mount(const CL_String &mount_point);

	/// \brief Return directory listing for path.
	CL_VirtualDirectoryListing get_directory_listing(const CL_String &path_rel);

	/// \brief Return true if the root of the filesystem contains the specified file.
	bool has_file(const CL_String &filename);

	/// \brief Return true if the root of the filesystem contains the specified directory.
	bool has_directory(const CL_String &directory);

	/// \brief Returns the file source for this file system.
	CL_VirtualFileSource *get_provider();

	/// \brief Get the identifier of this file source
	///
	/// The exact format of this identifier depends on the implementation of the underlying filesystem and mounts
	/// \return the identifier
	CL_String get_identifier() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Opens a virtual directory.
	CL_VirtualDirectory open_directory(const CL_String &path);

	/// \brief Opens a file.
	/** param: mode = CL_File::OpenMode modes
	    param: access = CL_File::AccessFlags flags
	    param: share = CL_File::ShareFlags flags
	    param: flags = CL_File::Flags flags
	    \return The CL_IODevice*/
	CL_IODevice open_file(const CL_String &filename,
		CL_File::OpenMode mode = CL_File::open_existing,
		unsigned int access = CL_File::access_read | CL_File::access_write,
		unsigned int share = CL_File::share_all,
		unsigned int flags = 0) const;

	/// \brief Mounts a file system at mount point.
	/** This is only available if CL_VirtualFileSystem was set
	    Filenames starting with "mount_point" at the start will be replaced by the filesystem specified by "fs"
	    (ie the the base_path is ignored)
	    For example:
	     CL_VirtualFileSystem new_vfs(new MyFileSource("Hello"));
	     vfs.mount("ABC", new_vfs);
	    param: mount_point = Mount alias name to use
	    param: fs = Filesystem to use*/
	void mount(const CL_String &mount_point, CL_VirtualFileSystem fs);

	/// \brief Mounts a file system at mount point.
	/** Filenames starting with "mount_point" at the start will be replaced by the path specified by "path"
	    (ie the the base_path is ignored)
	    param: mount_point = Mount alias name to use
	    param: path = Path which "mount_point" should point to
	    param: is_zip_file = false, create as a CL_VirtualFileSource_File, else create as a CL_VirtualFileSource_Zip*/
	void mount(const CL_String &mount_point, const CL_String &path, bool is_zip_file);

	/// \brief Unmount a file system.
	/** param: mount_point = The mount point to unmount*/
	void unmount(const CL_String &mount_point);

/// \}
/// \name Implementation
/// \{

private:
	/** !hide!*/
	class CL_NullVFS { };
	explicit CL_VirtualFileSystem(class CL_NullVFS null_fs);

	CL_SharedPtr<CL_VirtualFileSystem_Impl> impl;

	friend class CL_VirtualDirectory_Impl;
/// \}
};

/// \}
