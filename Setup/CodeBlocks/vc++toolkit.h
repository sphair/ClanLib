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

#include <windows.h>
#include <fstream>

void setup_vc_clanlib_workspace(
	const std::list<std::string> &defines_list,
	const std::string &input_lib_dir,
	const std::string &input_include_dir,
	const std::string &output_lib_dir,
	const std::string &output_include_dir
	);

void gen_vc_project(const Project &project, const Workspace &workspace);

void gen_vc_install_bat(const Project &project, const Workspace &workspace);

void install_mkdir(
	std::ofstream &dsp,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project);

void install_copydir(
	std::ofstream &dsp,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project);
