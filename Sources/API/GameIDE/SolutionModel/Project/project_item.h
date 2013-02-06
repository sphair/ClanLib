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

/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

#pragma once
namespace clan
{
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
}

/// \}
