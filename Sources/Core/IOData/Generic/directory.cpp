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

#include "Core/precomp.h"
#include "API/Core/IOData/directory.h"
#include "API/Core/IOData/directory_scanner.h"

#ifndef WIN32
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#else
#include <direct.h>
#ifndef chdir
#define _chdir chdir
#endif
#ifndef MAX_PATH
#define _MAX_PATH MAX_PATH
#endif
#endif

#ifdef __BORLANDC__
#include <dir.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

/////////////////////////////////////////////////////////////////////////////
// Operations

bool CL_Directory::create(const std::string &dir_name)
{
	if (dir_name.empty())
		return false;

	// this will be a full path
	std::string full_path; 	// calculate the full path

	#ifdef WIN32
		DWORD buff_len = ::GetFullPathName(dir_name.c_str(), 0, 0, 0);

		if (buff_len == 0)
			// can't calculate, return bad status
			return false;
		else
		{
			char * buffer = new char[buff_len + 1];
			char * buffer_ptr_to_filename = 0;
			// Obtaining full path
			buff_len = ::GetFullPathName(dir_name.c_str(), buff_len, buffer, &buffer_ptr_to_filename);
			if (buff_len == 0)
			{
				delete[] buffer;			
				// can't obtain full path, return bad status
				return false;
			}
		
			// ok, save it
			full_path = buffer;
			delete[] buffer;			
		}
	#else
		// TODO: add here Linux version of GetFullPathName
		full_path = dir_name;
	#endif

#ifdef WIN32
		return ::CreateDirectory(full_path.c_str(), NULL) != 0;
#else
		return ::mkdir(full_path.c_str(), 0755) == 0;
#endif
}

bool CL_Directory::remove(const std::string &dir_name, bool delete_files, bool delete_sub_directories)
{
	if (dir_name.empty())
		return false;

	// this will be a full path
	std::string full_path;

	// calculate the full path
	#ifdef WIN32
		DWORD buff_len = ::GetFullPathName(dir_name.c_str(), 0, 0, 0);

		if (buff_len == 0)
			// can't calculate, return bad status
			return false;
		else
		{
			char * buffer = new char[buff_len + 1];
			char * buffer_ptr_to_filename = 0;
			// Obtaining full path
			buff_len = ::GetFullPathName(dir_name.c_str(), buff_len, buffer, &buffer_ptr_to_filename);
			if (buff_len == 0)
			{
				// can't obtaing full path, return bad status
				delete[] buffer;			
				return false;
			}
		
			// ok, save it
			full_path = buffer;
			delete[] buffer;			

		}
	#else
		// TODO: add here Linux version of GetFullPathName
		full_path = dir_name;
	#endif

	// This scope needed for deleting directiory at end of function,
	// because scanner lock current dir :(
	{
		CL_DirectoryScanner scanner;

		if (!scanner.scan(full_path))
			// can't even start scaning
			return false;

		// FIXME: probably bug in directory_scanner, it return ""
		// for first file :(
		if (scanner.next())
		while(1)
		{
			// If found sub_directory, try remove it,
			// also checking for "." and "..", because they are unremovable
			if (scanner.is_directory() && delete_sub_directories &&
				scanner.get_name() != "." && scanner.get_name() != "..")
			{
				// FIXME: directory_scanner lock directory, so it can't be
				// removed, this is workaround
				std::string sub_dir_path = scanner.get_pathname();
				bool scann_successfull = scanner.next();

				// delete files in sub_directory
				if (!CL_Directory::remove(sub_dir_path.c_str(),
							  delete_files,
							  delete_sub_directories))
					return false;

				if (!scann_successfull)
					break;
				else
					continue;
			}
			else
			{
				// Check for deleting file (or whatever is not directory),
				// if this is allowed
				if (delete_files && !scanner.is_directory())
				{
					// delete a file
					#ifdef WIN32
						if (::DeleteFile(scanner.get_pathname().c_str()) == 0)
							return false;
					#else
						if (::remove(scanner.get_pathname().c_str()) != 0)
							return false;
					#endif
					if (!scanner.next())
						break;
				}
				// This is for "." and ".."
				else
				{
					if (!scanner.next())
						break;
				}
			}
		}
	}
	// Finaly remove the directory (or sub_directory if in recursion)
	#ifdef WIN32
		return ::RemoveDirectory(full_path.c_str()) != 0;
	#else
		return ::rmdir(full_path.c_str()) == 0;
	#endif
}

bool CL_Directory::change_to(const std::string &dir_name)
{
	return chdir(dir_name.c_str()) == 0;
}

std::string CL_Directory::get_current()
{
	char cwd_buffer[MAX_PATH];
	if (getcwd(cwd_buffer, MAX_PATH) == NULL)
		throw CL_Error("Working dir is more than legal length !");
	return cwd_buffer;
}
