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
#include "zip_file_entry.h"
#include <vector>

class CL_IODevice;
class CL_ZipArchive_Impl;

/// \brief Zip archive.
///
/// \xmlonly !group=Core/Zip! !header=core.h! \endxmlonly
class CL_API_CORE CL_ZipArchive
{
/// \name Construction
/// \{

public:
	/// \brief Constructs or loads a ZIP archive.
	///
	/// \param filename .zip archive to load.
	CL_ZipArchive();

	/// \brief Constructs a ZipArchive
	///
	/// \param input = IODevice
	CL_ZipArchive(CL_IODevice &input);

	/// \brief Constructs a ZipArchive
	///
	/// \param filename = String Ref
	CL_ZipArchive(const CL_StringRef &filename);

	/// \brief Constructs a ZipArchive
	///
	/// \param copy = Zip Archive
	CL_ZipArchive(const CL_ZipArchive &copy);

	~CL_ZipArchive();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief List of file entries in archive.
	std::vector<CL_ZipFileEntry> get_file_list();

	std::vector<CL_ZipFileEntry> get_file_list(const CL_StringRef &path);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Opens a file in the archive.
	CL_IODevice open_file(const CL_StringRef &filename);

	/// \brief Get full path to source:
	CL_String get_pathname(const CL_StringRef &filename);

	/// \brief Creates a new file entry
	CL_IODevice create_file(const CL_StringRef &filename, bool compress = true);

	/// \brief Adds a file to zip archive.
	/** <p>File is not added to zip file until it save() is called.</p>
	    \param filename Filename of file.*/
	void add_file(const CL_StringRef &input_filename, const CL_StringRef &filename_in_archive);

	/// \brief Saves zip archive.
	///
	/// \param filename Filename of zip archive. Must not be used to save to the same as loaded from.
	/// <p>If no filename parameter was passed, it will modify the zip
	/// archive loaded at construction time. It does this by creating a
	/// temporary file, saving the new archive, deletes the old one and
	/// renames the temp file to the original archive filename.</p>
	/// -
	/// <p>If the archive was created instead of loaded, a filename must
	/// be specify a filename. Likewise, if saving to same archive as
	/// loaded from, a filename must not be specified. Doing so will
	/// cause the save operation to fail.</p>
	void save();

	/// \brief Save
	///
	/// \param filename = the filename to save to
	void save(const CL_StringRef &filename);

	/// \brief Save
	///
	/// \param iodev = The file to save to
	void save(CL_IODevice iodev);

	/// \brief Loads the zip archive from a input device (done automatically at construction).
	void load(CL_IODevice &input);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ZipArchive_Impl> impl;
/// \}
};

/// \}
