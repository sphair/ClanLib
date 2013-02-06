
#include "precomp.h"
#include "solution.h"

using namespace clan;

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
