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
**    (if your name is missing here, please add it)
*/

#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "../directory_scanner_impl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

class CL_DirectoryScanner_Unix : public CL_DirectoryScanner_Impl
{
//!Construction:
public:
	CL_DirectoryScanner_Unix();

	~CL_DirectoryScanner_Unix();

//!Attributes:
public:
	bool scan (const CL_String& pathname);

	bool scan (const CL_String& pathname, const CL_String& pattern);

	/// \brief Returns the path of the directory being scanned.
	CL_String get_directory_path();

	/// \brief Returns the size of the current found file.
	int get_size();

	/// \brief Returns the name of the current found file.
	CL_String get_name();

	/// \brief Returns the name of the current found file, including the directory path.
	CL_String get_pathname();

	/// \brief Returns true if filename is a directory.
	bool is_directory();

	/// \brief Returns true if filename is hidden.
	bool is_hidden();

  	/// \brief Returns true if file is readable by current user.
	virtual bool is_readable();

	/// \brief Returns true if file is writable by current user.
	virtual bool is_writable();
	// todo: add other attributes of a file.

//!Operations:
public:
	/// \brief Find next file in directory scan. Returns false if no more files was found.
	bool next();

/// \name Implementation
/// \{

private:
	DIR *dir_temp;

	dirent *entry;

	struct stat statbuf;

	bool use_pattern;

	/// \brief Path stored without the trailing slash
	CL_String file_name;

	CL_String path_name;

	CL_String file_pattern;
/// \}
};


