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

#ifndef RESOURCEOPTIONS_H_
#define RESOURCEOPTIONS_H_

#include <map>
#include <string>
#include "resourceoption.h"

//: Resource options of a resource.
class ResourceOptions
{
//! Construction:
public:
	//: Resource Options constructor.
	ResourceOptions();

	//: Resource Options destructor.
	~ResourceOptions();

//! Attributes:
public:
	//: Returns a list of all the resource options.
	std::map<std::string, ResourceOption> &get_options();

	//: Returns a resource option.
	ResourceOption &get_option(const std::string &name);

	const ResourceOption &get_option(const std::string &name) const;

	//: Returns true if an option exists.
	bool exists(const std::string &name);

//! Operations:
public:
	//: Add a resource option.
	void add(const ResourceOption &option);
	void remove(const std::string&);

//! Implementation:
private:
	std::map<std::string, ResourceOption> options;
};

#endif
