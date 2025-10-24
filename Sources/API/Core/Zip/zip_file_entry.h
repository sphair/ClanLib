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
*/

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"
#include "../Text/string_types.h"
#include "../System/sharedptr.h"
#include "../IOData/datatypes.h"

class CL_ZipFileEntry_Impl;

/// \brief File entry in zip file.
///
/// \xmlonly !group=Core/Zip! !header=core.h! \endxmlonly
class CL_API_CORE CL_ZipFileEntry
{
/// \name Construction
/// \{

public:
	CL_ZipFileEntry();

	/// \brief Constructs a ZipFileEntry
	///
	/// \param copy = Zip File Entry
	CL_ZipFileEntry(const CL_ZipFileEntry &copy);

	~CL_ZipFileEntry();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the filename of file entry.
	CL_StringRef get_archive_filename() const;

	/// \brief Returns the filename of file entry.
	CL_StringRef get_input_filename() const;

	/// \brief Returns the uncompressed size of file entry.
	cl_int64 get_uncompressed_size();

	/// \brief Returns the compressed size of file entry.
	cl_int64 get_compressed_size();

	/// \brief Is Directory
	///
	/// \return true = directory
	bool is_directory() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Copy assignment operator.
	CL_ZipFileEntry &operator =(const CL_ZipFileEntry &copy);

	/// \brief Sets the filename of the file to be added to the archive.
	void set_input_filename(const CL_StringRef &filename);

	/// \brief Sets the filename of the file inside the archive.
	void set_archive_filename(const CL_StringRef &filename);

	/// \brief Set directory
	///
	/// \param is_directory = bool
	void set_directory(bool is_directory);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ZipFileEntry_Impl> impl;

	friend class CL_ZipArchive;

	friend class CL_ZipIODevice_FileEntry;
/// \}
};

/// \}
