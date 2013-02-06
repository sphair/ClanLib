
#pragma once

class Project;

class ProjectItem
{
public:
	ProjectItem();
	virtual ~ProjectItem();
	virtual std::string get_name() const;
	virtual Project *get_project() const { return parent()->get_project(); }

	std::string get_filename();
	std::string get_project_relative_filename();
	std::string get_location();

	ProjectItem *parent() const;
	const std::vector<ProjectItem *> &children() const;

	void add(ProjectItem *item);
	ProjectItem *remove(int index);
	void clear();

protected:
	virtual std::string parent_path_combine(bool project_relative);
	std::string get_parent_path(bool project_relative);

private:
	ProjectItem *_parent;
	std::vector<ProjectItem *> _children;
};

class ProjectRootItem : public ProjectItem
{
public:
	ProjectRootItem();
	std::string get_name() const;
	Project *get_project() const { return project; }

	Project *project;

protected:
	std::string parent_path_combine(bool project_relative);
};

class ProjectFolderItem : public ProjectItem
{
public:
	ProjectFolderItem(std::string path);
	std::string get_name() const;

	std::string path;

protected:
	std::string parent_path_combine(bool project_relative);
};

class ProjectFilterItem : public ProjectItem
{
public:
	ProjectFilterItem(std::string name);
	std::string get_name() const;

	std::string name;

protected:
	std::string parent_path_combine(bool project_relative);
};

class ProjectFileItem : public ProjectItem
{
public:
	ProjectFileItem(std::string filename);
	std::string get_name() const;

	std::string filename;

protected:
	std::string parent_path_combine(bool project_relative);
};
