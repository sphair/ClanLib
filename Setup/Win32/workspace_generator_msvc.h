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
*/

#ifndef header_workspace_generator_msvc
#define header_workspace_generator_msvc

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Generic/workspace.h"
#include <fstream>

#ifdef UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

std::string text_to_local8(const tstring &text);
tstring local8_to_text(const std::string &local8);

class WorkspaceGenerator_MSVC
{
protected:
	void install_mkdir(std::ofstream &dsp, const std::string &src_dir, const std::string &dest_dir, const Project *project);
	// Adds a directory to the install batch file.

	void install_copydir(std::ofstream &dsp, const std::string &src_dir, const std::string &dest_dir, const Project *project);
	// Adds copy commands for all files in a directory to the batch file.
};

class WorkspaceGenerator_MSVC6 : public WorkspaceGenerator_MSVC
// This class writes developer studio workspace and project files.
{
// Construction:
public:
	WorkspaceGenerator_MSVC6();
	// Constructs a msvc++ workspace generator.

// Attributes:
public:

// Operations:
public:
	void write(const Workspace &workspace);
	// Write MSVC6++ workspace and project files based on the passed generic template.

// Implementation:
private:
	void write_dsw(const Workspace &workspace);
	// Writes the Developer Studio workspace file.

	void write_dsw_examples(const Workspace &workspace);
	void write_dsw_tests(const Workspace &workspace);
	void write_dsw_utilities(const Workspace &workspace);
	void write_dsw_clanlib(const Workspace &workspace);

	void begin_project(std::ofstream &dsw, const std::string &name, const std::string &filename);
	// Writes the 'begin_project' text in a workspace file.

	void end_project(std::ofstream &dsw);
	// Writes the 'end_project' text in a workspace file.

	void add_project_dependency(std::ofstream &dsw, const std::string &dep_name);
	// Adds a project dependency to a project in a workspace file.
	
	void write_dsw_header(std::ofstream &dsw);
	// Writes workspace header.
	
	void write_dsw_tail(std::ofstream &dsw);
	// Writes workspace tail.

	void write_dsp(const Workspace &workspace, const Project &project);
	// Writes a Developer Studio project file.

	void begin_group(std::ofstream &dsp, const std::string &group_name);
	// Writes the 'begin_group' text of a project file.

	void end_group(std::ofstream &dsp);
	// Writes the 'end_group' text of a project file.

	void add_file(std::ofstream &dsp, const std::string &filename, const Project &project);
	// Adds a file to a group.

	void generate_source_files(std::ofstream &dsp, const Project &project);
	// Generate the list of source file groups.

	std::list<std::string> extract_path(const std::string &fullname);
	// Returns the path of a file as a linked list.

	Workspace workspace;
	// Generic description of the workspace.
};

class WorkspaceGenerator_MSVC7 : public WorkspaceGenerator_MSVC
// This class writes developer studio workspace and project files.
{
// Construction:
public:
	WorkspaceGenerator_MSVC7();
	// Constructs a msvc++ workspace generator.

// Attributes:
public:
	int target_version;

// Operations:
public:
	void write(const Workspace &workspace);
	// Write MSVC7++ workspace and project files based on the passed generic template.

// Implementation:
private:
	std::string get_project_guid(const std::string &project);

	std::string write_project(std::ofstream &sln, const Project &project, const std::string &vcproj_path);

	void generate_source_files(std::ofstream &vcproj, const Project &project);

	std::list<std::string> extract_path(const std::string &fullname);

	void begin_group(std::ofstream &vcproj, const std::string &group_name);

	void end_group(std::ofstream &vcproj);

	void add_file(std::ofstream &vcproj, const std::string &filename);

	Workspace workspace;
	// Generic description of the workspace.
};

#endif
