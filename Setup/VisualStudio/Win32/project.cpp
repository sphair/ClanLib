/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#ifdef WIN32
#pragma warning(disable : 4786)
#include <windows.h>
#endif

#include "project.h"

/////////////////////////////////////////////////////////////////////////////
// Project files exclude list:

/////////////////////////////////////////////////////////////////////////////
// Project construction:

Project::Project()
{
}

Project::Project(
	const std::string &name,
	const std::string &libname,
	const std::string &headername,
	const std::list<std::string> &libs_list_shared,
	const std::list<std::string> &libs_list_release,
	const std::list<std::string> &libs_list_debug,
	const std::list<std::string> &ignore_list,
	const std::list<std::string>& exclude_list)
:
	name(name),
	libname(libname),
	headername(headername),
	libs_shared(libs_list_shared),
	libs_release(libs_list_release),
	libs_debug(libs_list_debug)
{
	std::string lib_main_header;
	lib_main_header = std::string("Sources\\API\\") + headername;
 	files.push_back(ProjectFiles(false, lib_main_header));

	generate_dir(std::string("Sources\\API\\")+name, ignore_list, exclude_list);
	generate_dir(std::string("Sources\\")+name, ignore_list, exclude_list);
}

/////////////////////////////////////////////////////////////////////////////
// Project attributes:

/////////////////////////////////////////////////////////////////////////////
// Project operations:

/////////////////////////////////////////////////////////////////////////////
// Project implementation:

void Project::generate_dir(
	const std::string &dir,
	const std::list<std::string> &ignore_list,
	const std::list<std::string>& exclude_list, bool force_exclude_all)
{
	std::string path = dir;
	if (path[path.length()-1] != '\\') path += '\\';

	WIN32_FIND_DATAA	data;
	HANDLE handle = FindFirstFileA(std::string(path + "*.*").c_str(), &data);
	if (handle == INVALID_HANDLE_VALUE) return;

	do
	{
		bool skip = false;
		bool exclude = force_exclude_all;

		if (strncmp(data.cFileName, ".#", 2) == 0) continue; // don't add CVS revision backups.
		if (strchr(data.cFileName, '~') != NULL) continue;  //Don't get those emacs/bcc backup files

		for (auto it = ignore_list.begin(); it != ignore_list.end(); it++)
		{
			if (_stricmp(data.cFileName, it->c_str()) == 0)
				skip = true;
		}

		if (skip)
			continue;

		for (auto it = exclude_list.begin(); it != exclude_list.end(); it++)
		{
			if (_stricmp(data.cFileName, it->c_str()) == 0)
				exclude = true;
		}

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			generate_dir(path + data.cFileName, ignore_list, exclude_list, exclude);
		}
		else
		{
			files.push_back(ProjectFiles(exclude, path + data.cFileName));
		}

	} while (FindNextFileA(handle, &data));
}

