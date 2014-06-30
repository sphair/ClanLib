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

#include "GameIDE/precomp.h"
#include "API/GameIDE/SolutionModel/solution_model.h"

namespace clan
{

void SolutionModel::create_solution(std::string folder, std::string name)
{
	close_solution();
	Project::create(folder, name);
	solution.create(folder, name);
	user_options.clear();
	sig_model_updated();
}

void SolutionModel::open_solution(std::string filename)
{
	filename = PathHelp::make_absolute(Directory::get_current(), filename);
	close_solution();
	solution.load(filename);
	user_options.load(UserOptions::filename_from_solution(filename));
	for (size_t i = 0; i < solution.projects.size(); i++)
		get_project(solution.projects[i]); // Load all project now, if possible
	sig_model_updated();
}

void SolutionModel::close_solution()
{
	projects.clear();
	solution.close();
	user_options.clear();
	sig_model_updated();
}

ProjectPtr SolutionModel::get_project(ProjectReference reference)
{
	try
	{
		std::string ref_filename = PathHelp::make_absolute(PathHelp::get_fullpath(solution.filename), reference.filename);
		for (size_t i = 0; i < projects.size(); i++)
		{
			if (StringHelp::compare(projects[i]->filename, ref_filename, true) == 0)
			{
				return projects[i];
			}
		}
		ProjectPtr project(new Project());
		project->load(ref_filename);
		projects.push_back(project);
		return project;
	}
	catch (Exception &)
	{
		// This happens if PathHelp::make_absolute fails. That happens if the filename in the solution or reference is not valid.
		return ProjectPtr();
	}
}

void SolutionModel::add_file(ProjectItem *parent, std::string filename)
{
	std::string relative_path = PathHelp::make_relative(parent->get_filename(), filename);
	parent->add(new ProjectFileItem(relative_path));
	sig_model_updated();

	save_all(); // To do: this shouldn't be here since the app should ask for saving when you exit, but that's not coded yet
}

void SolutionModel::add_folder(ProjectItem *parent, std::string filename)
{
	std::string relative_path = PathHelp::make_relative(parent->get_filename(), filename);
	parent->add(new ProjectFolderItem(relative_path));
	sig_model_updated();

	save_all(); // To do: this shouldn't be here since the app should ask for saving when you exit, but that's not coded yet
}

void SolutionModel::remove_item(ProjectItem *item)
{
	if (item->parent() == 0)
		throw Exception("SolutionModel::remove_item misuse");

	for (size_t index = 0; index < item->parent()->children().size(); index++)
	{
		if (item->parent()->children()[index] == item)
		{
			delete item->parent()->remove(index);
			sig_model_updated();
			save_all(); // To do: this shouldn't be here since the app should ask for saving when you exit, but that's not coded yet
			break;
		}
	}
}

void SolutionModel::save_all()
{
	solution.save();
	for (size_t i = 0; i < projects.size(); i++)
	{
		projects[i]->save();
	}
	user_options.save(UserOptions::filename_from_solution(solution.filename));
}

bool SolutionModel::is_open(ProjectItem *item)
{
	Project *project = item->get_project();
	std::string filename = PathHelp::make_relative(PathHelp::get_fullpath(solution.filename), project->filename);
	std::string item_filename = item->get_project_relative_filename();
	for (size_t i = 0; i < user_options.projects.size(); i++)
	{
		if (user_options.projects[i].project_filename == filename)
		{
			for (size_t j = 0; j < user_options.projects[i].expanded_items.size(); j++)
			{
				if (user_options.projects[i].expanded_items[j] == item_filename)
					return true;
			}
		}
	}
	return false;
}

void SolutionModel::set_open(ProjectItem *item, bool open)
{
	Project *project = item->get_project();
	std::string filename = PathHelp::make_relative(PathHelp::get_fullpath(solution.filename), project->filename);
	std::string item_filename = item->get_project_relative_filename();
	for (size_t i = 0; i < user_options.projects.size(); i++)
	{
		if (user_options.projects[i].project_filename == filename)
		{
			for (size_t j = 0; j < user_options.projects[i].expanded_items.size(); j++)
			{
				if (user_options.projects[i].expanded_items[j] == item_filename)
				{
					if (!open)
					{
						user_options.projects[i].expanded_items.erase(user_options.projects[i].expanded_items.begin() + j);
						user_options.save(UserOptions::filename_from_solution(solution.filename));
					}
					return;
				}
			}
			if (open)
			{
				user_options.projects[i].expanded_items.push_back(item_filename);
				user_options.save(UserOptions::filename_from_solution(solution.filename));
			}
			return;
		}
	}

	if (open)
	{
		ProjectOptions options;
		options.project_filename = filename;
		options.expanded_items.push_back(item_filename);
		user_options.projects.push_back(options);
		user_options.save(UserOptions::filename_from_solution(solution.filename));
	}
}

std::vector<std::string> SolutionModel::get_files_with_extension(const std::string &extension)
{
	std::vector<std::string> files;
	for (size_t i = 0; i < projects.size(); i++)
	{
		std::vector<std::string> project_files = get_files_with_extension(&projects[i]->items, extension);
		files.insert(files.end(), project_files.begin(), project_files.end());
	}
	return files;
}

std::vector<std::string> SolutionModel::get_files_with_extension(ProjectItem *item, const std::string &extension)
{
	std::vector<std::string> files;
	ProjectFileItem *file_item = dynamic_cast<ProjectFileItem *>(item);
	if (file_item && StringHelp::compare(PathHelp::get_extension(file_item->filename), extension, true) == 0)
		files.push_back(file_item->get_filename());
	for (size_t i = 0; i < item->children().size(); i++)
	{
		std::vector<std::string> project_files = get_files_with_extension(item->children()[i], extension);
		files.insert(files.end(), project_files.begin(), project_files.end());
	}
	return files;
}

}

