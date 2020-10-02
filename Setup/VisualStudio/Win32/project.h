/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#ifndef header_project
#define header_project

#pragma once

#include <string>
#include <list>

class ProjectFiles
{
public:
	ProjectFiles(bool is_exclude_from_build, const std::string& filename) : exclude_from_build(is_exclude_from_build), name(filename) {}
	bool exclude_from_build;
	std::string name;
};

class Project
// Generic description of a clanlib project.
{
// Construction:
public:
	Project();
	// Construct an empty project.

	Project(
		const std::string &name,
		const std::string &libname,
		const std::string &headername,
		const std::list<std::string> &libs_list_shared,
		const std::list<std::string> &libs_list_release,
		const std::list<std::string> &libs_list_debug,
		const std::list<std::string> &ignore_list,
		const std::list<std::string>& exclude_list);
	// Construct a project by searching automatically for the files in the Sources subdir.

// Attributes:
public:
	std::string name;
	// Project subdir name. Eg. "Core".

	std::string libname;
	// Project library name. Eg. "libCore".

	std::string headername;
	// Project library name. Eg. "core.h".

	std::list<std::string> libs_shared;
	// Libraries to link with in all targets.

	std::list<std::string> libs_release;
	// Libraries to link with in release target.

	std::list<std::string> libs_debug;
	// Libraries to link with in debug target.

	std::list<ProjectFiles> files;
	// Files used by project.

// Operations:
public:

// Implementation:
private:
	void generate_dir(
		const std::string &dir,
		const std::list<std::string> &ignore_list,
		const std::list<std::string>& exclude_list, bool force_exclude_all = false);

	// Scan directory for files and add them to file list.
};

#endif
