
#pragma once

#include "project_item.h"

class Project
{
public:
	Project();
	static void create(std::string folder, std::string project_name);
	void load(std::string filename);
	void save();

	std::string get_path() const;

	std::string name;
	std::string filename;
	ProjectRootItem items;

	std::string output_directory;
	std::string intermediate_directory;

private:
	ProjectItem *load_item(clan::DomElement dom_item);
	void load_item_children(ProjectItem *item, clan::DomElement dom_items);
	void save_item(clan::DomElement dom_parent, ProjectItem *item);
	void save_item_children(clan::DomElement dom_parent, ProjectItem *item);

	Project(Project &other); // do not implement
	Project &operator =(Project &other); // do not implement
};

typedef std::shared_ptr<Project> ProjectPtr;
