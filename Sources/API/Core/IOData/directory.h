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

#ifndef header_directory
#define header_directory

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

#include <string>

//: Directory utility class
//- !group=Core/IO Data!
//- !header=core.h!
class CL_API_CORE CL_Directory
{
//! Operations:
public:
	//: Create directory.
	//- <p>return true on success or false on error.</p>
	//param dir_name: Directory name for create.
	static bool create(const std::string &dir_name);

	//: Remove directory.
	//- <p>Return true on success or false on error.</p>
	//param dir_name: Directory name for delete.
	//param delete_files: If true, function will delete files.
	//param delete_sub_directoies: If true, function will delete
	//param delete_sub_directoies: sub directoies too.
	static bool remove(
		const std::string &dir_name,
		bool delete_files = false,
		bool delete_sub_directories = false);

	//: Change current directory.
	//- <p>Return true on success or false on error.</p>
	//param path: Directory name to change to.
	static bool change_to(const std::string &path);

	//: Get current directory.
	//- <p>Returns the current directory path.</p>
	static std::string get_current();
};

#endif // header_directory
