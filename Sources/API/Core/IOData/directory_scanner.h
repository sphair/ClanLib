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

//! clanCore="I/O Data"
//! header=core.h

#ifndef header_directory_scanner
#define header_directory_scanner

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../System/sharedptr.h"
#include <string>

class CL_DirectoryScanner_Generic;

//: Directory scanning class.
//- !group=Core/IO Data!
//- !header=core.h!
//- <p>CL_DirectoryScanner is used to parse through directory trees and return information about files.</p>
//-
//- <p>Example that prints all files and directories found in the root directory:</p>
//- <pre>
//- CL_DirectoryScanner scanner;
//- if (scanner.scan("/", "*"))
//- {
//- 		while (scanner.next())
//- 		{
//- 				std::cout << scanner.get_name() << std::endl;
//- 		}
//- }
//- </pre>
class CL_API_CORE CL_DirectoryScanner
{
//! Construction:
public:
	//: Constructs directory scanner for iterating over a directory.
	CL_DirectoryScanner();

	CL_DirectoryScanner(const CL_DirectoryScanner &copy);

	//: Destructor.
	~CL_DirectoryScanner();

//! Attributes:
public:
	//: Gets the directory being scanned.
	//return: Directory being scanned.
	std::string get_directory_path();

	//: Gets the name of the current file.
	//return: The name of the current found file.
	std::string get_name();

	//: Gets the size of the current file.
	//return: The size of the current found file.
	int get_size();

	//: Gets the pathname of the current file.
	//return: The name of the current found file, including the directory path.
	std::string get_pathname();

	//: Returns true if the current file is a directory.
	//return: True if filename is a directory.
	bool is_directory();

	//: Returns true if the file is hidden.
	//return: True if filename is hidden.
	bool is_hidden();

	//: Returns true if the file is readable by the current user.
	//return: True if the file is readable.
	bool is_readable();

	//: Returns true if the file is writable by the current user.
	//return: True if the file is writable.
	bool is_writable();

//! Operations:
public:       
	//: Selects the directory to scan through.
	//- <p>Selects the directory to scan through and use a matching pattern on the files.
	//The pattern is normal DOS pattern matching (*.*).</p>
	//param pathname: Path to the directory to scan.
	//param pattern: Pattern to match files against.
	//return: true if the directory can be accessed.
	bool scan(const std::string& pathname);

	bool scan(const std::string& pathname, const std::string& pattern);

	//: Find next file in directory scan. 
	//return: false if no more files was found.
	bool next();

//! Implementation:
private:
	// Yada yada, usual data hiding.
	CL_SharedPtr<CL_DirectoryScanner_Generic> impl;
};

#endif
