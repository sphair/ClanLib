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
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "../api_core.h"
#include "../System/sharedptr.h"
#include "../Text/string_types.h"

class CL_DirectoryScanner_Impl;

/// \brief Directory scanning class.
///
///    <p>CL_DirectoryScanner is used to parse through directory trees and return information about files.</p>
///    -
///    <p>Example that prints all files and directories found in the root directory:</p>
///    <pre>
///    CL_DirectoryScanner scanner;
///    if (scanner.scan("/", "*"))
///    {
///    		while (scanner.next())
///    		{
///    				cl_console_write_line(scanner.get_name());
///    		}
///    }
///    </pre> 
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_DirectoryScanner
{
/// \name Construction
/// \{

public:
	/// \brief Constructs directory scanner for iterating over a directory.
	CL_DirectoryScanner();

	CL_DirectoryScanner(const CL_DirectoryScanner &copy);

	/// \brief Destructor.
	~CL_DirectoryScanner();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Gets the directory being scanned.
	/** \return Directory being scanned. (including the trailing slash)*/
	CL_String get_directory_path();

	/// \brief Gets the name of the current file.
	/** \return The name of the current found file.*/
	CL_String get_name();

	/// \brief Gets the size of the current file.
	/** \return The size of the current found file.*/
	int get_size();

	/// \brief Gets the pathname of the current file.
	/** \return The name of the current found file, including the directory path.*/
	CL_String get_pathname();

	/// \brief Returns true if the current file is a directory.
	/** \return True if filename is a directory.*/
	bool is_directory();

	/// \brief Returns true if the file is hidden.
	/** \return True if filename is hidden.*/
	bool is_hidden();

	/// \brief Returns true if the file is readable by the current user.
	/** \return True if the file is readable.*/
	bool is_readable();

	/// \brief Returns true if the file is writable by the current user.
	/** \return True if the file is writable.*/
	bool is_writable();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Selects the directory to scan through.
	/** <p>Selects the directory to scan through</p>
	    \param pathname Path to the directory to scan (without trailing slash)
	    \return true if the directory can be accessed.*/
	bool scan(const CL_String& pathname);

	/// \brief Selects the directory to scan through.
	/** <p>Selects the directory to scan through and use a matching pattern on the files.</p>
	    WIN32: The pattern is normal DOS pattern matching ("*.*", ?)
	    Unix: The pattern is normal pattern matching (*, ?)
	    \param pathname Path to the directory to scan (without trailing slash)
	    \param pattern Pattern to match files against.
	    \return true if the directory can be accessed.*/
	bool scan(const CL_String& pathname, const CL_String& pattern);

	/// \brief Find next file in directory scan.
	/** \return false if no more files was found.*/
	bool next();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_DirectoryScanner_Impl> impl;
/// \}
};


/// \}
