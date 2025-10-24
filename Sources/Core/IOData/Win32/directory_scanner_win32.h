/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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

#ifndef header_directory_scanner_win32
#define header_directory_scanner_win32

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Generic/directory_scanner_generic.h"

#include <io.h>
#include <direct.h>

class CL_DirectoryScanner_Win32: public CL_DirectoryScanner_Generic
{
//! Construction:
public:
	//: Construct initial directory scanner.
	CL_DirectoryScanner_Win32();

	//: Copy constructor.
	CL_DirectoryScanner_Win32(const CL_DirectoryScanner_Win32 &copy);

	virtual ~CL_DirectoryScanner_Win32();

//! Attributes:
public:
	//: Scan for files in a directory.
	virtual bool scan (const std::string &pathname);

	//: Scan for files matching a pattern.
	virtual bool scan (const std::string &pathname, const std::string &pattern);

	//: Returns the path of the directory being scanned.
	virtual std::string get_directory_path();

	//: Returns the size of the current found file.
	virtual int get_size();

	//: Returns the name of the current found file.
	virtual std::string get_name();
	
	//: Returns the name of the current found file, including the directory path.
	virtual std::string get_pathname();
	
	//: Returns true if filename is a directory.
	virtual bool is_directory();

	//: Returns true if filename is hidden.
	virtual bool is_hidden();

	//: Returns true if filename is readable.
	virtual bool is_readable();

	//: Returns true if filename is writable.
	virtual bool is_writable();

//! Operations:
public:
	//: Find next file in directory scan. Returns false if no more files was found.
	virtual bool next();

//! Implementation
private:
	std::string path_with_ending_slash(const std::string &path);

	WIN32_FIND_DATA fileinfo;

	HANDLE handle;

	bool first_next;

	std::string scan_exp;

	std::string directory_path;
};

#endif
