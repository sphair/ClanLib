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
**    (if your name is missing here, please add it)
*/

#include "resourceoptions.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions construction:

ResourceOptions::ResourceOptions()
{
}

ResourceOptions::~ResourceOptions()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions attributes:

std::map<std::string, ResourceOption> &ResourceOptions::get_options()
{
	return options;
}

ResourceOption &ResourceOptions::get_option(const std::string &name)
{
	return options[name];
}

const ResourceOption &ResourceOptions::get_option(const std::string &name) const
{
	return options.find(name)->second;
}

bool ResourceOptions::exists(const std::string &name)
{
	return options.find(name) != options.end();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions operations:

void ResourceOptions::add(const ResourceOption &option)
{
	options[option.get_name()] = option;
}

void ResourceOptions::remove(const std::string& name)
{
	options.erase(options.find(name));
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions implementation:
