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
**    Harry Storbacka
*/

#pragma once

#include "../System/sharedptr.h"
#include "../Text/string_types.h"

class CL_VirtualDirectoryListingEntry;
class CL_VirtualFileSource;
class CL_VirtualDirectoryListing_Impl;

/// \brief Virtual File System (VFS) directory listing class.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_VirtualDirectoryListing
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a virtual directory listening object.
	CL_VirtualDirectoryListing(CL_VirtualFileSource *provider, const CL_String &path);

	virtual ~CL_VirtualDirectoryListing();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the file name of the current item in the listing.
	CL_String get_filename();

	/// \brief Returns true if item is a directory.
	bool is_directory();

	/// \brief Returns true if item is hidden.
	bool is_hidden();

	/// \brief Returns true if item is writable.
	bool is_writable();

	/// \brief Returns true if item is readable.
	bool is_readable();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Advance to next item in listing.
	bool next();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_VirtualDirectoryListing_Impl> impl;
/// \}
};

