
#pragma once

#include "Solution/solution.h"
#include "Project/project.h"
#include "UserOptions/user_options.h"

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

	clan::Signal_v0 sig_model_updated;
};
