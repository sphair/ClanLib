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
#include "GameIDE/precomp.h"
#include "API/GameIDE/BuildSystem/build_operation.h"
#include "API/GameIDE/SolutionModel/solution_model.h"

namespace clan
{

BuildOperation::BuildOperation(SolutionModel *solution)
{
}

void BuildOperation::build()
{
}

void BuildOperation::stop_build()
{
}

bool BuildOperation::is_building() const
{
	return false;
}

std::string BuildOperation::get_info_log()
{
	return info_log;
}

void BuildOperation::write_log_line(const std::string &text)
{
	info_log += text;
	info_log += "\n";
}

void BuildOperation::add_file(const std::string &file)
{
	files[file] = true;
}

std::vector<std::string> BuildOperation::get_files_with_extension(const std::string &extension)
{
	std::vector<std::string> found_files;
	for (auto it = files.begin(); it != files.end(); ++it)
	{
		if (StringHelp::compare(PathHelp::get_extension(it->first), extension, true) == 0)
			found_files.push_back(it->first);
	}
	return found_files;
}

}