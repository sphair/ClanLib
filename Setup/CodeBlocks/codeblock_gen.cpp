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
**    Lih-Hern (Lf3T-Hn4D)
**    (if your name is missing here, please add it)
*/

#include "codeblock_gen.h"

#ifdef WIN32
	#define PATH_SEPERATOR "\\"
	#define PARENT_PATH "..\\"
#else
	#define PATH_SEPERATOR "/"
	#define PARENT_PATH "../"
#endif

void gen_workspace(const std::string &name, const std::string &basedir, Workspace &workspace)
{
	std::ifstream workspace_head("Setup/CodeBlocks/workspace_head.xml");
	std::ifstream workspace_foot("Setup/CodeBlocks/workspace_foot.xml");
	if(!workspace_head) std::cout << "Error! workspace_head.xml not found. Incomplete source." << std::endl; //Shit
	if(!workspace_foot) std::cout << "Error! workspace_foot.xml not found. Incomplete source." << std::endl; //Shit

	std::string filename = name + ".workspace";
	std::cout << "Generating workspace " << filename << "... ";

	std::ofstream workspace_file(filename.c_str());

	workspace_file << workspace_head.rdbuf();
	workspace_file.flush();

	// Write workspace name.
	workspace_file << "\t<Workspace title=\"" << name << "\">\n";

	// Write projects.
	std::list<Project>::iterator iter;
	bool first = true;
	for (iter = workspace.projects.begin(); iter != workspace.projects.end(); iter++)
	{
		if (first)
		{
			workspace_file << "\t\t<Project filename=\"" << basedir << "/" << (*iter).libname << ".cbp\" active=\"1\"/>\n";
			first = false;
		}
		else
		{
			workspace_file << "\t\t<Project filename=\"" << basedir << "/" << (*iter).libname << ".cbp\"/>\n";
		}
	}

	workspace_file << workspace_foot.rdbuf();
	workspace_file.flush();

	std::cout << "done." << std::endl;
}

void gen_project_head(std::ofstream &project_file)
{
	std::ifstream project_head("Setup/CodeBlocks/project_head.xml");
	if(!project_head) std::cout << "Error! project_head.xml not found. Incomplete source." << std::endl; //Shit

	project_file << project_head.rdbuf();
	project_file.flush();
}

void gen_project_foot(std::ofstream &project_file)
{
	std::ifstream project_foot("Setup/CodeBlocks/project_foot.xml");
	if(!project_foot) std::cout << "Error! project_foot.xml not found. Incomplete source." << std::endl; //Shit

	project_file << project_foot.rdbuf();
	project_file.flush();
}

void gen_compiler_options(
	std::ofstream &project_file,
	std::list<std::string> &options_list,
	std::list<std::string> &defines_list,
	std::list<std::string> &dir_list)
{
	project_file << "\t\t\t\t<Compiler>\n";

	std::list<std::string>::iterator iter;
	for (iter = options_list.begin(); iter != options_list.end(); iter++)
		project_file << "\t\t\t\t\t<Add option=\"/" << *iter << "\"/>\n";

	for (iter = defines_list.begin(); iter != defines_list.end(); iter++)
		project_file << "\t\t\t\t\t<Add option=\"/D" << *iter << "\"/>\n";

	for (iter = dir_list.begin(); iter != dir_list.end(); iter++)
		project_file << "\t\t\t\t\t<Add directory=\"" << *iter << "\"/>\n";

	project_file << "\t\t\t\t</Compiler>\n";
}

void gen_linker_options(
	std::ofstream &project_file,
	std::list<std::string> &options_list,
	std::list<std::string> &dir_list)
{
	project_file << "\t\t\t\t<Linker>\n";

	std::list<std::string>::iterator iter;
	for (iter = options_list.begin(); iter != options_list.end(); iter++)
		project_file << "\t\t\t\t\t<Add option=\"/" << *iter << "\"/>\n";

	for (iter = dir_list.begin(); iter != dir_list.end(); iter++)
		project_file << "\t\t\t\t\t<Add directory=\"" << *iter << "\"/>\n";

	project_file << "\t\t\t\t</Linker>\n";
}

void gen_file_units(
	std::ofstream &project_file,
	const std::string &basedir,
	std::list<std::string> file_list,
	std::list<std::string> &targets)
{
	std::string filename, file_ext, file_no_ext, first_dir;
	std::list<std::string>::iterator iter, target_iter;
	for (iter = file_list.begin(); iter != file_list.end(); iter++)
	{
		filename = *iter;

		// get the first directory from the path.
		first_dir = filename.substr(0, filename.find_first_of(PATH_SEPERATOR));

		// fix the path relative to the basedir.
		if (basedir == first_dir) filename = filename.substr(filename.find_first_of(PATH_SEPERATOR) + 1);
		else filename = PARENT_PATH + filename;

		project_file << "\t\t<Unit filename=\"" << filename << "\">\n";

		file_ext = filename.substr(filename.find_last_of("."));
		if (file_ext == ".cpp") project_file << "\t\t\t<Option compilerVar=\"CPP\"/>\n";
		else if (file_ext == ".c") project_file << "\t\t\t<Option compilerVar=\"C\"/>\n";
		else if (file_ext == ".rc")
		{
			file_no_ext = filename.substr(0, filename.size() - filename.find_last_of("."));
			project_file << "\t\t\t<Option compilerVar=\"WINDRES\"/>\n";
			project_file << "\t\t\t<Option objectName=\"" << file_no_ext << ".res\"/>\n";
		}
		else
		{
			project_file << "\t\t\t<Option compilerVar=\"\"/>\n";
			project_file << "\t\t\t<Option compile=\"0\"/>\n";
			project_file << "\t\t\t<Option link=\"0\"/>\n";
		}

		for (target_iter = targets.begin(); target_iter != targets.end(); target_iter++)
			project_file << "\t\t\t<Option target=\"" << *target_iter << "\"/>\n";

		project_file << "\t\t</Unit>\n";
	}
}
