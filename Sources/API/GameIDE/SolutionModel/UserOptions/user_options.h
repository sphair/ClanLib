/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

#pragma once

#include "project_options.h"
#include "opened_item.h"

namespace clan
{

class UserOptions
{
public:
	void load(const std::string &filename);
	void save(const std::string &filename);
	void clear();

	static std::string filename_from_solution(const std::string &solution_filename)
	{
		return clan::PathHelp::combine(clan::PathHelp::get_fullpath(solution_filename), clan::PathHelp::get_basename(solution_filename) + ".useroptions");
	}

	std::vector<ProjectOptions> projects;
	std::vector<OpenedItem> opened_items;
};

}

/// \}
