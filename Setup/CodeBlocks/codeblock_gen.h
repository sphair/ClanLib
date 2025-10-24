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

#include "../Generic/project.h"
#include "../Generic/workspace.h"

#include <iostream>
#include <fstream>
#include <list>

void gen_workspace(const std::string &name, const std::string &basedir, Workspace &workspace);

void gen_project_head(std::ofstream &project_file);
void gen_project_foot(std::ofstream &project_file);

void gen_compiler_options(
	std::ofstream &project_file,
	std::list<std::string> &options_list,
	std::list<std::string> &defines_list,
	std::list<std::string> &dir_list);

void gen_linker_options(
	std::ofstream &project_file,
	std::list<std::string> &options_list,
	std::list<std::string> &dir_list);

void gen_file_units(
	std::ofstream &project_file,
	const std::string &basedir,
	std::list<std::string> file_list,
	std::list<std::string> &targets);
