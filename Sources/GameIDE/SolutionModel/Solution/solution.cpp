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
#include "solution.h"

namespace clan
{

void Solution::create(std::string location, std::string solution_name)
{
	close();
	std::string solution_dir = PathHelp::combine(location, solution_name);
	name = solution_name;
	filename = PathHelp::combine(solution_dir, solution_name + ".gamesolution");
	projects.push_back(ProjectReference(name, solution_name + ".gameproject"));
	save();
}

void Solution::load(std::string new_filename)
{
	close();

	File file(new_filename);
	DomDocument dom(file, true);
	file.close();

	DomElement dom_solution = dom.get_document_element();
	if (dom_solution.get_tag_name() != "game-solution")
		throw Exception("Not a game solution file");

	filename = new_filename;
	name = PathHelp::get_basename(filename);

	std::vector<DomNode> dom_projects = dom.select_nodes("game-solution/projects/project");
	for (size_t i = 0; i < dom_projects.size(); i++)
	{
		DomElement dom_project = dom_projects[i].to_element();
		std::string name = dom_project.get_child_string("name");
		std::string filename = dom_project.get_child_string("filename");
		ProjectReference project(name, filename);
		projects.push_back(project);
	}
}

void Solution::save()
{
	DomDocument dom;
	DomElement dom_solution = dom.create_element("game-solution");
	DomElement dom_projects = dom.create_element("projects");
	for (size_t i = 0; i < projects.size(); i++)
	{
		DomElement dom_project = dom.create_element("project");
		dom_project.set_child_string("name", projects[i].name);
		dom_project.set_child_string("filename", projects[i].filename);
		dom_projects.append_child(dom_project);
	}
	dom_solution.append_child(dom_projects);
	dom.append_child(dom_solution);
	
	Directory::create(PathHelp::get_fullpath(filename), true);
	File file(filename, File::create_always, File::access_read_write);
	dom.save(file, true);
}

void Solution::close()
{
	name.clear();
	filename.clear();
	projects.clear();
}

}

