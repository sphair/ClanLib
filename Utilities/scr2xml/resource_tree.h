/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef RESOURCE_TREE_
#define RESOURCE_TREE_

#include <string>
#include <vector>

#include "resourceoptions.h"

class Tree;
class ResourceManager;

class ResourceTree
{
public:
	ResourceTree(std::vector<char> buffer);
	~ResourceTree();

	void add_section(std::string name);
	void end_section(void);
	void add_resource(std::string name, std::string location, const ResourceOptions &options);

	void write();
private:
	Tree *tree;
	ResourceManager* manager;
};

void init_resource_map();

#endif
