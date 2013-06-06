/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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


#pragma once

#include "Solution/solution.h"
#include "Project/project.h"
#include "UserOptions/user_options.h"

namespace clan
{
/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

class SolutionModel
{
public:
	void create_solution(std::string folder, std::string name);
	void open_solution(std::string filename);
	void close_solution();
	ProjectPtr get_project(ProjectReference reference);
	void add_file(ProjectItem *parent, std::string filename);
	void add_folder(ProjectItem *parent, std::string filename);
	void remove_item(ProjectItem *item);
	void save_all();
	bool is_open(ProjectItem *item);
	void set_open(ProjectItem *item, bool open);
	std::vector<std::string> get_files_with_extension(const std::string &extension);
	std::vector<std::string> get_files_with_extension(ProjectItem *item, const std::string &extension);

	Solution solution;
	std::vector<ProjectPtr> projects;
	UserOptions user_options;

	Signal_v0 sig_model_updated;
};

}

/// \}
