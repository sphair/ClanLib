
#include "precomp.h"
#include "project_item.h"
#include "project.h"

using namespace clan;

ProjectItem::ProjectItem()
: _parent(0)
{
}

ProjectItem::~ProjectItem()
{
	clear();
}

std::string ProjectItem::get_name() const
{
	return std::string();
}

std::string ProjectItem::get_filename()
{
	return PathHelp::normalize(parent_path_combine(false));
}

std::string ProjectItem::get_project_relative_filename()
{
	return PathHelp::normalize(parent_path_combine(true));
}

std::string ProjectItem::get_location()
{
	return PathHelp::normalize(get_parent_path(false));
}

ProjectItem *ProjectItem::parent() const
{
	return _parent;
}

const std::vector<ProjectItem *> &ProjectItem::children() const
{
	return _children;
}

void ProjectItem::add(ProjectItem *item)
{
	item->_parent = this;
	_children.push_back(item);
}

ProjectItem *ProjectItem::remove(int index)
{
	ProjectItem *item = _children[index];
	item->_parent = 0;
	_children.erase(_children.begin() + index);
	return item;
}

void ProjectItem::clear()
{
	for (size_t i = 0; i < _children.size(); i++)
		delete _children[i];
	_children.clear();
}

std::string ProjectItem::parent_path_combine(bool project_relative)
{
	return get_parent_path(project_relative);
}

std::string ProjectItem::get_parent_path(bool project_relative)
{
	if (_parent)
		return _parent->parent_path_combine(project_relative);
	else
		return "";
}

/////////////////////////////////////////////////////////////////////////////

ProjectRootItem::ProjectRootItem()
: project(0)
{
}

std::string ProjectRootItem::get_name() const
{
	return project->name;
}

std::string ProjectRootItem::parent_path_combine(bool project_relative)
{
	if (project_relative)
		return "";
	else
		return PathHelp::get_fullpath(project->filename);
}

/////////////////////////////////////////////////////////////////////////////

ProjectFolderItem::ProjectFolderItem(std::string path)
: path(path)
{
}

std::string ProjectFolderItem::get_name() const
{
	return PathHelp::get_filename(path);
}

std::string ProjectFolderItem::parent_path_combine(bool project_relative)
{
	return PathHelp::combine(get_parent_path(project_relative), path);
}

/////////////////////////////////////////////////////////////////////////////

ProjectFilterItem::ProjectFilterItem(std::string name)
: name(name)
{
}

std::string ProjectFilterItem::get_name() const
{
	return name;
}

std::string ProjectFilterItem::parent_path_combine(bool project_relative)
{
	return PathHelp::combine(get_parent_path(project_relative), name);
}

/////////////////////////////////////////////////////////////////////////////

ProjectFileItem::ProjectFileItem(std::string filename)
: filename(filename)
{
}

std::string ProjectFileItem::get_name() const
{
	return PathHelp::get_filename(filename);
}

std::string ProjectFileItem::parent_path_combine(bool project_relative)
{
	return PathHelp::combine(get_parent_path(project_relative), filename);
}
