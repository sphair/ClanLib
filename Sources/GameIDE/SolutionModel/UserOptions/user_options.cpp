
#include "precomp.h"
#include "user_options.h"

using namespace clan;

void UserOptions::load(const std::string &filename)
{
	clear();

	try
	{
		File file(filename);
		DomDocument dom(file, true);
		file.close();

		DomElement dom_solution = dom.get_document_element();
		if (dom_solution.get_tag_name() != "user-options")
			throw Exception("Not a game options file");

		std::vector<DomNode> dom_projects = dom.select_nodes("user-options/projects/project");
		for (size_t i = 0; i < dom_projects.size(); i++)
		{
			DomElement dom_project = dom_projects[i].to_element();
			std::string filename = dom_project.get_child_string("filename");
			ProjectOptions project;
			project.project_filename = filename;
			std::vector<DomNode> dom_expanded_items = dom_project.select_nodes("expanded-items/item");
			for (size_t j = 0; j < dom_expanded_items.size(); j++)
			{
				std::string item = dom_expanded_items[j].to_element().get_text();
				if (!item.empty())
					project.expanded_items.push_back(item);
			}
			projects.push_back(project);
		}

		std::vector<DomNode> dom_opened_items = dom.select_nodes("user-options/opened-items/item");
		for (size_t j = 0; j < dom_opened_items.size(); j++)
		{
			DomElement item = dom_opened_items[j].to_element();
			OpenedItem opened_item;
			opened_item.filename = item.get_child_string("filename");
			opened_item.editor = item.get_child_string("editor");
			opened_items.push_back(opened_item);
		}
	}
	catch (Exception &)
	{
		clear();
	}
}

void UserOptions::save(const std::string &filename)
{
	DomDocument dom;
	DomElement dom_solution = dom.create_element("user-options");

	DomElement dom_projects = dom.create_element("projects");
	for (size_t i = 0; i < projects.size(); i++)
	{
		DomElement dom_project = dom.create_element("project");
		dom_project.set_child_string("filename", projects[i].project_filename);

		DomElement dom_expanded_items = dom.create_element("expanded-items");
		for (size_t j = 0; j < projects[i].expanded_items.size(); j++)
		{
			DomElement dom_item = dom.create_element("item");
			dom_item.append_child(dom.create_text_node(projects[i].expanded_items[j]));
			dom_expanded_items.append_child(dom_item);
		}
		dom_project.append_child(dom_expanded_items);

		dom_projects.append_child(dom_project);
	}
	dom_solution.append_child(dom_projects);

	DomElement dom_opened_items = dom.create_element("opened-items");
	for (size_t i = 0; i < opened_items.size(); i++)
	{
		DomElement dom_item = dom.create_element("item");
		dom_item.set_child_string("filename", opened_items[i].filename);
		dom_item.set_child_string("editor", opened_items[i].editor);
		dom_opened_items.append_child(dom_item);
	}
	dom_solution.append_child(dom_opened_items);

	dom.append_child(dom_solution);
	
	try
	{
		File file(filename, File::create_always, File::access_read_write);
		dom.save(file, true);
	}
	catch (Exception &)
	{
	}
}

void UserOptions::clear()
{
	projects.clear();
	opened_items.clear();
}
