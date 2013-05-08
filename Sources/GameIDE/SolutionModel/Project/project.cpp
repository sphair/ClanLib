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
#include "API/GameIDE/SolutionModel/Project/project.h"

namespace clan
{

Project::Project()
{
	items.project = this;
}

std::string Project::get_path() const
{
	return PathHelp::get_fullpath(filename);
}

void Project::create(std::string folder, std::string project_name)
{
	std::string project_dir = PathHelp::combine(folder, project_name);
	Project proj;
	proj.name = project_name;
	proj.filename = PathHelp::combine(project_dir, project_name + ".gameproject");
	proj.save();
}

void Project::load(std::string new_filename)
{
	File file(new_filename);
	DomDocument dom(file, true);
	file.close();

	DomElement dom_project = dom.get_document_element();
	if (dom_project.get_tag_name() != "project")
		throw Exception("Not a game project file");

	filename = new_filename;
	name = PathHelp::get_basename(filename);
	items.clear();

	std::vector<DomNode> items_nodes = dom_project.select_nodes("items");
	for (size_t i = 0; i < items_nodes.size(); i++)
	{
		load_item_children(&items, dom_project.select_node("items").to_element());
	}
}

void Project::save()
{
	DomDocument dom;
	DomElement dom_project = dom.create_element("project");
	save_item_children(dom_project, &items);
	dom.append_child(dom_project);

	Directory::create(PathHelp::get_fullpath(filename), true);
	File file(filename, File::create_always, File::access_read_write);
	dom.save(file, true);
}

ProjectItem *Project::load_item(DomElement dom_item)
{
	std::unique_ptr<ProjectItem> item;
	if (dom_item.get_tag_name() == "folder")
	{
		std::string path = dom_item.get_child_string("path");
		item.reset(new ProjectFolderItem(path));
	}
	else if (dom_item.get_tag_name() == "filter")
	{
		std::string name = dom_item.get_child_string("name");
		item.reset(new ProjectFilterItem(name));
	}
	else if (dom_item.get_tag_name() == "file")
	{
		std::string filename = dom_item.get_child_string("filename");
		item.reset(new ProjectFileItem(filename));
	}
	else
	{
		throw Exception("Unknown item type");
	}

	std::vector<DomNode> items_nodes = dom_item.select_nodes("items");
	for (size_t i = 0; i < items_nodes.size(); i++)
	{
		DomNode items_node = items_nodes[i];
		load_item_children(item.get(), items_node.to_element());
	}
	return item.release();
}

void Project::load_item_children(ProjectItem *item, DomElement dom_items)
{
	DomElement cur = dom_items.get_first_child_element();
	while (!cur.is_null())
	{
		item->add(load_item(cur));
		cur = cur.get_next_sibling_element();
	}
}

void Project::save_item(DomElement dom_parent, ProjectItem *item)
{
	DomElement dom_item;
	if (dynamic_cast<ProjectFolderItem*>(item))
	{
		ProjectFolderItem *folder_item = dynamic_cast<ProjectFolderItem*>(item);
		dom_item = dom_parent.get_owner_document().create_element("folder");
		dom_item.set_child_string("path", folder_item->path);
	}
	else if (dynamic_cast<ProjectFilterItem*>(item))
	{
		ProjectFilterItem *filter_item = dynamic_cast<ProjectFilterItem*>(item);
		dom_item = dom_parent.get_owner_document().create_element("filter");
		dom_item.set_child_string("name", filter_item->name);
	}
	else if (dynamic_cast<ProjectFileItem*>(item))
	{
		ProjectFileItem *file_item = dynamic_cast<ProjectFileItem*>(item);
		dom_item = dom_parent.get_owner_document().create_element("file");
		dom_item.set_child_string("filename", file_item->filename);
	}
	else
	{
		throw Exception("Unknown item type");
	}

	save_item_children(dom_item, item);
	dom_parent.append_child(dom_item);
}

void Project::save_item_children(DomElement dom_parent, ProjectItem *item)
{
	if (!item->children().empty())
	{
		DomElement dom_items = dom_parent.get_owner_document().create_element("items");
		for (size_t i = 0; i < item->children().size(); i++)
		{
			save_item(dom_items, item->children()[i]);
		}
		dom_parent.append_child(dom_items);
	}
}

}
