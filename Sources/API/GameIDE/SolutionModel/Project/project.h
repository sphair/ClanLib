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


#pragma once

#include "project_item.h"

namespace clan
{
/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

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
	ProjectItem *load_item(DomElement dom_item);
	void load_item_children(ProjectItem *item, DomElement dom_items);
	void save_item(DomElement dom_parent, ProjectItem *item);
	void save_item_children(DomElement dom_parent, ProjectItem *item);

	Project(Project &other); // do not implement
	Project &operator =(Project &other); // do not implement
};

typedef std::shared_ptr<Project> ProjectPtr;

}

/// \}
