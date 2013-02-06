
#pragma once

#include "project_options.h"
#include "opened_item.h"

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
