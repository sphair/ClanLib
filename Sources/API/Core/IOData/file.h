/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "iodevice.h"

class CL_SecurityDescriptor;

/// \brief File I/O device.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_File : public CL_IODevice
{
/// \name Enumerations
/// \{

public:
	/// \brief Access flags.
	/** <p>File access types available.</p>*/
	enum AccessFlags
	{
		/// \brief Generic read access.
		access_read  = 1,

		/// \brief Generic write access.
		access_write = 2
	};

	/// \brief File sharing flags.
	enum ShareFlags
	{
		/// \brief Allow others to open the file for reading.
		share_read   = 1,

		/// \brief Allow others to open the file for writing.
		share_write  = 2,

		/// \brief Allow others to delete the file.
		share_delete = 4,

		/// \brief All other sharing flags combined.
		share_all    = share_read + share_write + share_delete
	};

	/// \brief File opening modes.
	enum OpenMode
	{
		/// \brief Open file or create it if it does not exist.
		open_always,

		/// \brief Open existing file.  Fails if it does not exist.
		open_existing,

		/// \brief Open existing file and truncate it.
		open_existing_truncate,

		/// \brief Create file, even if it already exists.
		create_always,

		/// \brief Create a new file.  Fails if it already exists.
		create_new
	};

	/// \brief Optimization Flags.
	enum Flags
	{
		flag_write_through   = 1,
		flag_no_buffering    = 2,
		flag_random_access   = 4,
		flag_sequential_scan = 8
	};


/// \}
/// \name Construction
/// \{

public:
	/// \brief Constructs a file object.
	CL_File();

	/// \brief Constructs a file object.
	/** CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file) is called*/
	CL_File(
		const CL_String &filename,
		OpenMode mode = open_existing,
		unsigned int access = access_read | access_write,
		unsigned int share = share_all,
		unsigned int flags = 0);

	/// \brief Constructs a file object.
	/** CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file) is called*/
	CL_File(
		const CL_String &filename,
		OpenMode mode,
		const CL_SecurityDescriptor &permissions,
		unsigned int access = access_read | access_write,
		unsigned int share = share_all,
		unsigned int flags = 0);

	~CL_File();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the file permissions.
	CL_SecurityDescriptor get_permissions() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Opens a file.
	/** CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file) is called*/
	bool open(
		const CL_String &filename,
		OpenMode mode = open_existing,
		unsigned int access = access_read | access_write,
		unsigned int share = share_all,
		unsigned int flags = 0);

	/// \brief Opens a file.
	/** CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file) is called*/
	bool open(
		const CL_String &filename,
		OpenMode mode,
		const CL_SecurityDescriptor &permissions,
		unsigned int access = access_read | access_write,
		unsigned int share = share_all,
		unsigned int flags = 0);

	/// \brief Close file.
	void close();

	/// \brief Change file permissions.
	bool set_permissions(const CL_SecurityDescriptor &permissions);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
