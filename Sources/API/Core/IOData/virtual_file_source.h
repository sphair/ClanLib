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
**    Harry Storbacka
*/

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{


#pragma once


#include "../api_core.h"
#include "../System/sharedptr.h"
#include "file.h"

class CL_IODevice;
class CL_VirtualDirectoryListingEntry;

/// \brief Virtual File System (VFS) file source.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_VirtualFileSource
{
/// \name Construction
/// \{

public:
	virtual ~CL_VirtualFileSource() { return; }


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	/// \brief Open file from this source.
	/** param: filename = The filename to use
	    param: mode = CL_File::OpenMode modes
	    param: access = CL_File::AccessFlags flags
	    param: share = CL_File::ShareFlags flags
	    param: flags = CL_File::Flags flags
	    \return The CL_IODevice*/
	virtual CL_IODevice open_file(const CL_String &filename,
		CL_File::OpenMode mode = CL_File::open_existing,
		unsigned int access = CL_File::access_read | CL_File::access_write,
		unsigned int share = CL_File::share_all,
		unsigned int flags = 0) = 0;

	/// \brief Initiate directory listing.
	virtual bool initialize_directory_listing(const CL_String &path) = 0;

	/// \brief Update directory listing item.
	virtual bool next_file(CL_VirtualDirectoryListingEntry &entry) = 0;

	/// \brief Return the path of this file source.
	virtual CL_String get_path() const = 0;


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
